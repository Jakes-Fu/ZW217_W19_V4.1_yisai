/******************************************************************************
 ** File Name:      msensor_lis3dh.c                                          *
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
#define LIS3DH_DEBUG
//#define SENSOR_INT_MODE
#ifdef LIS3DH_DEBUG
#define LIS3DH_TRACE			SCI_TRACE_LOW
#else
#define LIS3DH_TRACE(x)
#endif

#ifdef WIN32
#define __func__ 
#endif

// registers map
#define LIS3DH_STATUS_2_REG           0x27
#define LIS3DH_STATUS_WAI_REG           0x0f
#define     LIS3DH_STATUS_WAI_REG_ID        0x33 // lis3dh device id
#define LIS3DH_CTRL_REG1_RW             0X20
#define     LIS3DH_CTRL_REG1_PWD_DOWN       0x00
#define     LIS3DH_CTRL_REG1_PWD_ON         0x47 // 10Hz, xyz enable
//#define LIS3DH_CTRL_REG2_RW             0X21
//#define LIS3DH_CTRL_REG3_RW             0X22

#define LIS3DH_OUT_X_L_REG              0X28
#define LIS3DH_OUT_X_H_REG              0X29

#define LIS3DH_OUT_Y_L_REG              0X2A
#define LIS3DH_OUT_Y_H_REG              0X2B

#define LIS3DH_OUT_Z_L_REG              0X2C
#define LIS3DH_OUT_Z_H_REG              0X2D


// according to datasheet, 1 mg/digit on 0--2G scale, MOCOR's unit: mg
//#define LIS3DH_DIGIT_TO_MG             19

//#define LIS3DH_THRESHOLD_0_3G          300
#define LIS3DH_THRESHOLD_0_5G            500
// #define LIS3DH_THRESHOLD_0_6G         600
#define LIS3DH_THRESHOLD_0_7G            700
#define LIS3DH_THRESHOLD_0_8G            800
#define LIS3DH_THRESHOLD_0_8_5G          850
#define LIS3DH_THRESHOLD_0_9G            900
#define LIS3DH_THRESHOLD_1_0G            1000
#define LIS3DH_THRESHOLD_1_1G            1100
#define LIS3DH_THRESHOLD_1_2G            1200
#define LIS3DH_THRESHOLD_1_3G            1300
#define LIS3DH_THRESHOLD_1_5G            1500
#define LIS3DH_LOW_THRESHOLD             200

#define LIS3DH_SHAKE_DETECT_TIME         200
#define LIS3DH_DESHAKE_TIME              300

/**---------------------------------------------------------------------------*
 **                         DataStruct Definition                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Variables and function                            *
 **---------------------------------------------------------------------------*/ 
LOCAL uint32 LIS3DH_init(void);
LOCAL uint32 LIS3DH_open(void);
LOCAL uint32 LIS3DH_close(void);
LOCAL void LIS3DH_TimerCallback(uint32 param);
LOCAL uint32 LIS3DH_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);
LOCAL uint32 LIS3DH_SetSensi(MSENSOR_SENSI_E sensi_val);
 
PUBLIC MSENSOR_CFG_T g_lis3dh_msensor_info = 
{
    "ST LIS3DH",
	MSENSOR_TYPE_ACCELEROMETER,
    MSENSOR_STATUS_NOT_INIT,
	LIS3DH_init,
	LIS3DH_open,
	LIS3DH_close,
	LIS3DH_ioctl
};
LOCAL MSENSOR_MISC_CFG_T s_lis3dh_misc_cfg;
LOCAL int32 s_LIS3DH_IIC_Handler = -1;

#define TIMER_PERIOD_LIS3DH                  1000
LOCAL SCI_TIMER_PTR 		s_LIS3DH_timer;
// LOCAL SCI_SEMAPHORE_PTR    	s_LIS3DH_sm    = PNULL;
LOCAL int32                 s_LIS3DH_sh_th; // shake threshhold for sensitivity
LOCAL int32                 s_LIS3DH_or_th; // orientation threshhold for sensitivity
/*****************************************************************************/
//  Description:    read chip register content
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint32 LIS3DH_read(uint8* buffer, uint32 bytes)
{
	if((s_LIS3DH_IIC_Handler == -1) && 
		((s_LIS3DH_IIC_Handler = I2C_HAL_Open(&(s_lis3dh_misc_cfg.i2c_cfg))) == -1))
	{
		//LIS3DH_TRACE:"[REF_MSENSOR] %s : open IIC failed!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_123_112_2_18_0_34_20_1902,(uint8*)"s", __func__);
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}
#if 0
	if(I2C_HAL_Read(s_LIS3DH_IIC_Handler, buffer, buffer + 1, bytes) == 0)
		return MSENSOR_ERROR_IIC_READ_FAILED;
	//READ OK!
#else
{   uint32 i = 0;
    uint8  reg_addr = buffer[0];

    for(i = 0; i< bytes; i++)
    {
        I2C_HAL_Read(s_LIS3DH_IIC_Handler, &reg_addr, &buffer[i+1], 1);
        //LIS3DH_TRACE:"[REF_MSENSOR] %s : addr: 0x%02X = 0x%02X  "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_137_112_2_18_0_34_20_1903,(uint8*)"sdd", __func__, reg_addr, buffer[i+1]);
        reg_addr++; // reg address increase

    }

}
#endif
	return MSENSOR_SUCCESS;
}

