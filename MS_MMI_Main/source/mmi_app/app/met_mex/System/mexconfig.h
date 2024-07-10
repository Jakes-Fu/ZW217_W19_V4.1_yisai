/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexConfig.c
* 文件标识：
* 内容摘要： // 定义各种配置变量，供不同厂商不同系统定制
*
************************************************************************/
#if defined(MEX_TARGET_SERVER)

#ifndef __MEX_CONFIG_H__
#define __MEX_CONFIG_H__

#if !defined(__MEXLIB_STD_SERVER_H__)
#error Please Include MexLib_Std.h First!!!
#endif



extern funcptr *MEX_C_LIB_FUNC_NAME_STD;





#endif // __MEX_CONFIG_H__

#endif // MEX_TARGET_SERVER
