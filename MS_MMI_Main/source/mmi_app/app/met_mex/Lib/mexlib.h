/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexLib.h
* 文件标识：
* 内容摘要： // mex平台头文件用于定义不同的编译配置
*
************************************************************************/

#ifndef __MEXLIB_H__
#define __MEXLIB_H__

#if defined(WIN32)
#if defined(MEX_PLATFORM_MTK)
#undef MEX_PLATFORM_MTK
#define MEX_PLATFORM_MODIS
#endif // MEX_PLATFORM_MTK
#if defined(MEX_PLATFORM_SPREAD)
#undef MEX_PLATFORM_SPREAD
#define MEX_PLATFORM_SIMULATOR
#endif // MEX_PLATFORM_SPREAD
#endif // WIN32

#if !defined(MEX_PLATFORM_MODIS) && !defined(MEX_PLATFORM_MTK) && !defined(MEX_PLATFORM_WIN32) && !defined(MEX_PLATFORM_SPREAD) && !defined(MEX_PLATFORM_SIMULATOR)
	#error Mex Platform Not Defined!!!
#endif

#if !defined(MEX_TARGET_SERVER) && !defined(MEX_TARGET_CLIENT)
	#error Mex Target Not Defined!!!
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_WIN32)
#pragma warning( disable: 4312 )
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_WIN32) || defined(MEX_PLATFORM_SIMULATOR)
	#define MEX_MEMORY_GUARD			//in release version undefine it
	#define MEX_USE_ASSERT				//in release version undefine it
	#define MEX_DEBUG					//in release version undefine it
#endif

#define	MEX_BUILD_FINAL_USER		1		//如果是在做最终用户手机上的版本，请设置成1，否则设置为0
#define	MEX_BUILD_SDK_VERSION		0		//如果是在做SDK 版本，请设置为1，否则，请设置为0

#define MEX_SDK_VER_SUPPORT_MAX     1034    //最大支持的SDK版本，随着api函数的增加而增加，第一个版本为1000

#if defined(MEX_TARGET_SERVER)				//真机rom版永远使用最大支持的api库
	#define MEX_SDK_VER		MEX_SDK_VER_SUPPORT_MAX
#else										//应用程序版本
	#if defined(MEX_PLATFORM_MODIS)			//模拟器版本，也用最大支持的版本
		#define MEX_SDK_VER		MEX_SDK_VER_SUPPORT_MAX
	#else
		#ifndef MEX_SDK_VER					//真机版，编译器指定；如果没有指定，使用最大支持的版本
			#define MEX_SDK_VER		MEX_SDK_VER_SUPPORT_MAX
		#endif
	#endif
#endif

#if defined(MEX_PLATFORM_SPREAD) || defined(MEX_PLATFORM_SIMULATOR)
#define MEX_MTK_CODE_VERSION (0)
#endif

#if !MEX_BUILD_FINAL_USER
#define MEX_ENABLE_PROFILING		//in release version undefine it
#endif

//是否开启屏幕配置
#if MEX_BUILD_FINAL_USER
#define MEX_SCREEN_ESPECIAL_VERSION		(0)
#else
#define MEX_SCREEN_ESPECIAL_VERSION		(1)
#endif
//是否开启独立入口测试
#if MEX_BUILD_FINAL_USER
#else
#if defined(MEX_PLATFORM__FOR_SPRD6800H)
#define  AUTO_APP_TEST
#elif defined(MEX_PLATFORM__FOR_SPRD6600L)
#if defined(MAINLCD_SIZE_240X320)||defined(MAINLCD_SIZE_240X400)||defined(MAINLCD_SIZE_320X480)||defined(MAINLCD_SIZE_320X240)
#define  AUTO_APP_TEST
#endif
#endif
#endif
#if ((!MEX_BUILD_FINAL_USER) && (!MEX_BUILD_SDK_VERSION))  //检查是否为用户或SDK版本
#ifndef MEX_QA_MODE
#define MEX_QA_MODE
#endif

#ifndef MEX_ST_SUPPORT
	#define MEX_ST_SUPPORT
#endif
#endif 

#define MEX_DOWNLOADER_FONT_SIZE  1 

#define MEX_SOC_GPRS_SIM_SELECT   0
#define MEX_GPRS_ALLOW    0
#ifdef __MEX_FILEMGR_PAK_SUPPORT__
#define MEX_INSTALL_RUN_PACKAGE_SUPPORT
#endif

#if ((MEX_SDK_VER>=1034) && (defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)))
#define MEX_SUPPORT_FOREIGN_VERSION (1)
#endif

#include "MexLib_Def.h"
#define MEX_ROMFILE_INTERFACE (1)

#define MEX_PATH_ADAPTIVE (0)
#endif
