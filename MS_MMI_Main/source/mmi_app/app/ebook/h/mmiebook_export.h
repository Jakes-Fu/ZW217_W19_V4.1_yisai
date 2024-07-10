/*****************************************************************************
** File Name:      mmiebook_export.h                                         *
** Author:                                                                   *
** Date:           03/02/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ebook                       *          
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       songbin.zeng       Create
** 02/2012       xiyuan.ma          Edit
******************************************************************************/


#ifndef _MMIEBOOK_EXPORT_H_
#define _MMIEBOOK_EXPORT_H_
#ifdef EBOOK_SUPPORT
#include "mmi_filemgr.h"

typedef enum
{
	MMIEBOOK_CODING_TYPE_GB,
	MMIEBOOK_CODING_TYPE_BIG5,
	MMIEBOOK_CODING_TYPE_MAX
}MMIEBOOK_SET_CODING_TYPE_E;//枚举可供设置的编码方式

typedef enum
{
	MMIEBOOK_FONT_BIG,
	MMIEBOOK_FONT_NORMAL,
	MMIEBOOK_FONT_SMALL,
	MMIEBOOK_FONT_SIZE_MAX
}MMIEBOOK_FONT_SIZE_E;//枚举可供设置的字体大小

typedef enum//自动浏览时间
{
	MMIEBOOK_AUTO_READ_OFF,
	MMIEBOOK_AUTO_READ_5S,
	MMIEBOOK_AUTO_READ_15S,
	MMIEBOOK_AUTO_READ_30S,
	MMIEBOOK_AUTO_READ_60S,
	MMIEBOOK_AUTO_READ_MAX
}MMIEBOOK_SET_AUTO_READ_E;//枚举可供设置的编码方式

/*****************************************************************************/
//  Description : 电子书入口函数
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_ManagerWinMsg (BOOLEAN is_need_privacy_protection);

/*****************************************************************************/
//  Description : 电子书模块初始化
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_Init(void);

/*****************************************************************************/
//  Description : 电子书从我的文档进入的入口函数
//  Global resource dependence : 
//  Author: rongjin
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_ManagerWinMsg_4Fmm (
                                            uint8 file_dev, 
                                            const wchar* name_ptr,
                                            uint32 name_length, 
                                            uint32 file_size,
                                            const wchar* full_name_ptr, 
                                            uint32 full_name_length
                                            );
                                            

/*****************************************************************************/
//  Description : 判断指定的文件时候在使用和已经被打开
//  Global resource dependence : 
//  Author: 
//  Parameter: full_path_name_ptr[IN]:指定的文件名
//             full_path_name_len[IN]:指定文件的全路径名长度
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
/*PUBLIC BOOLEAN MMIAPIEBOOK_IsFileUsed(
                                      const wchar *full_path_name_ptr,
                                      const uint16 full_path_name_len
                                      );*/

/*****************************************************************************/
//  Description : 创建所有存储设备电子书目录
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIAPIEBOOK_CreateAllDevEbookDir(void);

/*****************************************************************************/
//  FUNCTION:      MMIAPIEBOOK_InitModule
// 	Description :   
//	Global resource dependence : 
//  Author:        maggie.ren
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_InitModule(void);

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_StopOperaSDPlug(void);


  /*****************************************************************************/
 //     Description : 获得编码方式
 // Global resource dependence : 
 //  Author: zhaohui
 // Note:
 /*****************************************************************************/
 PUBLIC MMIEBOOK_SET_CODING_TYPE_E MMIAPIEBOOK_GetCodingType( void );
  
 /*****************************************************************************/
 // 	Description : get the info of browse page methed
 //	Global resource dependence : 
 //  Author: zhaohui
 //	Note:
 /*****************************************************************************/
 PUBLIC void MMIAPIEBOOK_GetFontType( void );
 
/*****************************************************************************/
// 	Description : MMIAPIEBOOK_IS_WaitingWinOpen 
//	Global resource dependence : 
//  Author: zhaohui
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEBOOK_IS_WaitingWinOpen(void);

/*****************************************************************************/
// 	Description : ebook restore factory setting 
//	Global resource dependence : 
//  Author: jian.ma
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEBOOK_RestoreFactorySetting(void);
/*****************************************************************************/
// 	Description : ebook restore factory setting 
//	Global resource dependence : 
//  Author: jian.ma
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_WaitExit(void);

/*****************************************************************************/
//  Description : privacy call back of ebook module 
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_ValidatePrivacyAppEntry(void);

/*****************************************************************************/
//     Description : 获得字体大小
// Global resource dependence : 
//  Author: zhaohui
// Note:
/*****************************************************************************/
PUBLIC MMIEBOOK_FONT_SIZE_E MMIAPIEBOOK_GetFontSize( void );

/*****************************************************************************/
 //     Description : 读取是否全屏显示NV
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
/*****************************************************************************/
 PUBLIC void MMIAPIEBOOK_InitIsFullScreenOff( void );

/*****************************************************************************/
//     Description : 获得自动浏览方式
// Global resource dependence : 
//  Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC MMIEBOOK_SET_AUTO_READ_E MMIAPIEBOOK_GetAutoReadType( void );

/*****************************************************************************/
// 	Description : ebook set  TTS On or OFF
//	Global resource dependence : 
//  Author: koki.gao
//	Note:   
/*****************************************************************************/
#ifdef TTS_SUPPORT
PUBLIC void MMIAPIEBOOK_SetTTSOnOFF(BOOLEAN isrun);
#endif

/*****************************************************************************/
//  Description : Stop ebook applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_StopApplet(BOOLEAN is_minimize);

/*****************************************************************************/
//  Description : privacy call back of ebook module 
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_ValidatePrivacyAppEntry(void);

/*****************************************************************************/
//  Description : MMIAPIEBOOK_IsRunning
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEBOOK_IsRunning(void);
#endif
#endif
