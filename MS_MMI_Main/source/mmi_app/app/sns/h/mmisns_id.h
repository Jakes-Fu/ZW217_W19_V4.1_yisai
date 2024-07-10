/*****************************************************************************
** File Name:      mmisns_id.h                                           *
** Author:          CBK                                                  *
** Date:             31/12/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser id              *
*****************************************************************************/
#ifdef SNS_SUPPORT
#ifndef MMISNS_ID_H_
#define MMISNS_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmisns_wintable.h"

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
 
#define WIN_ID_DEF(win_id)          win_id

#define MMISNS_MODULE_ID MMI_MODULE_MMISNS
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MMI_SNS_BEGIN_WIN_ID = (MMI_MODULE_MMISNS<< 16),                   // begin
    #include "mmisns_id.def"
    MMISNS_WINDOW_ID_NUMBER          // 窗口ID的总数目
}MMISNS_WINDOW_ID_E;

typedef enum
{
       MMISNS_ACCOUNTLIST1_CTRL_ID=MMISNS_WINDOW_ID_NUMBER,
       MMISNS_LIST_ACC_CTRL_ID,
       MMISNS_LABEL_ADDRESS_CTRL_ID,
	MMISNS_FORM_ADDTYPE_CTRL_ID,
	MMISNS_SUBFORM1_ADDTYPE_CTRL_ID,
	MMISNS_SUBFORM2_ADDTYPE_CTRL_ID,
	MMISNS_BUTTON_TYPE_TWIT_CTRL_ID,
	MMISNS_BUTTON_TYPE_FB_CTRL_ID,
	MMISNS_BUTTON_TYPE_SINA_CTRL_ID,
	MMISNS_LABEL_ACC_ADD_NAME_CTRL_ID,
	MMISNS_LABEL_ACC_ADD_PSW_CTRL_ID,	
	MMISNS_EDIT_ACC_ADD_NAME_CTRL_ID,
	MMISNS_EDIT_ACC_ADD_PSW_CTRL_ID,
	MMISNS_LABEL_ACC_DETAIL_NAME_CTRL_ID,
	MMISNS_LABEL_ACC_DETAIL_PSW_CTRL_ID,	
	MMISNS_EDIT_ACC_DETAIL_NAME_CTRL_ID,
	MMISNS_EDIT_ACC_DETAIL_PSW_CTRL_ID,
	MMISNS_OPTMENU_ACC_LISTOPT_CTRL_ID,
	MMISNS_EDIT_CTRL_ID,
	MMISNS_LABEL_PASSWORD_CTRL_ID,
	MMISNS_EDIT_PASSWORD_CTRL_ID,
       MMISNS_FORM_ACC_DETAIL_CTRL_ID,
	MMISNS_SUBFORM1_ACC_DETAIL_CTRL_ID,
	MMISNS_SUBFORM2_ACC_DETAIL_CTRL_ID,	
	MMISNS_SUBFORM3_ACC_DETAIL_CTRL_ID
}MMISNS_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
