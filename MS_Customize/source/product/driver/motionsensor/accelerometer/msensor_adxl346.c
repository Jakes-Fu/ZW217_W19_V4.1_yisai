/******************************************************************************
 ** File Name:      msensor_adxl246.c                                          *
 ** Author:         Will.Jiang                                                *
 ** DATE:           2012.01.03                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2012.01.03   Will.Jiang            Create.                               *
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
#define ADXL346_DEBUG
//#define SENSOR_INT_MODE
#ifdef ADXL346_DEBUG
#define ADXL346_TRACE			SCI_TRACE_LOW
#else
#define ADXL346_TRACE(x)
#endif

#ifdef WIN32
#define __func__ 
#endif

// registers map
#define     ADXL346_DEVID_REG               0x00
#define     ADXL346_DEVID_REG_ID            0xE6 // ADXL346 device id

#define     ADXL346_BWRATE_REG              0X2C
#define     ADXL346_BWRATE_REG_DATA         0x8  // Normal mode, 25Hz

#define     ADXL346_POWERCTL_REG            0X2D
#define     ADXL346_POWERCTL_REG_SLEEP      0x00 // only clear "MEARSURE"
#define     ADXL346_POWERCTL_REG_WORK       0x08 // start to measure

#define     ADXL346_ORIENT_REG              0X3B
#define     ADXL346_ORIENT_REG_DATA         0XA9 // 2D/3D int enable, div = ODR/22 = 25Hz/22 == 1Hz
#define     ADXL346_ORIENT_REG_FORMAT       0X31 // bit3: FULL_RES:1 : 16bit mode; 0:10bit mode


#define     ADXL346_INTCTL_REG              0x2E
#define     ADXL346_INTCTL_REG_DISABLE      0x00


#define     ADXL346_OUT_X_REG_0             0X32
#define     ADXL346_OUT_X_REG_1             0X33

#define     ADXL346_OUT_Y_REG_0             0X34
#define     ADXL346_OUT_Y_REG_1             0X35

#define     ADXL346_OUT_Z_REG_0             0X36
#define     ADXL346_OUT_Z_REG_1             0X37


#define DATA_VECTOR_LENGTH 4
#define SHAKE_ABSTH ADXL346_THRESHOLD_2_0G

// according to datasheet, 1 mg/digit on 0--2G scale, MOCOR's unit: mg
#define ADXL346_DIGIT_TO_MG             2

#define ADXL346_THRESHOLD_0_2G            200
#define ADXL346_THRESHOLD_0_5G            512
#define ADXL346_THRESHOLD_0_6G            600
#define ADXL346_THRESHOLD_0_7G            700
#define ADXL346_THRESHOLD_0_8G            800
#define ADXL346_THRESHOLD_0_9G            900
#define ADXL346_THRESHOLD_1_0G            1024  

#define ADXL346_THRESHOLD_1_1G            1100
#define ADXL346_THRESHOLD_1_2G            1200
#define ADXL346_THRESHOLD_1_5G            1500
#define ADXL346_THRESHOLD_1_8G            1800
#define ADXL346_THRESHOLD_2_0G            2000  
#define ADXL346_THRESHOLD_2_5G            2500  


#define TIMER_PERIOD_ADXL346         100

#define SHAKE_INIT  0x00
#define SHAKE_FIRST_PEAK 0x01
#define SHAKE_SENCOND_PEAK 0x02 
#define SHAKE_THIRD_JUDGE 0x03
#define SHAKE_WAIT  0x04

#define SHAKEDELAY_TIMEOUT  10
#define SHAKEDELAY_INTERVAL 15

/**---------------------------------------------------------------------------*
 **                         DataStruct Definition                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Variables and function                            *
 **---------------------------------------------------------------------------*/ 
LOCAL uint32 ADXL346_init(void);
LOCAL uint32 ADXL346_open(void);
LOCAL uint32 ADXL346_close(void);
LOCAL void   ADXL346_TimerCallback(uint32 param);
LOCAL uint32 ADXL346_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);
LOCAL uint32 ADXL346_SetSensi(MSENSOR_SENSI_E sensi_val);