LOCAL uint32 LIS3DH_read_xyz(uint16 *x_ptr, uint16 *y_ptr, uint16 *z_ptr)
{
    uint8 data_addr = LIS3DH_OUT_X_L_REG;
    uint8 data[6] = {0};
    
	if(I2C_HAL_Read(s_LIS3DH_IIC_Handler, &data_addr, data, 6) == 0)
		return MSENSOR_ERROR_IIC_READ_FAILED;

    //LIS3DH_TRACE:"[REF_MSENSOR] %s : data: 0x%X%X, 0x%X%X, 0x%X%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_156_112_2_18_0_34_20_1904,(uint8*)"sdddddd", __func__,data[1], data[0], data[3], data[2], data[5], data[4]);
	//READ OK!
	*x_ptr = (data[1] << 8) | data[0];
	*y_ptr = (data[3] << 8) | data[2];
	*z_ptr = (data[5] << 8) | data[3];

	return MSENSOR_SUCCESS;
}
/*****************************************************************************/
//  Description:    write configuration data to chip registers
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint32 LIS3DH_write(uint8* cmd)
{
	if((s_LIS3DH_IIC_Handler == -1) && 
		((s_LIS3DH_IIC_Handler = I2C_HAL_Open(&(s_lis3dh_misc_cfg.i2c_cfg))) == -1))
	{
		//LIS3DH_TRACE:"[REF_MSENSOR] %s : open IIC failed!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_172_112_2_18_0_34_20_1905,(uint8*)"s", __func__);
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}

	if(I2C_HAL_Write(s_LIS3DH_IIC_Handler, cmd, cmd + 1, 1) == 0)
		return MSENSOR_ERROR_IIC_WRITE_FAILED;
	//WRITE OK!
	return MSENSOR_SUCCESS;	
}

