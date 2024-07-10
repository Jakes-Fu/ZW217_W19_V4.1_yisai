/***************************************************************************
** File Name:      mmismscb_api.c                                          *
** Author:                                                                 *
** Date:           04/2006                                                   *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to define the api function.           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2006          Tracy Zhang       Modularize
****************************************************************************/
#if defined(MMI_SMSCB_SUPPORT)

/**------------------------------------------------------------------------*
**                         Include Files                                   *
**-------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_export.h"
#include "mmismscb_wintab.h"

/**------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                             *
**-------------------------------------------------------------------------*/
/***************************************************************************/
//     Description : 初始化小区广播接口函数(在手机初始化时调用)
//    Global resource dependence : none
//  Author:肖凡
//    Note:
/***************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPISMSCB_Init(MN_DUAL_SYS_E dual_sys)
{
    return MMISMSCB_Init(dual_sys);
}

/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenMainMenu(void)
{
    MMISMSCB_OpenMainMenu();
}

/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_RestoreFactorySetting(void)
{
    MMISMSCB_SwitchSmsCBStatus(FALSE); //恢复出厂设置，关闭小区广播。    
}

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenSettingWin(void)
{
    MMISMSCB_OpenSettingWin();
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_OpenCBList(void)
{
    MMISMSCB_OpenCBList();

    return TRUE;
}

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set SMS CB Status
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SwitchSmsCBStatus(BOOLEAN cb_enabled)
{
    MMISMSCB_SwitchSmsCBStatus(cb_enabled);
}
#endif
#endif//#if defined(MMI_SMSCB_SUPPORT)

