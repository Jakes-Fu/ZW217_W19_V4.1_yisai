/*****************************************************************************
** File Name:      mmiim_internal.h                                          *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/

#ifndef _MMIIM_INTERNAL_H_
#define _MMIIM_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "guilcd.h"
#include "mmiim_base.h"

#if defined IM_ENGINE_CSTAR
#include "csk2api.h"
#endif

#if defined IM_ENGINE_SOGOU
#include "WD_def.h"
#endif

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
// 模糊音标志
#define MMIIM_MAX_PY_FUZZY      10
#define MMIIM_DEFAULT_PY_FUZZY  0

#define MMIIM_FUZZY_Z_ZH        0x0001    /* 打开模糊音 z zh*/
#define MMIIM_FUZZY_C_CH        0x0002    /* 打开模糊音 c ch*/
#define MMIIM_FUZZY_S_SH        0x0004    /* 打开模糊音 s sh */
#define MMIIM_FUZZY_H_F         0x0008    /* 打开模糊音 h f */
#define MMIIM_FUZZY_N_L         0x0010    /* 打开模糊音 n l */
#define MMIIM_FUZZY_IN_ING      0x0020    /* 打开模糊音 in ing */
#define MMIIM_FUZZY_EN_ENG      0x0040    /* 打开模糊音 en eng */
#define MMIIM_FUZZY_AN_ANG      0x0080    /* 打开模糊音 an ang */
#define MMIIM_FUZZY_IAN_IANG    0x0100    /* 打开模糊音 ian iang */
#define MMIIM_FUZZY_UAN_UANG    0x0200    /* 打开模糊音 uan uang */


#define MMIIM_ASSO_CHS_TYPE         1       //简体
#define MMIIM_ASSO_CHT_TYPE         0       //繁体

// 输入法的版本，以下5个宏有且只能定义一个
#if defined(LOW_MEMORY_SUPPORT) 
	#if defined LOW_MEMORY_SUPPORT_32X16
	#define INPUT_METHOD_CSTAR_TINY       // 最小版本
	#else
	#define INPUT_METHOD_CSTAR_MINI       // 小版本
	//#define INPUT_METHOD_CSTAR_MINI_GBK   // GBK小版本
	#endif
#else
#define INPUT_METHOD_CSTAR_PLAIN        // 普通版本
//#define INPUT_METHOD_CSTAR_GBK        // GB版本
#endif

#if defined(INPUT_METHOD_CSTAR_MINI) || defined(INPUT_METHOD_CSTAR_MINI_GBK) || defined(INPUT_METHOD_CSTAR_TINY)
#define INPUT_METHOD_CSTAR_COMMON_MINI
#endif

// 输入法的特殊丝印
#if defined MMI_SPECIAL_ARABIC_PERSIAN
#define SPECIAL_ARABIC_PERSIAN        // this is for ETSI arabic and persian
#endif
#if defined MMI_SPECIAL_THAI
#define SPECIAL_THAI                // this is for THAI 
#endif
#if defined MMI_SPECIAL_HINDI
#define SPECIAL_HINDI           
#endif 

//输入法自绘键盘占全键盘百分比
#define MMIIM_PANEL_PERCENT_BIG (52)
#ifdef ADULT_WATCH_SUPPORT
#define MMIIM_PANEL_PERCENT_NORMAL (70)
#else
#define MMIIM_PANEL_PERCENT_NORMAL (46)
#endif
#define MMIIM_PANEL_PERCENT_SMALL (40)

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
#define MMIIM_HW_FS_PANEL_PERCENT_BIG (36)
#define MMIIM_HW_FS_PANEL_PERCENT_NORMAL (30)
#define MMIIM_HW_FS_PANEL_PERCENT_SMALL (24)
#endif

#if (defined (IM_ENGINE_T9)&& defined (MMI_IME_SMART_TYPE_SUPPORT) && defined (IM_UDB_SUPPORT))
#define MMI_IME_USER_DATABASE
#endif

#if (defined (IM_ENGINE_XT9)&& defined (MMI_IME_SMART_TYPE_SUPPORT) && defined (IM_UDB_SUPPORT))
#define MMI_IME_XT9_USER_DATABASE
#endif


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//手写识别允许的识别速度
typedef enum
{
    MMIIM_HW_SPEED_FAST,
    MMIIM_HW_SPEED_NORMAL,
    MMIIM_HW_SPEED_SLOW,
    MMIIM_HW_SPEED_MAX
} MMIIM_HW_SPEED_E;