/*****************************************************************************/
//  Description:    Init g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 LIS3DH_init(void)
{
	uint8   reg[2] = {0};
	
    MSensor_GetMiscCfg(&g_lis3dh_msensor_info, &s_lis3dh_misc_cfg);
    LIS3DH_SetSensi(MSENSOR_SENSI_MID);

    reg[0] = LIS3DH_STATUS_WAI_REG;
    LIS3DH_read(reg,1);
    //LIS3DH_TRACE:"[REF_MSENSOR] %s : id = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_194_112_2_18_0_34_20_1906,(uint8*)"sd",__func__, reg[1]);
    if(LIS3DH_STATUS_WAI_REG_ID != reg[1])
    {
        //return MSENSOR_ERROR_NOT_SUPPORT; //temp disable in debug
    }

	//POWER DOWN SENSOR
	reg[0] = LIS3DH_CTRL_REG1_RW;
	reg[1] = LIS3DH_CTRL_REG1_PWD_DOWN;
	LIS3DH_write(reg);

	I2C_HAL_Close(s_LIS3DH_IIC_Handler);
	s_LIS3DH_IIC_Handler = -1;
	
	return MSENSOR_SUCCESS;
}
LOCAL uint32 _LIS3DH_dumpreg(void)
{
	uint8   reg[0x4F] = {0};
	uint8   count     = 0x3D;
    int32   i;
    
	LIS3DH_read(reg, count);
	for(i=0; i< count; i += 4)
	{
        //LIS3DH_TRACE:"[REF_MSENSOR] LIS3DH 0x%X = 0x%2X, 0x%2X, 0x%2X, 0x%2X,"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_220_112_2_18_0_34_20_1907,(uint8*)"ddddd",i, reg[i+1], reg[i+2], reg[i+3], reg[i+4] );
        
    }
}
/*****************************************************************************/
//  Description:    open g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 LIS3DH_open(void)
{
	uint8   reg[2] = {0};
    GPIO_CFG_INFO_T_PTR gpio_int_cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
    
	
    reg[0] = LIS3DH_STATUS_WAI_REG;
    LIS3DH_read(reg, 1);
    //LIS3DH_TRACE:"[REF_MSENSOR] %s 1: id = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_235_112_2_18_0_34_20_1908,(uint8*)"sd", __func__, reg[1]);

	reg[0] = LIS3DH_CTRL_REG1_RW;
	reg[1] = LIS3DH_CTRL_REG1_PWD_ON;
	LIS3DH_write(reg);
	
    reg[0] = LIS3DH_CTRL_REG1_RW;
    LIS3DH_read(reg, 1);
    //LIS3DH_TRACE:"[REF_MSENSOR] %s 2: CTRL1 = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_243_112_2_18_0_34_20_1909,(uint8*)"sd", __func__, reg[1]);
    
    _LIS3DH_dumpreg();

    reg[0] = LIS3DH_STATUS_WAI_REG;
    LIS3DH_read(reg, 1);
    //LIS3DH_TRACE:"[REF_MSENSOR] %s 3: id = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_249_112_2_18_0_34_20_1910,(uint8*)"sd", __func__, reg[1]);
    
	// if no gpio int defination, then use timer polling
    if(!gpio_int_cfg_ptr)
    {
    	if(s_LIS3DH_timer == NULL)
    	{
            s_LIS3DH_timer = SCI_CreateTimer("LIS3DH_Timer", LIS3DH_TimerCallback,
    			MSENSOR_TYPE_ACCELEROMETER, TIMER_PERIOD_LIS3DH, SCI_AUTO_ACTIVATE);
    	}
    	else
    	{
    		SCI_ChangeTimer(s_LIS3DH_timer, LIS3DH_TimerCallback, TIMER_PERIOD_LIS3DH);
    		SCI_ActiveTimer(s_LIS3DH_timer);	
    	}
    }
	return MSENSOR_SUCCESS;
}	
/*****************************************************************************/
//  Description:    close g-sensor chip
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 LIS3DH_close(void)
{
	uint8   reg[2] = {0};

	//Timer and semaphore need NOT be deleted. Just to deactive them.
	if(s_LIS3DH_timer != NULL)
	{
		SCI_DeactiveTimer(s_LIS3DH_timer);
	}

	reg[0] = LIS3DH_CTRL_REG1_RW;
	reg[1] = LIS3DH_CTRL_REG1_PWD_DOWN; 
	LIS3DH_write(reg);

	if(s_LIS3DH_IIC_Handler != -1)
	{
		I2C_HAL_Close(s_LIS3DH_IIC_Handler);
		s_LIS3DH_IIC_Handler = -1;
	}

	return MSENSOR_SUCCESS;
}

