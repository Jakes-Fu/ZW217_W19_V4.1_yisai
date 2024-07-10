/******************************************************************************
 ** File Name:      SC6531EFM_sc6531efm_bar_version.c                                 *
 ** Author:         		                                                  *
 ** DATE:           1/15/2016                                                 *	
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file include sw version of project  	              *
 **					SC6531EFM.								                  	  * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME            DESCRIPTION                               *
 ** 1/15/2016  		        		Create.                                   *
 ******************************************************************************/
#include "version.h"
#include "version_common.c"
#include "version_software_mocor.h"
#include "version_software_ap.h"
#include "version_software_gsm_base.h"
#include "version_log.h"

#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
#include "version_software_gps.h"
#endif

//lint -e786
#ifdef _SW_ZDT_PRODUCT_
const char* const  s_version_info[MAX_VERSION_NUM] = 
{
#ifdef _VERSION_RELEASE_  
    "Platform Version: MOCOR_W217_"_MOCOR_SW_VERSION_"_Release",
#else
    "Platform Version: MOCOR_W217_"_MOCOR_SW_VERSION_"_Debug",
#endif
    ZDT_SFR_SW_VER,
    ZDT_SFR_SW_VER,
    "AP SVN :"AP_SW_VERSION,
    "CP SVN :",
    "BASE  Version:     BASE_"_BASE_SW_VERSION_,  
    ZDT_SFR_HW_VER,    
    "",    //calibration  version
    "",     //download     version
    PROJECT_BUILD_TIME,
    "LOG Version:      "_VERSION_LOG_,
    "RH77",//MEMORY_NAME
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    "GE2 Version:      "_MOCOR_GE2_VERSION_,
#endif
};
#else
const char* const  s_version_info[MAX_VERSION_NUM] = 
{
#ifdef _VERSION_RELEASE_  
    "Platform Version: MOCOR_W217_"_MOCOR_SW_VERSION_"_Release",
#else
    "Platform Version: MOCOR_W217_"_MOCOR_SW_VERSION_"_Debug",
#endif
    "Project Version:"_MOCOR_SW_PROJECT_,
    "MMI Version:     UWS6121E",
    "AP SVN :"AP_SW_VERSION,
    "CP SVN :",
    "BASE  Version:     BASE_"_BASE_SW_VERSION_,  
    "HW Version:UWS6121E",    
    "",    //calibration  version
    "",     //download     version
    PROJECT_BUILD_TIME,
    "LOG Version:      "_VERSION_LOG_,
    "RH77",//MEMORY_NAME
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    "GE2 Version:      "_MOCOR_GE2_VERSION_,
#endif
};
#endif

/*Bug 1070436*//*Bug 1070447*//*Bug 1070507*//*Bug 1070638*//*Bug 1070614*/
const char  * const s_ims_ua_version_buf = {"Unisoc 220 4G "};
//lint +e786

