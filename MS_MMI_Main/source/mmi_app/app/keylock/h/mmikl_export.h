/*****************************************************************************
** File Name:      mmikl_export.h                                             *
** Author:         murphy.xie                                                 *
** Date:           02/06/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe kelock export interface            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012        murphy.xie	       Create
******************************************************************************/

#ifndef _MMI_KEYLOCK_EXPORT_H_
#define _MMI_KEYLOCK_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmiset_lock_effect_export.h"
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

#define  KEY_UNLOCK_CONFIRM_TIME             3000//3s

#define  KEYLOCK_COUNT_STRING_SIZE     5

#ifndef ARGB_GET_A
#define ARGB_GET_A(_argb) (((_argb) >> 24) & 0xff)
#endif
#ifndef ARGB_GET_R
#define ARGB_GET_R(_argb) (((_argb) >> 16) & 0xff)
#endif
#ifndef ARGB_GET_G
#define ARGB_GET_G(_argb) (((_argb) >> 8) & 0xff)
#endif
#ifndef ARGB_GET_B
#define ARGB_GET_B(_argb) ((_argb) & 0xff)
#endif

#ifndef RGB888_TO_RGB565
#define RGB888_TO_RGB565(r, g, b)  (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | ((b >> 3) & 0x1f))
#endif

#ifndef BLEND_MASK
#define BLEND_MASK 0x7E0F81F
#endif

#define ACLOCK12_TIME      12

#define GET_12STYLE_HOUR(hour, is_am)    \
do  \
{   \
    if (0 == (hour))    \
    {   \
        (hour) = ACLOCK12_TIME;    \
    }   \
    else if((hour) >= ACLOCK12_TIME)   \
    {   \
        if ((hour) > ACLOCK12_TIME)    \
        {   \
            (hour) = (hour) - ACLOCK12_TIME;   \
        }   \
        (is_am) = FALSE;    \
    }	\
}while(0);

typedef enum
{
    KL_LOCKED,              //锁定。此时界面左软键显示"解锁"。
    KL_UNLOCKED_ONCE,       //解锁操作一次。此时界面右软键显示"解锁"。
    KL_UNLOCKED,            //解锁。此时应该关闭锁定窗口。
    KL_PREVIEW,             //预览界面
    KL_STATUS_MAX
}MMI_KL_WIN_STATUS_E;

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT

//#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
#define MMI_KL_PASSWARD_POINT_MAX 10
//#endif

#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
//#define KEY_LOCK_PSW_LEN    4
#define MMI_KL_PSW_LEN    4

#define MMI_KL_OPEN_PSW_INPUT_ENTER     0x10
#define MMI_KL_OPEN_PSW_INPUT_SWITCHOFF     0x20
#define MMI_KL_OPEN_PSW_INPUT_CHANGE    0x30

#endif

#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef void (*MMI_PSW_FUNC)(void);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : key lock init
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_Init(void);

/*****************************************************************************/
// 	Description : lock phone
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_LockPhone(void);

/*****************************************************************************/
// 	Description : unlock phone
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_UnlockPhone(void);

/*****************************************************************************/
// 	Description : get whether phone is locked
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_IsPhoneLocked(void);

/*****************************************************************************/
// 	Description : set new sms arrived status
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_SetNewSMSStatus(void);

#if defined(MMI_VCARD_SUPPORT)
/*****************************************************************************/
// 	Description : set new vcard arrived status
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_SetNewVcardStatus(void);
#endif

#if defined(MMI_VCALENDAR_SUPPORT)
/*****************************************************************************/
// 	Description : set new vcalendar arrived status
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_SetNewVcalendarStatus(void);
#endif

/*****************************************************************************/
// 	Description : set new mms arrived status
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
// PUBLIC BOOLEAN MMIKL_SetNewMMSStatus(void);

/*****************************************************************************/
// 	Description : set missed call status
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_SetMissedCallStatus(void);

/*****************************************************************************/
// 	Description : send minute and hour interrupt to kl window
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKL_SendHourOrMinuteInd(void);

/******************************************************************************/
// 	Description : check whether key lock window is on top
//	Global resource dependence : none
//  Author: 
//	Note:	If ture: when user press left or right soft key, I will response at once
//			If false : I will make no response
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_IsKLWinOnTop(void);

/*****************************************************************************/
// 	Description : set locked status
//	Global resource dependence :
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
// PUBLIC void MMIKL_SetLockedStatus(void);

/*****************************************************************************/
//  Description : set is later handle slide key
//  Global resource dependence :
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKL_SetIsLaterHandleSlideKey(BOOLEAN is_later_handle_slide);

/*****************************************************************************/
//  Description : play key lock ring
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKL_PlayKeyLockRing(void);

/*****************************************************************************/
//  Description : get is later handle slide key
//  Global resource dependence :
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_GetIsLaterHandleSlideKey(void);

/*****************************************************************************/
//  Description : open lock preview phone
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKL_OpenPreviewLockPhone(uint16 cur_item_index);

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
/******************************************************************************/
//  Description : handle kl password setting.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_HandleKLPswInputWinMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        );

/******************************************************************************/
//  Description : Open Psw window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKL_OpenPswWin(BOOLEAN is_from_app, MMI_PSW_FUNC call_back);

/******************************************************************************/
//  Description : Close Psw window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_IsNeedPsw(void);

/******************************************************************************/
//  Description : Open Psw window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_IsPswWinOpen(void);

/******************************************************************************/
//  Description : Close Psw window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKL_ClosePswWin(void);

#endif
#endif


/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#if 1  //def MMI_FUNC_COMPATIBLE_SUPPORT
#define MMIKL_Init                          MMIAPIKL_Init
#define MMIKL_LockPhone                     MMIAPIKL_LockPhone
#define MMIKL_UnlockPhone                   MMIAPIKL_UnlockPhone
#define MMIKL_IsPhoneLocked                 MMIAPIKL_IsPhoneLocked
#define MMIKL_SetNewSMSStatus               MMIAPIKL_SetNewSMSStatus
#if defined(MMI_VCARD_SUPPORT)
#define MMIKL_SetNewVcardStatus            MMIAPIKL_SetNewVcardStatus
#endif
#if defined(MMI_VCALENDAR_SUPPORT)
#define MMIKL_SetNewVcalendarStatus        MMIAPIKL_SetNewVcalendarStatus
#endif
#define MMIKL_SetMissedCallStatus          MMIAPIKL_SetMissedCallStatus
#define MMIKL_SendHourOrMinuteInd          MMIAPIKL_SendHourOrMinuteInd
#define MMIKL_IsKLWinOnTop                  MMIAPIKL_IsKLWinOnTop
#define MMIKL_SetIsLaterHandleSlideKey     MMIAPIKL_SetIsLaterHandleSlideKey
#define MMIKL_PlayKeyLockRing               MMIAPIKL_PlayKeyLockRing
#define MMIKL_GetIsLaterHandleSlideKey     MMIAPIKL_GetIsLaterHandleSlideKey
#define MMIKL_OpenPreviewLockPhone          MMIAPIKL_OpenPreviewLockPhone
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
#define MMIKL_HandleKLPswInputWinMsg        MMIAPIKL_HandleKLPswInputWinMsg
#define MMIKL_OpenPswWin                     MMIAPIKL_OpenPswWin
#define MMIKL_IsPswWinOpen                   MMIAPIKL_IsPswWinOpen
#define MMIKL_ClosePswWin                    MMIAPIKL_ClosePswWin
#endif
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_KEYLOCK_EXPORT_H_

