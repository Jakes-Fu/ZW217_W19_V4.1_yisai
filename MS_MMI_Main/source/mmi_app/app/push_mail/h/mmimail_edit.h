/***************************************************************************
** File Name:      mmimail_edit.h                                          *
** Author:         liyuanyuan                                              *
** Date:           2010/05/07                                              *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.        *
** Description:    This file is used to define module internal relative    *
**                 contents.                                               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIMAIL_EDIT_H_    
#define  _MMIMAIL_EDIT_H_   

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "mmimail_internal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**----------------------------------------------------------------------*
 **                         MACRO DEFINITION                             *
 **----------------------------------------------------------------------*/

/**----------------------------------------------------------------------*
 **                         TYPE AND STRUCT                              *
 **----------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         FUNCTION DECLARATION                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : 保存邮件结束处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SaveFinished(BOOLEAN is_success);

/*****************************************************************************/
//	Description : 发送邮件结束处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SendFinished(BOOLEAN is_success);

/*****************************************************************************/
//	Description : 销毁 editbody 占用的空间
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_DestroyEditBody(MAIL_MESSAGE_T * editbody_ptr);

#ifdef   __cplusplus
    }
#endif
#endif