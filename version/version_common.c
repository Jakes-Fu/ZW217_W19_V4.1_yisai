#include <string.h>
#include "os_api.h"
#ifndef TF_FDL_BUILD
#include "buildtime_ver.h"
#endif


#ifdef __ARMCC_VERSION	// Check validation of ARM compiler tool

#ifdef COMPILER_ADS_V12
#if __ARMCC_VERSION < 120848
#error Spreadtrum Caution: Please update your ADS 1.2 version to Patch 848 !!
#endif
#endif//COMPILER_ADS_V12

#ifdef COMPILER_RVDS_V31
//#if (__ARMCC_VERSION < 310836 ||__ARMCC_VERSION > 320000)
//#error Spreadtrum Caution: Please update your RVDS 3.1 version to Patch 836 !!
//#endif
#endif//COMPILER_RVDS_V31

#endif

#ifndef PROJECT_BUILD_TIME
#define PROJECT_BUILD_TIME    "Compile time is invaild!"
#endif

#define MAX_MEMORY_INFO_SIZE    300

extern const uint16  s_calibrationparam_version;
extern const uint16  s_downloadparam_version;
extern const char*  const s_version_info[];
extern const char*  const s_ims_ua_version_buf;

LOCAL uint8  s_version_buf[50];

#ifdef _SW_ZDT_PRODUCT_
LOCAL char s_at_version_buf[100] = {0};
#endif

#pragma  arm section  zidata="MEMORY_VERSION"

LOCAL char   project_version_info[MAX_MEMORY_INFO_SIZE] = {0};

#pragma  arm section zidata

/*****************************************************************************/
//  Description:    return the project version
//	Global resource dependence:
//  Author:         fancier.fan
//	Note: 2004 1 16
/*****************************************************************************/
const char * COMMON_GetProjectVersionInfo(void)     // return software version 
{
    if(0 == project_version_info[0])
    {
        project_version_info[MAX_MEMORY_INFO_SIZE - 1] = 0xAA;

        strcpy(project_version_info,"\n");
        strcat(project_version_info,s_version_info[PLATFORM_VERSION]);
        strcat(project_version_info,"\n");
        strcat(project_version_info,s_version_info[PROJECT_VERSION]);
        strcat(project_version_info,"\n");
        strcat(project_version_info,s_version_info[BASE_VERSION]);
        strcat(project_version_info,"\n");
        strcat(project_version_info,s_version_info[HW_VERSION]);     
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
        strcat(project_version_info,"\n");
        strcat(project_version_info,s_version_info[GE2_VER]);
#endif
        strcat(project_version_info,"\n");
        strcat(project_version_info,s_version_info[BUILD_TIME]);   

        strcat(project_version_info,"\n");
        strcat(project_version_info,"By RVDS V4.1");

        SCI_ASSERT(0xAA == project_version_info[MAX_MEMORY_INFO_SIZE - 1]);
    }
    
    return project_version_info;
}   

#ifdef _SW_ZDT_PRODUCT_
const char * COMMON_GetATVersionInfo(void)
{
    if(0 == s_at_version_buf[0])
    {
        strcpy(s_at_version_buf,s_version_info[HW_VERSION]);
        strcat(s_at_version_buf,",");
        strcat(s_at_version_buf,s_version_info[PROJECT_VERSION]);
    }
    
    return s_at_version_buf;
}
#endif

/*****************************************************************************/
//  Description:    return calibration version
//	Global resource dependence:
//  Author:         steven.niu
//	Note: 2004 1 16
/*****************************************************************************/
uint16 COMMON_GetCalibrationParamVersion(void)     // return calibration param version
{
    return s_calibrationparam_version;
}    

/*****************************************************************************/
//  Description:    return downloadparam version
//	Global resource dependence:
//  Author:         steven.niu
//	Note: 2004 1 16
/*****************************************************************************/
uint16 COMMON_GetDownloadParamVersion(void)     // return calibration param version
{
    return s_downloadparam_version;
}    



const char*  VERSION_GetInfo(uint32 type)
{
    const char* version_ptr = NULL;
	uint16  svn_version_cp=0;
    if(type >= MAX_VERSION_NUM)    
    {
       return NULL; 
    }
    
    switch(type)
    {
    case CALIBRATION_VERSION:
    {
        sprintf( (char*)s_version_buf, "Calibration Version: 0x%04X", s_calibrationparam_version);
        version_ptr = (const char*)s_version_buf;
        break;    
    }    
    
    case DOWNLOAD_VERSION:
    {
        sprintf( (char*)s_version_buf, "Download Version: 0x%04X", s_downloadparam_version);
        version_ptr = (const char*)s_version_buf;
        break;    
    }   

	case CP_SVN_VERSION:
    {
        sprintf( (char*)s_version_buf, "CP SVN : %d", svn_version_cp);
        version_ptr = (const char*)s_version_buf;
        break;    
    } 
	
    default:
    {
        
        version_ptr = s_version_info[type]; 
        break;    
    }    
    }
    
    return version_ptr;
}


uint32  VERSION_GetInfoNum(void)
{
    return MAX_VERSION_NUM;    
}

/*+CR79304*/
/*****************************************************************************/
//  Description:    return the version specified by version_type param
//	Global resource dependence:
//  Author:         minqian.qian
//	Note: 
/*****************************************************************************/
const char * COMMON_GetVersionInfo(
                uint8 version_type      //refer to VERSION_TYPE_E
                )
{

    if (version_type < MAX_VERSION_NUM)
    {
        return s_version_info[version_type];
    }

    return NULL;  
}   
/*-CR79304*/


#ifdef PLATFORM_UWS6121E
const char * COMMON_GetImsUaVersionInfo(void)
{	 
   return (const char*)s_ims_ua_version_buf;
}   
#endif


