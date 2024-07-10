/**
***********************************************************************************************************************
* Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
*
* @file        ota_type.h
*
* @brief
*
* @revision
* Date         Author       Notes
* 2021-06-16   gongsui      First Version
***********************************************************************************************************************
*/

#ifndef __OTA_TYPE_H__
#define __OTA_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef char                 ota_int8;
typedef short                ota_int16;
typedef int                  ota_int32;
typedef long long            ota_int64;
typedef unsigned char        ota_uint8;
typedef unsigned short       ota_uint16;
typedef unsigned int         ota_uint32;
typedef unsigned int         ota_size_t;
typedef unsigned long long   ota_uint64;

typedef ota_int32            ota_bool;
#define OTA_FALSE            ((ota_bool)0)
#define OTA_TRUE             ((ota_bool)(!OTA_FALSE))

#ifdef __cplusplus
}
#endif

#endif