PUBLIC MSENSOR_CFG_T g_adxl346_msensor_info = 
{
    "ADI ADXL346",
	MSENSOR_TYPE_ACCELEROMETER,
    MSENSOR_STATUS_NOT_INIT,
	ADXL346_init,
	ADXL346_open,
	ADXL346_close,
	ADXL346_ioctl
};

LOCAL MSENSOR_MISC_CFG_T s_adxl346_misc_cfg ;
LOCAL int32              s_ADXL346_IIC_Handler = -1;
LOCAL SCI_TIMER_PTR      s_ADXL346_timer;
// LOCAL SCI_SEMAPHORE_PTR    	s_ADXL346_sm    = PNULL;

LOCAL int32                 s_ADXL346_sh_th = ADXL346_THRESHOLD_1_2G; // shake threshhold for sensitivity
LOCAL int32                 s_ADXL346_or_th = ADXL346_THRESHOLD_0_7G; // orientation threshhold for sensitivity
/*****************************************************************************/
//  Description:    read chip register content
//  Author:         
//  Note:       
/*****************************************************************************/


LOCAL int32 adxl346_init_i2c_handler(void)
{

	if((s_ADXL346_IIC_Handler == -1) && 
		((s_ADXL346_IIC_Handler = I2C_HAL_Open(&s_adxl346_misc_cfg.i2c_cfg)) == -1))
	{
		return -1;
	}
	
	return s_ADXL346_IIC_Handler;
}



LOCAL int32 adxl346_release_i2c_handler(void)
{
	if(s_ADXL346_IIC_Handler != -1)
	{
 		I2C_HAL_Close(s_ADXL346_IIC_Handler);
		s_ADXL346_IIC_Handler = -1;
	}
	return s_ADXL346_IIC_Handler;
}


/*****************************************************************************/
//  Description:    read chip register content
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint8 adxl346_reg_read(uint8 addr)
{
    uint8 val = 0;
    
    if(-1 == s_ADXL346_IIC_Handler)
    {
        adxl346_init_i2c_handler();
    }
    I2C_HAL_Read(s_ADXL346_IIC_Handler, &addr, &val, 1);    
    return val;
}

/*****************************************************************************/
//  Description:    write configuration data to chip registers
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL void adxl346_reg_write(uint8 addr, uint8 val)
{
    if(-1 == s_ADXL346_IIC_Handler)
    {
        adxl346_init_i2c_handler();
    }
    I2C_HAL_Write(s_ADXL346_IIC_Handler, &addr, &val, 1);    
}


/*****************************************************************************/
//  Description:    Init g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 ADXL346_init(void)
{
	uint8   device_id = 0;

    MSensor_GetMiscCfg(&g_adxl346_msensor_info, &s_adxl346_misc_cfg); 
    
    adxl346_init_i2c_handler();
    
    //ADXL346_SetSensi(MSENSOR_SENSI_MID); 
    
    device_id = adxl346_reg_read(ADXL346_DEVID_REG);
    ADXL346_TRACE("ADXL346_init : id = 0x%x",device_id);
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_ADXL346_205_112_2_18_0_34_16_1879,(uint8*)"sd",__func__, device_id);
    if(ADXL346_DEVID_REG_ID != device_id)
    {
        return MSENSOR_ERROR_NOT_SUPPORT;
    }

	//POWER DOWN SENSOR
	adxl346_reg_write(ADXL346_POWERCTL_REG,ADXL346_POWERCTL_REG_SLEEP);
    adxl346_reg_write(ADXL346_INTCTL_REG,ADXL346_INTCTL_REG_DISABLE);


    adxl346_release_i2c_handler();
	
	return MSENSOR_SUCCESS;
}

