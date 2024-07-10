/*****************************************************************************
** File Name:      mmiqbtheme_export.h                                      *
** Author:         cheng.luo                                               *
** Date:           2011/09/17                                                 *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is header of qbtheme                                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/09/17     cheng.luo 	     Creat
******************************************************************************/

#ifndef _MMI_QBTHEME_H_
#define _MMI_QBTHEME_H_

/**--------------------------------------------------------------------------*
 **                         Include files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mn_type.h"
//#include "mmiset.h"
#include "mmiset_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         CONSTANT DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define STYLE_IS_SUPPORT_QBTHEME   MMIAPIQBTHEME_IsSupport(MMIAPISET_GetIdleStyle())
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DECLARE                        *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIAPIQBTHEME_InitModule(void);
PUBLIC void MMIAPIQBTHEME_Init(void);
PUBLIC BOOLEAN MMIAPIQBTHEME_IsIconHaveBg(void);
PUBLIC BOOLEAN MMIAPIQBTHEME_IsDefaultTheme(void);
PUBLIC BOOLEAN MMIAPIQBTHEME_GetNeedTransform(void);
PUBLIC BOOLEAN MMIAPIQBTHEME_Install(const wchar* pkg_path_ptr);
PUBLIC void MMIAPIQBTHEME_OpenDownLoadUrl(void);
PUBLIC void MMIAPIQBTHEME_OpenMainWin(void);
PUBLIC MMI_HANDLE_T MMIAPIQBTHEME_OpenWallpaperWin(void);
PUBLIC void MMIAPIQBTHEME_StyleStart(void);
PUBLIC void MMIAPIQBTHEME_StyleStop(void);
PUBLIC BOOLEAN MMIAPIQBTHEME_IsSupport(MMISET_IDLE_STYLE_E style);
PUBLIC BOOLEAN IsVersionMatch(uint32 ver);

//qbtheme模块
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIQBTHEME_IsIconHaveBg             MMIAPIQBTHEME_IsIconHaveBg
#define MMIQBTHEME_IsDefaultTheme           MMIAPIQBTHEME_IsDefaultTheme
#define MMIQBTHEME_Install                  MMIAPIQBTHEME_Install
#define MMIQBTHEME_OpenDownLoadUrl          MMIAPIQBTHEME_OpenDownLoadUrl
#define MMIQBTHEME_OpenMainWin              MMIAPIQBTHEME_OpenMainWin
#define MMIQBTHEME_OpenWallpaperWin         MMIAPIQBTHEME_OpenWallpaperWin
#define MMIQBTHEME_StyleStart               MMIAPIQBTHEME_StyleStart
#define MMIQBTHEME_StyleStop                MMIAPIQBTHEME_StyleStop
#define MMIQBTHEME_IsSupport                MMIAPIQBTHEME_IsSupport

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMI_QBTHEME_H_
