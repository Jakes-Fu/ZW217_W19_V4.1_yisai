/******************************************************************************
 ** File Name:      freq_cfg.c                                               *
 ** Author:         yuhua.shi                                                 *
 ** DATE:           01/25/2008                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the frequency level config for the      *
 **                 application specified by customer.                                              *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/25/2008     yuhua.shi        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "freq_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define APP_BOOT_CLK_LVL                   ARM_CLK_LVL5	
#define APP_CLK_LVL_HIGH                   ARM_CLK_LVL5	
#define JAVA_CLK_LVL_LOW     		ARM_CLK_LVL2
#define JAVA_CLK_LVL_MID      		ARM_CLK_LVL4
#define JAVA_CLK_LVL_HIGH    		ARM_CLK_LVL5
#define GPS_CLK_LVL_LOW       		ARM_CLK_LVL2
#define GPS_CLK_LVL_MID      		       ARM_CLK_LVL4
#define GPS_CLK_LVL_HIGH    		       ARM_CLK_LVL5
#define TVOUT_CLK_LVL                         ARM_CLK_LVL5            
#define SLIDE_WIN_CLK_LVL                 ARM_CLK_LVL5            
#define WWW_CLK_LVL                          ARM_CLK_LVL5

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
//this table defines the freq config for the application specified by customer
PUBLIC uint32 s_customized_freq_cfg[FREQ_INDEX_PROD_MAX-FREQ_INDEX_APP_BASE] = 
{
	APP_BOOT_CLK_LVL,                   //FREQ_INDEX_APP_BOOT
	APP_CLK_LVL_HIGH,                   //FREQ_INDEX_APP_HIGH
	JAVA_CLK_LVL_LOW,                 //FREQ_INDEX_JAVA_LOW
	JAVA_CLK_LVL_MID,                  //FREQ_INDEX_JAVA_MID
	JAVA_CLK_LVL_HIGH,                 //FREQ_INDEX_JAVA_HIGH
	GPS_CLK_LVL_LOW,                 //FREQ_INDEX_GPS_LOW
	GPS_CLK_LVL_MID,                  //FREQ_INDEX_GPS_MID
	GPS_CLK_LVL_HIGH,                 //FREQ_INDEX_GPS_HIGH
	TVOUT_CLK_LVL,                     //FREQ_INDEX_TVOUT
	SLIDE_WIN_CLK_LVL,              //FREQ_INDEX_SLIDE_WIN
	WWW_CLK_LVL,                      //FREQ_INDEX_WWW
}; 

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to get the customized freq config(base address) 
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
PUBLIC uint32* PROD_GetCustomFreqCfg(void)
{
	return s_customized_freq_cfg;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
