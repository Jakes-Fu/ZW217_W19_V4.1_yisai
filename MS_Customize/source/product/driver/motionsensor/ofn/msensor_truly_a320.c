/******************************************************************************
 ** File Name:      msensor_ofn_truly_a320.c                                  *    
 ** Author:         guobin.zhu                                                *          
 ** DATE:           2011.06.02                                                *          
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *                                                          
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  
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
#include "dal_keypad.h"
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
#define A320_PRODUCTID               0x00    // 0x83
#define A320_REVISIONID              0x01    // 0x01
#define A320_MOTION                  0x02
#define A320_DELTAX                  0x03
#define A320_DELTAY                  0x04
#define A320_SQUAL                   0x05
#define A320_SHUTTERUPPER            0x06
#define A320_SHUTTERLOWER            0x07
#define A320_MAXIMUMPIXEL            0x08
#define A320_PIXELSUM                0x09
#define A320_MINIMUMPIXEL            0x0A
#define A320_PIXELGRAB               0x0B
#define A320_CRC0                    0x0C
#define A320_CRC1                    0x0D
#define A320_CRC2                    0x0E
#define A320_CRC3                    0x0F
#define A320_SELFTEST                0x10
#define A320_CONFIGURATIONBITS       0x11
#define A320_LED_CONTROL             0x1A
#define A320_IO_MODE                 0x1C
#define A320_MOTION_CONTROL          0x1D
#define A320_OBSERVATION             0x2E
#define A320_SOFTRESET               0x3A    
#define A320_SHUTTER_MAX_HI          0x3B
#define A320_SHUTTER_MAX_LO          0x3C
#define A320_INVERSEREVISIONID       0x3E    
#define A320_INVERSEPRODUCTID        0x3F    
#define A320_OFN_ENGINE              0x60
#define A320_OFN_RESOLUTION          0x62
#define A320_OFN_SPEED_CONTROL       0x63
#define A320_OFN_SPEED_ST12          0x64
#define A320_OFN_SPEED_ST21          0x65
#define A320_OFN_SPEED_ST23          0x66
#define A320_OFN_SPEED_ST32          0x67
#define A320_OFN_SPEED_ST34          0x68
#define A320_OFN_SPEED_ST43          0x69
#define A320_OFN_SPEED_ST45          0x6A
#define A320_OFN_SPEED_ST54          0x6B
#define A320_OFN_AD_CTRL             0x6D
#define A320_OFN_AD_ATH_HIGH         0x6E
#define A320_OFN_AD_DTH_HIGH         0x6F
#define A320_OFN_AD_ATH_LOW          0x70
#define A320_OFN_AD_DTH_LOW          0x71
#define A320_OFN_QUANTIZE_CTRL       0x73
#define A320_OFN_XYQ_THRESH          0x74
#define A320_OFN_FPD_CTRL            0x75
#define A320_OFN_ORIENTATION_CTRL    0x77

#define A320_OFN_DESHAKE_TIME        150

#define A320_OFN_SENSITIVE 7

#define TRULY_SHUTDOWN_PIN 81
#define TRULY_RESET_PIN 79
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/
LOCAL uint32 truly_init(void);
LOCAL uint32 truly_open(void);
LOCAL uint32 truly_close(void);
LOCAL uint32 truly_ioctl(uint32 cmd, void * data_ptr);
/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/

typedef struct //msensor_misc_cfg_tag
{
	I2C_DEV  	            i2c_cfg;        // i2c config of BB to control msensor
    MSENSOR_LCD_PLACEMENT_E lcd_face;       // face lcd the same or reverse
    MSENSOR_PCB_PLACEMENT_E pin1;           // pin1 at PCB placement
}OFN_MISC_CFG_T, *OFN_MISC_CFG_T_PRT;

PUBLIC MSENSOR_CFG_T g_truly_ofn_info = 
{
    "ofn truly a320",
	MSENSOR_TYPE_OFN,
    MSENSOR_STATUS_NOT_INIT,
	truly_init,
	truly_open,
	truly_close,
	truly_ioctl
};

LOCAL OFN_MISC_CFG_T s_truly_misc_cfg = {0};
LOCAL int32 s_truly_IIC_Handler = -1;

/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/

/******************************************************************************/
// Description:   Init truly chip ---- power down chip!
// Dependence: 
// Author:        
// Note:
/******************************************************************************/
LOCAL uint32 truly_init(void)
{
    MSensor_GetMiscCfg(&g_truly_ofn_info, &s_truly_misc_cfg);
	if((s_truly_IIC_Handler == -1) && 
	  ((s_truly_IIC_Handler = I2C_HAL_Open(&(s_truly_misc_cfg.i2c_cfg))) == -1))
	{
		//SCI_TRACE_LOW:"[DRV_OFN] open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_TRULY_A320_135_112_2_18_0_34_23_1937,(uint8*)"");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}
	truly_close();	
    return MSENSOR_SUCCESS;
}
/*****************************************************************************/
//  Description:    write configuration data to chip registers
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint32 truly_write(uint8* cmd)
{
	if(I2C_HAL_Write(s_truly_IIC_Handler, cmd, cmd + 1, 1) == 0)
		return MSENSOR_ERROR_IIC_WRITE_FAILED;
	//WRITE OK!
	return MSENSOR_SUCCESS;	
}

/******************************************************************************/
// Description: 
// Note:
/******************************************************************************/
LOCAL uint32 truly_read(uint8 * cmd)
{      
	if(I2C_HAL_Read(s_truly_IIC_Handler, cmd, cmd + 1, 1) == 0)
		return MSENSOR_ERROR_IIC_READ_FAILED;
	//READ OK!
	return MSENSOR_SUCCESS;	
}

LOCAL void OFN_ID_Test(void)
{
    //reserved
    return;
}

LOCAL void OFN_Self_Test(void)
{
    //reserved
    return;
}
/*****************************************************************************/
//  Description:    reset 
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL void truly_reset(void)
{
    GPIO_SetValue(TRULY_SHUTDOWN_PIN,0); //ACTIVE OFN
	SCI_Sleep(200);
	//RESET
	GPIO_SetValue(TRULY_RESET_PIN,1);
	SCI_Sleep(1);
	GPIO_SetValue(TRULY_RESET_PIN,0);
	SCI_Sleep(1);
	GPIO_SetValue(TRULY_RESET_PIN,1);
	SCI_Sleep(15);
}
/******************************************************************************/
// Description:   truly_open ---- OPEN I2C and power on chip
// Dependence: 
// Author:        
// Note:
/******************************************************************************/
LOCAL uint32 truly_open(void)
{
	uint8 iic_buff[2];
//	BOOLEAN gpio_value;
	uint32 i;
	
	static const uint8 init_cfg[][2] = 
	{
		{A320_OFN_ENGINE, 0xe6},
		{A320_OFN_RESOLUTION, 0x12},
		{A320_OFN_SPEED_CONTROL, 0x0e},
		{A320_OFN_SPEED_ST12, 0x08},
		{A320_OFN_SPEED_ST21, 0x06},
		{A320_OFN_SPEED_ST23, 0x40},
		{A320_OFN_SPEED_ST32, 0x08},
		{A320_OFN_SPEED_ST34, 0x48},
		{A320_OFN_SPEED_ST43, 0x0a},
		{A320_OFN_SPEED_ST45, 0x50},
		{A320_OFN_SPEED_ST54, 0x48},
		{A320_OFN_AD_CTRL, 0xc4},
		{A320_OFN_AD_ATH_HIGH, 0x34},
		{A320_OFN_AD_DTH_HIGH, 0x3c},
		{A320_OFN_AD_ATH_LOW, 0x18},
		{A320_OFN_AD_DTH_LOW, 0x20},
		{A320_OFN_FPD_CTRL, 0x50},
		{A320_OFN_QUANTIZE_CTRL, 0x99},
		{A320_OFN_XYQ_THRESH, 0x02},
		//{A320_SELFTEST, 0x1c},
	};
	uint8* p_init = (uint8*)init_cfg;
	
    truly_reset();
	if((s_truly_IIC_Handler == -1) && 
		((s_truly_IIC_Handler = I2C_HAL_Open(&(s_truly_misc_cfg.i2c_cfg))) == -1))
	{
		//SCI_TRACE_LOW:"[DRV_OFN] open IIC failed!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_TRULY_A320_227_112_2_18_0_34_23_1938,(uint8*)"");
		return MSENSOR_ERROR_IIC_OPEN_FAILED;
	}

#ifdef OFN_DBGMODE			//OFN Test
	OFN_ID_Test(); 
	OFN_Self_Test(); 
#endif /* OFN_DBGMODE */
    
	iic_buff[0] = A320_SOFTRESET;	
	iic_buff[1] = 0x5a;
	truly_write(iic_buff);
	
	SCI_Sleep(15);
	for(i = 0; i < (sizeof(init_cfg) / 2); i++)
	{
		truly_write(p_init);
		p_init += 2;			//next cfg.
	}

    return SCI_SUCCESS;
}


/******************************************************************************/
// Description:   truly_close ---- Close IIC and power down chip
// Dependence: 
// Author:        
// Note:
/******************************************************************************/
LOCAL uint32 truly_close(void)
{
	GPIO_SetValue(TRULY_SHUTDOWN_PIN,1); //SHUTDOWN OFN
	
	if(s_truly_IIC_Handler != -1)
	{
		I2C_HAL_Close(s_truly_IIC_Handler);
		s_truly_IIC_Handler = -1;
	}

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description:   truly_getevent
// Dependence: 
// Author:     
/******************************************************************************/
LOCAL uint32 truly_GetEvent(MSENSOR_MOTION_TYPE_E * event_ptr)
{
    const int ofn_result[] = 
	{                           		
		SCI_VK_UP,           
		SCI_VK_RIGHT,           
		SCI_VK_DOWN,          
		SCI_VK_LEFT,          
	};

    uint8 reg[2];
    uint8 status;
    int8 x;
    int8 y;
    static uint32 count_tick = 0;
    
    reg[0] = A320_MOTION;
	truly_read(reg);
	status = reg[1];
	
    reg[0] = A320_DELTAX;
	truly_read(reg);
	x = (int8)reg[1];
	
	reg[0] = A320_DELTAY;
	truly_read(reg);
	y = (int8)reg[1];
	
//	SCI_TRACE_LOW("truly_status %d %d %d",status,x,y);

	if(x>A320_OFN_SENSITIVE)
	{
		if(x>=abs(y))
		{
            *event_ptr = ofn_result[(s_truly_misc_cfg.pin1+0)%4];
		}
	}
	else if(x<-A320_OFN_SENSITIVE)
	{
		if(abs(x)>=abs(y))
		{
            *event_ptr = ofn_result[(s_truly_misc_cfg.pin1+2)%4];
		}
	}

	if(y>A320_OFN_SENSITIVE)
	{
		if(y>abs(x))
		{
            *event_ptr = ofn_result[(s_truly_misc_cfg.pin1+3)%4];
		}
	}
	else if(y<-A320_OFN_SENSITIVE)
	{
		if(abs(y)>abs(x))
		{
            *event_ptr = ofn_result[(s_truly_misc_cfg.pin1+1)%4];
		}
	}
	
    //only one move event during A320_OFN_DESHAKE_TIME to avoid too much move event
	if(*event_ptr)
	{
	    if(SCI_GetTickCount() - count_tick < A320_OFN_DESHAKE_TIME)
	    *event_ptr = 0;
	    count_tick = SCI_GetTickCount();
	}

	return SCI_SUCCESS;   
}

/******************************************************************************/
// Description:   truly_ioctl
// Note:          
/******************************************************************************/
LOCAL uint32 truly_ioctl(uint32 cmd, void * data_ptr)
{
	uint32 ret = 0;
	GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_OFN_INT_ID);
    //SCI_TRACE_LOW:"[DRV_OFN] cmd = %d \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_TRULY_A320_348_112_2_18_0_34_24_1939,(uint8*)"d", cmd);

    switch(cmd)
    {
        case MSENSOR_IOCTL_GET_DATA:
        {
			//ret = truly_GetVector((OFN_MOTION_DATA_T *)data_ptr);
			break;
        }
        
        case MSENSOR_IOCTL_GET_EVENT:
        {
            if(cfg_ptr)
            {
                GPIO_DisableIntCtl(cfg_ptr->gpio_num);          
            }    
			ret = truly_GetEvent((MSENSOR_MOTION_TYPE_E *)data_ptr);
			//resume interrupt
            if(cfg_ptr)
            {
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
//			ret = truly_SetSensi(*(OFN_SENSI_E *)data_ptr);
			break;
        }   
        case MSENSOR_IOCTL_SET_TIMER:
        {
//			ret = truly_ChangeTimer(*(uint32 *)data_ptr);
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
