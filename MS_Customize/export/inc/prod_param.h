/******************************************************************************
 ** File Name:      prod_param.h                                                *
 ** Author:         Liangwen.zhen                                             *
 ** DATE:           10/24/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 production                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/24/2006     Liangwen.zhen     Create.                                  *
 ******************************************************************************/

#ifndef _PROD_PARAM_H
#define _PROD_PARAM_H

#include "os_api.h"
#include "nv_productionparam_type.h"
#include "audio_prod.h"
#include "admm_III.h"
#include "dal_lcd.h"
#include "dal_img.h"
#include "dal_chr.h"
#include "adc_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 
typedef struct
{
    uint32    adc[2];           // calibration of ADC, two test point
    uint32    battery[2];       // calibraton of battery(include resistance), two test point
    uint32    reserved[8];      // reserved for feature use.
}ADC_T;
// reserved[6] : REF config : refer to REFPARAM_GetUsbAutoModeFlag()
#define REF_CONFIG_USB_AUTO_MODE_BIT 0x00000001  // reserved[6].bit0

// reserved[7] : mobile production marker: refer to REFPARAM_GetCaliMarkerValue()

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

 
/*****************************************************************************/
//  Description:    The function initialize PROD parameters which on NVItem.
//	Global resource dependence: 
//  Author:         Benjamin.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void PROD_InitProdParam(void);

/*****************************************************************************/
//  Description:    The function read PROD parameters about audio from NVItem .
//	Global resource dependence: 
//  Author:         Benjamin.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 PROD_ReadAudioParam(  
             NV_AUDIO_E   audio_mode,
             void*        mode_aram_ptr
             );

/*****************************************************************************/
//  Description:    The function write PROD parameters about audio to NVItem .
//	Global resource dependence: 
//  Author:         Benjamin.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 PROD_WriteAudioParam(
            NV_AUDIO_E       audio_mode,
            void*        mode_param_ptr,
            BOOLEAN      is_flush
    );

/*****************************************************************************/
//  Description:    The function gets charge nvitem parameters.
//	Global resource dependence: 
//  Author:         Benjamin.Wang
//	Note:           
/*****************************************************************************/
PUBLIC CHGMNG_PARAM_PROD_T* PROD_GetChargeParam(void);
   
//
// Get the Mobile serial number. if the bufferLen is less than the serial number len,
// only the byte 
//
PUBLIC void PROD_GetMobileSerialNumber( uint8 *serialNumberBuffer, uint32  bufferLen );

/*****************************************************************************/
//  Description:    Get LCD module id in the nvram.
//	Global resource dependence: 
//				
//  Author:        Feifei.gu
//	Note:           return lcd module id.
/*****************************************************************************/
PUBLIC uint16 PROD_GetLcdModuleId( LCD_ID_E lcd_id );

/*****************************************************************************/
//  Description:    This function to  initialize customer config setting
//  Author:         liangwen.zhen
//	Note:           
/*****************************************************************************/
PUBLIC void PROD_CusCfgInit(void);

/*****************************************************************************/
// Get the Mobile serial number. if the bufferLen is less than the serial number len,
// only the byte 
/*****************************************************************************/
PUBLIC void PROD_GetMobileSerialNumber( uint8 *serialNumberBuffer, uint32  bufferLen );

/*****************************************************************************/
//  Description:    The function reads one mode of audio parameters from flash .
//	Global resource dependence: 
//  Author:         Cherry.Liu
//	Note:           
/*****************************************************************************/
PUBLIC AUDMOD_RESULT_E PROD_ReadAudioParamFromFlash(
    const uint8* pucModeName, 
    AUDMOD_DEVMOD_T* pModeParam //out ptr
    );


/*****************************************************************************/
//  Description:    The function writes one mode of audio parameters to flash .
//	Global resource dependence: 
//  Author:         Cherry.Liu
//	Note:           
/*****************************************************************************/
PUBLIC AUDMOD_RESULT_E PROD_WriteAudioParamToFlash(
    const uint8* pucModeName, 
    AUDMOD_DEVMOD_T* pModeParam
    );

/*****************************************************************************/
//  Description:    This function do some initialization work needed by products.
//  Author:         Benjamin.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void PROD_Init(void);

/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC  uint32 PROD_InitByStep(uint32  step);

/*****************************************************************************/
//  Description:    This function return logo address from resource
//  param:          id: main lcd /sub lcd
//  return          the address of logo res
//	Note:           
/*****************************************************************************/
PUBLIC uint8 * PROD_GetLogoAddrFromRes(LCD_ID_E id,IMG_RES_SRC_T* img_src);


/*****************************************************************************/
//  Description:    This function set peripheral nv parameter.
//  Author:           Yi.Qiu
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN PROD_SetChargeNVParam(CHG_SWITPOINT_E 	point);

/*****************************************************************************/
//  Description:    get the ADC channel for control battery temperature
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC ADC_E PROD_ADCGetVbatTemperCh(void);

/*****************************************************************************/
//  Description:    get the ADC channel for controlling environment temperature
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC ADC_E PROD_ADCGetEnvTemperCh(void);

/*****************************************************************************/
//  Description:    The function read PROD parameters about testing result of the
//                  engine mode.
//	Global resource dependence: 
//  Author:         Liangwen.zhen
//	Note:           
/*****************************************************************************/
PUBLIC uint32 PROD_ReadEngModeTestResult(  void* result_buf_ptr, uint32 buf_len);


/*****************************************************************************/
//  Description:    The function write PROD parameters about testing result of the
//                  engine mode.
//	Global resource dependence: 
//  Author:         Benjamin.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 PROD_WriteEngModeTestResult(  void* result_buf_ptr, uint32 buf_len);


/*****************************************************************************/
//  Description:    The function read Flash ID .
//	Global resource dependence: 
//  Author:         fei.zhang
//	Note:           
/*****************************************************************************/
PUBLIC void PROD_GetFlashID(uint32 *pManu_id, uint32 *pDev_id, uint32 *pExtend_id);

/*****************************************************************************/
//  Description:   update NV, then update necessary module
//	Note:      20110808: Will.Jiang_NEWMS00111228 : added
/*****************************************************************************/
PUBLIC void PROD_UpdateWithNewNV(void);

/*****************************************************************************/
//  Description:    Save battery capacity in NV when power off.
//  Author:         Tianyu.Yang
//  Note:
/*****************************************************************************/
PUBLIC void PROD_Capacity_Save_NV(void);

/*****************************************************************************/
//  Description:    Get battery capacity from NV when power on.
//  Author:         Tianyu.Yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 PROD_Capacity_Get_NV(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _PROD_PARAM_H

// End of prod_param.h
