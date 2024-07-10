/*****************************************************************************
** File Name:      mmiset_lock_effect_export.h                                                  *
** Author:                                                                   *
** Date:           10/02/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2004       Jassmine.Meng       Create
** 02/2010       xiyuan.ma           Edite
******************************************************************************/
#ifndef _MMISET_LOCK_EFFECT_EXPORT_H_
#define _MMISET_LOCK_EFFECT_EXPORT_H_ 

/**---------------------------------------------------------------------------*
** Macro Declaration *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMISET_OpenKeyLockEffect5SubWin      MMIAPISET_OpenKeyLockEffect5SubWin
#define MMISET_SetKeyLockPassward            MMIAPISET_SetKeyLockPassward
#define MMISET_CreateLockStyleWin            MMIAPISET_CreateLockStyleWin
#define MMISET_GetKeyLockPassward            MMIAPISET_GetKeyLockPassward
#define MMISET_GetVisibleKeyLock             MMIAPISET_GetVisibleKeyLock
#define MMISET_SetEnableKeyLock              MMIAPISET_SetEnableKeyLock

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

typedef enum 
{
/* Ripple style key unlock */
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
    MMISET_KEY_LOCK_EFFECT0,
#endif

/* Slide down unlock style with arc picture */
#ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT        
    MMISET_KEY_LOCK_EFFECT1,
#endif

/* Slide right arc bar unlock style */
#ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
    MMISET_KEY_LOCK_EFFECT2,
#endif

/* Slide down the rectangle bar unlock style */
#ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT
    MMISET_KEY_LOCK_EFFECT3,
#endif

/* Slide right block unlock style */
#ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT
    MMISET_KEY_LOCK_EFFECT4,
#endif

/* Pattern unlock style */
#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
    MMISET_KEY_LOCK_EFFECT5,
#endif

/* Icon moving unlock style */
#ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT
    MMISET_KEY_LOCK_EFFECT6,
#endif
/* fruit split unlock style */
#ifdef  MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT
    MMISET_KEY_LOCK_FRUIT_SPLIT,
#endif
//michael wang
#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
     MMISET_KEY_LOCK_DOUBLE_CIRCLE,
#endif

#ifdef MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
     MMISET_KEY_LOCK_DOUBLE_CIRCLE_SPEC,
#endif
#ifdef MMI_KEY_LOCK_EFFECT_QQVGA
     MMISET_KEY_LOCK_QQVGA_SPEC,
#endif  
//michael wang
    MMISET_KEY_LOCK_EFFECT_MAX
}MMISET_KEY_LOCK_EFFECT_E;

#ifndef MMI_KEY_LOCK_EFFECT_QQVGA
#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
#define MMISET_KEY_LOCK_DEFAULT MMISET_KEY_LOCK_DOUBLE_CIRCLE
#elif defined MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
#define MMISET_KEY_LOCK_DEFAULT MMISET_KEY_LOCK_EFFECT0
#else
    #if defined MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT
        #define MMISET_KEY_LOCK_DEFAULT MMISET_KEY_LOCK_EFFECT1
    #else
        #define MMISET_KEY_LOCK_DEFAULT MMISET_KEY_LOCK_EFFECT_MAX
    #endif
#endif

#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
#define MMISET_KEY_LOCK_ASTYLE     MMISET_KEY_LOCK_DOUBLE_CIRCLE
#else
    #if defined MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
        #define MMISET_KEY_LOCK_ASTYLE MMISET_KEY_LOCK_EFFECT2
    #else
        #define MMISET_KEY_LOCK_ASTYLE MMISET_KEY_LOCK_EFFECT_MAX
    #endif
#endif


#ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT
#define MMISET_KEY_LOCK_ISTYLE MMISET_KEY_LOCK_EFFECT4
#else 
    #if defined MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT
    #define MMISET_KEY_LOCK_ISTYLE MMISET_KEY_LOCK_EFFECT1
    #else
    #define MMISET_KEY_LOCK_ISTYLE MMISET_KEY_LOCK_EFFECT_MAX
    #endif
#endif


/*****************************************************************************/
// 	Description : Get key lock display style
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
PUBLIC MMISET_KEY_LOCK_EFFECT_E MMIAPISET_GetKeyLockEffect(void);

/*****************************************************************************/
// 	Description : Set key lock display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT_E effect);

#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
/*****************************************************************************/
//  Description : open key lock effect5 sun win
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_OpenKeyLockEffect5SubWin(void);

/*****************************************************************************/
// 	Description : MMIAPISET_SetKeyLockPassward
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetKeyLockPassward(int8* kl_passward);

/*****************************************************************************/
// 	Description : MMIAPISET_GetKeyLockPassward
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetKeyLockPassward(int8* kl_passward);

/*****************************************************************************/
//  Description : MMIAPISET_GetVisibleKeyLock
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPISET_GetVisibleKeyLock(void);

/*****************************************************************************/
//  Description : MMIAPISET_SetEnableKeyLock
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_SetEnableKeyLock(BOOLEAN is_enable_keylock) ;
#endif

/*****************************************************************************/
// 	Description : MMIAPISET_CreateLockStyleWin
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateLockStyleWin(void);

/*****************************************************************************/
// 	Description : Get key lock display style
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note:
/*****************************************************************************/
PUBLIC MMISET_KEY_LOCK_EFFECT_E MIAPISET_GetKeyLockEffect(void);

/*****************************************************************************/
// 	Description : Set key lock display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT_E effect);



/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif    
#endif
