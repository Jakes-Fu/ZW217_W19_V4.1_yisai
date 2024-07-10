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
** 22/12/2009     jian.ma           Create
****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmipim_export.h"
#ifdef PIM_SUPPORT
#include "mmipim_wintab.h"
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :打开号簿管家调试参数设置窗口
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIM_OpenDebugMenuWin(void)
{
    MMIPIM_OpenDebugMenuWin();
}
/*****************************************************************************/
//  Description :MMIAPIPIM_ResetFactorySetting
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIM_ResetFactorySetting(void)
{
    MMIPIM_ResetFactorySetting();
}

/*****************************************************************************/
//  FUNCTION:      MMIPIM_GetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author:   
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPIM_GetServerConfig(server_setting_info_type * p_server_setting)
{
    return MMIPIM_GetServerConfig(p_server_setting);
}

/*****************************************************************************/
//  FUNCTION:      MMIPIM_SetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_SetServerConfig(server_setting_info_type * p_server_setting)
{
    MMIPIM_SetServerConfig(p_server_setting);
}
/*****************************************************************************/
//  FUNCTION:      MMIAPIPIM_Main
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_Main(PIM_ENTRY_TYPE entry_type)
{
    MMIPIM_Main(entry_type);
}

/*****************************************************************************/
//  FUNCTION:      MMIPIM_OpenNetQueryWinFromPB
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_OpenNetQueryWinFromPB(void)
{
    MMIPIM_OpenNetQueryWinFromPB();
}

/*****************************************************************************/
//  FUNCTION:      MMIAPIPIM_InitModule
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_InitModule(void)
{
    MMIPIM_InitModule();
}
 /*****************************************************************************/
//  Description : 初始化Pim的全局变量、回调函数等
//  Global resource dependence : 
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPIPIM_InitApp (void)
{
    MMIPIM_InitApp();
}
#else
/*****************************************************************************/
//  Description :打开号簿管家调试参数设置窗口
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIM_OpenDebugMenuWin(void)
{
}
/*****************************************************************************/
//  Description :MMIAPIPIM_ResetFactorySetting
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIM_ResetFactorySetting(void)
{
}

/*****************************************************************************/
//  FUNCTION:      MMIPIM_GetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author:   
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPIM_GetServerConfig(server_setting_info_type * p_server_setting)
{
    return FALSE;
}
/*****************************************************************************/
//  FUNCTION:      MMIPIM_SetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_SetServerConfig(server_setting_info_type * p_server_setting)
{

}

/*****************************************************************************/
//  FUNCTION:      MMIAPIPIM_Main
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_Main(PIM_ENTRY_TYPE entry_type)
{
}
/*****************************************************************************/
//  FUNCTION:      MMIPIM_OpenNetQueryWinFromPB
// 	Description :   
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_OpenNetQueryWinFromPB(void)
{

}
/*****************************************************************************/
//  FUNCTION:      MMIAPIPIM_InitModule
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPIM_InitModule(void)
{
    
}
 /*****************************************************************************/
//  Description : 初始化Pim的全局变量、回调函数等
//  Global resource dependence : 
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPIPIM_InitApp (void)
{

}
#endif
#if defined(__cplusplus)
}
#endif

