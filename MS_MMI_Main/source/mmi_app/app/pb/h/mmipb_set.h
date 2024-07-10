/*****************************************************************************
** File Name:      mmipb_set.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with standard data format and specific    *
                   physical device data format conversation*
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012        xiaohua.liu       Create
*****************************************************************************/

#ifndef PB_SET_H_
#define PB_SET_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmipb_adapt.h"
#include "mmi_signal_ext.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

 /**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    PB_SAVE_POSITION_PHONE,
    PB_SAVE_POSITION_SIM_BEGAIN,
    PB_SAVE_POSITION_SIM_END = PB_SAVE_POSITION_SIM_BEGAIN + MMI_DUAL_SYS_MAX -1,    
    PB_SAVE_POSITION_ALWAYS_QUERY,
    PB_SAVE_POSITION_MAX
}PB_SAVE_POSITION_TYPE_E;

typedef enum
{
    MMIPB_CONTACT_DISPLAY_ALL,
    MMIPB_CONTACT_DISPLAY_PHONE,
    MMIPB_CONTACT_DISPLAY_SIM1,
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIPB_CONTACT_DISPLAY_SIM2,      
    #if (defined MMI_MULTI_SIM_SYS_TRI||defined MMI_MULTI_SIM_SYS_QUAD)
    MMIPB_CONTACT_DISPLAY_SIM3,       
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    MMIPB_CONTACT_DISPLAY_SIM4,
    #endif
    #endif
    MMIPB_DISPLAY_MAX
}MMIPB_CONTACT_DISPLAY_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建pb设置保存位置选择窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenSetSavePosWin(MMI_HANDLE_T applet_handle);
 /*****************************************************************************/
// 	Description : read new contact save positon
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC PB_SAVE_POSITION_TYPE_E  MMIPB_ReadSavePos(void);
/*****************************************************************************/
// 	Description : read contact list display option, 按bit位计算，bit0表示phone,bit1表示sim
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_DISPLAY_TYPE_E  MMIPB_ReadDisplayOpt(void);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create message
//  Global resource dependence : none
//  Author: gao.ping
//  Note: 
/*****************************************************************************/
PUBLIC void MMICL_CreateDetailMessageChildWin(MMI_WIN_ID_T	parent_win_id);
#endif

#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
/*****************************************************************************/
//  Description : 创建pb 设置显示选项选择窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenSetDisplayOptWin(MMI_HANDLE_T applet_handle);
#endif

#ifdef   __cplusplus
    }
#endif

#endif
