/******************************************************************************
 ** File Name:      version.h                                                 *
 ** Author:         Tony Yao                                                  *
 ** DATE:           23/08/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 23/08/2002     Tony.Yao         Create.                                   *
 ** 03/11/2003     Xueliang.Wang    Modify.                                   *
 ******************************************************************************/
#ifndef _VERSION_H
#define _VERSION_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef _SW_ZDT_PRODUCT_

#if defined(ZDT_W19_VER_CUSTOM)// W19 ver here ,NEW ver W19. same as w18
//厂商
#define ZDT_SFR_MANUF 			"W19"
//型号
#define ZDT_SFR_TYPE 			"W19"
//自注册编码
#define ZDT_SFR_MODEL 			"W19"
//软件版本号
#define ZDT_SFR_SW_VER 			"ZW201_W19_ARW_240X284_V41.0"//"ZW201_W18_240X284_V01"//"NM-ESC-A_V2.3"
//硬件版本号
#define ZDT_SFR_HW_VER 			"UWS6121E"
#else
//厂商
#define ZDT_SFR_MANUF 			"NM-ESC-A"
//型号
#define ZDT_SFR_TYPE 			"NM-ESC-A"
//自注册编码
#define ZDT_SFR_MODEL 			"NM-ESC-A"
//软件版本号
#define ZDT_SFR_SW_VER 			"ZW201_W18_ARW_240X284_V01"//"ZW201_W18_240X284_V01"//"NM-ESC-A_V2.3"
//硬件版本号
#define ZDT_SFR_HW_VER 			"UWS6121E"
#endif

#define ZDT_FOTA_VER 			ZDT_SFR_SW_VER//"W19_V1.0"

#define ZDT_PlatOS 			"RTOS"
#define ZDT_PlatVer 			"MOCOR20B"
#define ZDT_PlatRAM 			"16M"
#define ZDT_PlatROM 			"16M"
#define ZDT_PlatCPU 			"500MHz"

#define ZDT_CMCC_APP_KEY "M100001039"
#define ZDT_CMCC_AES_KEY "39UT82ajn2pYEh3007A73S5bgR203X53"
#define ZDT_CMCC_AES_KEYLEN (32)
#endif

typedef enum
{
    PLATFORM_VERSION,
    PROJECT_VERSION,
    MMI_VERSION,
    AP_SVN_VERSION,
    CP_SVN_VERSION,
    BASE_VERSION,
    HW_VERSION,
    CALIBRATION_VERSION,
    DOWNLOAD_VERSION,
    BUILD_TIME,
    LOG_VERSION,
    MEMORY_NAME,
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    GE2_VER,
#endif
    MAX_VERSION_NUM 
}VERSION_TYPE_E;


const char*  VERSION_GetInfo(uint32 type);

uint32  VERSION_GetInfoNum(void);


/*****************************************************************************/
//  Description:    return the project version
//	Global resource dependence:
//  Author:         fancier.fan
//	Note: 2004 1 16
/*****************************************************************************/
const char * COMMON_GetProjectVersionInfo(void);     // return software version 
#ifdef _SW_ZDT_PRODUCT_
const char * COMMON_GetATVersionInfo(void);
#endif

/*+CR79304*/
/*****************************************************************************/
//  Description:    return the version specified by version_type param
//	Global resource dependence:
//  Author:         minqian.qian
//	Note: 
/*****************************************************************************/
const char * COMMON_GetVersionInfo(
                uint8 version_type      //refer to VERSION_TYPE_E
                );
/*-CR79304*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* End of version.h*/
