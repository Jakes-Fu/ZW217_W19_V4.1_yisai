/****************************************************************************
** File Name:      mmiikeybackup_id.h                                       *
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

#ifndef _MMIIKEYBACKUP_ID_H_
#define _MMIIKEYBACKUP_ID_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"


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
#define WIN_ID_DEF(win_id)          win_id

// window ID
typedef enum
{
    MMIIKB_WIN_ID_START = (MMI_MODULE_IKEYBACKUP << 16),

#include "mmiikeybackup_id.def"

    MMIIKB_MAX_WIN_ID
}MMIIKB_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIIKB_CTRL_ID_ID_START = MMIIKB_MAX_WIN_ID,
    MMIIKB_TAB_CTRL_ID,
    MMIIKB_BACKUP_LIST_FORM_CTRL_ID,
    MMIIKB_BACKUP_LIST_CTRL_ID,
    MMIIKB_RESTORE_LIST_FORM_CTRL_ID,
    MMIIKB_RESTORE_LIST_CTRL_ID,
    MMIIKB_RESTORE_ITEM_SEL_FORM_CTRL_ID,
    MMIIKB_RESTORE_ITEM_SEL_CTRL_ID,
    MMIIKB_SETTING_LIST_FORM_CTRL_ID,
    MMIIKB_SETTING_LIST_CTRL_ID,
    MMIIKB_BACKUP_POS_LIST_CTRL_ID,
    MMIIKB_ENCRYPT_DATA_FORM_CTRL_ID,
    MMIIKB_ENCRYPT_DATA_LIST_CTRL_ID,
    MMIIKB_ENTER_PSW_FORM_CTRL_ID,
    MMIIKB_PSW_LABEL_CTRL_ID,
    MMIIKB_SET_BACKUP_PSW_CTRL_ID,
    MMIIKB_PSW_VERFY_LABEL_CTRL_ID,
    MMIIKB_SET_BACKUP_VERFY_PSW_CTRL_ID,
    MMIIKB_ENTER_RES_PSW_FORM_CTRL_ID,
    MMIIKB_RES_PSW_LABEL_CTRL_ID,
    MMIIKB_SET_RESTORE_PSW_CTRL_ID,
    MMIIKB_RESTORE_LIST_LONGOK_OPT_CTRL_ID,
    MMIIKB_ID_MENU_DELETE_PACK,
    MMIIKB_RESTORE_LIST_OPT_CTRL_ID,
    MMIIKB_ID_MENU_LEFT_OPT_RESTORE,
    MMIIKB_ID_MENU_LEFT_OPT_DELETE_PACK,
    MMIIKB_ID_MENU_LEFT_OPT_DELETE_ALL_PACK,
    MMIIKB_RES_DETAIL_FORM_CTRL_ID,
    MMIIKB_RES_DETAIL_FINISH_LABEL_CTRL_ID,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_0 = MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START+0,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_1 = MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START+1,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_2 = MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START+2,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_3 = MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START+3,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_4 = MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START+4,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_5 = MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START+5,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_6 = MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START+6,
    MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_END,
    
    MMIIKB_MAX_CTRL_ID
}MMIIKB_CONTROL_ID_E;



/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

 /*****************************************************************************/
//  Description : register 1-keybackup wintab name
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_RegWinIdNameArr(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //IKEYBACKUP_SUPPORT

#endif //_MMIIKEYBACKUP_ID_H_


