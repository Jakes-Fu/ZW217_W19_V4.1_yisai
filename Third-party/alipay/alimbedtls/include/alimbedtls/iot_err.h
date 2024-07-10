/* Copyright (c) 2018 ANT. All Rights Reserved.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */

/** @file
 *
 * @defgroup iot_sdk
 * @{
 * @ingroup iot_err
 * @brief  iot sdk error typedef
 *
 * @details
 *
 * @note
 *
 */

#ifndef _IOT_ERR_H
#define _IOT_ERR_H



#ifdef __cplusplus

extern "C" {
    
#endif
    
    
    /*** include files ***/
    
#include <string.h>
#include <stdio.h>
    
    
    
#define IOT_MAC_BYTE_SIZE        6
#define IOT_MAC_STRING_BYTES     17
#define IOT_IP_INFO_BYTE_SIZE    15
#define IOT_RTOS_VERSION         "8.2.0"
    
    
    
    /**@brief iot sdk error enum. */
    typedef enum
    {
        IOT_ERR_SUCCESS          =  0,
        IOT_ERR_NULL             = -1,
        IOT_ERR_INVALID_PARMS    = -2,
        IOT_ERR_INVALID_STATE    = -3,
        IOT_ERR_VENDOR_CALL      = -4,
        IOT_ERR_WIFI_DISCON      = -5,
    }iot_err_edef;
    
    
    
#ifdef __cplusplus
    
}

#endif

#endif



