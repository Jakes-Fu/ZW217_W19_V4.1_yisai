/*****************************************************************************
** File Name:      tp_export.h                                                   *
** Author:                                                                   *
** Date:           10/24/2005                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe csk2 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2005       wancan.you       Create
******************************************************************************/

#ifndef _TP_EXPORT_H_
#define _TP_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : coordinate window open
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPITP_COORDINATE_OpenWin(BOOLEAN is_first);  // 是否第一次开机
                                                                

/*****************************************************************************/
// 	Description : 开机时初始化触摸屏参数
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPITP_COORDINATE_Init(void);

/*****************************************************************************/
// 	Description : intial tp module  
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITP_InitModule(void);

/*****************************************************************************/
// 	Description : 是否时第一次开机
//	Global resource dependence : 
//  Author: Robert
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITP_IsPowerOnFirst(void);

/*****************************************************************************/
// 	Description : 清除已经设置的坐标value
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
//PUBLIC void TP_COORDINATE_ResetValue(void);
PUBLIC void MMIAPITP_Coordinate_ResetValue(void);

/*****************************************************************************/
//  Description : is coordinate win open
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITP_Coordinate_IsWinOpen(void);


/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMITP_Coordinate_ResetValue				     MMIAPITP_Coordinate_ResetValue
#define MMITP_Coordinate_IsWinOpen				     MMIAPITP_Coordinate_IsWinOpen
#define MMIPTSCR_PreCheck				                 MMIAPIPTSCR_PreCheck
#define MMIPTSCR_GetRunMode				             MMIAPIPTSCR_GetRunMode

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