/*****************************************************************************/
//  Description:    open g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 ADXL346_open(void)
{  
    adxl346_reg_write(ADXL346_INTCTL_REG,ADXL346_INTCTL_REG_DISABLE);
	adxl346_reg_write(ADXL346_BWRATE_REG,ADXL346_BWRATE_REG_DATA);
	adxl346_reg_write(ADXL346_ORIENT_REG,ADXL346_ORIENT_REG_DATA);
	adxl346_reg_write(ADXL346_POWERCTL_REG,ADXL346_POWERCTL_REG_WORK);
	
	if(s_ADXL346_timer == NULL)
	{
        s_ADXL346_timer = SCI_CreateTimer("ADXL346_Timer", ADXL346_TimerCallback,
			MSENSOR_TYPE_ACCELEROMETER, TIMER_PERIOD_ADXL346, SCI_AUTO_ACTIVATE);
	}
	else
	{
		SCI_ChangeTimer(s_ADXL346_timer, ADXL346_TimerCallback, TIMER_PERIOD_ADXL346);
		SCI_ActiveTimer(s_ADXL346_timer);	
	}
  
	return MSENSOR_SUCCESS;
}	
/*****************************************************************************/
//  Description:    close g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 ADXL346_close(void)
{
	//Timer and semaphore need NOT be deleted. Just to deactive them.
	if(s_ADXL346_timer != NULL)
	{
		SCI_DeactiveTimer(s_ADXL346_timer);
	}

	//POWER DOWN SENSOR
	adxl346_reg_write(ADXL346_POWERCTL_REG,ADXL346_POWERCTL_REG_SLEEP);
    adxl346_reg_write(ADXL346_INTCTL_REG,ADXL346_INTCTL_REG_DISABLE);
	
	I2C_HAL_Close(s_ADXL346_IIC_Handler);
	s_ADXL346_IIC_Handler = -1;

	return MSENSOR_SUCCESS;
}