/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 LIS3DH_GetVector(MSENSOR_MOTION_DATA_T * data_ptr)
{   uint8 rawdata[7] = {0};
    uint8 rawdata1[7] = {0};
	int32  x;
	int32  y;
	int32  z;
    int32  temp;
#if 0
    rawdata[0]= LIS3DH_OUT_X_L_REG;
    LIS3DH_read(rawdata, 6);
    x = (rawdata[2] << 8) | rawdata[1];
	y = (rawdata[4] << 8) | rawdata[3];
	z = (rawdata[6] << 8) | rawdata[5];
#endif

    rawdata[0] = LIS3DH_OUT_X_L_REG;
    LIS3DH_read(rawdata, 1);
    
    rawdata1[0] = LIS3DH_OUT_X_H_REG;
    LIS3DH_read(rawdata1, 1);
    x = ((rawdata1[1] << 8) | rawdata[1])  >> 4;

    rawdata[0] = LIS3DH_OUT_Y_L_REG;
    LIS3DH_read(rawdata, 1);
    
    rawdata1[0] = LIS3DH_OUT_Y_H_REG;
    LIS3DH_read(rawdata1, 1);
    y = ((rawdata1[1] << 8) | rawdata[1])  >> 4;


    rawdata[0] = LIS3DH_OUT_Z_L_REG;
    LIS3DH_read(rawdata, 1);
    
    rawdata1[0] = LIS3DH_OUT_Z_H_REG;
    LIS3DH_read(rawdata1, 1);
    z = ((rawdata1[1] << 8) | rawdata[1])  >> 4;


    rawdata[0] = LIS3DH_STATUS_2_REG;
    LIS3DH_read(rawdata, 1);
    

    //LIS3DH_TRACE:"MSENSOR: placement:  lcd_face = %d, pin1 = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_340_112_2_18_0_34_20_1911,(uint8*)"dd",s_lis3dh_misc_cfg.lcd_face, s_lis3dh_misc_cfg.pin1);
	//LIS3DH_TRACE:"MSENSOR: SENSOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_342_112_2_18_0_34_20_1912,(uint8*)"dddd", x, y, z, rawdata[1]);
	
    // convert to MOCOR coordinate
    // refer to msensor_drv.h and dedicated doc/datasheet
    switch(s_lis3dh_misc_cfg.pin1)
    {
        case MSENSOR_PIN1_RIGHT_UP:
        {
            if(MSENSOR_LCD_FACE_SAME == s_lis3dh_misc_cfg.lcd_face)
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
            if(MSENSOR_LCD_FACE_SAME == s_lis3dh_misc_cfg.lcd_face)
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
            if(MSENSOR_LCD_FACE_SAME == s_lis3dh_misc_cfg.lcd_face)
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
            if(MSENSOR_LCD_FACE_SAME == s_lis3dh_misc_cfg.lcd_face)
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

    if( MSENSOR_LCD_FACE_SAME == s_lis3dh_misc_cfg.lcd_face)
    {
        z = -z;
    }
	//LIS3DH_TRACE:"MSENSOR: MOCOR_COOR: axis_x = %d, axis_y = %d, axis_z = %d,"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_414_112_2_18_0_34_20_1913,(uint8*)"ddd",x, y, z);
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
LOCAL void LIS3DH_TimerCallback(uint32 param)
{
    MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_LIS3DH_timer, LIS3DH_TimerCallback, TIMER_PERIOD_LIS3DH);
	SCI_ActiveTimer(s_LIS3DH_timer);
	return ;
}

/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 LIS3DH_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr)
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
    LIS3DH_GetVector(&data_val);
    
    x = data_val.u.vector.x;
    y = data_val.u.vector.y;
    z = data_val.u.vector.z;

    //LIS3DH_SHAKE_DETECT_TIME ms for detect shaking
    count_tick = SCI_GetTickCount();
#ifdef SENSOR_INT_MODE
    if(count_tick - count_tick_old > LIS3DH_SHAKE_DETECT_TIME)
	{
	  x_old = 0;
	}
	//only x for detect shaking event
    if((x > LIS3DH_LOW_THRESHOLD) && (x_old < -s_LIS3DH_sh_th))
    {
        motion_type = MSENSOR_MOTION_TYPE_RIGHT;
        x_old = 0;
    }
    else if((x < -LIS3DH_LOW_THRESHOLD) && (x_old > s_LIS3DH_sh_th))
    {
        motion_type = MSENSOR_MOTION_TYPE_LEFT;
        x_old = 0;
    }

    //when x > s_LIS3DH_sh_th,start timer
    else if(abs(x) > s_LIS3DH_sh_th)
    {
    	count_tick_old = count_tick;
  	    x_old = x;
    }
    //the time(abs(x_old) > s_LIS3DH_sh_th) for detect shaking,ignore orientation event
    else if(abs(x_old) > s_LIS3DH_sh_th)
        NULL;
#else
    if(x > s_LIS3DH_sh_th)    
    {
        motion_type = MSENSOR_MOTION_TYPE_LEFT;
    }
    else if(x < -s_LIS3DH_sh_th)
    {
        motion_type = MSENSOR_MOTION_TYPE_RIGHT;
    }
#endif    
    else
    {
    	// quiescent status. 
    	// Forward or Backward side, 
    	if((x < LIS3DH_THRESHOLD_0_5G && x > -LIS3DH_THRESHOLD_0_5G) &&
    	   (y < LIS3DH_THRESHOLD_0_5G && y > -LIS3DH_THRESHOLD_0_5G))
    	{
    		if(z < -LIS3DH_THRESHOLD_0_9G)
    			motion_type = MSENSOR_MOTION_TYPE_FORWRAD_SIDE;
    		else if(z > LIS3DH_THRESHOLD_0_9G)
    			motion_type = MSENSOR_MOTION_TYPE_BACKWARD_SIDE;
    	}
    	// left or right side
    	if((z < LIS3DH_THRESHOLD_0_8G && z > -LIS3DH_THRESHOLD_0_8G) &&
    	   (y < LIS3DH_THRESHOLD_0_8G && y > -LIS3DH_THRESHOLD_0_8G))
    	{
    		if(x < -s_LIS3DH_or_th)
    			motion_type = MSENSOR_MOTION_TYPE_RIGHT_SIDE;
    		else if(x > s_LIS3DH_or_th)
    			motion_type = MSENSOR_MOTION_TYPE_LEFT_SIDE;
    	}
       // up or down
    	if((z < LIS3DH_THRESHOLD_0_8G && z > -LIS3DH_THRESHOLD_0_8G) &&
    	   (x < LIS3DH_THRESHOLD_0_8G && x > -LIS3DH_THRESHOLD_0_8G))
    	{
    		if(y < -s_LIS3DH_or_th)
    			motion_type = MSENSOR_MOTION_TYPE_UP_SIDE;
    		else if(y > s_LIS3DH_or_th)
    			motion_type = MSENSOR_MOTION_TYPE_DOWN_SIDE;
    	}
    }
    
    //LIS3DH_TRACE:"MSENSOR::"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_529_112_2_18_0_34_21_1914,(uint8*)"dd",s_last_motion_type, motion_type);
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
    
    //only one shaking event within LIS3DH_DESHAKE_TIME ms
    //avoid double shake
	if(motion_type == MSENSOR_MOTION_TYPE_LEFT || motion_type == MSENSOR_MOTION_TYPE_RIGHT)
	{
	//    count_tick = SCI_GetTickCount();
	    
    	if(count_tick - count_tick_deshake < LIS3DH_DESHAKE_TIME)
    	{
    	    *event_ptr = MSENSOR_EVENT_NONE;  
    	}
    	count_tick_deshake = count_tick;   	
    }
	return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   LIS3DH_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 LIS3DH_SetSensi(MSENSOR_SENSI_E sensi_val)
{
    switch(sensi_val)
    {
        case MSENSOR_SENSI_LOW:
        {
            s_LIS3DH_sh_th = LIS3DH_THRESHOLD_1_5G;
            s_LIS3DH_or_th = LIS3DH_THRESHOLD_1_0G;
            break;
        }
        case MSENSOR_SENSI_MID:
        {
            s_LIS3DH_sh_th = LIS3DH_THRESHOLD_1_2G;
            s_LIS3DH_or_th = LIS3DH_THRESHOLD_0_9G;
            break;
        }
        case MSENSOR_SENSI_HIGH:
        {
            s_LIS3DH_sh_th = LIS3DH_THRESHOLD_1_0G;
            s_LIS3DH_or_th = LIS3DH_THRESHOLD_0_7G;
            break;
        }
        default:
        {
            s_LIS3DH_sh_th = LIS3DH_THRESHOLD_1_1G;
            s_LIS3DH_or_th = LIS3DH_THRESHOLD_0_8G;
            break;
        }
    }    
    
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   LIS3DH_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 LIS3DH_ChangeTimer(uint32 timer_expire)
{
	SCI_ChangeTimer(s_LIS3DH_timer, LIS3DH_TimerCallback, timer_expire);
	SCI_ActiveTimer(s_LIS3DH_timer);
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   LIS3DH_GetCapability
// Note:          
/******************************************************************************/
LOCAL uint32 LIS3DH_GetCapability(uint32 * Capability)
{
    *Capability = MSENSOR_MOTION_CAP_XY_AXIS | MSENSOR_MOTION_CAP_Z_AXIS | 
                  MSENSOR_MOTION_CAP_SHAKE   | MSENSOR_MOTION_CAP_DATA;
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   LIS3DH_ioctl
// Note:          
/******************************************************************************/
LOCAL uint32 LIS3DH_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
	uint32 ret = MSENSOR_SUCCESS;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
    //LIS3DH_TRACE:"[REF_MSENSOR] %s : cmd = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_LIS3DH_613_112_2_18_0_34_21_1915,(uint8*)"sd", __func__,cmd);

    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
        {
			ret = LIS3DH_GetVector((MSENSOR_MOTION_DATA_T *)data_ptr);
			break;
        }
        
        case MSENSOR_IOCTL_GET_EVENT:
        {
            if(cfg_ptr)
            {
                GPIO_DisableIntCtl(cfg_ptr->gpio_num);
            }
			ret = LIS3DH_GetEvent((MSENSOR_MOTION_TYPE_E *)data_ptr);
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
			ret = LIS3DH_SetSensi(*(MSENSOR_SENSI_E *)data_ptr);
			break;
        }        
        case MSENSOR_IOCTL_SET_TIMER:
        {
			ret = LIS3DH_ChangeTimer(*(uint32 *)data_ptr);
			break;
        }
        case MSENSOR_IOCTL_GET_CAP:
        {
            ret = LIS3DH_GetCapability((uint32 *)data_ptr);
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


