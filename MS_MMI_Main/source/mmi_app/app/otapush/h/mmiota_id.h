/****************************************************************************
** File Name:      mmiota_id.h                                             *
** Author:         minghu.mao                                                 *
** Date:           08/10/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2009        minghu.mao          create
** 
****************************************************************************/

#ifndef _MMIOTA_ID_H
#define _MMIOTA_ID_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

        
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
// window ID
// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: MMICC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
    MMIOTA_WIN_ID_START = (MMI_MODULE_OTA << 16),
    #include "mmiota_id.def"
    MMIOTA_MAX_WIN_ID
}MMIOTA_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIOTA_CTRL_ID_START = MMIOTA_MAX_WIN_ID,
    MMIOTA_CONFIG_INFO_DETAIL_CTRL_ID,
    MMIOTA_CONFIG_SET_MENU_CTRL_ID,
    MMIOTA_CONFIG_ITEM_DETAIL_CTRL_ID,
    MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID,
    MMIOTA_MENU_PUSHMSG_CTRL_ID,
    MMIOTA_COM_SELECT_SIM_CTRL_ID,
    
    MMIOTA_MAX_CTRL_ID
}MMIOTA_CONTROL_ID_E;
#ifdef MMI_OTA_SUPPORT
/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : Register acc menu group
//    Global resource dependence : none
//  Author: haiyang.hu
//    Note:
/*****************************************************************************/
PUBLIC void MMIOTA_RegWinIdNameArr(void);

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }

#endif
#endif  /* _MMIMMS_ID_H*/