//手写识别粗细
typedef enum
{
    MMIIM_HW_THICKNESS_THICK,
    MMIIM_HW_THICKNESS_NORMAL,
    MMIIM_HW_THICKNESS_THIN,
    MMIIM_HW_THICKNESS_MAX
} MMIIM_HW_THICKNESS_E;

//手写识别颜色
typedef enum
{
    MMIIM_HW_COLOR_1,
    MMIIM_HW_COLOR_2,
    MMIIM_HW_COLOR_3,
    MMIIM_HW_COLOR_4,
    MMIIM_HW_COLOR_5,
    MMIIM_HW_COLOR_6,
    MMIIM_HW_COLOR_7,
    MMIIM_HW_COLOR_8,
    MMIIM_HW_COLOR_MAX
} MMIIM_HW_COLOR_E;

//Handwriting color button
typedef struct
{
    GUI_RECT_T          v_rect;
    GUI_RECT_T          h_rect;
    GUI_COLOR_T         pressed_color;
    GUI_COLOR_T         released_color;
    MMI_CTRL_ID_T       ctrl_id;
} MMIIM_HW_COLOR_BUTTON_T;

//键盘大小定义
typedef enum
{
    MMIIM_KEYBOARD_SIZE_BIG,
    MMIIM_KEYBOARD_SIZE_NORMAL,
    MMIIM_KEYBOARD_SIZE_SMALL,
    MMIIM_KEYBOARD_SIZE_MAX
} MMIIM_KEYBOARD_SIZE_T;

//手写键盘区域类型定义
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
typedef enum 
{
    MMIIM_HW_AREA_TYPE_HALF_SCREEN,
    MMIIM_HW_AREA_TYPE_FULL_SCREEN,
    MMIIM_HW_AREA_TYPE_MAX,
}MMIIM_HW_AREA_TYPE_E;
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get im pinyin fuzzy
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIM_GetPyFuzzy(void);

/*****************************************************************************/
//  Description : set im pinyin fuzzy
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetPyFuzzy(
                             uint16     pinyin_fuzzy
                             );


/*****************************************************************************/
//  Description : get im handwriting speed
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMIIM_HW_SPEED_E MMIIM_GetHwSpeed(void);

/*****************************************************************************/
//  Description : set im handwriting speed
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetHwSpeed(
                             MMIIM_HW_SPEED_E   hw_speed
                             );

/*****************************************************************************/
//  Description : get im handwriting thickness
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMIIM_HW_THICKNESS_E MMIIM_GetHwThickness(void);

/*****************************************************************************/
//  Description : set im handwriting thickness
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetHwThickness(
                                 MMIIM_HW_THICKNESS_E   hw_thickness
                                 );

/*****************************************************************************/
//  Description : get im handwriting color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIIM_GetHwColor(void);

/*****************************************************************************/
//  Description : set im handwriting color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetHwColor(
                             GUI_COLOR_T    hw_color
                             );

#ifdef MMI_IM_PDA_SUPPORT
PUBLIC MMIIM_KEYBOARD_SIZE_T MMIIM_GetKeyboardSize(void);
PUBLIC void MMIIM_SetKeyboardSize(MMIIM_KEYBOARD_SIZE_T kb_size);
#endif

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
/*==============================================================================
Description: MMIAPIIM_HWGetAreaType
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC MMIIM_HW_AREA_TYPE_E MMIAPIIM_HWGetAreaType (void);


/*==============================================================================
Description: MMIAPIIM_HWSetAreaType
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void MMIAPIIM_HWSetAreaType (MMIIM_HW_AREA_TYPE_E area_type);
#endif

/*****************************************************************************/
//  Description : 获取键盘类型
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 0: 9key; 1: 26key
/*****************************************************************************/
PUBLIC uint16 MMIIM_GetKeyBoradType(void);

/*****************************************************************************/
//  Description : 存储键盘类型
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 0: 9key; 1: 26key
/*****************************************************************************/
PUBLIC void MMIIM_SetKeyBoradType(uint16 kb_type);

#ifdef IM_ENGINE_CSTAR
/******************************************************************************/
// Description: 转化输入法类型
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
PUBLIC CS_UINT MMIIM_ConvertMethod(GUIIM_METHOD_T method);

/*****************************************************************************/
//  Description : MMIIM_SaveUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIM_IME_FindUserCacheByIM(
    GUIIM_METHOD_T method,
    void **user_data,
    uint *user_data_size
);
#endif

#if ((defined IM_ENGINE_CSTAR) || (defined IM_ENGINE_T9))
/*****************************************************************************/
//  Description : MMIIM_GetImKerState
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIIM_GetImKerState(void);

/*****************************************************************************/
//  Description : MMIIM_SetImKerState
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIM_SetImKerState(uint16 im_initialized);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
