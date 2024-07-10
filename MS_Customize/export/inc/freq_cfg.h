/******************************************************************************
 ** File Name:      freq_cfg.h                                               *
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
#ifndef _FREQ_CFG_H_
#define _FREQ_CFG_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "chng_freq.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines                                     
 **---------------------------------------------------------------------------*/
/*define the customized freq index
if the customer define new item here, you must add new item in the related table(s_customized_freq_cfg) also.*/
#define FREQ_INDEX_APP_BASE 	FREQ_INDEX_CUSTOMIZED_BASE
typedef enum
{
	FREQ_INDEX_APP_BOOT = FREQ_INDEX_APP_BASE, //the start index is base on FREQ_INDEX_CUSTOMIZED_BASE
	FREQ_INDEX_APP_HIGH,                                        //app high freq index        
	FREQ_INDEX_JAVA_LOW,  					   //java low freq index
	FREQ_INDEX_JAVA_MID,  						   //java middle freq index
	FREQ_INDEX_JAVA_HIGH,  					   //java high freq index
	FREQ_INDEX_GPS_LOW,  					          //gps low freq index
	FREQ_INDEX_GPS_MID,  						   //gps middle freq index
	FREQ_INDEX_GPS_HIGH,  					          //gps high freq index
	FREQ_INDEX_TVOUT,  					          //tv_out freq index
	FREQ_INDEX_SLIDE_WIN,  				          //slide win freq index
	FREQ_INDEX_WWW,  				                        //www browser freq index
	
	FREQ_INDEX_PROD_MAX					          //max freq index, the max value should less than FREQ_INDEX_MAX
} PROD_FREQ_INDEX_E;

/**--------------------------------------------------------------------------*
 **                         Data Structures                                  *
 **--------------------------------------------------------------------------*/ 
 extern PUBLIC uint32 s_customized_freq_cfg[FREQ_INDEX_PROD_MAX-FREQ_INDEX_APP_BASE]; 

 /**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to get the customized freq config(base address) 
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
PUBLIC uint32* PROD_GetCustomFreqCfg(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

#endif
// end freq_cfg.h
