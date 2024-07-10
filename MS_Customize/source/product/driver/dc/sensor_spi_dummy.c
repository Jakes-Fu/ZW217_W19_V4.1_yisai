/******************************************************************************
 ** Copyright (c) 
 ** File Name:												  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for . 
 ** 														 
 ******************************************************************************

 ******************************************************************************
 ** 					   Edit History 									  *
 ** ------------------------------------------------------------------------- *
 ** DATE		   NAME 			DESCRIPTION 							  *
 ** 	  
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 ** 						Dependencies									  *
 **---------------------------------------------------------------------------*/
#include "sensor_cfg.h"
#include "sensor_drv.h"
//#include "i2c_drvapi.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"

/**---------------------------------------------------------------------------*
 ** 						Compiler Flag									  *
 **---------------------------------------------------------------------------*/
#ifdef	 __cplusplus
	extern	 "C" 
	{
#endif
extern PUBLIC  SENSOR_OUTPUT_MODE_E AutoTest_GetSensorOutPutMode(void);
/*lint -save -e785 */
/**---------------------------------------------------------------------------*
 ** 					Extern Function Declaration 						  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 ** 						Const variables 								  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** 						   Macro Define
 **---------------------------------------------------------------------------*/

 
/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/

/*lint -save -e533 */

PUBLIC uint32 _Spi_Sensor_PowerOn(uint32 power_on);


LOCAL uint32 _Spi_Sensor_Identify(uint32 param);


LOCAL SENSOR_REG_TAB_INFO_T s_at_spi_sensor_resolution_Tab_YUV[]=
{	
	// COMMON INIT
	{ PNULL, 0, 120, 120, 24, SENSOR_IMAGE_FORMAT_YUV422},
	
	// YUV422 PREVIEW 1
	{ PNULL, 0, 120, 120, 24, SENSOR_IMAGE_FORMAT_YUV422},
     
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0}


};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_at_spi_sensor_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    _Spi_Sensor_PowerOn,
    PNULL,
    _Spi_Sensor_Identify,

    PNULL,			// write register
    PNULL,			// read  register	
    PNULL,
    PNULL,

    // External
    PNULL,
    PNULL,
    PNULL,

    PNULL,
    PNULL,
    PNULL,
    PNULL,

    PNULL,	
    PNULL,

    PNULL, 
    PNULL, 

    PNULL,

    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,	
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
};

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_at_spi_sensor_yuv_info =
{
	NULL,				// salve i2c write address
	NULL, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_P|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_P,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved											
											
	// preview mode
	NULL,		
	
	// image effect
	NULL,
	
	// while balance mode
	0,
		
	0,								// bit[0:7]: count of step in brightness, contrast, sharpness, saturation
									// bit[8:31] reseved
	
	0,			// reset pulse level
	0,								// reset pulse width(ms)
	
	0,			// 1: high level valid; 0: low level valid
	
	0,								// count of identify code
	0x00, 0x00,						// supply two code to identify sensor.
	0x00, 0x00,						// for Example: index = 0-> Device id, index = 1 -> version id	
									
	SENSOR_AVDD_2800MV,				// voltage of avdd	

	240,							// max width of source image
	320,							// max height of source image
	"spi sensor",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_UYVY,	// pattern of input image form sensor;			

	s_at_spi_sensor_resolution_Tab_YUV,	// point to resolution table information structure
	&s_at_spi_sensor_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	0,                     // skip frame num before preview 
	0,                      // skip frame num before capture
	0,                      // deci frame num during preview	
	0,                       // deci frame num during video preview
	0,
	0,
	0,
	0,
	SENSOR_OUTPUT_MODE_SPI_1BIT,
	SENSOR_OUTPUT_ENDIAN_BIG,
};

PUBLIC uint32 _Spi_Sensor_PowerOn(uint32 power_on)
{
    SENSOR_AVDD_VAL_E dvdd_val=g_at_spi_sensor_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E avdd_val=g_at_spi_sensor_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E iovdd_val=g_at_spi_sensor_yuv_info.iovdd_val;  

	//Sensor_PowerOn(power_on);

	#if 0
    if(SCI_TRUE==power_on)
    {
	    Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
		OS_TickDelay(1);
		Sensor_SetMCLK_8910();   
		hwp_camera->CMD_SET = CAMERA_PWDN;
		OS_TickDelay(1);
		hwp_camera->CMD_CLR = CAMERA_PWDN;
        
    }
    else
    {
		hwp_camera->CMD_SET = CAMERA_PWDN;
		OS_TickDelay(1);
		Sensor_disableMCLK_8910();
		Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);

		OS_TickDelay(1);
		hwp_camera->CMD_CLR = CAMERA_PWDN;
    }
    #endif
	
    return SCI_SUCCESS;
}

LOCAL uint32 _Spi_Sensor_Identify(uint32 param)
{
	uint32 ret=SCI_ERROR;

    if(SENSOR_OUTPUT_MODE_SPI_1BIT == AutoTest_GetSensorOutPutMode())	
    {
        SCI_TRACE_LOW("it is Parallel_Sensor_Identify");	
        ret=SCI_SUCCESS;
    }	

	return  ret;
}


/*lint -restore */

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
