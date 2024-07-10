/******************************************************************************
 ** File Name:      sensor_cfg.h                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           04/20/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of sensor*
 **                				                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/20/2006     Liangwen.Zhen     Create.                                  *
 ******************************************************************************/

#ifndef _SENSOR_CFG_H_
#define _SENSOR_CFG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "sci_types.h"
#include "sensor_drv.h"
/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

#if 0
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
extern SENSOR_INFO_T g_OV7670_yuv_info;
extern SENSOR_INFO_T g_OV9655_yuv_info;
extern SENSOR_INFO_T g_OV2640_yuv_info;
extern SENSOR_INFO_T g_OV2655_yuv_info;
extern SENSOR_INFO_T g_GC0306_yuv_info;
extern SENSOR_INFO_T g_SIV100A_yuv_info;
extern SENSOR_INFO_T g_SIV100B_yuv_info;
extern SENSOR_INFO_T g_OV3640_yuv_info;
extern SENSOR_INFO_T g_mt9m112_yuv_info;
extern SENSOR_INFO_T g_OV7690_yuv_info;

extern SENSOR_INFO_T g_OV9660_yuv_info;


/**---------------------------------------------------------------------------*
 **                         analog tv                                         *
 **---------------------------------------------------------------------------*/
extern SENSOR_INFO_T g_tlg1120_yuv_info;
extern SENSOR_INFO_T  g_qn8610_yuv_info;
#endif

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef enum
{
    CCIR_PCLK_SOURCE_CCIR=0x00,
    CCIR_PCLK_SOURCE_MTCK,
    CCIR_PCLK_SOURCE_ATV,
    CCIR_PCLK_SOURCE_MAX,
}CCIR_PCLK_SOURCE_E;

typedef enum
{
    CCIR_D0_SOURCE_CCIRD0=0x00,
    CCIR_D0_SOURCE_MTDI,
    CCIR_D0_SOURCE_MAX,
}CCIR_D0_SOURCE_E;

typedef enum
{
    CCIR_VSYNC_SOURCE_CCIRVS=0x00,
    CCIR_VSYNC_SOURCE_MTMS,
    CCIR_VSYNC_SOURCE_MAX,
}CCIR_VSYNC_SOURCE_E;

typedef enum
{
    CCIR_D1_SOURCE_CCIRD1=0x00,
    CCIR_D1_SOURCE_SDI1_0,
    CCIR_D1_SOURCE_SDI1_1,
    CCIR_D1_SOURCE_MAX,
}CCIR_D1_SOURCE_E;

typedef struct _ccir_src_cfg_tag
{
    CCIR_PCLK_SOURCE_E      main_sensor_pclk_src;
    CCIR_PCLK_SOURCE_E      sub_sensor_pclk_src;
    CCIR_PCLK_SOURCE_E      atv_pclk_src;
    
    CCIR_D0_SOURCE_E        main_sensor_d0_src;
    CCIR_D0_SOURCE_E        sub_sensor_d0_src;
    CCIR_D0_SOURCE_E        atv_d0_src;
    
    CCIR_VSYNC_SOURCE_E     main_sensor_vsync_src;
    CCIR_VSYNC_SOURCE_E     sub_sensor_vsync_src;
    CCIR_VSYNC_SOURCE_E     atv_vsync_src;

    CCIR_D1_SOURCE_E     main_sensor_d1_src;
    CCIR_D1_SOURCE_E     sub_sensor_d1_src;
    CCIR_D1_SOURCE_E     atv_d1_src;
} CCIR_SRC_CFG_T;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:    This function is used to select sensor data format    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void Sensor_SelectSensorFormat(SENSOR_IMAGE_FORMAT data_format);

/*****************************************************************************/
//  Description:    This function is used to get sensor sensor data format    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_IMAGE_FORMAT Sensor_GetSensorFormat(void);

/*****************************************************************************/
//  Description:    This function is used to get sensor information table    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_INFO_T ** Sensor_GetInforTab(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used to get sensor information table    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 Sensor_GetInforTabLenght(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is register diag command routine for sensor test
//  Author:         Jianping.wang
//  Note:           
/*****************************************************************************/
PUBLIC void Sensor_RegisterCmdRoutine (void);

/*****************************************************************************/
//  Description:    This function is used for ccir pclk source   
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_Get_Ccir_Pclk_Src(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used for ccir d0  
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_Get_Ccir_D0_Src(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used for ccir Vsync  
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_Get_Ccir_Vsync_Src(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used for ccir d1  
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_Get_Ccir_D1_Src(SENSOR_ID_E sensor_id);


/*****************************************************************************/
//  Description:    This function is used to get default i2c port  
//  Author:         X.Z
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_I2C_PORT_E Sensor_GetDefaultI2cPort(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used to set mclk  
//  Author:         Eddy.Wei
//  Note:           
/*****************************************************************************/
PUBLIC void Sensor_MclkConfig(uint32 clk_src, uint32 clk_div);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _SENSOR_CFG_H_

// End of sensor_cfg.h


