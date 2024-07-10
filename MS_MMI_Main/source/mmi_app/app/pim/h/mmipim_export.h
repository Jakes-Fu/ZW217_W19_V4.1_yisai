/****************************************************************************
** File Name:      mmipim_export.h                                          *
** Author:         jian.ma                                                  *
** Date:           22/12/2009                                               *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the main application.      *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 22/12/2009    jian.ma           Create
****************************************************************************/
#ifndef  _MMIPIM_EXPORT_H_    
#define  _MMIPIM_EXPORT_H_   


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef PIM_SUPPORT
#include "mmipim_text.h"
#endif
#include "mmipim.h"
#include "mmipim_pb.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :打开号簿管家调试参数设置窗口
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIM_OpenDebugMenuWin(void);

/*****************************************************************************/
//  Description :MMIAPIPIM_ResetFactorySetting
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIM_ResetFactorySetting(void);

/*****************************************************************************/
//  FUNCTION:      MMIPIM_GetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author:   
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPIM_GetServerConfig(server_setting_info_type * p_server_setting);

/*****************************************************************************/
//  FUNCTION:      MMIPIM_SetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_SetServerConfig(server_setting_info_type * p_server_setting);

/*****************************************************************************/
//  FUNCTION:      MMIAPIPIM_Main
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_Main(PIM_ENTRY_TYPE entry_type);
/*****************************************************************************/
//  FUNCTION:      MMIPIM_OpenNetQueryWinFromPB
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_OpenNetQueryWinFromPB(void);

/*****************************************************************************/
//  FUNCTION:      MMIAPIPIM_InitModule
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_InitModule(void);
 /*****************************************************************************/
//  Description : 初始化Pim的全局变量、回调函数等
//  Global resource dependence : 
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPIPIM_InitApp (void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
