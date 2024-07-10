/***************************************************************************
** File Name:      mmifdn_export.h                                                                               *
** Author:                                                                                                                    *
** Date:           09/07/2009                                                                                          *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                             *
** Description:    This file is used to fdn define                                      *
****************************************************************************
**                         Important Edit History                                                                       *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                  *
** 09/2009        wancan.you    Create
****************************************************************************/
#ifndef  _MMIFDN_EXPORT_H_    
#define  _MMIFDN_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_signal_ext.h"
#include "mmipb_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum//FDN菜单的操作项
{
    MMIFDN_OPER_NONE,
    MMIFDN_ADD_OPER,
    MMIFDN_EDIT_OPER,
    MMIFDN_DEL_OPER,
    MMIFDN_ENABLE_OPER,
    MMIFDN_DISABLE_OPER,
    MMIFDN_VIEW_EDIT_OPER,
    MMIFDN_OPER_MAX	
}MMIFDN_OPERATE_E; 

/*FDN的相关信息全局变量*/
typedef struct _MMIFDN_CONTEXT_T_
{
    MMIFDN_OPERATE_E oper_type;
    BOOLEAN is_usim;
    BOOLEAN is_fdn_enable;//是否使能关闭FDN	
    BOOLEAN is_fdn_exist;//if support FDN	
    BOOLEAN is_adn_enable;//是否使能关闭ADN	
    MMIPB_STORAGE_INFO_T fdn_info;//fdn info
    uint16 	entry_id;//记录的id号
    uint8* entry_state;
}MMIFDN_CONTEXT_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#ifdef FDN_SUPPORT
   /*****************************************************************************/
//  Description : set reread phonebook state
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_SetReReadPhoneBookState(BOOLEAN is_reread);

  /*****************************************************************************/
//  Description : get reread phonebook state
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFDN_GetReReadPhoneBookState(void);

 /*****************************************************************************/
//  Description : set operate fdn SIM id
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_SetCurSIM(MN_DUAL_SYS_E dual_sys);
#endif
#if 0
 /*****************************************************************************/
//  Description : get operate fdn SIM id
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIFDN_GetCurSIM(void);
#endif
#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : FDN Initial
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_Init(void);

/*****************************************************************************/
// 	Description : Handle FDN Message
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_HandleCallReadyInd(DPARAM param);

/*****************************************************************************/
// 	Description : Initial FDN Info From PB
//	Global resource dependence : none
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_InitFDNInfo(MN_DUAL_SYS_E dual_sys);
#endif
/*****************************************************************************/
// 	Description : Get FDN Context
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIFDN_CONTEXT_T * MMIAPIFDN_GetFDNContext(MN_DUAL_SYS_E dual_sys);
#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : 判断是否是2G fdn开启
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFDN_Is2GFdnEnable(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : if fdn enable
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFDN_IsFdnExist(MN_DUAL_SYS_E dual_sys);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif



