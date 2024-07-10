/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
* 
* �ļ����ƣ� // MexLib.h
* �ļ���ʶ��
* ����ժҪ�� // mexƽ̨ͷ�ļ����ڶ��岻ͬ�ı�������
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

#define	MEX_BUILD_FINAL_USER		1		//��������������û��ֻ��ϵİ汾�������ó�1����������Ϊ0
#define	MEX_BUILD_SDK_VERSION		0		//���������SDK �汾��������Ϊ1������������Ϊ0

#define MEX_SDK_VER_SUPPORT_MAX     1034    //���֧�ֵ�SDK�汾������api���������Ӷ����ӣ���һ���汾Ϊ1000

#if defined(MEX_TARGET_SERVER)				//���rom����Զʹ�����֧�ֵ�api��
	#define MEX_SDK_VER		MEX_SDK_VER_SUPPORT_MAX
#else										//Ӧ�ó���汾
	#if defined(MEX_PLATFORM_MODIS)			//ģ�����汾��Ҳ�����֧�ֵİ汾
		#define MEX_SDK_VER		MEX_SDK_VER_SUPPORT_MAX
	#else
		#ifndef MEX_SDK_VER					//����棬������ָ�������û��ָ����ʹ�����֧�ֵİ汾
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

//�Ƿ�����Ļ����
#if MEX_BUILD_FINAL_USER
#define MEX_SCREEN_ESPECIAL_VERSION		(0)
#else
#define MEX_SCREEN_ESPECIAL_VERSION		(1)
#endif
//�Ƿ���������ڲ���
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
#if ((!MEX_BUILD_FINAL_USER) && (!MEX_BUILD_SDK_VERSION))  //����Ƿ�Ϊ�û���SDK�汾
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
