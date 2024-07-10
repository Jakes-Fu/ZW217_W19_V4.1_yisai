#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************
** File Name:      mmidyna_export.h                                                   *
** Author:                                                                   *
** Date:           20/08/2006                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe csk2 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2008       peng                Create
******************************************************************************/

#ifndef _MMIDYNA_EXPORT_H_
#define _MMIDYNA_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef enum
{
    MMIDYNA_ID_MENU_START = (MMI_MODULE_DYNA_APPLIST<<16),
    
    ID_DYNA_OPT_DETAIL, 
    ID_DYNA_OPT_DELAPP,
    ID_DYNA_OPT_ADDAPP,
    
    MMIDYNA_ID_MENU_MAX
}MMIDYNA_MENU_ID_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    DYNA_MENU_ID_START = (MMI_MODULE_DYNA_APPLIST<< 16),       
    #include "mmidyna_menutable.def"
    DYNA_MAX_MENU_ID
} MMIDYNA_MENU_LABEL_E;

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         INTERFACE TO OTHER MMI MODULE                                      *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :  进入小程序列表窗口
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDYNA_OpenMainListWindow(void);

/*****************************************************************************/
// 	Description :  安装应用
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDYNA_InstallApp( const uint16* full_file_name );

/*****************************************************************************/
// 	Description : initialize DYNA module
//	Global resource dependence : none
//  Author: peng.chen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDYNA_InitModule(void);

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIDYNA_OpenMainListWindow  MMIAPIDYNA_OpenMainListWindow
#define MMIDYNA_InstallApp          MMIAPIDYNA_InstallApp

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef   __cplusplus
    }
#endif

#endif
#endif