/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 ADXL346_GetVector(MSENSOR_MOTION_DATA_T * data_ptr)
{   
    int32  x,y,z,temp;
	uint8  x0,y0,z0;
	uint8  x1,y1,z1;
	
    x0 = adxl346_reg_read(ADXL346_OUT_X_REG_0);
    x1 = adxl346_reg_read(ADXL346_OUT_X_REG_1);
    y0 = adxl346_reg_read(ADXL346_OUT_Y_REG_0);
    y1 = adxl346_reg_read(ADXL346_OUT_Y_REG_1);
    z0 = adxl346_reg_read(ADXL346_OUT_Z_REG_0);
    z1 = adxl346_reg_read(ADXL346_OUT_Z_REG_1);

	x = (int32)(((x1 << 24) >> 16)| x0) << ADXL346_DIGIT_TO_MG;
	y = (int32)(((y1 << 24) >> 16)| y0) << ADXL346_DIGIT_TO_MG;
	z = (int32)(((z1 << 24) >> 16)| z0) << ADXL346_DIGIT_TO_MG;
	
              	
    // convert to MOCOR coordinate
    // refer to msensor_drv.h and dedicated doc/datasheet

    switch(s_adxl346_misc_cfg.pin1)
    {
        case MSENSOR_PIN1_RIGHT_UP:
        {
            if(MSENSOR_LCD_FACE_SAME == s_adxl346_misc_cfg.lcd_face)
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
            if(MSENSOR_LCD_FACE_SAME == s_adxl346_misc_cfg.lcd_face)
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
            if(MSENSOR_LCD_FACE_SAME == s_adxl346_misc_cfg.lcd_face)
            {   
            }
            else
            {
                x = -x;
            }
            break;
        }
        case MSENSOR_PIN1_LEFT_UP:
        {
            if(MSENSOR_LCD_FACE_SAME == s_adxl346_misc_cfg.lcd_face)
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

    if( MSENSOR_LCD_FACE_SAME == s_adxl346_misc_cfg.lcd_face)
    {
        z = -z;
    }

      
	ADXL346_TRACE("MSENSOR: MOCOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d",x,y,z);
	              
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
LOCAL void ADXL346_TimerCallback(uint32 param)
{	    
    MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_ADXL346_timer, ADXL346_TimerCallback, TIMER_PERIOD_ADXL346);
	SCI_ActiveTimer(s_ADXL346_timer);
	return ;
}



/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Jackey.Ling
// Note:
/******************************************************************************/
LOCAL MSENSOR_MOTION_TYPE_E ADXL346_get_shake_sense(MSENSOR_MOTION_VECTOR_T* data_in)
{
    static int32 Xfst = 0;
    static int32 Xsnd = 0;
    static int32 Xdif = 0;
    static uint32 ShakeDelay = 0;
    static uint32 ShakeSenseStep = SHAKE_INIT;

    MSENSOR_MOTION_VECTOR_T data_value = *data_in;
    MSENSOR_MOTION_TYPE_E ShakeReport = MSENSOR_MOTION_TYPE_NONE;    
    int32 Xnew_value = data_value.x;

    switch(ShakeSenseStep)
    {
    case(SHAKE_FIRST_PEAK):             //Step 1 to track axes's peak value: find the first peak
      ShakeSenseStep = SHAKE_SENCOND_PEAK;
      if((ShakeDelay--)== 0)
      {
        ShakeSenseStep = SHAKE_INIT;
        return MSENSOR_MOTION_TYPE_NONE;
      }
                         //If the first value is positive and it's increasing, refresh the peak value
      if(Xdif > 0)
      {
        if(Xnew_value > Xfst)
        {
            Xfst = Xnew_value;
            Xsnd = Xnew_value;
            ShakeSenseStep = SHAKE_FIRST_PEAK;
        }
        else if( Xnew_value < 0)
        {
            Xsnd = Xnew_value;
        }
      }
                         //If the first value is negative and it's decreasing, refresh the peak value
      else if(Xdif<0)
      {
        if(Xnew_value<Xfst)
        {
            Xfst = Xnew_value;
            Xsnd = Xnew_value;
            ShakeSenseStep = SHAKE_FIRST_PEAK;
        }
        else if(Xnew_value > 0)
        {
            Xsnd = Xnew_value;
        }
      }           
    break;                            // If the first peak values found, enter step 2
    
    case(SHAKE_SENCOND_PEAK):                          // Step 2: find the second peak
      ShakeSenseStep = SHAKE_THIRD_JUDGE;
      if((ShakeDelay--)==0)
      {
        ShakeSenseStep = SHAKE_INIT;
        return MSENSOR_MOTION_TYPE_NONE;
      }
                            //If the first peak is positive, find the miminum value for second peak
      if((Xfst > s_ADXL346_sh_th)&&( Xnew_value<Xsnd ))
      {
        Xsnd = Xnew_value;
        ShakeSenseStep = SHAKE_SENCOND_PEAK;
      }
                            //If the first peak is negative, find the maximum value for second peak
      else if((Xfst < -s_ADXL346_sh_th)&&( Xnew_value>Xsnd ))
      {
        Xsnd = Xnew_value;
        ShakeSenseStep = SHAKE_SENCOND_PEAK;
      }
	  	  
      if(ShakeSenseStep == SHAKE_THIRD_JUDGE)      //If the second peak found, enter step 3: judge Shake direction and axis
      {
        
        ShakeSenseStep = SHAKE_WAIT;
        ShakeReport = MSENSOR_MOTION_TYPE_RIGHT;      //Assume Shake on axis X and positive direction
        ShakeDelay = SHAKEDELAY_INTERVAL;
		
        Xsnd = abs(Xfst - Xsnd); //The axis with maximum absolute value between the first and second peaks is the Shakeing axis
        		
        if(Xfst<0) ShakeReport = MSENSOR_MOTION_TYPE_LEFT;     //If the first peak is negative, Shake direction is negative
        
        if(Xsnd < SHAKE_ABSTH)
        {
          ShakeReport = MSENSOR_MOTION_TYPE_NONE;
          ShakeDelay = 0;
          ShakeSenseStep = SHAKE_INIT;         // Allow to restart shake detection immediately
        }       
      }
    break;
    
    case(SHAKE_WAIT):                          // Step 4: delay between Shakes
      if(!(ShakeDelay--)) ShakeSenseStep = SHAKE_INIT;    //Back to step 0 when delay finished
    break;
    
    default:
      Xfst = 0;
      Xsnd = 0;                                     
      Xdif = 0;                     // Check if any axis value beyond threshold
      
    if(abs(Xnew_value) > ADXL346_THRESHOLD_0_9G)     // If any axis g-value beyond threshold, start tracking
    {                           
        ShakeSenseStep = SHAKE_FIRST_PEAK;
        Xdif = Xfst = Xnew_value;
        ShakeDelay = SHAKEDELAY_TIMEOUT;
    }
    break;
  }  
  
  return ShakeReport;   
}


LOCAL MSENSOR_MOTION_VECTOR_T data_filter_average(MSENSOR_MOTION_VECTOR_T* data_in)
{   
    static MSENSOR_MOTION_VECTOR_T data_vector[DATA_VECTOR_LENGTH] = {0};  
    
    MSENSOR_MOTION_VECTOR_T data_sum_average = {0};
    
    static int32 vector_ptr = 0;
    int32 i =0;
    
    if(vector_ptr < DATA_VECTOR_LENGTH - 1)
    {      
       vector_ptr ++; 
    }
    else
    {      
      vector_ptr = 0;  
    }
    
    data_vector[vector_ptr] =  *data_in;        

    for(i = 0;i < DATA_VECTOR_LENGTH;i++ )
    {
        data_sum_average.x += data_vector[i].x;
        data_sum_average.y += data_vector[i].y;
        data_sum_average.z += data_vector[i].z;
    }
    
    data_sum_average.x = data_sum_average.x / DATA_VECTOR_LENGTH;
    data_sum_average.y = data_sum_average.y / DATA_VECTOR_LENGTH;
    data_sum_average.z = data_sum_average.z / DATA_VECTOR_LENGTH;
    
    return  data_sum_average;
}


LOCAL MSENSOR_MOTION_TYPE_E ADXL346_get_rotate_sense(MSENSOR_MOTION_VECTOR_T* data_in)
{ 

    static MSENSOR_MOTION_TYPE_E s_last_motion_type = MSENSOR_MOTION_TYPE_NONE;
    MSENSOR_MOTION_TYPE_E cur_motion_type = MSENSOR_MOTION_TYPE_NONE;
    MSENSOR_MOTION_VECTOR_T data_average = {0};  

    data_average = data_filter_average(data_in);
              
    if( data_average.x > s_ADXL346_or_th )
    {
        cur_motion_type = MSENSOR_MOTION_TYPE_LEFT_SIDE;
    }
    else if(data_average.x < -s_ADXL346_or_th)
    {
        cur_motion_type = MSENSOR_MOTION_TYPE_RIGHT_SIDE;
    }


    if( data_average.y > s_ADXL346_or_th)
    {
        cur_motion_type = MSENSOR_MOTION_TYPE_DOWN_SIDE;
    }
    else if(data_average.y < -s_ADXL346_or_th)
    {
        cur_motion_type = MSENSOR_MOTION_TYPE_UP_SIDE;
    }

    if(( abs(data_average.x) < ADXL346_THRESHOLD_0_5G )&&
       ( abs(data_average.y) < ADXL346_THRESHOLD_0_5G ))
      {
        if( data_average.z > s_ADXL346_or_th)
        {
            cur_motion_type = MSENSOR_MOTION_TYPE_BACKWARD_SIDE ;
        }
        else if(data_average.z < -s_ADXL346_or_th)
        {
            cur_motion_type = MSENSOR_MOTION_TYPE_FORWRAD_SIDE;
        }
      }  

    if(cur_motion_type != s_last_motion_type) 
    {
        s_last_motion_type = cur_motion_type;
        return cur_motion_type;
    }
    else
    {
       return MSENSOR_MOTION_TYPE_NONE; 
    }
}

LOCAL uint32 ADXL346_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr)
{
    MSENSOR_MOTION_TYPE_E motion_type_rotate = MSENSOR_MOTION_TYPE_NONE;
    MSENSOR_MOTION_TYPE_E motion_type_shake = MSENSOR_MOTION_TYPE_NONE;
    
	MSENSOR_MOTION_DATA_T data_val ={0};	
    ADXL346_GetVector(&data_val);   
    
    motion_type_shake = ADXL346_get_shake_sense(&data_val.u.vector);   
    if(MSENSOR_MOTION_TYPE_NONE != motion_type_shake)
    {
        *event_ptr = motion_type_shake;           
         ADXL346_TRACE("ADXL346_GetEvent shake: %d",*event_ptr);
    }
    else 
    {   
        motion_type_rotate = ADXL346_get_rotate_sense(&data_val.u.vector);
        if(MSENSOR_MOTION_TYPE_NONE != motion_type_rotate)
        {
            *event_ptr = motion_type_rotate;    
            ADXL346_TRACE("ADXL346_GetEvent rotate: %d",*event_ptr);
        }
        else
        {
            *event_ptr = MSENSOR_MOTION_TYPE_NONE;        
        }
    }
           
	return MSENSOR_SUCCESS;
}


/******************************************************************************/
// Description:   ADXL346_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 ADXL346_SetSensi(MSENSOR_SENSI_E sensi_val)
{
    switch(sensi_val)
    {
        case MSENSOR_SENSI_LOW:
        {
            s_ADXL346_sh_th = ADXL346_THRESHOLD_1_5G;
            s_ADXL346_or_th = ADXL346_THRESHOLD_0_8G;
            break;
        }
        case MSENSOR_SENSI_MID:
        {
            s_ADXL346_sh_th = ADXL346_THRESHOLD_1_2G;
            s_ADXL346_or_th = ADXL346_THRESHOLD_0_7G;
            break;
        }
        case MSENSOR_SENSI_HIGH:
        {
            s_ADXL346_sh_th = ADXL346_THRESHOLD_1_0G;
            s_ADXL346_or_th = ADXL346_THRESHOLD_0_6G;
            break;
        }
        default:
        {
            s_ADXL346_sh_th = ADXL346_THRESHOLD_1_2G;
            s_ADXL346_or_th = ADXL346_THRESHOLD_0_7G;
            break;
        }
    }    
        return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   ADXL346_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 ADXL346_ChangeTimer(uint32 timer_expire)
{
	SCI_ChangeTimer(s_ADXL346_timer, ADXL346_TimerCallback, timer_expire);
	SCI_ActiveTimer(s_ADXL346_timer);
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   ADXL346_GetCapability
// Note:          
/******************************************************************************/
LOCAL uint32 ADXL346_GetCapability(uint32 * Capability)
{
    *Capability = MSENSOR_MOTION_CAP_XY_AXIS | MSENSOR_MOTION_CAP_Z_AXIS | 
                  MSENSOR_MOTION_CAP_SHAKE   | MSENSOR_MOTION_CAP_DATA;
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   ADXL346_ioctl
// Note:          
/******************************************************************************/
LOCAL uint32 ADXL346_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
	uint32 ret = MSENSOR_SUCCESS;
	
    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
        {
			ret = ADXL346_GetVector((MSENSOR_MOTION_DATA_T *)data_ptr);
			break;
        }
        
        case MSENSOR_IOCTL_GET_EVENT:
        {
			ret = ADXL346_GetEvent((MSENSOR_MOTION_TYPE_E *)data_ptr);			
			break;
        }
        case MSENSOR_IOCTL_SET_SENSI:
        {
			ret = ADXL346_SetSensi(*(MSENSOR_SENSI_E *)data_ptr);
			break;
        }        
        case MSENSOR_IOCTL_SET_TIMER:
        {
			ret = ADXL346_ChangeTimer(*(uint32 *)data_ptr);
			break;
        }
        case MSENSOR_IOCTL_GET_CAP:
        {
            ret = ADXL346_GetCapability((uint32 *)data_ptr);
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


