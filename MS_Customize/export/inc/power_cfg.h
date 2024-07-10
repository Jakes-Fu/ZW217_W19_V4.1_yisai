/******************************************************************************
 ** File Name:      power_cfg.h                                               *
 ** Author:         Mingwei.Zhang                                             *
 ** DATE:           01/10/2011                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of Power *
 **					about product
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/10/2011     Mingwei.Zhang    Create.                                   *
 ******************************************************************************/
 
#ifndef _POWER_CFG_H_
#define _POWER_CFG_H_

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "ldo_drvapi.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
typedef struct
{
    LDO_ID_E            ldo_id;
    LDO_VOLT_LEVEL_E    ldo_level;     ///initial voltage level of ldo
    BOOLEAN             ldo_on;        ///initial status of ldo
}LDO_PROD_CFG_T;
#else
typedef struct
{
    LDO_ID_E            ldo_id;
    uint32                  ldo_val;     ///initial voltage value of ldo
    BOOLEAN             ldo_on;        ///initial status of ldo
}LDO_PROD_CFG_T;
#endif

// enum: voltage level of sensor 
typedef enum
{
	POWER_VDD_3300MV = 0,
	POWER_VDD_3000MV ,
	POWER_VDD_2800MV,
	POWER_VDD_2500MV,
	POWER_VDD_1800MV,
	POWER_VDD_1500MV,
	POWER_VDD_1300MV,	
	POWER_VDD_1200MV,	
	POWER_VDD_CLOSED,
	POWER_VDD_UNUSED,
	POWER_VDD_MAX
}POWER_SENSOR_VDD_VAL_E;

// enum: sensor id main img sensor sub img senosr or analog tv
typedef enum
{
	POWER_SENSOR_MAIN = 0,
	POWER_SENSOR_SUB,
	POWER_SENSOR_ATV,
	POWER_SENSOR_ID_MAX
}POWER_SENSOR_ID_E;


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
extern void LDO_Prod_Init(void);
extern LDO_PROD_CFG_T* LDO_GetProdCFGTab(void);

/*****************************************************************************/
//  Description:    This function is used to set AVDD
//  Author:         Eddy.Wei
//  Note:          
/*****************************************************************************/
PUBLIC void Power_SetSensorVoltage(POWER_SENSOR_ID_E sensor_id,POWER_SENSOR_VDD_VAL_E dvdd_val, POWER_SENSOR_VDD_VAL_E avdd_val, POWER_SENSOR_VDD_VAL_E iodd_val);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif


