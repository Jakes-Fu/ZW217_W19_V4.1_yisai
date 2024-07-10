/*****************************************************************************
** File Name:      mmiset_lock_effect_internal.h                                                  *
** Author:                                                                   *
** Date:           25/05/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2004       Jassmine.Meng       Create
** 02/2012       xiyuan.ma           Edite
******************************************************************************/
#ifndef _MMISET_LOCK_EFFECT_INTERNAL_H_
#define _MMISET_LOCK_EFFECT_INTERNAL_H_ 

#include "mmiset_lock_effect_export.h"

#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISET_GetEnableKeyLock(void);
#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : Set key lock control's param.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetKeyLockParam(void);
#endif

#if 0
/*****************************************************************************/
//  Description : lock  set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_CreateKeyLockEffectWin(void);
#endif

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
