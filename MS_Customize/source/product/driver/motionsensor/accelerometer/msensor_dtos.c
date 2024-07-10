/******************************************************************************
 ** File Name:      mems_dtos.c                                               *
 ** Author:         Haifeng.Yang                                              *
 ** DATE:           2010.09.02                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2010.09.02   Haifeng.Yang          Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "msensor_drv.h"
#include "msensor_cfg.h"
#include "gpio_drvapi.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define MEMS_DTOS_DEBUG
#ifdef MEMS_DTOS_DEBUG
#define MEMS_DTOS_TRACE			SCI_TRACE_LOW
#else
#define MEMS_DTOS_TRACE(x)
#endif

#ifdef WIN32
#define __func__ 
#endif

//I2C ADDRESS
#define DTOS_I2C_WRITE_ADDR         0x2a
#define DTOS_PD_SHM_MASK            (0xC0)
#define DTOS_ORC_LOW                (0X0 << 0) //orientation count
#define DTOS_SHC_LOW                (0X0 << 2) //shake count
#define DTOS_SHTH_LOW               (0X0 << 4) //shake threshold

#define DTOS_ORC_MID                (0X1 << 0) //orientation count
#define DTOS_SHC_MID                (0X1 << 2) //shake count
#define DTOS_SHTH_MID               (0X1 << 4)	//shake threshold

#define DTOS_ORC_HIGH               (0X2 << 0) //orientation count
#define DTOS_SHC_HIGH               (0X2 << 2) //shake count
#define DTOS_SHTH_HIGH              (0X2 << 4) //shake threshold

#define DTOS_SHM                    (0x0 << 6) //shake mode bit
#define DTOS_PD                     (0X1 << 7) //power down
#define DTOS_STAT_SH                (0x60)     // SH bits
#define DTOS_STAT_OR                (0x3)      // OR bits
// according to datasheet, 15 ~~ 21 mg/digit, MOCOR's unit: mg
#define DTOS_DIGIT_TO_MG_SHIFT      4

/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/
LOCAL uint32 dtos_init(void);
LOCAL uint32 dtos_open(void);
LOCAL uint32 dtos_close(void);
LOCAL void dtos_TimerCallback(uint32 param);
LOCAL uint32 dtos_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr);
LOCAL uint32 dtos_ReadData(uint8 * buffer, uint32 bytes);
/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/
PUBLIC MSENSOR_CFG_T g_dtos_msensor_info = 
{
    "mems dtos",
	MSENSOR_TYPE_ACCELEROMETER,
    MSENSOR_STATUS_NOT_INIT,
	dtos_init,
	dtos_open,
	dtos_close,
	dtos_ioctl
};

LOCAL MSENSOR_MISC_CFG_T s_dtos_misc_cfg = {0};
LOCAL int32 s_dtos_IIC_Handler = -1;

#define TIMER_PERIOD_DTOS              200
LOCAL SCI_TIMER_PTR 	s_dtos_timer  = PNULL;
// LOCAL SCI_SEMAPHORE_PTR s_dtos_sm     = NULL;
/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/

/******************************************************************************/
// Description:   Init dtos chip ---- power down chip!
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 dtos_init(void)
{
    MSensor_GetMiscCfg(&g_dtos_msensor_info, &s_dtos_misc_cfg);
	if((s_dtos_IIC_Handler == -1) && 
	  ((s_dtos_IIC_Handler = I2C_HAL_Open(&(s_dtos_misc_cfg.i2c_cfg))) == -1))
	{
		//MEMS_DTOS_TRACE:"[DRV_MSENSOR] open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_110_112_2_18_0_34_17_1883,(uint8*)"");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}

	dtos_close();	
    return SCI_SUCCESS;
}

/******************************************************************************/
// Description:   dtos_open ---- OPEN I2C and power on chip
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 dtos_open(void)
{
	uint8 iic_buff[2];
	uint8 reg = 0;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
	if((s_dtos_IIC_Handler == -1) && 
		((s_dtos_IIC_Handler = I2C_HAL_Open(&(s_dtos_misc_cfg.i2c_cfg))) == -1))
	{
		//MEMS_DTOS_TRACE:"[DRV_MSENSOR] open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_130_112_2_18_0_34_17_1884,(uint8*)"");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}

	iic_buff[0] = 4;	//DETECTION Register
	iic_buff[1] = DTOS_SHM | DTOS_SHTH_MID | DTOS_SHC_MID | DTOS_ORC_MID;
	if(I2C_HAL_Write(s_dtos_IIC_Handler, iic_buff, iic_buff + 1, 1) == 0)
		return MSENSOR_ERROR_IIC_WRITE_FAILED;	
		
	//guobin.zhu cr105782.read status register clear int
    reg = 0x2;  //status register
    dtos_ReadData(&reg, 1);	
	
    //if cfg_ptr is PNULL,use timer mode.
    if(!cfg_ptr)
    {   
    	if(s_dtos_timer == NULL)
    	{
            s_dtos_timer = SCI_CreateTimer("dtos_Timer", dtos_TimerCallback,
    			MSENSOR_TYPE_ACCELEROMETER, TIMER_PERIOD_DTOS, SCI_AUTO_ACTIVATE);
    	}
    	else
    	{
    		SCI_ChangeTimer(s_dtos_timer, dtos_TimerCallback, TIMER_PERIOD_DTOS);
    		SCI_ActiveTimer(s_dtos_timer);	
    	}
    }

	// msensor_drv.c has a semaphore already
    #if 0
	if(s_dtos_sm == NULL)
	{
		s_dtos_sm = SCI_CreateSemaphore ("dtos SEMAPHORE", 1);
		SCI_ASSERT(s_dtos_sm != NULL);
	}
	#endif
    return SCI_SUCCESS;
}


/******************************************************************************/
// Description:   dtos_close ---- Close IIC and power down chip
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 dtos_close(void)
{
	uint8 iic_buff[2];
    
	iic_buff[0] = 4;	//DETECTION Register
	iic_buff[1] = DTOS_PD;

    //power down mode
	if(I2C_HAL_Write(s_dtos_IIC_Handler, iic_buff, iic_buff + 1, 1) == 0)
		return MSENSOR_ERROR_IIC_WRITE_FAILED;	
		
	if(s_dtos_IIC_Handler != -1)
	{
		I2C_HAL_Close(s_dtos_IIC_Handler);
		s_dtos_IIC_Handler = -1;
	}

	//Timer and semaphore need NOT be deleted. Just to deactive them.
	if(s_dtos_timer != NULL)
	{
		SCI_DeactiveTimer(s_dtos_timer);
	}
	
    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: 
// Note:
/******************************************************************************/
LOCAL uint32 dtos_ReadData(uint8 * buffer, uint32 bytes)
{   uint32 ret = 0;
  	// msensor_drv.c has a semaphore already
	// SCI_GetSemaphore(s_dtos_sm, SCI_WAIT_FOREVER);
	ret = I2C_HAL_Read(s_dtos_IIC_Handler, buffer, buffer + 1, bytes);
	// SCI_PutSemaphore(s_dtos_sm);
	return ret;
}


/******************************************************************************/
// Description:   dtos_getevent
// Dependence: 
// Author:        Haifeng.Yang
// Note:	STATUS BIT MAP
//        BIT7   BIT6   BIT5   BIT4   BIT3   BIT2   BIT1  BIT0
//        INT    SH[1]  SH[0]  TILT   ORI[1] ORI[0] OR[1] OR[0]
//OR[1:0] = 
//00 - device is rotated 90 degrees counterclockwise; 
//01 - device is vertical in upright orientation; 
//10 - device is rotated 90 degrees clockwise; 
//11 - device is vertical in inverted orientation. 
//The bits OR[1:0] are indicative of "long-term" orientation.

//ORI[1:0] is the instantaneous device orientation. It follows the same scheme 
//as OR[1:0], except that it is updated every time a valid orientation measurement
//is made, not subject to the same low-pass filtering as OR[1:0].

//TILT is an indication of whether there is enough acceleration signal strength 
//to make a valid orientation measurement. If TILT = 0, the orientation measurement 
//is valid, if TILT = 1, then the orientation measurement is invalid. TILT is 
//updated every measurement cycle.

//SH[1:0] indicate whether a shake event has taken place, and if so, its direction.
//Shake can only be detected in a direction perpendicular to the vertical orientation
//of the device. When an acceleration perpendicular to the device orientation (ax for
//OR[1:0] = 01 or 11; ay for OR[1:0] = 00 or 10) is sensed that has a magnitude
//greater than the value set by bits SHTH[1:0] in the DETECTION register, then 
//shake detection begins. For a shake event to be written to SH[1:0], the 
//perpendicular acceleration must again exceed the magnitude set by SHTH[1:0] 
//but with the opposite sign (if bit SHM = 0 in the DETECTION register), or
//just reverse its sign (if bit SHM = 1 in the DETECTION register). The above 
//mentioned second acceleration events must occur within a certain amount of time,
//set by SHC[1:0] in the DETECTION register, of the original breaking of the threshold.
/******************************************************************************/
LOCAL uint32 dtos_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr)
{
    const MSENSOR_MOTION_TYPE_E shake_result[] = 
	{                           			//SH[1]  SH[0]  
		MSENSOR_MOTION_TYPE_NONE,           // 0       0
		MSENSOR_MOTION_TYPE_LEFT,           // 0       1
		MSENSOR_MOTION_TYPE_RIGHT,          // 1       0
		MSENSOR_MOTION_TYPE_NONE,           // 1       1
	};
    // MSENSOR_LCD_FACE_SAME + MSENSOR_PIN1_RIGHT_UP = index
	const MSENSOR_MOTION_TYPE_E lcd_same_orie_result[] = 
	{                           			// OR[1] OR[0]
		MSENSOR_MOTION_TYPE_UP_SIDE,        //   0      0   
		MSENSOR_MOTION_TYPE_LEFT_SIDE,      //   0      1
		MSENSOR_MOTION_TYPE_DOWN_SIDE,      //   1      0
		MSENSOR_MOTION_TYPE_RIGHT_SIDE,     //   1      1
	};
    // MSENSOR_LCD_FACE_REVERSE + MSENSOR_PIN1_RIGHT_UP = index
	const MSENSOR_MOTION_TYPE_E lcd_reve_orie_result[] = 
	{                           			// OR[1] OR[0]
		MSENSOR_MOTION_TYPE_UP_SIDE,        //   0      0   
		MSENSOR_MOTION_TYPE_RIGHT_SIDE,     //   0      1
		MSENSOR_MOTION_TYPE_DOWN_SIDE,      //   1      0
		MSENSOR_MOTION_TYPE_LEFT_SIDE,      //   1      1
	};
	uint8 iic_buff[4] = {0};
	uint8 x=0, y=0, status=0, sh_value=0, or_value=0, or_index=0;
    MSENSOR_MOTION_TYPE_E event_result = MSENSOR_MOTION_TYPE_NONE;
	static MSENSOR_MOTION_TYPE_E s_last_event_result = MSENSOR_MOTION_TYPE_NONE;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
    
	iic_buff[0] = 0;
	if( 3 != dtos_ReadData(iic_buff, 3))
	{
		return MSENSOR_ERROR_IIC_READ_FAILED;	
	}

	x = iic_buff[1];
	y = iic_buff[2];
	status = iic_buff[3];
	sh_value = (status & DTOS_STAT_SH) >> 5;
	or_value = (status & DTOS_STAT_OR);
    //MEMS_DTOS_TRACE:"[DRV_MSENSOR] x = %d(%d), y = %d(%d), status = 0x%X, "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_288_112_2_18_0_34_17_1885,(uint8*)"ddddddd", x,(int8)x, y, (int8)y, status, sh_value, or_value);

	if(status & BIT_4)		//TLTB = 1, invalid measurement!
	{
	    *event_ptr = MSENSOR_EVENT_NONE;
		return MSENSOR_SUCCESS;
	}
	
    // "shake" first
    event_result = shake_result[sh_value];
    // if no "shake" then orientation
	if( event_result == MSENSOR_MOTION_TYPE_NONE)
	{   // or_index is 0 ~~ DTOS_STAT_OR
        or_index = ((uint8)((int8)or_value - (int8)s_dtos_misc_cfg.pin1)) 
                    & DTOS_STAT_OR; 
        if(MSENSOR_LCD_FACE_SAME == s_dtos_misc_cfg.lcd_face)
        {
            event_result = lcd_same_orie_result[or_index];
        }
        else
        {   
            event_result = lcd_reve_orie_result[or_index];
        }
    }

    if(cfg_ptr || s_last_event_result != event_result)
    {   // always report at INT mode or New Event
     	*event_ptr  = event_result;      
    }
	else
	{	// do NOT report the same event
        *event_ptr = MSENSOR_EVENT_NONE;  
	}

    //MEMS_DTOS_TRACE:"[DRV_MSENSOR] or_index=%d, "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_324_112_2_18_0_34_18_1886,(uint8*)"ddd", or_index, s_last_event_result, event_result);
					
    s_last_event_result = event_result;
	
	return MSENSOR_SUCCESS;   
}/*lint !e533*/

/******************************************************************************/
// Description:   dtos_getvector
// Dependence: 
// Author:        Haifeng.Yang
// Note:          See dtos_getevent notes
/******************************************************************************/
LOCAL uint32 dtos_GetVector(MSENSOR_MOTION_DATA_T * data_ptr)
{
	uint8 iic_buff[4] = {0};
	int32 x = 0;
	int32 y = 0;
	int32 temp = 0;
	uint8 status = 0;
	
	iic_buff[0] = 0;
	if( 3 != dtos_ReadData(iic_buff, 3))
	{
		return MSENSOR_ERROR_IIC_READ_FAILED;	
	}
	// conver to MOCOR unit: mg
	x = (int8)iic_buff[1] << DTOS_DIGIT_TO_MG_SHIFT;
	y = (int8)iic_buff[2] << DTOS_DIGIT_TO_MG_SHIFT;
	status = iic_buff[3];
    //MEMS_DTOS_TRACE:"[DRV_MSENSOR] x = %d, y = %d, status= 0x%xrn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_353_112_2_18_0_34_18_1887,(uint8*)"ddd",x, y, status);

	if(status & BIT_4)		//TLTB = 1, invalid measurement!
	{
		return MSENSOR_ERROR_INVALID_DATA;
	}
    //MEMS_DTOS_TRACE:"[DRV_MSENSOR] lcd_face = %d, pin1 = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_360_112_2_18_0_34_18_1888,(uint8*)"dd",s_dtos_misc_cfg.lcd_face, s_dtos_misc_cfg.pin1);
	
    // convert to MOCOR coordinate
    // refer to msensor_drv.h and dedicated doc/datasheet
    switch(s_dtos_misc_cfg.pin1)
    {
        case MSENSOR_PIN1_RIGHT_UP:
        {
            if(MSENSOR_LCD_FACE_SAME == s_dtos_misc_cfg.lcd_face)
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
        case MSENSOR_PIN1_RIGHT_DOWN:
        {
            if(MSENSOR_LCD_FACE_SAME == s_dtos_misc_cfg.lcd_face)
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
        case MSENSOR_PIN1_LEFT_DONW:
        {
            if(MSENSOR_LCD_FACE_SAME == s_dtos_misc_cfg.lcd_face)
            {
                temp = x;
                x    = -y;
                y    = temp;
            }
            else
            {
                temp = x;
                x    = -y;
                y    = temp;
            }
            break;
        }
        case MSENSOR_PIN1_LEFT_UP:
        {
            if(MSENSOR_LCD_FACE_SAME == s_dtos_misc_cfg.lcd_face)
            {   
            }
            else
            {
                x    = -x;
            }
            break;
        }
        default:
            break;
    }    

	//MEMS_DTOS_TRACE:"[DRV_MSENSOR] MOCOR_COOR: x = %d, y = %d, "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_428_112_2_18_0_34_18_1889,(uint8*)"dd",x, y);
	data_ptr->u.vector.x = x;
	data_ptr->u.vector.y = y;
	data_ptr->u.vector.z = 0;	// DTOS is 2-axis accmeter only, z is always zero.
	data_ptr->type     = MSENSOR_TYPE_ACCELEROMETER;
    return MSENSOR_SUCCESS;
}

/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void dtos_TimerCallback(uint32 param)
{
    MSensor_TimerHandler(param);
	SCI_ChangeTimer(s_dtos_timer, dtos_TimerCallback, TIMER_PERIOD_DTOS);
	SCI_ActiveTimer(s_dtos_timer);
	return ;
}
/******************************************************************************/
// Description:   dtos_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 dtos_SetSensi(MSENSOR_SENSI_E sensi_val)
{
	uint8 iic_buff[2], det_val;
	
	if((s_dtos_IIC_Handler == -1) && 
		((s_dtos_IIC_Handler = I2C_HAL_Open(&(s_dtos_misc_cfg.i2c_cfg))) == -1))
	{
		//MEMS_DTOS_TRACE:"[DRV_MSENSOR] open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_456_112_2_18_0_34_18_1890,(uint8*)"");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}
    // read DETECTION Register
	iic_buff[0] = 4;	
	if( 1 != dtos_ReadData(iic_buff, 1))
	{
		return MSENSOR_ERROR_IIC_READ_FAILED;	
	}
    det_val = iic_buff[1];
    det_val &= DTOS_PD_SHM_MASK; 
    
    // write DETECTION Register
    //MEMS_DTOS_TRACE:"[DRV_MSENSOR]DETECTION = 0x%X, sensi_val = %d rn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_470_112_2_18_0_34_18_1891,(uint8*)"dd",iic_buff[1], sensi_val);
    switch(sensi_val)
    {
        case MSENSOR_SENSI_LOW:
        {
            det_val |= DTOS_SHTH_LOW | DTOS_SHC_LOW | DTOS_ORC_LOW;
            break;
        }
        case MSENSOR_SENSI_MID:
        {
            det_val |= DTOS_SHTH_MID | DTOS_SHC_MID | DTOS_ORC_MID;
            break;
        }
        case MSENSOR_SENSI_HIGH:
        {
            det_val |= DTOS_SHTH_HIGH | DTOS_SHC_HIGH | DTOS_ORC_HIGH;
            break;
        }
        default:
        {
            det_val |= DTOS_SHTH_MID | DTOS_SHC_MID | DTOS_ORC_MID;
            break;
        }
    }    
    
	iic_buff[0] = 4;	//DETECTION Register
	iic_buff[1] = det_val;
	if(I2C_HAL_Write(s_dtos_IIC_Handler, iic_buff, iic_buff + 1, 1) == 0)
		return MSENSOR_ERROR_IIC_WRITE_FAILED;	
		
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   lis35DE_SetSensi
// Note:          
/******************************************************************************/
LOCAL uint32 dtos_ChangeTimer(uint32 timer_expire)
{
	SCI_ChangeTimer(s_dtos_timer, dtos_TimerCallback, timer_expire);
	SCI_ActiveTimer(s_dtos_timer);
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   dtos_GetCapability
// Note:          
/******************************************************************************/
LOCAL uint32 dtos_GetCapability(uint32 * Capability)
{
    *Capability = MSENSOR_MOTION_CAP_XY_AXIS | MSENSOR_MOTION_CAP_SHAKE | MSENSOR_MOTION_CAP_DATA;
    return MSENSOR_SUCCESS;
}
/******************************************************************************/
// Description:   dtos_ioctl
// Note:          
/******************************************************************************/
LOCAL uint32 dtos_ioctl(MSENSOR_IOCTL_E cmd, void * data_ptr)
{
	uint32 ret = MSENSOR_SUCCESS;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_GSENSOR_INT_ID);
	
    //MEMS_DTOS_TRACE:"[DRV_MSENSOR] cmd = %d \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_DTOS_524_112_2_18_0_34_18_1892,(uint8*)"d", cmd);

    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
        {
			ret = dtos_GetVector((MSENSOR_MOTION_DATA_T *)data_ptr);
			break;
        }
        
        case MSENSOR_IOCTL_GET_EVENT:
        {   
            if(cfg_ptr)
            {
                GPIO_DisableIntCtl(cfg_ptr->gpio_num);
            }
            //GPIO_GSensorIntCtrl(FALSE, GPIO_INT_LEVEL_HIGH);
            
			ret = dtos_GetEvent((MSENSOR_MOTION_TYPE_E *)data_ptr);
			
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
			//GPIO_GSensorIntCtrl(TRUE, GPIO_INT_LEVEL_HIGH);
			break;
        }
        case MSENSOR_IOCTL_SET_SENSI:
        {
			ret = dtos_SetSensi(*(MSENSOR_SENSI_E *)data_ptr);
			break;
        }   
        case MSENSOR_IOCTL_SET_TIMER:
        {
			ret = dtos_ChangeTimer(*(uint32 *)data_ptr);
			break;
        }  
        case MSENSOR_IOCTL_GET_CAP:
        {
            ret = dtos_GetCapability((uint32 *)data_ptr);
			break;
        }   
        default:
            ret = MSENSOR_ERROR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/******************************************************************************/
