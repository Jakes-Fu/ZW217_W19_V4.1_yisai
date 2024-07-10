/****************************************************************************
** File Name:      mmiikeybackup_wintab.h                                   *
** Author:         haiwu.chen                                               *
** Date:           12/25/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/25/2011     haiwu.chen        Create                                 *
****************************************************************************/

#ifdef IKEYBACKUP_SUPPORT

#ifndef _MMIIKEYBACKUP_WINTAB_H_
#define _MMIIKEYBACKUP_WINTAB_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiikeybackup_api.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 销毁主窗口，退出实例
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 本函数算是一键备份还原的UI层实例退出
/*****************************************************************************/
PUBLIC void MMIIKB_UITerminate(void);

/*****************************************************************************/
//  Description : MMIIKB_AppendRadioListItem
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIKB_AppendRadioListItem(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T  text_1,
    uint32 user_data
);

/*****************************************************************************/
//  Description : MMIIKB_AppendCheckBoxListItem
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIKB_AppendCheckBoxListItem(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T  text_1,
    uint32 user_data
);

/*****************************************************************************/
//  Description : MMIIKB_SetListMarkable
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_SetListMarkable(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_markable
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //IKEYBACKUP_SUPPORT

#endif //_MMIIKEYBACKUP_WINTAB_H_


