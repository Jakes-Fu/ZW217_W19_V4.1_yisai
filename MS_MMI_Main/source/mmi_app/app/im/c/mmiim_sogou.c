/*******************************************************************************
** File Name: mmiim_sogou.c
** Author: Ming.Song
** Date: 2011/04/9
** Copyright: 2011 Sogou, Incorporated. All Rights Reserved.
** Description:处理sogou键盘输入的应用
********************************************************************************
** Edit History
**------------------------------------------------------------------------------
** DATE         NAME            DESCRIPTION
** 2011/04/9   Ming.Song  Create.
*******************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmi_app_im_trc.h"
#include "mmitheme_pos.h"
#include "mmiim_sogou.h"
#include "mmiim.h"
#include "mmiim_nv.h"

#if defined(MMIIM_USE_KB_SOGOU)
#include "guires.h"
#include "mmk_msg.h"
#include "mmiim_im_switching.h"
#include "mmiim_sp_symbol.h"
#include "wdp_update_wintab.h"
#include "jpeg_interface.h"
#include "block_mem.h"
#include "mmiim_text.h"
#include "mmiim_image.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SKIN_FILEPATH_LEN 10
#define MIN_SKIN_FILE_BUF_SIZE (50 * 1024)
#define IME_LIST_MAX        10
#define SG_BLOCK_MEM_POOL_PIC_SOURCE_SIZE      (MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*2)  //lcd width*height*2//2011-7-27

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

// 搜狗输入法引擎用全局变量
PUBLIC uint8        g_FlashData[FLASH_CAPACITY] = { 0 };

// 搜狗输入法指针变量
PUBLIC  WD_imeResult g_imeResult = { 0 };
PUBLIC uint8 g_sg_save_position = 1;//0:D盘,1:E盘,2:非D非E//皮肤放到E盘//2012_11_6
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
// 普通输入法键盘分布类型
LOCAL const unsigned char s_common_app_keymap[] =
{
    WDK_NORMAL_Q, WDK_NORMAL_W, WDK_NORMAL_E, WDK_NORMAL_R, WDK_NORMAL_T, WDK_NORMAL_Y, WDK_NORMAL_U, WDK_NORMAL_I, WDK_NORMAL_O, WDK_NORMAL_P,
    WDK_NORMAL_A, WDK_NORMAL_S, WDK_NORMAL_D, WDK_NORMAL_F, WDK_NORMAL_G, WDK_NORMAL_H, WDK_NORMAL_J, WDK_NORMAL_K, WDK_NORMAL_L, WDK_ENTER,
    WDK_NORMAL_Z, WDK_NORMAL_X, WDK_NORMAL_C, WDK_NORMAL_V, WDK_NORMAL_B, WDK_NORMAL_N, WDK_NORMAL_M, ',',          '.',          SOGOU_VK_NONE, 0x0F
};

// 笔画输入法下键盘分布类型
#if defined QWERTY_KEYPAD_BLACKBERRY
LOCAL const unsigned short s_stroke_app_keymap[] =
{
    SOGOU_VK_NONE, WDK_NORMAL_W, WDK_NORMAL_E, WDK_NORMAL_R, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE,
    SOGOU_VK_NONE, WDK_NORMAL_S, WDK_NORMAL_D, WDK_NORMAL_F, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE,
    WDK_NORMAL_Z,  WDK_NORMAL_X, WDK_NORMAL_C, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE
};
#elif defined QWERTY_KEYPAD_CUST
LOCAL const unsigned short s_stroke_app_keymap[] =
{
    SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, WDK_NORMAL_R, WDK_NORMAL_T, WDK_NORMAL_Y, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE,
    SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, WDK_NORMAL_F, WDK_NORMAL_G, WDK_NORMAL_H, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE,
    SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE,  SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE
};
#else
LOCAL const unsigned short s_stroke_app_keymap[] =
{
    SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, WDK_NORMAL_W, WDK_NORMAL_E, WDK_NORMAL_R, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE,
    SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, WDK_NORMAL_S, WDK_NORMAL_D, WDK_NORMAL_F, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE,
    SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, WDK_NORMAL_Z,  WDK_NORMAL_X, WDK_NORMAL_C, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE, SOGOU_VK_NONE
};
#endif

#endif

// 皮肤图片指针
LOCAL GUIIMG_BITMAP_T* s_sg_skin_bitmap_ptr = NULL;
// 是否使用默认皮肤
LOCAL BOOLEAN s_is_use_default_skin = TRUE;
/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                            */
/*---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: Initialize
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr);

/******************************************************************************/
// Description: Terminate
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL BOOLEAN Terminate(void *handle);

/******************************************************************************/
// Description: HandleMsg
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleMsg(void *handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *event_data_ptr);

/******************************************************************************/
// Description: 处理系统消息
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsg(MMIIM_SOGOU_DATA_T *data_ptr, GUIIM_EVENT_DATA_U *event_data_ptr);

/******************************************************************************/
// Description: SetMethod
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL BOOLEAN SetMethod(void *handle, GUIIM_METHOD_T method);

/******************************************************************************/
// Description: 设置输入法显示区域
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void SetImeDisplayArea(MMIIM_SOGOU_DATA_T *data_ptr);

/******************************************************************************/
// Description: 转化按键信息
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL uint32 ConvertKey(MMIIM_SOGOU_DATA_T *data_ptr, GUIIM_EVENT_DATA_U *event_data_ptr, SG_UINT *pEvent);

/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void DrawPreEditWin(MMIIM_SOGOU_DATA_T *data_ptr, BOOLEAN is_draw_immediately);

/******************************************************************************/
// Description: 选字窗口
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void DrawLetterWin(MMIIM_SOGOU_DATA_T *data_ptr, BOOLEAN is_draw_immediately);

/******************************************************************************/
// Description: 选字窗口
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void DrawCandWin(MMIIM_SOGOU_DATA_T *data_ptr, BOOLEAN is_draw_immediately);

#ifdef SOGOU_USE_PDA_SKIN
/******************************************************************************/
// Description: 显示搜狗皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
LOCAL BOOLEAN SG_DisplaySkin(MMIIM_SOGOU_DATA_T *data_ptr);
#else
/******************************************************************************/
// Description: 如果不存在皮肤，则需要绘制背景，否则字符刷新时，
//                    候选字等会出现重叠
// Global resource dependence: NONE
// Author: yaoguang.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN SG_DisplayBg (MMIIM_SOGOU_DATA_T *data_ptr);
#endif
/*****************************************************************************/
//  Description : 是否平台自带输入法
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPlatfromNativeIME(GUIIM_METHOD_T method);

/*****************************************************************************/
//  Description : 打开切换输入法窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenSwitchIMWin(MMIIM_SOGOU_DATA_T *data_ptr, GUI_BOTH_RECT_T *pop_menu_rect_ptr);

/*****************************************************************************/
//  Description : 打开符号窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenSysbolWin(MMIIM_SOGOU_DATA_T *data_ptr);

#ifdef SOGOU_USE_PDA_SKIN
/******************************************************************************/
// Description: 设置皮肤为默认皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
LOCAL void SG_SetupDefaultSkin(void);
#endif
/******************************************************************************/
// Description: 获取当前输入法库控制指针
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
LOCAL WD_imeResult* SOGOU_GetImeResult(MMIIM_SOGOU_DATA_T *data_ptr);

/*******************************************************************************************/
//  Description : get last disp
//  Global resource dependence :
//  Author:
//  Note:
/*********************************************************************************************/
LOCAL uint8 SOGOU_GetLastDisp(MMIIM_SOGOU_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : ime init
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint8 SOGOU_ImeInit(MMIIM_SOGOU_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : 获取当前输入法ID号
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint8 SOGOU_GetInputMethod(MMIIM_SOGOU_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : 处理输入法按键消息
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint8 SOGOU_HandleIMEOnKeyEvent(MMIIM_SOGOU_DATA_T *data_ptr,uint16  sg_vk);

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
PUBLIC GUIIM_KERNEL_APIS_T const g_guiim_sogou_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};

LOCAL MMIIM_SOGOU_METHOD_RELATION_T const s_sogou_method_relation_list[] =
{
    {GUIIM_M_123,                           WDI_DIGIT},
    {GUIIM_M_ENGLISH_LEAD_UPPER,            WDI_ENGLISH+(2<<16)},
    {GUIIM_M_ENGLISH_LOWER,                 WDI_ENGLISH+(1<<16)},
    {GUIIM_M_ENGLISH_UPPER,                 WDI_ENGLISH+(3<<16)},
    {GUIIM_M_ENGLISH_ABC_LEAD_UPPER,        WDI_ENGLISH_ABC+(2<<16)},
    {GUIIM_M_ENGLISH_ABC_LOWER,             WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_ENGLISH_ABC_UPPER,             WDI_ENGLISH_ABC+(3<<16)},

    {GUIIM_M_Q_123,                         WDI_DIGIT},
    {GUIIM_M_Q_ENGLISH_LEAD_UPPER,          WDI_ENGLISH+(2<<16)},
    {GUIIM_M_Q_ENGLISH_LOWER,               WDI_ENGLISH+(1<<16)},
    {GUIIM_M_Q_ENGLISH_UPPER,               WDI_ENGLISH+(3<<16)},
    {GUIIM_M_Q_ENGLISH_ABC_LEAD_UPPER,      WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_Q_ENGLISH_ABC_LOWER,           WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_Q_ENGLISH_ABC_UPPER,           WDI_ENGLISH_ABC+(3<<16)},

#ifdef IM_SIMP_CHINESE_SUPPORT
    {GUIIM_M_PINYIN,                        WDI_CN_CPINYIN},
    {GUIIM_M_STROKE,                        WDI_CN_BIHUA},
    {GUIIM_M_Q_PINYIN,                      WDI_CN_FULLPINYIN},
    {GUIIM_M_Q_STROKE,                      WDI_CN_BIHUA},
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
    {GUIIM_M_ZHUYIN,                        WDI_CN_ZHUYIN},
    {GUIIM_M_STROKE_CHT,                    WDI_CN_BIHUAF},
    {GUIIM_M_Q_ZHUYIN,                      WDI_CN_FULLZHUYIN},
    {GUIIM_M_Q_STROKE_CHT,                  WDI_CN_BIHUAF},
#endif

#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_RUSSIAN_LEAD_UPPER,             WDI_RUSSIAN+(2<<16)},
    {GUIIM_M_RUSSIAN_LOWER,                  WDI_RUSSIAN+(1<<16)},
    {GUIIM_M_RUSSIAN_UPPER,                  WDI_RUSSIAN+(3<<16)},
    {GUIIM_M_RUSSIAN_ABC_LEAD_UPPER,              WDI_RUSSIAN_ABC+(2<<16)},
    {GUIIM_M_RUSSIAN_ABC_LOWER,              WDI_RUSSIAN_ABC+(1<<16)},
    {GUIIM_M_RUSSIAN_ABC_UPPER,              WDI_RUSSIAN_ABC+(3<<16)},

    {GUIIM_M_Q_RUSSIAN_LEAD_UPPER,           WDI_RUSSIAN+(2<<16)},
    {GUIIM_M_Q_RUSSIAN_LOWER,                WDI_RUSSIAN+(1<<16)},
    {GUIIM_M_Q_RUSSIAN_UPPER,                WDI_RUSSIAN+(3<<16)},
    {GUIIM_M_Q_RUSSIAN_ABC_LEAD_UPPER,            WDI_RUSSIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_RUSSIAN_ABC_LOWER,            WDI_RUSSIAN_ABC+(1<<16)},
    {GUIIM_M_Q_RUSSIAN_ABC_UPPER,            WDI_RUSSIAN_ABC+(3<<16)},
#endif

#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_ARABIC,                        WDI_ARABIC},
    {GUIIM_M_ARABIC_ABC,                    WDI_ARABIC_ABC},
    {GUIIM_M_Q_ARABIC,                      WDI_ARABIC},
    {GUIIM_M_Q_ARABIC_ABC,                  WDI_ARABIC_ABC},
#endif

#if defined (IM_ASSAMESE_SUPPORT)
    {GUIIM_M_ASSAMESE,                        WDI_ASSAMESE},
    {GUIIM_M_ASSAMESE_ABC,                    WDI_ASSAMESE_ABC},
    {GUIIM_M_Q_ASSAMESE,                      WDI_ASSAMESE},
    {GUIIM_M_Q_ASSAMESE_ABC,                  WDI_ASSAMESE_ABC},
#endif

#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_BENGALI,                        WDI_BENGALI},
    {GUIIM_M_BENGALI_ABC,                    WDI_BENGALI_ABC},
    {GUIIM_M_Q_BENGALI,                      WDI_BENGALI},
    {GUIIM_M_Q_BENGALI_ABC,                  WDI_BENGALI_ABC},
#endif

#if defined (IM_GEORGIAN_SUPPORT)
    {GUIIM_M_GEORGIAN,                        WDI_GEORGIAN},
    {GUIIM_M_GEORGIAN_ABC,                    WDI_GEORGIAN_ABC},
    {GUIIM_M_Q_GEORGIAN,                      WDI_GEORGIAN},
    {GUIIM_M_Q_GEORGIAN_ABC,                  WDI_GEORGIAN_ABC},
#endif

#if defined (IM_GUJARATI_SUPPORT)
    {GUIIM_M_GUJARATI,                        WDI_GUJARATI},
    {GUIIM_M_GUJARATI_ABC,                    WDI_GUJARATI_ABC},
    {GUIIM_M_Q_GUJARATI,                      WDI_GUJARATI},
    {GUIIM_M_Q_GUJARATI_ABC,                  WDI_GUJARATI_ABC},
#endif

#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_HEBREW,                        WDI_HEBREW},
    {GUIIM_M_HEBREW_ABC,                    WDI_HEBREW_ABC},
    {GUIIM_M_Q_HEBREW,                      WDI_HEBREW},
    {GUIIM_M_Q_HEBREW_ABC,                  WDI_HEBREW_ABC},
#endif

#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_HINDI,                        WDI_HINDI},
    {GUIIM_M_HINDI_ABC,                    WDI_HINDI_ABC},
    {GUIIM_M_Q_HINDI,                      WDI_HINDI},
    {GUIIM_M_Q_HINDI_ABC,                  WDI_HINDI_ABC},
#endif

#if defined (IM_KANNADA_SUPPORT)
    {GUIIM_M_KANNADA,                        WDI_KANNADA},
    {GUIIM_M_KANNADA_ABC,                    WDI_KANNADA_ABC},
    {GUIIM_M_Q_KANNADA,                      WDI_KANNADA},
    {GUIIM_M_Q_KANNADA_ABC,                  WDI_KANNADA_ABC},
#endif

#if defined (IM_MALAYALAM_SUPPORT)
    {GUIIM_M_MALAYALAM,                        WDI_MALAYALAM},
    {GUIIM_M_MALAYALAM_ABC,                    WDI_MALAYALAM_ABC},
    {GUIIM_M_Q_MALAYALAM,                      WDI_MALAYALAM},
    {GUIIM_M_Q_MALAYALAM_ABC,                  WDI_MALAYALAM_ABC},
#endif

#if defined (IM_MYANMAR_SUPPORT)
    {GUIIM_M_MYANMAR,                        WDI_BURMESE},
    {GUIIM_M_MYANMAR_ABC,                    WDI_BURMESE_ABC},
    {GUIIM_M_Q_MYANMAR,                      WDI_BURMESE},
    {GUIIM_M_Q_MYANMAR_ABC,                  WDI_BURMESE_ABC},
#endif

#if defined (IM_MARATHI_SUPPORT)
    {GUIIM_M_MARATHI,                        WDI_MARATHI},
    {GUIIM_M_MARATHI_ABC,                    WDI_MARATHI_ABC},
    {GUIIM_M_Q_MARATHI,                      WDI_MARATHI},
    {GUIIM_M_Q_MARATHI_ABC,                  WDI_MARATHI_ABC},
#endif

#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_PERSIAN,                        WDI_PERSIAN},
    {GUIIM_M_PERSIAN_ABC,                    WDI_PERSIAN_ABC},
    {GUIIM_M_Q_PERSIAN,                      WDI_PERSIAN},
    {GUIIM_M_Q_PERSIAN_ABC,                  WDI_PERSIAN_ABC},
#endif

#if defined (IM_PUNJABI_SUPPORT)
    {GUIIM_M_PUNJABI,                        WDI_PUNJABI},
    {GUIIM_M_PUNJABI_ABC,                    WDI_PUNJABI_ABC},
    {GUIIM_M_Q_PUNJABI,                      WDI_PUNJABI},
    {GUIIM_M_Q_PUNJABI_ABC,                  WDI_PUNJABI_ABC},
#endif

#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_M_TAGALOG,                        WDI_TAGALOG},
    {GUIIM_M_TAGALOG_ABC,                    WDI_TAGALOG_ABC},
    {GUIIM_M_Q_TAGALOG,                      WDI_TAGALOG},
    {GUIIM_M_Q_TAGALOG_ABC,                  WDI_TAGALOG_ABC},
#endif

#if defined (IM_TAMIL_SUPPORT)
    {GUIIM_M_TAMIL,                        WDI_TAMIL},
    {GUIIM_M_TAMIL_ABC,                    WDI_TAMIL_ABC},
    {GUIIM_M_Q_TAMIL,                      WDI_TAMIL},
    {GUIIM_M_Q_TAMIL_ABC,                  WDI_TAMIL_ABC},
#endif

#if defined (IM_TELUGU_SUPPORT)
    {GUIIM_M_TELUGU,                        WDI_TELUGU},
    {GUIIM_M_TELUGU_ABC,                    WDI_TELUGU_ABC},
    {GUIIM_M_Q_TELUGU,                      WDI_TELUGU},
    {GUIIM_M_Q_TELUGU_ABC,                  WDI_TELUGU_ABC},
#endif

#ifdef IM_THAI_SUPPORT
    {GUIIM_M_THAI,                        WDI_THAI},
    {GUIIM_M_THAI_ABC,                    WDI_THAI_ABC},
    {GUIIM_M_Q_THAI,                      WDI_THAI},
    {GUIIM_M_Q_THAI_ABC,                  WDI_THAI_ABC},
#endif

#ifdef IM_URDU_SUPPORT
    {GUIIM_M_URDU,                        WDI_URDU},
    {GUIIM_M_URDU_ABC,                    WDI_URDU_ABC},
    {GUIIM_M_Q_URDU,                      WDI_URDU},
    {GUIIM_M_Q_URDU_ABC,                  WDI_URDU_ABC},
#endif

#if defined (IM_AFRIKAANS_SUPPORT)
    {GUIIM_M_AFRIKAANS_LEAD_UPPER,             WDI_AFRIKAANS+(2<<16)},
    {GUIIM_M_AFRIKAANS_LOWER,                  WDI_AFRIKAANS+(1<<16)},
    {GUIIM_M_AFRIKAANS_UPPER,                  WDI_AFRIKAANS+(3<<16)},

    {GUIIM_M_Q_AFRIKAANS_LEAD_UPPER,           WDI_AFRIKAANS+(2<<16)},
    {GUIIM_M_Q_AFRIKAANS_LOWER,                WDI_AFRIKAANS+(1<<16)},
    {GUIIM_M_Q_AFRIKAANS_UPPER,                WDI_AFRIKAANS+(3<<16)},
#endif

#if defined (IM_AZERBAIJANI_SUPPORT)
    {GUIIM_M_AZERBAIJANI_LEAD_UPPER,             WDI_AZERBAIJANI+(2<<16)},
    {GUIIM_M_AZERBAIJANI_LOWER,                  WDI_AZERBAIJANI+(1<<16)},
    {GUIIM_M_AZERBAIJANI_UPPER,                  WDI_AZERBAIJANI+(3<<16)},
    {GUIIM_M_AZERBAIJANI_ABC_LEAD_UPPER,              WDI_AZERBAIJANI_ABC+(2<<16)},
    {GUIIM_M_AZERBAIJANI_ABC_LOWER,              WDI_AZERBAIJANI_ABC+(1<<16)},
    {GUIIM_M_AZERBAIJANI_ABC_UPPER,              WDI_AZERBAIJANI_ABC+(3<<16)},

    {GUIIM_M_Q_AZERBAIJANI_LEAD_UPPER,           WDI_AZERBAIJANI+(2<<16)},
    {GUIIM_M_Q_AZERBAIJANI_LOWER,                WDI_AZERBAIJANI+(1<<16)},
    {GUIIM_M_Q_AZERBAIJANI_UPPER,                WDI_AZERBAIJANI+(3<<16)},
    {GUIIM_M_Q_AZERBAIJANI_ABC_LEAD_UPPER,            WDI_AZERBAIJANI_ABC+(2<<16)},    
    {GUIIM_M_Q_AZERBAIJANI_ABC_LOWER,            WDI_AZERBAIJANI_ABC+(1<<16)},
    {GUIIM_M_Q_AZERBAIJANI_ABC_UPPER,            WDI_AZERBAIJANI_ABC+(3<<16)},
#endif

#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_M_BULGARIAN_LEAD_UPPER,             WDI_BULGARIAN+(2<<16)},
    {GUIIM_M_BULGARIAN_LOWER,                  WDI_BULGARIAN+(1<<16)},
    {GUIIM_M_BULGARIAN_UPPER,                  WDI_BULGARIAN+(3<<16)},
    {GUIIM_M_BULGARIAN_ABC_LEAD_UPPER,              WDI_BULGARIAN_ABC+(2<<16)},
    {GUIIM_M_BULGARIAN_ABC_LOWER,              WDI_BULGARIAN_ABC+(1<<16)},
    {GUIIM_M_BULGARIAN_ABC_UPPER,              WDI_BULGARIAN_ABC+(3<<16)},

    {GUIIM_M_Q_BULGARIAN_LEAD_UPPER,           WDI_BULGARIAN+(2<<16)},
    {GUIIM_M_Q_BULGARIAN_LOWER,                WDI_BULGARIAN+(1<<16)},
    {GUIIM_M_Q_BULGARIAN_UPPER,                WDI_BULGARIAN+(3<<16)},
    {GUIIM_M_Q_BULGARIAN_ABC_LEAD_UPPER,            WDI_BULGARIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_BULGARIAN_ABC_LOWER,            WDI_BULGARIAN_ABC+(1<<16)},
    {GUIIM_M_Q_BULGARIAN_ABC_UPPER,            WDI_BULGARIAN_ABC+(3<<16)},
#endif

#if defined (IM_CATALAN_SUPPORT)
    {GUIIM_M_CATALAN_LEAD_UPPER,             WDI_CATALAN+(2<<16)},
    {GUIIM_M_CATALAN_LOWER,                  WDI_CATALAN+(1<<16)},
    {GUIIM_M_CATALAN_UPPER,                  WDI_CATALAN+(3<<16)},
    {GUIIM_M_CATALAN_ABC_LEAD_UPPER,              WDI_CATALAN_ABC+(2<<16)},
    {GUIIM_M_CATALAN_ABC_LOWER,              WDI_CATALAN_ABC+(1<<16)},
    {GUIIM_M_CATALAN_ABC_UPPER,              WDI_CATALAN_ABC+(3<<16)},

    {GUIIM_M_Q_CATALAN_LEAD_UPPER,           WDI_CATALAN+(2<<16)},
    {GUIIM_M_Q_CATALAN_LOWER,                WDI_CATALAN+(1<<16)},
    {GUIIM_M_Q_CATALAN_UPPER,                WDI_CATALAN+(3<<16)},
    {GUIIM_M_Q_CATALAN_ABC_LEAD_UPPER,            WDI_CATALAN_ABC+(2<<16)},    
    {GUIIM_M_Q_CATALAN_ABC_LOWER,            WDI_CATALAN_ABC+(1<<16)},
    {GUIIM_M_Q_CATALAN_ABC_UPPER,            WDI_CATALAN_ABC+(3<<16)},
#endif

#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_CROATIAN_LEAD_UPPER,             WDI_CROATIAN+(2<<16)},
    {GUIIM_M_CROATIAN_LOWER,                  WDI_CROATIAN+(1<<16)},
    {GUIIM_M_CROATIAN_UPPER,                  WDI_CROATIAN+(3<<16)},
    {GUIIM_M_CROATIAN_ABC_LEAD_UPPER,              WDI_CROATIAN_ABC+(2<<16)},
    {GUIIM_M_CROATIAN_ABC_LOWER,              WDI_CROATIAN_ABC+(1<<16)},
    {GUIIM_M_CROATIAN_ABC_UPPER,              WDI_CROATIAN_ABC+(3<<16)},

    {GUIIM_M_Q_CROATIAN_LEAD_UPPER,           WDI_CROATIAN+(2<<16)},
    {GUIIM_M_Q_CROATIAN_LOWER,                WDI_CROATIAN+(1<<16)},
    {GUIIM_M_Q_CROATIAN_UPPER,                WDI_CROATIAN+(3<<16)},
    {GUIIM_M_Q_CROATIAN_ABC_LEAD_UPPER,            WDI_CROATIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_CROATIAN_ABC_LOWER,            WDI_CROATIAN_ABC+(1<<16)},
    {GUIIM_M_Q_CROATIAN_ABC_UPPER,            WDI_CROATIAN_ABC+(3<<16)},
#endif

#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_CZECH_LEAD_UPPER,             WDI_CZECH+(2<<16)},
    {GUIIM_M_CZECH_LOWER,                  WDI_CZECH+(1<<16)},
    {GUIIM_M_CZECH_UPPER,                  WDI_CZECH+(3<<16)},
    {GUIIM_M_CZECH_ABC_LEAD_UPPER,              WDI_CZECH_ABC+(2<<16)},
    {GUIIM_M_CZECH_ABC_LOWER,              WDI_CZECH_ABC+(1<<16)},
    {GUIIM_M_CZECH_ABC_UPPER,              WDI_CZECH_ABC+(3<<16)},

    {GUIIM_M_Q_CZECH_LEAD_UPPER,           WDI_CZECH+(2<<16)},
    {GUIIM_M_Q_CZECH_LOWER,                WDI_CZECH+(1<<16)},
    {GUIIM_M_Q_CZECH_UPPER,                WDI_CZECH+(3<<16)},
    {GUIIM_M_Q_CZECH_ABC_LEAD_UPPER,            WDI_CZECH_ABC+(2<<16)},    
    {GUIIM_M_Q_CZECH_ABC_LOWER,            WDI_CZECH_ABC+(1<<16)},
    {GUIIM_M_Q_CZECH_ABC_UPPER,            WDI_CZECH_ABC+(3<<16)},
#endif

#ifdef IM_DANISH_SUPPORT
    {GUIIM_M_DANISH_LEAD_UPPER,             WDI_DANISH+(2<<16)},
    {GUIIM_M_DANISH_LOWER,                  WDI_DANISH+(1<<16)},
    {GUIIM_M_DANISH_UPPER,                  WDI_DANISH+(3<<16)},
    {GUIIM_M_DANISH_ABC_LEAD_UPPER,              WDI_DANISH_ABC+(2<<16)},
    {GUIIM_M_DANISH_ABC_LOWER,              WDI_DANISH_ABC+(1<<16)},
    {GUIIM_M_DANISH_ABC_UPPER,              WDI_DANISH_ABC+(3<<16)},

    {GUIIM_M_Q_DANISH_LEAD_UPPER,           WDI_DANISH+(2<<16)},
    {GUIIM_M_Q_DANISH_LOWER,                WDI_DANISH+(1<<16)},
    {GUIIM_M_Q_DANISH_UPPER,                WDI_DANISH+(3<<16)},
    {GUIIM_M_Q_DANISH_ABC_LEAD_UPPER,            WDI_DANISH_ABC+(2<<16)},    
    {GUIIM_M_Q_DANISH_ABC_LOWER,            WDI_DANISH_ABC+(1<<16)},
    {GUIIM_M_Q_DANISH_ABC_UPPER,            WDI_DANISH_ABC+(3<<16)},
#endif

#ifdef IM_DUTCH_SUPPORT
    {GUIIM_M_DUTCH_LEAD_UPPER,             WDI_DUTCH+(2<<16)},
    {GUIIM_M_DUTCH_LOWER,                  WDI_DUTCH+(1<<16)},
    {GUIIM_M_DUTCH_UPPER,                  WDI_DUTCH+(3<<16)},
    {GUIIM_M_DUTCH_ABC_LEAD_UPPER,              WDI_DUTCH_ABC+(2<<16)},
    {GUIIM_M_DUTCH_ABC_LOWER,              WDI_DUTCH_ABC+(1<<16)},
    {GUIIM_M_DUTCH_ABC_UPPER,              WDI_DUTCH_ABC+(3<<16)},

    {GUIIM_M_Q_DUTCH_LEAD_UPPER,           WDI_DUTCH+(2<<16)},
    {GUIIM_M_Q_DUTCH_LOWER,                WDI_DUTCH+(1<<16)},
    {GUIIM_M_Q_DUTCH_UPPER,                WDI_DUTCH+(3<<16)},
    {GUIIM_M_Q_DUTCH_ABC_LEAD_UPPER,            WDI_DUTCH_ABC+(2<<16)},    
    {GUIIM_M_Q_DUTCH_ABC_LOWER,            WDI_DUTCH_ABC+(1<<16)},
    {GUIIM_M_Q_DUTCH_ABC_UPPER,            WDI_DUTCH_ABC+(3<<16)},
#endif
    
#if defined (IM_ESTONIAN_SUPPORT)
    {GUIIM_M_ESTONIAN_LEAD_UPPER,             WDI_ESTONIAN+(2<<16)},
    {GUIIM_M_ESTONIAN_LOWER,                  WDI_ESTONIAN+(1<<16)},
    {GUIIM_M_ESTONIAN_UPPER,                  WDI_ESTONIAN+(3<<16)},
    {GUIIM_M_ESTONIAN_ABC_LEAD_UPPER,              WDI_ESTONIAN_ABC+(2<<16)},
    {GUIIM_M_ESTONIAN_ABC_LOWER,              WDI_ESTONIAN_ABC+(1<<16)},
    {GUIIM_M_ESTONIAN_ABC_UPPER,              WDI_ESTONIAN_ABC+(3<<16)},

    {GUIIM_M_Q_ESTONIAN_LEAD_UPPER,           WDI_ESTONIAN+(2<<16)},
    {GUIIM_M_Q_ESTONIAN_LOWER,                WDI_ESTONIAN+(1<<16)},
    {GUIIM_M_Q_ESTONIAN_UPPER,                WDI_ESTONIAN+(3<<16)},
    {GUIIM_M_Q_ESTONIAN_ABC_LEAD_UPPER,            WDI_ESTONIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_ESTONIAN_ABC_LOWER,            WDI_ESTONIAN_ABC+(1<<16)},
    {GUIIM_M_Q_ESTONIAN_ABC_UPPER,            WDI_ESTONIAN_ABC+(3<<16)},
#endif

#if defined (IM_FINNISH_SUPPORT)
    {GUIIM_M_FINNISH_LEAD_UPPER,             WDI_FINNISH+(2<<16)},
    {GUIIM_M_FINNISH_LOWER,                  WDI_FINNISH+(1<<16)},
    {GUIIM_M_FINNISH_UPPER,                  WDI_FINNISH+(3<<16)},
    {GUIIM_M_FINNISH_ABC_LEAD_UPPER,              WDI_FINNISH_ABC+(2<<16)},
    {GUIIM_M_FINNISH_ABC_LOWER,              WDI_FINNISH_ABC+(1<<16)},
    {GUIIM_M_FINNISH_ABC_UPPER,              WDI_FINNISH_ABC+(3<<16)},

    {GUIIM_M_Q_FINNISH_LEAD_UPPER,           WDI_FINNISH+(2<<16)},
    {GUIIM_M_Q_FINNISH_LOWER,                WDI_FINNISH+(1<<16)},
    {GUIIM_M_Q_FINNISH_UPPER,                WDI_FINNISH+(3<<16)},
    {GUIIM_M_Q_FINNISH_ABC_LEAD_UPPER,            WDI_FINNISH_ABC+(2<<16)},    
    {GUIIM_M_Q_FINNISH_ABC_LOWER,            WDI_FINNISH_ABC+(1<<16)},
    {GUIIM_M_Q_FINNISH_ABC_UPPER,            WDI_FINNISH_ABC+(3<<16)},
#endif

#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_FRENCH_LEAD_UPPER,             WDI_FRENCH+(2<<16)},
    {GUIIM_M_FRENCH_LOWER,                  WDI_FRENCH+(1<<16)},
    {GUIIM_M_FRENCH_UPPER,                  WDI_FRENCH+(3<<16)},
    {GUIIM_M_FRENCH_ABC_LEAD_UPPER,              WDI_FRENCH_ABC+(2<<16)},
    {GUIIM_M_FRENCH_ABC_LOWER,              WDI_FRENCH_ABC+(1<<16)},
    {GUIIM_M_FRENCH_ABC_UPPER,              WDI_FRENCH_ABC+(3<<16)},

    {GUIIM_M_Q_FRENCH_LEAD_UPPER,           WDI_FRENCH+(2<<16)},
    {GUIIM_M_Q_FRENCH_LOWER,                WDI_FRENCH+(1<<16)},
    {GUIIM_M_Q_FRENCH_UPPER,                WDI_FRENCH+(3<<16)},
    {GUIIM_M_Q_FRENCH_ABC_LEAD_UPPER,            WDI_FRENCH_ABC+(2<<16)},    
    {GUIIM_M_Q_FRENCH_ABC_LOWER,            WDI_FRENCH_ABC+(1<<16)},
    {GUIIM_M_Q_FRENCH_ABC_UPPER,            WDI_FRENCH_ABC+(3<<16)},
#endif

#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_GERMAN_LEAD_UPPER,             WDI_GERMAN+(2<<16)},
    {GUIIM_M_GERMAN_LOWER,                  WDI_GERMAN+(1<<16)},
    {GUIIM_M_GERMAN_UPPER,                  WDI_GERMAN+(3<<16)},
    {GUIIM_M_GERMAN_ABC_LEAD_UPPER,              WDI_GERMAN_ABC+(2<<16)},
    {GUIIM_M_GERMAN_ABC_LOWER,              WDI_GERMAN_ABC+(1<<16)},
    {GUIIM_M_GERMAN_ABC_UPPER,              WDI_GERMAN_ABC+(3<<16)},

    {GUIIM_M_Q_GERMAN_LEAD_UPPER,           WDI_GERMAN+(2<<16)},
    {GUIIM_M_Q_GERMAN_LOWER,                WDI_GERMAN+(1<<16)},
    {GUIIM_M_Q_GERMAN_UPPER,                WDI_GERMAN+(3<<16)},
    {GUIIM_M_Q_GERMAN_ABC_LEAD_UPPER,            WDI_GERMAN_ABC+(2<<16)},    
    {GUIIM_M_Q_GERMAN_ABC_LOWER,            WDI_GERMAN_ABC+(1<<16)},
    {GUIIM_M_Q_GERMAN_ABC_UPPER,            WDI_GERMAN_ABC+(3<<16)},
#endif

#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_GREEK_LEAD_UPPER,             WDI_GREEK+(2<<16)},
    {GUIIM_M_GREEK_LOWER,                  WDI_GREEK+(1<<16)},
    {GUIIM_M_GREEK_UPPER,                  WDI_GREEK+(3<<16)},
    {GUIIM_M_GREEK_ABC_LEAD_UPPER,              WDI_GREEK_ABC+(2<<16)},
    {GUIIM_M_GREEK_ABC_LOWER,              WDI_GREEK_ABC+(1<<16)},
    {GUIIM_M_GREEK_ABC_UPPER,              WDI_GREEK_ABC+(3<<16)},

    {GUIIM_M_Q_GREEK_LEAD_UPPER,           WDI_GREEK+(2<<16)},
    {GUIIM_M_Q_GREEK_LOWER,                WDI_GREEK+(1<<16)},
    {GUIIM_M_Q_GREEK_UPPER,                WDI_GREEK+(3<<16)},
    {GUIIM_M_Q_GREEK_ABC_LEAD_UPPER,            WDI_GREEK_ABC+(2<<16)},    
    {GUIIM_M_Q_GREEK_ABC_LOWER,            WDI_GREEK_ABC+(1<<16)},
    {GUIIM_M_Q_GREEK_ABC_UPPER,            WDI_GREEK_ABC+(3<<16)},
#endif

#if defined (IM_HAUSA_SUPPORT)
    {GUIIM_M_HAUSA_LEAD_UPPER,             WDI_HAUSA+(2<<16)},
    {GUIIM_M_HAUSA_LOWER,                  WDI_HAUSA+(1<<16)},
    {GUIIM_M_HAUSA_UPPER,                  WDI_HAUSA+(3<<16)},
    {GUIIM_M_HAUSA_ABC_LEAD_UPPER,              WDI_HAUSA_ABC+(2<<16)},
    {GUIIM_M_HAUSA_ABC_LOWER,              WDI_HAUSA_ABC+(1<<16)},
    {GUIIM_M_HAUSA_ABC_UPPER,              WDI_HAUSA_ABC+(3<<16)},

    {GUIIM_M_Q_HAUSA_LEAD_UPPER,           WDI_HAUSA+(2<<16)},
    {GUIIM_M_Q_HAUSA_LOWER,                WDI_HAUSA+(1<<16)},
    {GUIIM_M_Q_HAUSA_UPPER,                WDI_HAUSA+(3<<16)},
    {GUIIM_M_Q_HAUSA_ABC_LEAD_UPPER,            WDI_HAUSA_ABC+(2<<16)},    
    {GUIIM_M_Q_HAUSA_ABC_LOWER,            WDI_HAUSA_ABC+(1<<16)},
    {GUIIM_M_Q_HAUSA_ABC_UPPER,            WDI_HAUSA_ABC+(3<<16)},
#endif

#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_HUNGARIAN_LEAD_UPPER,             WDI_HUNGARIAN+(2<<16)},
    {GUIIM_M_HUNGARIAN_LOWER,                  WDI_HUNGARIAN+(1<<16)},
    {GUIIM_M_HUNGARIAN_UPPER,                  WDI_HUNGARIAN+(3<<16)},
    {GUIIM_M_HUNGARIAN_ABC_LEAD_UPPER,              WDI_HUNGARIAN_ABC+(2<<16)},
    {GUIIM_M_HUNGARIAN_ABC_LOWER,              WDI_HUNGARIAN_ABC+(1<<16)},
    {GUIIM_M_HUNGARIAN_ABC_UPPER,              WDI_HUNGARIAN_ABC+(3<<16)},

    {GUIIM_M_Q_HUNGARIAN_LEAD_UPPER,           WDI_HUNGARIAN+(2<<16)},
    {GUIIM_M_Q_HUNGARIAN_LOWER,                WDI_HUNGARIAN+(1<<16)},
    {GUIIM_M_Q_HUNGARIAN_UPPER,                WDI_HUNGARIAN+(3<<16)},
    {GUIIM_M_Q_HUNGARIAN_ABC_LEAD_UPPER,            WDI_HUNGARIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_HUNGARIAN_ABC_LOWER,            WDI_HUNGARIAN_ABC+(1<<16)},
    {GUIIM_M_Q_HUNGARIAN_ABC_UPPER,            WDI_HUNGARIAN_ABC+(3<<16)},
#endif

#if defined (IM_ICELANDIC_SUPPORT)
    {GUIIM_M_ICELANDIC_LEAD_UPPER,             WDI_ICELANDIC+(2<<16)},
    {GUIIM_M_ICELANDIC_LOWER,                  WDI_ICELANDIC+(1<<16)},
    {GUIIM_M_ICELANDIC_UPPER,                  WDI_ICELANDIC+(3<<16)},
    {GUIIM_M_ICELANDIC_ABC_LEAD_UPPER,              WDI_ICELANDIC_ABC+(2<<16)},
    {GUIIM_M_ICELANDIC_ABC_LOWER,              WDI_ICELANDIC_ABC+(1<<16)},
    {GUIIM_M_ICELANDIC_ABC_UPPER,              WDI_ICELANDIC_ABC+(3<<16)},

    {GUIIM_M_Q_ICELANDIC_LEAD_UPPER,           WDI_ICELANDIC+(2<<16)},
    {GUIIM_M_Q_ICELANDIC_LOWER,                WDI_ICELANDIC+(1<<16)},
    {GUIIM_M_Q_ICELANDIC_UPPER,                WDI_ICELANDIC+(3<<16)},
    {GUIIM_M_Q_ICELANDIC_ABC_LEAD_UPPER,            WDI_ICELANDIC_ABC+(2<<16)},    
    {GUIIM_M_Q_ICELANDIC_ABC_LOWER,            WDI_ICELANDIC_ABC+(1<<16)},
    {GUIIM_M_Q_ICELANDIC_ABC_UPPER,            WDI_ICELANDIC_ABC+(3<<16)},
#endif

#if defined (IM_IGBO_SUPPORT)
    {GUIIM_M_IGBO_LEAD_UPPER,             WDI_IGBO+(2<<16)},
    {GUIIM_M_IGBO_LOWER,                  WDI_IGBO+(1<<16)},
    {GUIIM_M_IGBO_UPPER,                  WDI_IGBO+(3<<16)},
    {GUIIM_M_IGBO_ABC_LEAD_UPPER,              WDI_IGBO_ABC+(2<<16)},
    {GUIIM_M_IGBO_ABC_LOWER,              WDI_IGBO_ABC+(1<<16)},
    {GUIIM_M_IGBO_ABC_UPPER,              WDI_IGBO_ABC+(3<<16)},

    {GUIIM_M_Q_IGBO_LEAD_UPPER,           WDI_IGBO+(2<<16)},
    {GUIIM_M_Q_IGBO_LOWER,                WDI_IGBO+(1<<16)},
    {GUIIM_M_Q_IGBO_UPPER,                WDI_IGBO+(3<<16)},
    {GUIIM_M_Q_IGBO_ABC_LEAD_UPPER,            WDI_IGBO_ABC+(2<<16)},    
    {GUIIM_M_Q_IGBO_ABC_LOWER,            WDI_IGBO_ABC+(1<<16)},
    {GUIIM_M_Q_IGBO_ABC_UPPER,            WDI_IGBO_ABC+(3<<16)},
#endif

#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_INDONESIAN_LEAD_UPPER,             WDI_INDONESIAN+(2<<16)},
    {GUIIM_M_INDONESIAN_LOWER,                  WDI_INDONESIAN+(1<<16)},
    {GUIIM_M_INDONESIAN_UPPER,                  WDI_INDONESIAN+(3<<16)},
    {GUIIM_M_INDONESIAN_ABC_LEAD_UPPER,              WDI_INDONESIAN_ABC+(2<<16)},
    {GUIIM_M_INDONESIAN_ABC_LOWER,              WDI_INDONESIAN_ABC+(1<<16)},
    {GUIIM_M_INDONESIAN_ABC_UPPER,              WDI_INDONESIAN_ABC+(3<<16)},

    {GUIIM_M_Q_INDONESIAN_LEAD_UPPER,           WDI_INDONESIAN+(2<<16)},
    {GUIIM_M_Q_INDONESIAN_LOWER,                WDI_INDONESIAN+(1<<16)},
    {GUIIM_M_Q_INDONESIAN_UPPER,                WDI_INDONESIAN+(3<<16)},
    {GUIIM_M_Q_INDONESIAN_ABC_LEAD_UPPER,            WDI_INDONESIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_INDONESIAN_ABC_LOWER,            WDI_INDONESIAN_ABC+(1<<16)},
    {GUIIM_M_Q_INDONESIAN_ABC_UPPER,            WDI_INDONESIAN_ABC+(3<<16)},
#endif

#if defined (IM_IRISH_SUPPORT)
    {GUIIM_M_IRISH_LEAD_UPPER,             WDI_IRISH+(2<<16)},
    {GUIIM_M_IRISH_LOWER,                  WDI_IRISH+(1<<16)},
    {GUIIM_M_IRISH_UPPER,                  WDI_IRISH+(3<<16)},
    {GUIIM_M_IRISH_ABC_LEAD_UPPER,              WDI_IRISH_ABC+(2<<16)},
    {GUIIM_M_IRISH_ABC_LOWER,              WDI_IRISH_ABC+(1<<16)},
    {GUIIM_M_IRISH_ABC_UPPER,              WDI_IRISH_ABC+(3<<16)},

    {GUIIM_M_Q_IRISH_LEAD_UPPER,           WDI_IRISH+(2<<16)},
    {GUIIM_M_Q_IRISH_LOWER,                WDI_IRISH+(1<<16)},
    {GUIIM_M_Q_IRISH_UPPER,                WDI_IRISH+(3<<16)},
    {GUIIM_M_Q_IRISH_ABC_LEAD_UPPER,            WDI_IRISH_ABC+(2<<16)},    
    {GUIIM_M_Q_IRISH_ABC_LOWER,            WDI_IRISH_ABC+(1<<16)},
    {GUIIM_M_Q_IRISH_ABC_UPPER,            WDI_IRISH_ABC+(3<<16)},
#endif

#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_ITALIAN_LEAD_UPPER,            WDI_ITALIAN+(2<<16)},
    {GUIIM_M_ITALIAN_LOWER,                 WDI_ITALIAN+(1<<16)},
    {GUIIM_M_ITALIAN_UPPER,                 WDI_ITALIAN+(3<<16)},
    {GUIIM_M_ITALIAN_ABC_LOWER,             WDI_ITALIAN_ABC+(1<<16)},
    {GUIIM_M_ITALIAN_ABC_UPPER,             WDI_ITALIAN_ABC+(3<<16)},

    {GUIIM_M_Q_ITALIAN_LEAD_UPPER,          WDI_ITALIAN+(2<<16)},
    {GUIIM_M_Q_ITALIAN_LOWER,               WDI_ITALIAN+(1<<16)},
    {GUIIM_M_Q_ITALIAN_UPPER,               WDI_ITALIAN+(3<<16)},
    {GUIIM_M_Q_ITALIAN_ABC_LOWER,           WDI_ITALIAN_ABC+(1<<16)},
    {GUIIM_M_Q_ITALIAN_ABC_UPPER,           WDI_ITALIAN_ABC+(3<<16)},
#endif

#if defined (IM_KAZAKH_SUPPORT)
    {GUIIM_M_KAZAKH_LEAD_UPPER,            WDI_KAZAKH+(2<<16)},
    {GUIIM_M_KAZAKH_LOWER,                 WDI_KAZAKH+(1<<16)},
    {GUIIM_M_KAZAKH_UPPER,                 WDI_KAZAKH+(3<<16)},
    {GUIIM_M_KAZAKH_ABC_LOWER,             WDI_KAZAKH_ABC+(1<<16)},
    {GUIIM_M_KAZAKH_ABC_UPPER,             WDI_KAZAKH_ABC+(3<<16)},

    {GUIIM_M_Q_KAZAKH_LEAD_UPPER,          WDI_KAZAKH+(2<<16)},
    {GUIIM_M_Q_KAZAKH_LOWER,               WDI_KAZAKH+(1<<16)},
    {GUIIM_M_Q_KAZAKH_UPPER,               WDI_KAZAKH+(3<<16)},
    {GUIIM_M_Q_KAZAKH_ABC_LOWER,           WDI_KAZAKH_ABC+(1<<16)},
    {GUIIM_M_Q_KAZAKH_ABC_UPPER,           WDI_KAZAKH_ABC+(3<<16)},
#endif

#if defined (IM_LATVIAN_SUPPORT)
    {GUIIM_M_LATVIAN_LEAD_UPPER,             WDI_LATVIAN+(2<<16)},
    {GUIIM_M_LATVIAN_LOWER,                  WDI_LATVIAN+(1<<16)},
    {GUIIM_M_LATVIAN_UPPER,                  WDI_LATVIAN+(3<<16)},
    {GUIIM_M_LATVIAN_ABC_LEAD_UPPER,              WDI_LATVIAN_ABC+(2<<16)},
    {GUIIM_M_LATVIAN_ABC_LOWER,              WDI_LATVIAN_ABC+(1<<16)},
    {GUIIM_M_LATVIAN_ABC_UPPER,              WDI_LATVIAN_ABC+(3<<16)},

    {GUIIM_M_Q_LATVIAN_LEAD_UPPER,           WDI_LATVIAN+(2<<16)},
    {GUIIM_M_Q_LATVIAN_LOWER,                WDI_LATVIAN+(1<<16)},
    {GUIIM_M_Q_LATVIAN_UPPER,                WDI_LATVIAN+(3<<16)},
    {GUIIM_M_Q_LATVIAN_ABC_LEAD_UPPER,            WDI_LATVIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_LATVIAN_ABC_LOWER,            WDI_LATVIAN_ABC+(1<<16)},
    {GUIIM_M_Q_LATVIAN_ABC_UPPER,            WDI_LATVIAN_ABC+(3<<16)},
#endif

#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_M_LITHUANIAN_LEAD_UPPER,             WDI_LITHUANIAN+(2<<16)},
    {GUIIM_M_LITHUANIAN_LOWER,                  WDI_LITHUANIAN+(1<<16)},
    {GUIIM_M_LITHUANIAN_UPPER,                  WDI_LITHUANIAN+(3<<16)},

    {GUIIM_M_Q_LITHUANIAN_LEAD_UPPER,           WDI_LITHUANIAN+(2<<16)},
    {GUIIM_M_Q_LITHUANIAN_LOWER,                WDI_LITHUANIAN+(1<<16)},
    {GUIIM_M_Q_LITHUANIAN_UPPER,                WDI_LITHUANIAN+(3<<16)},
#endif

#if defined (IM_MACEDONIAN_SUPPORT)
    {GUIIM_M_MACEDONIAN_LEAD_UPPER,             WDI_MACEDONIAN+(2<<16)},
    {GUIIM_M_MACEDONIAN_LOWER,                  WDI_MACEDONIAN+(1<<16)},
    {GUIIM_M_MACEDONIAN_UPPER,                  WDI_MACEDONIAN+(3<<16)},
    {GUIIM_M_MACEDONIAN_ABC_LEAD_UPPER,              WDI_MACEDONIAN_ABC+(2<<16)},
    {GUIIM_M_MACEDONIAN_ABC_LOWER,              WDI_MACEDONIAN_ABC+(1<<16)},
    {GUIIM_M_MACEDONIAN_ABC_UPPER,              WDI_MACEDONIAN_ABC+(3<<16)},

    {GUIIM_M_Q_MACEDONIAN_LEAD_UPPER,           WDI_MACEDONIAN+(2<<16)},
    {GUIIM_M_Q_MACEDONIAN_LOWER,                WDI_MACEDONIAN+(1<<16)},
    {GUIIM_M_Q_MACEDONIAN_UPPER,                WDI_MACEDONIAN+(3<<16)},
    {GUIIM_M_Q_MACEDONIAN_ABC_LEAD_UPPER,            WDI_MACEDONIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_MACEDONIAN_ABC_LOWER,            WDI_MACEDONIAN_ABC+(1<<16)},
    {GUIIM_M_Q_MACEDONIAN_ABC_UPPER,            WDI_MACEDONIAN_ABC+(3<<16)},
#endif

#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_MALAY_LEAD_UPPER,             WDI_MALAY+(2<<16)},
    {GUIIM_M_MALAY_LOWER,                  WDI_MALAY+(1<<16)},
    {GUIIM_M_MALAY_UPPER,                  WDI_MALAY+(3<<16)},
    {GUIIM_M_MALAY_ABC_LEAD_UPPER,              WDI_MALAY_ABC+(2<<16)},
    {GUIIM_M_MALAY_ABC_LOWER,              WDI_MALAY_ABC+(1<<16)},
    {GUIIM_M_MALAY_ABC_UPPER,              WDI_MALAY_ABC+(3<<16)},

    {GUIIM_M_Q_MALAY_LEAD_UPPER,           WDI_MALAY+(2<<16)},
    {GUIIM_M_Q_MALAY_LOWER,                WDI_MALAY+(1<<16)},
    {GUIIM_M_Q_MALAY_UPPER,                WDI_MALAY+(3<<16)},
    {GUIIM_M_Q_MALAY_ABC_LEAD_UPPER,            WDI_MALAY_ABC+(2<<16)},    
    {GUIIM_M_Q_MALAY_ABC_LOWER,            WDI_MALAY_ABC+(1<<16)},
    {GUIIM_M_Q_MALAY_ABC_UPPER,            WDI_MALAY_ABC+(3<<16)},
#endif

#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_M_NORWEGIAN_LEAD_UPPER,             WDI_NORWEGIAN+(2<<16)},
    {GUIIM_M_NORWEGIAN_LOWER,                  WDI_NORWEGIAN+(1<<16)},
    {GUIIM_M_NORWEGIAN_UPPER,                  WDI_NORWEGIAN+(3<<16)},
    {GUIIM_M_NORWEGIAN_ABC_LEAD_UPPER,              WDI_NORWEGIAN_ABC+(2<<16)},
    {GUIIM_M_NORWEGIAN_ABC_LOWER,              WDI_NORWEGIAN_ABC+(1<<16)},
    {GUIIM_M_NORWEGIAN_ABC_UPPER,              WDI_NORWEGIAN_ABC+(3<<16)},

    {GUIIM_M_Q_NORWEGIAN_LEAD_UPPER,           WDI_NORWEGIAN+(2<<16)},
    {GUIIM_M_Q_NORWEGIAN_LOWER,                WDI_NORWEGIAN+(1<<16)},
    {GUIIM_M_Q_NORWEGIAN_UPPER,                WDI_NORWEGIAN+(3<<16)},
    {GUIIM_M_Q_NORWEGIAN_ABC_LEAD_UPPER,            WDI_NORWEGIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_NORWEGIAN_ABC_LOWER,            WDI_NORWEGIAN_ABC+(1<<16)},
    {GUIIM_M_Q_NORWEGIAN_ABC_UPPER,            WDI_NORWEGIAN_ABC+(3<<16)},
#endif

#ifdef IM_POLISH_SUPPORT
    {GUIIM_M_POLISH_LEAD_UPPER,             WDI_POLISH+(2<<16)},
    {GUIIM_M_POLISH_LOWER,                  WDI_POLISH+(1<<16)},
    {GUIIM_M_POLISH_UPPER,                  WDI_POLISH+(3<<16)},
    {GUIIM_M_POLISH_ABC_LEAD_UPPER,              WDI_POLISH_ABC+(2<<16)},
    {GUIIM_M_POLISH_ABC_LOWER,              WDI_POLISH_ABC+(1<<16)},
    {GUIIM_M_POLISH_ABC_UPPER,              WDI_POLISH_ABC+(3<<16)},

    {GUIIM_M_Q_POLISH_LEAD_UPPER,           WDI_POLISH+(2<<16)},
    {GUIIM_M_Q_POLISH_LOWER,                WDI_POLISH+(1<<16)},
    {GUIIM_M_Q_POLISH_UPPER,                WDI_POLISH+(3<<16)},
    {GUIIM_M_Q_POLISH_ABC_LEAD_UPPER,            WDI_POLISH_ABC+(2<<16)},    
    {GUIIM_M_Q_POLISH_ABC_LOWER,            WDI_POLISH_ABC+(1<<16)},
    {GUIIM_M_Q_POLISH_ABC_UPPER,            WDI_POLISH_ABC+(3<<16)},
#endif

#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_PORTUGUESE_LEAD_UPPER,             WDI_PORTUGUESE+(2<<16)},
    {GUIIM_M_PORTUGUESE_LOWER,                  WDI_PORTUGUESE+(1<<16)},
    {GUIIM_M_PORTUGUESE_UPPER,                  WDI_PORTUGUESE+(3<<16)},
    {GUIIM_M_PORTUGUESE_ABC_LEAD_UPPER,              WDI_PORTUGUESE_ABC+(2<<16)},
    {GUIIM_M_PORTUGUESE_ABC_LOWER,              WDI_PORTUGUESE_ABC+(1<<16)},
    {GUIIM_M_PORTUGUESE_ABC_UPPER,              WDI_PORTUGUESE_ABC+(3<<16)},

    {GUIIM_M_Q_PORTUGUESE_LEAD_UPPER,           WDI_PORTUGUESE+(2<<16)},
    {GUIIM_M_Q_PORTUGUESE_LOWER,                WDI_PORTUGUESE+(1<<16)},
    {GUIIM_M_Q_PORTUGUESE_UPPER,                WDI_PORTUGUESE+(3<<16)},
    {GUIIM_M_Q_PORTUGUESE_ABC_LEAD_UPPER,            WDI_PORTUGUESE_ABC+(2<<16)},    
    {GUIIM_M_Q_PORTUGUESE_ABC_LOWER,            WDI_PORTUGUESE_ABC+(1<<16)},
    {GUIIM_M_Q_PORTUGUESE_ABC_UPPER,            WDI_PORTUGUESE_ABC+(3<<16)},
#endif

#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_ROMANIAN_LEAD_UPPER,             WDI_ROMANIAN+(2<<16)},
    {GUIIM_M_ROMANIAN_LOWER,                  WDI_ROMANIAN+(1<<16)},
    {GUIIM_M_ROMANIAN_UPPER,                  WDI_ROMANIAN+(3<<16)},
    {GUIIM_M_ROMANIAN_ABC_LEAD_UPPER,              WDI_ROMANIAN_ABC+(2<<16)},
    {GUIIM_M_ROMANIAN_ABC_LOWER,              WDI_ROMANIAN_ABC+(1<<16)},
    {GUIIM_M_ROMANIAN_ABC_UPPER,              WDI_ROMANIAN_ABC+(3<<16)},

    {GUIIM_M_Q_ROMANIAN_LEAD_UPPER,           WDI_ROMANIAN+(2<<16)},
    {GUIIM_M_Q_ROMANIAN_LOWER,                WDI_ROMANIAN+(1<<16)},
    {GUIIM_M_Q_ROMANIAN_UPPER,                WDI_ROMANIAN+(3<<16)},
    {GUIIM_M_Q_ROMANIAN_ABC_LEAD_UPPER,            WDI_ROMANIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_ROMANIAN_ABC_LOWER,            WDI_ROMANIAN_ABC+(1<<16)},
    {GUIIM_M_Q_ROMANIAN_ABC_UPPER,            WDI_ROMANIAN_ABC+(3<<16)},
#endif

#if defined (IM_SERBIAN_SUPPORT)
    {GUIIM_M_SERBIAN_LEAD_UPPER,             WDI_SERBIAN+(2<<16)},
    {GUIIM_M_SERBIAN_LOWER,                  WDI_SERBIAN+(1<<16)},
    {GUIIM_M_SERBIAN_UPPER,                  WDI_SERBIAN+(3<<16)},
    {GUIIM_M_SERBIAN_ABC_LEAD_UPPER,              WDI_SERBIAN_ABC+(2<<16)},
    {GUIIM_M_SERBIAN_ABC_LOWER,              WDI_SERBIAN_ABC+(1<<16)},
    {GUIIM_M_SERBIAN_ABC_UPPER,              WDI_SERBIAN_ABC+(3<<16)},

    {GUIIM_M_Q_SERBIAN_LEAD_UPPER,           WDI_SERBIAN+(2<<16)},
    {GUIIM_M_Q_SERBIAN_LOWER,                WDI_SERBIAN+(1<<16)},
    {GUIIM_M_Q_SERBIAN_UPPER,                WDI_SERBIAN+(3<<16)},
    {GUIIM_M_Q_SERBIAN_ABC_LEAD_UPPER,            WDI_SERBIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_SERBIAN_ABC_LOWER,            WDI_SERBIAN_ABC+(1<<16)},
    {GUIIM_M_Q_SERBIAN_ABC_UPPER,            WDI_SERBIAN_ABC+(3<<16)},
#endif

#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_SLOVAK_LEAD_UPPER,             WDI_SLOVAK+(2<<16)},
    {GUIIM_M_SLOVAK_LOWER,                  WDI_SLOVAK+(1<<16)},
    {GUIIM_M_SLOVAK_UPPER,                  WDI_SLOVAK+(3<<16)},
    {GUIIM_M_SLOVAK_ABC_LEAD_UPPER,              WDI_SLOVAK_ABC+(2<<16)},
    {GUIIM_M_SLOVAK_ABC_LOWER,              WDI_SLOVAK_ABC+(1<<16)},
    {GUIIM_M_SLOVAK_ABC_UPPER,              WDI_SLOVAK_ABC+(3<<16)},

    {GUIIM_M_Q_SLOVAK_LEAD_UPPER,           WDI_SLOVAK+(2<<16)},
    {GUIIM_M_Q_SLOVAK_LOWER,                WDI_SLOVAK+(1<<16)},
    {GUIIM_M_Q_SLOVAK_UPPER,                WDI_SLOVAK+(3<<16)},
    {GUIIM_M_Q_SLOVAK_ABC_LEAD_UPPER,            WDI_SLOVAK_ABC+(2<<16)},    
    {GUIIM_M_Q_SLOVAK_ABC_LOWER,            WDI_SLOVAK_ABC+(1<<16)},
    {GUIIM_M_Q_SLOVAK_ABC_UPPER,            WDI_SLOVAK_ABC+(3<<16)},
#endif

#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_SLOVENIAN_LEAD_UPPER,             WDI_SLOVENIAN+(2<<16)},
    {GUIIM_M_SLOVENIAN_LOWER,                  WDI_SLOVENIAN+(1<<16)},
    {GUIIM_M_SLOVENIAN_UPPER,                  WDI_SLOVENIAN+(3<<16)},
    {GUIIM_M_SLOVENIAN_ABC_LEAD_UPPER,              WDI_SLOVENIAN_ABC+(2<<16)},
    {GUIIM_M_SLOVENIAN_ABC_LOWER,              WDI_SLOVENIAN_ABC+(1<<16)},
    {GUIIM_M_SLOVENIAN_ABC_UPPER,              WDI_SLOVENIAN_ABC+(3<<16)},

    {GUIIM_M_Q_SLOVENIAN_LEAD_UPPER,           WDI_SLOVENIAN+(2<<16)},
    {GUIIM_M_Q_SLOVENIAN_LOWER,                WDI_SLOVENIAN+(1<<16)},
    {GUIIM_M_Q_SLOVENIAN_UPPER,                WDI_SLOVENIAN+(3<<16)},
    {GUIIM_M_Q_SLOVENIAN_ABC_LEAD_UPPER,            WDI_SLOVENIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_SLOVENIAN_ABC_LOWER,            WDI_SLOVENIAN_ABC+(1<<16)},
    {GUIIM_M_Q_SLOVENIAN_ABC_UPPER,            WDI_SLOVENIAN_ABC+(3<<16)},
#endif

#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_SPANISH_LEAD_UPPER,             WDI_SPANISH+(2<<16)},
    {GUIIM_M_SPANISH_LOWER,                  WDI_SPANISH+(1<<16)},
    {GUIIM_M_SPANISH_UPPER,                  WDI_SPANISH+(3<<16)},
    {GUIIM_M_SPANISH_ABC_LEAD_UPPER,              WDI_SPANISH_ABC+(2<<16)},
    {GUIIM_M_SPANISH_ABC_LOWER,              WDI_SPANISH_ABC+(1<<16)},
    {GUIIM_M_SPANISH_ABC_UPPER,              WDI_SPANISH_ABC+(3<<16)},

    {GUIIM_M_Q_SPANISH_LEAD_UPPER,           WDI_SPANISH+(2<<16)},
    {GUIIM_M_Q_SPANISH_LOWER,                WDI_SPANISH+(1<<16)},
    {GUIIM_M_Q_SPANISH_UPPER,                WDI_SPANISH+(3<<16)},
    {GUIIM_M_Q_SPANISH_ABC_LEAD_UPPER,            WDI_SPANISH_ABC+(2<<16)},    
    {GUIIM_M_Q_SPANISH_ABC_LOWER,            WDI_SPANISH_ABC+(1<<16)},
    {GUIIM_M_Q_SPANISH_ABC_UPPER,            WDI_SPANISH_ABC+(3<<16)},
#endif

#if defined (IM_SWAHILI_SUPPORT)
    {GUIIM_M_SWAHILI_LEAD_UPPER,             WDI_SWAHILI+(2<<16)},
    {GUIIM_M_SWAHILI_LOWER,                  WDI_SWAHILI+(1<<16)},
    {GUIIM_M_SWAHILI_UPPER,                  WDI_SWAHILI+(3<<16)},
    {GUIIM_M_SWAHILI_ABC_LEAD_UPPER,              WDI_SWAHILI_ABC+(2<<16)},
    {GUIIM_M_SWAHILI_ABC_LOWER,              WDI_SWAHILI_ABC+(1<<16)},
    {GUIIM_M_SWAHILI_ABC_UPPER,              WDI_SWAHILI_ABC+(3<<16)},

    {GUIIM_M_Q_SWAHILI_LEAD_UPPER,           WDI_SWAHILI+(2<<16)},
    {GUIIM_M_Q_SWAHILI_LOWER,                WDI_SWAHILI+(1<<16)},
    {GUIIM_M_Q_SWAHILI_UPPER,                WDI_SWAHILI+(3<<16)},
    {GUIIM_M_Q_SWAHILI_ABC_LEAD_UPPER,            WDI_SWAHILI_ABC+(2<<16)},    
    {GUIIM_M_Q_SWAHILI_ABC_LOWER,            WDI_SWAHILI_ABC+(1<<16)},
    {GUIIM_M_Q_SWAHILI_ABC_UPPER,            WDI_SWAHILI_ABC+(3<<16)},
#endif

#if defined (IM_SWEDISH_SUPPORT)
    {GUIIM_M_SWEDISH_LEAD_UPPER,             WDI_SWEDISH+(2<<16)},
    {GUIIM_M_SWEDISH_LOWER,                  WDI_SWEDISH+(1<<16)},
    {GUIIM_M_SWEDISH_UPPER,                  WDI_SWEDISH+(3<<16)},
    {GUIIM_M_SWEDISH_ABC_LEAD_UPPER,              WDI_SWEDISH_ABC+(2<<16)},
    {GUIIM_M_SWEDISH_ABC_LOWER,              WDI_SWEDISH_ABC+(1<<16)},
    {GUIIM_M_SWEDISH_ABC_UPPER,              WDI_SWEDISH_ABC+(3<<16)},

    {GUIIM_M_Q_SWEDISH_LEAD_UPPER,           WDI_SWEDISH+(2<<16)},
    {GUIIM_M_Q_SWEDISH_LOWER,                WDI_SWEDISH+(1<<16)},
    {GUIIM_M_Q_SWEDISH_UPPER,                WDI_SWEDISH+(3<<16)},
    {GUIIM_M_Q_SWEDISH_ABC_LEAD_UPPER,            WDI_SWEDISH_ABC+(2<<16)},    
    {GUIIM_M_Q_SWEDISH_ABC_LOWER,            WDI_SWEDISH_ABC+(1<<16)},
    {GUIIM_M_Q_SWEDISH_ABC_UPPER,            WDI_SWEDISH_ABC+(3<<16)},
#endif

#if defined (IM_TURKISH_SUPPORT)
    {GUIIM_M_TURKISH_LEAD_UPPER,             WDI_TURKISH+(2<<16)},
    {GUIIM_M_TURKISH_LOWER,                  WDI_TURKISH+(1<<16)},
    {GUIIM_M_TURKISH_UPPER,                  WDI_TURKISH+(3<<16)},
    {GUIIM_M_TURKISH_ABC_LEAD_UPPER,              WDI_TURKISH_ABC+(2<<16)},
    {GUIIM_M_TURKISH_ABC_LOWER,              WDI_TURKISH_ABC+(1<<16)},
    {GUIIM_M_TURKISH_ABC_UPPER,              WDI_TURKISH_ABC+(3<<16)},

    {GUIIM_M_Q_TURKISH_LEAD_UPPER,           WDI_TURKISH+(2<<16)},
    {GUIIM_M_Q_TURKISH_LOWER,                WDI_TURKISH+(1<<16)},
    {GUIIM_M_Q_TURKISH_UPPER,                WDI_TURKISH+(3<<16)},
    {GUIIM_M_Q_TURKISH_ABC_LEAD_UPPER,            WDI_TURKISH_ABC+(2<<16)},    
    {GUIIM_M_Q_TURKISH_ABC_LOWER,            WDI_TURKISH_ABC+(1<<16)},
    {GUIIM_M_Q_TURKISH_ABC_UPPER,            WDI_TURKISH_ABC+(3<<16)},
#endif

#if defined (IM_UKRAINIAN_SUPPORT)
    {GUIIM_M_UKRAINIAN_LEAD_UPPER,             WDI_UKRAINIAN+(2<<16)},
    {GUIIM_M_UKRAINIAN_LOWER,                  WDI_UKRAINIAN+(1<<16)},
    {GUIIM_M_UKRAINIAN_UPPER,                  WDI_UKRAINIAN+(3<<16)},
    {GUIIM_M_UKRAINIAN_ABC_LEAD_UPPER,              WDI_UKRAINIAN_ABC+(2<<16)},
    {GUIIM_M_UKRAINIAN_ABC_LOWER,              WDI_UKRAINIAN_ABC+(1<<16)},
    {GUIIM_M_UKRAINIAN_ABC_UPPER,              WDI_UKRAINIAN_ABC+(3<<16)},

    {GUIIM_M_Q_UKRAINIAN_LEAD_UPPER,           WDI_UKRAINIAN+(2<<16)},
    {GUIIM_M_Q_UKRAINIAN_LOWER,                WDI_UKRAINIAN+(1<<16)},
    {GUIIM_M_Q_UKRAINIAN_UPPER,                WDI_UKRAINIAN+(3<<16)},
    {GUIIM_M_Q_UKRAINIAN_ABC_LEAD_UPPER,            WDI_UKRAINIAN_ABC+(2<<16)},    
    {GUIIM_M_Q_UKRAINIAN_ABC_LOWER,            WDI_UKRAINIAN_ABC+(1<<16)},
    {GUIIM_M_Q_UKRAINIAN_ABC_UPPER,            WDI_UKRAINIAN_ABC+(3<<16)},
#endif

#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_VIETNAMESE_LEAD_UPPER,             WDI_VIETNAMESE+(2<<16)},
    {GUIIM_M_VIETNAMESE_LOWER,                  WDI_VIETNAMESE+(1<<16)},
    {GUIIM_M_VIETNAMESE_UPPER,                  WDI_VIETNAMESE+(3<<16)},
    {GUIIM_M_VIETNAMESE_ABC_LEAD_UPPER,              WDI_VIETNAMESE_ABC+(2<<16)},
    {GUIIM_M_VIETNAMESE_ABC_LOWER,              WDI_VIETNAMESE_ABC+(1<<16)},
    {GUIIM_M_VIETNAMESE_ABC_UPPER,              WDI_VIETNAMESE_ABC+(3<<16)},

    {GUIIM_M_Q_VIETNAMESE_LEAD_UPPER,           WDI_VIETNAMESE+(2<<16)},
    {GUIIM_M_Q_VIETNAMESE_LOWER,                WDI_VIETNAMESE+(1<<16)},
    {GUIIM_M_Q_VIETNAMESE_UPPER,                WDI_VIETNAMESE+(3<<16)},
    {GUIIM_M_Q_VIETNAMESE_ABC_LEAD_UPPER,            WDI_VIETNAMESE_ABC+(2<<16)},    
    {GUIIM_M_Q_VIETNAMESE_ABC_LOWER,            WDI_VIETNAMESE_ABC+(1<<16)},
    {GUIIM_M_Q_VIETNAMESE_ABC_UPPER,            WDI_VIETNAMESE_ABC+(3<<16)},
#endif

#if defined (IM_ZULU_SUPPORT)
    {GUIIM_M_ZULU_LEAD_UPPER,             WDI_ZULU+(2<<16)},
    {GUIIM_M_ZULU_LOWER,                  WDI_ZULU+(1<<16)},
    {GUIIM_M_ZULU_UPPER,                  WDI_ZULU+(3<<16)},
    {GUIIM_M_ZULU_ABC_LEAD_UPPER,              WDI_ZULU_ABC+(2<<16)},
    {GUIIM_M_ZULU_ABC_LOWER,              WDI_ZULU_ABC+(1<<16)},
    {GUIIM_M_ZULU_ABC_UPPER,              WDI_ZULU_ABC+(3<<16)},

    {GUIIM_M_Q_ZULU_LEAD_UPPER,           WDI_ZULU+(2<<16)},
    {GUIIM_M_Q_ZULU_LOWER,                WDI_ZULU+(1<<16)},
    {GUIIM_M_Q_ZULU_UPPER,                WDI_ZULU+(3<<16)},
    {GUIIM_M_Q_ZULU_ABC_LEAD_UPPER,            WDI_ZULU_ABC+(2<<16)},    
    {GUIIM_M_Q_ZULU_ABC_LOWER,            WDI_ZULU_ABC+(1<<16)},
    {GUIIM_M_Q_ZULU_ABC_UPPER,            WDI_ZULU_ABC+(3<<16)},
#endif

#if defined (IM_KHMER_SUPPORT)
    {GUIIM_M_KHMER,              WDI_KHMER + (1<<16)},
    {GUIIM_M_KHMER_ABC,              WDI_KHMER_ABC+(3<<16)},

    {GUIIM_M_Q_KHMER,                WDI_KHMER+(1<<16)},
    {GUIIM_M_Q_KHMER_ABC,            WDI_KHMER_ABC+(1 << 16)},
#endif

#if defined (IM_LAO_SUPPORT)
    {GUIIM_M_LAO,              WDI_LAO + (1<<16)},
    {GUIIM_M_LAO_ABC,              WDI_LAO_ABC+(3<<16)},

    {GUIIM_M_Q_LAO,                WDI_LAO+(1<<16)},
    {GUIIM_M_Q_LAO_ABC,            WDI_LAO_ABC+(1 << 16)},
#endif

#if defined (IM_UYGHUR_SUPPORT)
    {GUIIM_M_UYGHUR,              WDI_UIGHUR + (1<<16)},
    {GUIIM_M_UYGHUR_ABC,              WDI_UIGHUR_ABC+(3<<16)},

    {GUIIM_M_Q_UYGHUR,                WDI_UIGHUR+(1<<16)},
    {GUIIM_M_Q_UYGHUR_ABC,            WDI_UIGHUR_ABC+(1 << 16)},
#endif

#if defined (IM_SINHALESE_SUPPORT)
    {GUIIM_M_SINHALESE,              WDI_SINHALESE + (1<<16)},
    {GUIIM_M_SINHALESE_ABC,              WDI_SINHALESE_ABC+(3<<16)},

    {GUIIM_M_Q_SINHALESE,                WDI_SINHALESE+(1<<16)},
    {GUIIM_M_Q_SINHALESE_ABC,            WDI_SINHALESE_ABC+(1 << 16)},
#endif
};


LOCAL MMIIM_SOGOU_METHOD_RELATION_T const s_sogou_tp_method_relation_list[] =
{
    {GUIIM_M_TP_123,                           WDI_DIGIT},
    {GUIIM_M_TP_ENGLISH_LEAD_UPPER,            WDI_ENGLISH+(2<<16)},
    {GUIIM_M_TP_ENGLISH_LOWER,                 WDI_ENGLISH+(1<<16)},
    {GUIIM_M_TP_ENGLISH_UPPER,                 WDI_ENGLISH+(3<<16)},
    {GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER,        WDI_ENGLISH_ABC+(2<<16)},
    {GUIIM_M_TP_ENGLISH_ABC_LOWER,             WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_TP_ENGLISH_ABC_UPPER,             WDI_ENGLISH_ABC+(3<<16)},

    {GUIIM_M_TPQ_123,                         WDI_DIGIT},
    {GUIIM_M_TPQ_ENGLISH_LEAD_UPPER,          WDI_ENGLISH+(2<<16)},
    {GUIIM_M_TPQ_ENGLISH_LOWER,               WDI_ENGLISH+(1<<16)},
    {GUIIM_M_TPQ_ENGLISH_UPPER,               WDI_ENGLISH+(3<<16)},
    {GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER,      WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_ENGLISH_ABC_LOWER,           WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_ENGLISH_ABC_UPPER,           WDI_ENGLISH_ABC+(3<<16)},

#ifdef IM_SIMP_CHINESE_SUPPORT
    {GUIIM_M_TP_PINYIN,                        WDI_CN_CPINYIN},
    {GUIIM_M_TP_STROKE,                        WDI_CN_BIHUA},
    {GUIIM_M_TPQ_PINYIN,                      WDI_CN_FULLPINYIN},
    {GUIIM_M_TPQ_STROKE,                      WDI_CN_BIHUA},
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
    {GUIIM_M_TP_ZHUYIN,                        WDI_CN_ZHUYIN},
    {GUIIM_M_TP_STROKE_CHT,                    WDI_CN_BIHUAF},
    {GUIIM_M_TPQ_ZHUYIN,                      WDI_CN_FULLZHUYIN},
    {GUIIM_M_TPQ_STROKE_CHT,                  WDI_CN_BIHUAF},
#endif

#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_TP_RUSSIAN_LEAD_UPPER,             WDI_RUSSIAN+(2<<16)},
    {GUIIM_M_TP_RUSSIAN_LOWER,                  WDI_RUSSIAN+(1<<16)},
    {GUIIM_M_TP_RUSSIAN_UPPER,                  WDI_RUSSIAN+(3<<16)},
    {GUIIM_M_TP_RUSSIAN_ABC_LEAD_UPPER,              WDI_RUSSIAN_ABC+(2<<16)},
    {GUIIM_M_TP_RUSSIAN_ABC_LOWER,              WDI_RUSSIAN_ABC+(1<<16)},
    {GUIIM_M_TP_RUSSIAN_ABC_UPPER,              WDI_RUSSIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_RUSSIAN_LEAD_UPPER,           WDI_RUSSIAN+(2<<16)},
    {GUIIM_M_TPQ_RUSSIAN_LOWER,                WDI_RUSSIAN+(1<<16)},
    {GUIIM_M_TPQ_RUSSIAN_UPPER,                WDI_RUSSIAN+(3<<16)},
    {GUIIM_M_TPQ_RUSSIAN_ABC_LEAD_UPPER,            WDI_RUSSIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_RUSSIAN_ABC_LOWER,            WDI_RUSSIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_RUSSIAN_ABC_UPPER,            WDI_RUSSIAN_ABC+(3<<16)},
#endif

#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_TP_ARABIC,                        WDI_ARABIC},
    {GUIIM_M_TP_ARABIC_ABC,                    WDI_ARABIC_ABC},
    {GUIIM_M_TPQ_ARABIC,                      WDI_ARABIC},
    {GUIIM_M_TPQ_ARABIC_ABC,                  WDI_ARABIC_ABC},
#endif

#if defined (IM_ASSAMESE_SUPPORT)
    {GUIIM_M_TP_ASSAMESE,                        WDI_ASSAMESE},
    {GUIIM_M_TP_ASSAMESE_ABC,                    WDI_ASSAMESE_ABC},
    {GUIIM_M_TPQ_ASSAMESE,                      WDI_ASSAMESE},
    {GUIIM_M_TPQ_ASSAMESE_ABC,                  WDI_ASSAMESE_ABC},
#endif

#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_TP_BENGALI,                        WDI_BENGALI},
    {GUIIM_M_TP_BENGALI_ABC,                    WDI_BENGALI_ABC},
    {GUIIM_M_TPQ_BENGALI,                      WDI_BENGALI},
    {GUIIM_M_TPQ_BENGALI_ABC,                  WDI_BENGALI_ABC},
#endif

#if defined (IM_GEORGIAN_SUPPORT)
    {GUIIM_M_TP_GEORGIAN,                        WDI_GEORGIAN},
    {GUIIM_M_TP_GEORGIAN_ABC,                    WDI_GEORGIAN_ABC},
    {GUIIM_M_TPQ_GEORGIAN,                      WDI_GEORGIAN},
    {GUIIM_M_TPQ_GEORGIAN_ABC,                  WDI_GEORGIAN_ABC},
#endif

#if defined (IM_GUJARATI_SUPPORT)
    {GUIIM_M_TP_GUJARATI,                        WDI_GUJARATI},
    {GUIIM_M_TP_GUJARATI_ABC,                    WDI_GUJARATI_ABC},
    {GUIIM_M_TPQ_GUJARATI,                      WDI_GUJARATI},
    {GUIIM_M_TPQ_GUJARATI_ABC,                  WDI_GUJARATI_ABC},
#endif

#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_TP_HEBREW,                        WDI_HEBREW},
    {GUIIM_M_TP_HEBREW_ABC,                    WDI_HEBREW_ABC},
    {GUIIM_M_TPQ_HEBREW,                      WDI_HEBREW},
    {GUIIM_M_TPQ_HEBREW_ABC,                  WDI_HEBREW_ABC},
#endif

#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_TP_HINDI,                        WDI_HINDI},
    {GUIIM_M_TP_HINDI_ABC,                    WDI_HINDI_ABC},
    {GUIIM_M_TPQ_HINDI,                      WDI_HINDI},
    {GUIIM_M_TPQ_HINDI_ABC,                  WDI_HINDI_ABC},
#endif

#if defined (IM_KANNADA_SUPPORT)
    {GUIIM_M_TP_KANNADA,                        WDI_KANNADA},
    {GUIIM_M_TP_KANNADA_ABC,                    WDI_KANNADA_ABC},
    {GUIIM_M_TPQ_KANNADA,                      WDI_KANNADA},
    {GUIIM_M_TPQ_KANNADA_ABC,                  WDI_KANNADA_ABC},
#endif

#if defined (IM_MALAYALAM_SUPPORT)
    {GUIIM_M_TP_MALAYALAM,                        WDI_MALAYALAM},
    {GUIIM_M_TP_MALAYALAM_ABC,                    WDI_MALAYALAM_ABC},
    {GUIIM_M_TPQ_MALAYALAM,                      WDI_MALAYALAM},
    {GUIIM_M_TPQ_MALAYALAM_ABC,                  WDI_MALAYALAM_ABC},
#endif

#if defined (IM_MYANMAR_SUPPORT)
    {GUIIM_M_TP_MYANMAR,                        WDI_BURMESE},
    {GUIIM_M_TP_MYANMAR_ABC,                    WDI_BURMESE_ABC},
    {GUIIM_M_TPQ_MYANMAR,                      WDI_BURMESE},
    {GUIIM_M_TPQ_MYANMAR_ABC,                  WDI_BURMESE_ABC},
#endif

#if defined (IM_MARATHI_SUPPORT)
    {GUIIM_M_TP_MARATHI,                        WDI_MARATHI},
    {GUIIM_M_TP_MARATHI_ABC,                    WDI_MARATHI_ABC},
    {GUIIM_M_TPQ_MARATHI,                      WDI_MARATHI},
    {GUIIM_M_TPQ_MARATHI_ABC,                  WDI_MARATHI_ABC},
#endif

#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_TP_PERSIAN,                        WDI_PERSIAN},
    {GUIIM_M_TP_PERSIAN_ABC,                    WDI_PERSIAN_ABC},
    {GUIIM_M_TPQ_PERSIAN,                      WDI_PERSIAN},
    {GUIIM_M_TPQ_PERSIAN_ABC,                  WDI_PERSIAN_ABC},
#endif

#if defined (IM_PUNJABI_SUPPORT)
    {GUIIM_M_TP_PUNJABI,                        WDI_PUNJABI},
    {GUIIM_M_TP_PUNJABI_ABC,                    WDI_PUNJABI_ABC},
    {GUIIM_M_TPQ_PUNJABI,                      WDI_PUNJABI},
    {GUIIM_M_TPQ_PUNJABI_ABC,                  WDI_PUNJABI_ABC},
#endif

#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_M_TP_TAGALOG,                        WDI_TAGALOG},
    {GUIIM_M_TP_TAGALOG_ABC,                    WDI_TAGALOG_ABC},
    {GUIIM_M_TPQ_TAGALOG,                      WDI_TAGALOG},
    {GUIIM_M_TPQ_TAGALOG_ABC,                  WDI_TAGALOG_ABC},
#endif

#if defined (IM_TAMIL_SUPPORT)
    {GUIIM_M_TP_TAMIL,                        WDI_TAMIL},
    {GUIIM_M_TP_TAMIL_ABC,                    WDI_TAMIL_ABC},
    {GUIIM_M_TPQ_TAMIL,                      WDI_TAMIL},
    {GUIIM_M_TPQ_TAMIL_ABC,                  WDI_TAMIL_ABC},
#endif

#if defined (IM_TELUGU_SUPPORT)
    {GUIIM_M_TP_TELUGU,                        WDI_TELUGU},
    {GUIIM_M_TP_TELUGU_ABC,                    WDI_TELUGU_ABC},
    {GUIIM_M_TPQ_TELUGU,                      WDI_TELUGU},
    {GUIIM_M_TPQ_TELUGU_ABC,                  WDI_TELUGU_ABC},
#endif

#ifdef IM_THAI_SUPPORT
    {GUIIM_M_TP_THAI,                        WDI_THAI},
    {GUIIM_M_TP_THAI_ABC,                    WDI_THAI_ABC},
    {GUIIM_M_TPQ_THAI,                      WDI_THAI},
    {GUIIM_M_TPQ_THAI_ABC,                  WDI_THAI_ABC},
#endif

#ifdef IM_URDU_SUPPORT
    {GUIIM_M_TP_URDU,                        WDI_URDU},
    {GUIIM_M_TP_URDU_ABC,                    WDI_URDU_ABC},
    {GUIIM_M_TPQ_URDU,                      WDI_URDU},
    {GUIIM_M_TPQ_URDU_ABC,                  WDI_URDU_ABC},
#endif

#if defined (IM_AFRIKAANS_SUPPORT)
    {GUIIM_M_TP_AFRIKAANS_LEAD_UPPER,             WDI_AFRIKAANS+(2<<16)},
    {GUIIM_M_TP_AFRIKAANS_LOWER,                  WDI_AFRIKAANS+(1<<16)},
    {GUIIM_M_TP_AFRIKAANS_UPPER,                  WDI_AFRIKAANS+(3<<16)},

    {GUIIM_M_TPQ_AFRIKAANS_LEAD_UPPER,           WDI_AFRIKAANS+(2<<16)},
    {GUIIM_M_TPQ_AFRIKAANS_LOWER,                WDI_AFRIKAANS+(1<<16)},
    {GUIIM_M_TPQ_AFRIKAANS_UPPER,                WDI_AFRIKAANS+(3<<16)},
#endif

#if defined (IM_AZERBAIJANI_SUPPORT)
    {GUIIM_M_TP_AZERBAIJANI_LEAD_UPPER,             WDI_AZERBAIJANI+(2<<16)},
    {GUIIM_M_TP_AZERBAIJANI_LOWER,                  WDI_AZERBAIJANI+(1<<16)},
    {GUIIM_M_TP_AZERBAIJANI_UPPER,                  WDI_AZERBAIJANI+(3<<16)},
    {GUIIM_M_TP_AZERBAIJANI_ABC_LEAD_UPPER,              WDI_AZERBAIJANI_ABC+(2<<16)},
    {GUIIM_M_TP_AZERBAIJANI_ABC_LOWER,              WDI_AZERBAIJANI_ABC+(1<<16)},
    {GUIIM_M_TP_AZERBAIJANI_ABC_UPPER,              WDI_AZERBAIJANI_ABC+(3<<16)},

    {GUIIM_M_TPQ_AZERBAIJANI_LEAD_UPPER,           WDI_AZERBAIJANI+(2<<16)},
    {GUIIM_M_TPQ_AZERBAIJANI_LOWER,                WDI_AZERBAIJANI+(1<<16)},
    {GUIIM_M_TPQ_AZERBAIJANI_UPPER,                WDI_AZERBAIJANI+(3<<16)},
    {GUIIM_M_TPQ_AZERBAIJANI_ABC_LEAD_UPPER,            WDI_AZERBAIJANI_ABC+(2<<16)},    
    {GUIIM_M_TPQ_AZERBAIJANI_ABC_LOWER,            WDI_AZERBAIJANI_ABC+(1<<16)},
    {GUIIM_M_TPQ_AZERBAIJANI_ABC_UPPER,            WDI_AZERBAIJANI_ABC+(3<<16)},
#endif

#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_M_TP_BULGARIAN_LEAD_UPPER,             WDI_BULGARIAN+(2<<16)},
    {GUIIM_M_TP_BULGARIAN_LOWER,                  WDI_BULGARIAN+(1<<16)},
    {GUIIM_M_TP_BULGARIAN_UPPER,                  WDI_BULGARIAN+(3<<16)},
    {GUIIM_M_TP_BULGARIAN_ABC_LEAD_UPPER,              WDI_BULGARIAN_ABC+(2<<16)},
    {GUIIM_M_TP_BULGARIAN_ABC_LOWER,              WDI_BULGARIAN_ABC+(1<<16)},
    {GUIIM_M_TP_BULGARIAN_ABC_UPPER,              WDI_BULGARIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_BULGARIAN_LEAD_UPPER,           WDI_BULGARIAN+(2<<16)},
    {GUIIM_M_TPQ_BULGARIAN_LOWER,                WDI_BULGARIAN+(1<<16)},
    {GUIIM_M_TPQ_BULGARIAN_UPPER,                WDI_BULGARIAN+(3<<16)},
    {GUIIM_M_TPQ_BULGARIAN_ABC_LEAD_UPPER,            WDI_BULGARIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_BULGARIAN_ABC_LOWER,            WDI_BULGARIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_BULGARIAN_ABC_UPPER,            WDI_BULGARIAN_ABC+(3<<16)},
#endif

#if defined (IM_CATALAN_SUPPORT)
    {GUIIM_M_TP_CATALAN_LEAD_UPPER,             WDI_CATALAN+(2<<16)},
    {GUIIM_M_TP_CATALAN_LOWER,                  WDI_CATALAN+(1<<16)},
    {GUIIM_M_TP_CATALAN_UPPER,                  WDI_CATALAN+(3<<16)},
    {GUIIM_M_TP_CATALAN_ABC_LEAD_UPPER,              WDI_CATALAN_ABC+(2<<16)},
    {GUIIM_M_TP_CATALAN_ABC_LOWER,              WDI_CATALAN_ABC+(1<<16)},
    {GUIIM_M_TP_CATALAN_ABC_UPPER,              WDI_CATALAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_CATALAN_LEAD_UPPER,           WDI_CATALAN+(2<<16)},
    {GUIIM_M_TPQ_CATALAN_LOWER,                WDI_CATALAN+(1<<16)},
    {GUIIM_M_TPQ_CATALAN_UPPER,                WDI_CATALAN+(3<<16)},
    {GUIIM_M_TPQ_CATALAN_ABC_LEAD_UPPER,            WDI_CATALAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_CATALAN_ABC_LOWER,            WDI_CATALAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_CATALAN_ABC_UPPER,            WDI_CATALAN_ABC+(3<<16)},
#endif

#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_TP_CROATIAN_LEAD_UPPER,             WDI_CROATIAN+(2<<16)},
    {GUIIM_M_TP_CROATIAN_LOWER,                  WDI_CROATIAN+(1<<16)},
    {GUIIM_M_TP_CROATIAN_UPPER,                  WDI_CROATIAN+(3<<16)},
    {GUIIM_M_TP_CROATIAN_ABC_LEAD_UPPER,              WDI_CROATIAN_ABC+(2<<16)},
    {GUIIM_M_TP_CROATIAN_ABC_LOWER,              WDI_CROATIAN_ABC+(1<<16)},
    {GUIIM_M_TP_CROATIAN_ABC_UPPER,              WDI_CROATIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_CROATIAN_LEAD_UPPER,           WDI_CROATIAN+(2<<16)},
    {GUIIM_M_TPQ_CROATIAN_LOWER,                WDI_CROATIAN+(1<<16)},
    {GUIIM_M_TPQ_CROATIAN_UPPER,                WDI_CROATIAN+(3<<16)},
    {GUIIM_M_TPQ_CROATIAN_ABC_LEAD_UPPER,            WDI_CROATIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_CROATIAN_ABC_LOWER,            WDI_CROATIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_CROATIAN_ABC_UPPER,            WDI_CROATIAN_ABC+(3<<16)},
#endif

#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_TP_CZECH_LEAD_UPPER,             WDI_CZECH+(2<<16)},
    {GUIIM_M_TP_CZECH_LOWER,                  WDI_CZECH+(1<<16)},
    {GUIIM_M_TP_CZECH_UPPER,                  WDI_CZECH+(3<<16)},
    {GUIIM_M_TP_CZECH_ABC_LEAD_UPPER,              WDI_CZECH_ABC+(2<<16)},
    {GUIIM_M_TP_CZECH_ABC_LOWER,              WDI_CZECH_ABC+(1<<16)},
    {GUIIM_M_TP_CZECH_ABC_UPPER,              WDI_CZECH_ABC+(3<<16)},

    {GUIIM_M_TPQ_CZECH_LEAD_UPPER,           WDI_CZECH+(2<<16)},
    {GUIIM_M_TPQ_CZECH_LOWER,                WDI_CZECH+(1<<16)},
    {GUIIM_M_TPQ_CZECH_UPPER,                WDI_CZECH+(3<<16)},
    {GUIIM_M_TPQ_CZECH_ABC_LEAD_UPPER,            WDI_CZECH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_CZECH_ABC_LOWER,            WDI_CZECH_ABC+(1<<16)},
    {GUIIM_M_TPQ_CZECH_ABC_UPPER,            WDI_CZECH_ABC+(3<<16)},
#endif

#ifdef IM_DANISH_SUPPORT
    {GUIIM_M_TP_DANISH_LEAD_UPPER,             WDI_DANISH+(2<<16)},
    {GUIIM_M_TP_DANISH_LOWER,                  WDI_DANISH+(1<<16)},
    {GUIIM_M_TP_DANISH_UPPER,                  WDI_DANISH+(3<<16)},
    {GUIIM_M_TP_DANISH_ABC_LEAD_UPPER,              WDI_DANISH_ABC+(2<<16)},
    {GUIIM_M_TP_DANISH_ABC_LOWER,              WDI_DANISH_ABC+(1<<16)},
    {GUIIM_M_TP_DANISH_ABC_UPPER,              WDI_DANISH_ABC+(3<<16)},

    {GUIIM_M_TPQ_DANISH_LEAD_UPPER,           WDI_DANISH+(2<<16)},
    {GUIIM_M_TPQ_DANISH_LOWER,                WDI_DANISH+(1<<16)},
    {GUIIM_M_TPQ_DANISH_UPPER,                WDI_DANISH+(3<<16)},
    {GUIIM_M_TPQ_DANISH_ABC_LEAD_UPPER,            WDI_DANISH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_DANISH_ABC_LOWER,            WDI_DANISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_DANISH_ABC_UPPER,            WDI_DANISH_ABC+(3<<16)},
#endif

#ifdef IM_DUTCH_SUPPORT
    {GUIIM_M_TP_DUTCH_LEAD_UPPER,             WDI_DUTCH+(2<<16)},
    {GUIIM_M_TP_DUTCH_LOWER,                  WDI_DUTCH+(1<<16)},
    {GUIIM_M_TP_DUTCH_UPPER,                  WDI_DUTCH+(3<<16)},
    {GUIIM_M_TP_DUTCH_ABC_LEAD_UPPER,              WDI_DUTCH_ABC+(2<<16)},
    {GUIIM_M_TP_DUTCH_ABC_LOWER,              WDI_DUTCH_ABC+(1<<16)},
    {GUIIM_M_TP_DUTCH_ABC_UPPER,              WDI_DUTCH_ABC+(3<<16)},

    {GUIIM_M_TPQ_DUTCH_LEAD_UPPER,           WDI_DUTCH+(2<<16)},
    {GUIIM_M_TPQ_DUTCH_LOWER,                WDI_DUTCH+(1<<16)},
    {GUIIM_M_TPQ_DUTCH_UPPER,                WDI_DUTCH+(3<<16)},
    {GUIIM_M_TPQ_DUTCH_ABC_LEAD_UPPER,            WDI_DUTCH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_DUTCH_ABC_LOWER,            WDI_DUTCH_ABC+(1<<16)},
    {GUIIM_M_TPQ_DUTCH_ABC_UPPER,            WDI_DUTCH_ABC+(3<<16)},
#endif
    
#if defined (IM_ESTONIAN_SUPPORT)
    {GUIIM_M_TP_ESTONIAN_LEAD_UPPER,             WDI_ESTONIAN+(2<<16)},
    {GUIIM_M_TP_ESTONIAN_LOWER,                  WDI_ESTONIAN+(1<<16)},
    {GUIIM_M_TP_ESTONIAN_UPPER,                  WDI_ESTONIAN+(3<<16)},
    {GUIIM_M_TP_ESTONIAN_ABC_LEAD_UPPER,              WDI_ESTONIAN_ABC+(2<<16)},
    {GUIIM_M_TP_ESTONIAN_ABC_LOWER,              WDI_ESTONIAN_ABC+(1<<16)},
    {GUIIM_M_TP_ESTONIAN_ABC_UPPER,              WDI_ESTONIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_ESTONIAN_LEAD_UPPER,           WDI_ESTONIAN+(2<<16)},
    {GUIIM_M_TPQ_ESTONIAN_LOWER,                WDI_ESTONIAN+(1<<16)},
    {GUIIM_M_TPQ_ESTONIAN_UPPER,                WDI_ESTONIAN+(3<<16)},
    {GUIIM_M_TPQ_ESTONIAN_ABC_LEAD_UPPER,            WDI_ESTONIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_ESTONIAN_ABC_LOWER,            WDI_ESTONIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_ESTONIAN_ABC_UPPER,            WDI_ESTONIAN_ABC+(3<<16)},
#endif

#if defined (IM_FINNISH_SUPPORT)
    {GUIIM_M_TP_FINNISH_LEAD_UPPER,             WDI_FINNISH+(2<<16)},
    {GUIIM_M_TP_FINNISH_LOWER,                  WDI_FINNISH+(1<<16)},
    {GUIIM_M_TP_FINNISH_UPPER,                  WDI_FINNISH+(3<<16)},
    {GUIIM_M_TP_FINNISH_ABC_LEAD_UPPER,              WDI_FINNISH_ABC+(2<<16)},
    {GUIIM_M_TP_FINNISH_ABC_LOWER,              WDI_FINNISH_ABC+(1<<16)},
    {GUIIM_M_TP_FINNISH_ABC_UPPER,              WDI_FINNISH_ABC+(3<<16)},

    {GUIIM_M_TPQ_FINNISH_LEAD_UPPER,           WDI_FINNISH+(2<<16)},
    {GUIIM_M_TPQ_FINNISH_LOWER,                WDI_FINNISH+(1<<16)},
    {GUIIM_M_TPQ_FINNISH_UPPER,                WDI_FINNISH+(3<<16)},
    {GUIIM_M_TPQ_FINNISH_ABC_LEAD_UPPER,            WDI_FINNISH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_FINNISH_ABC_LOWER,            WDI_FINNISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_FINNISH_ABC_UPPER,            WDI_FINNISH_ABC+(3<<16)},
#endif

#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_TP_FRENCH_LEAD_UPPER,             WDI_FRENCH+(2<<16)},
    {GUIIM_M_TP_FRENCH_LOWER,                  WDI_FRENCH+(1<<16)},
    {GUIIM_M_TP_FRENCH_UPPER,                  WDI_FRENCH+(3<<16)},
    {GUIIM_M_TP_FRENCH_ABC_LEAD_UPPER,              WDI_FRENCH_ABC+(2<<16)},
    {GUIIM_M_TP_FRENCH_ABC_LOWER,              WDI_FRENCH_ABC+(1<<16)},
    {GUIIM_M_TP_FRENCH_ABC_UPPER,              WDI_FRENCH_ABC+(3<<16)},

    {GUIIM_M_TPQ_FRENCH_LEAD_UPPER,           WDI_FRENCH+(2<<16)},
    {GUIIM_M_TPQ_FRENCH_LOWER,                WDI_FRENCH+(1<<16)},
    {GUIIM_M_TPQ_FRENCH_UPPER,                WDI_FRENCH+(3<<16)},
    {GUIIM_M_TPQ_FRENCH_ABC_LEAD_UPPER,            WDI_FRENCH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_FRENCH_ABC_LOWER,            WDI_FRENCH_ABC+(1<<16)},
    {GUIIM_M_TPQ_FRENCH_ABC_UPPER,            WDI_FRENCH_ABC+(3<<16)},
#endif

#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_TP_GERMAN_LEAD_UPPER,             WDI_GERMAN+(2<<16)},
    {GUIIM_M_TP_GERMAN_LOWER,                  WDI_GERMAN+(1<<16)},
    {GUIIM_M_TP_GERMAN_UPPER,                  WDI_GERMAN+(3<<16)},
    {GUIIM_M_TP_GERMAN_ABC_LEAD_UPPER,              WDI_GERMAN_ABC+(2<<16)},
    {GUIIM_M_TP_GERMAN_ABC_LOWER,              WDI_GERMAN_ABC+(1<<16)},
    {GUIIM_M_TP_GERMAN_ABC_UPPER,              WDI_GERMAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_GERMAN_LEAD_UPPER,           WDI_GERMAN+(2<<16)},
    {GUIIM_M_TPQ_GERMAN_LOWER,                WDI_GERMAN+(1<<16)},
    {GUIIM_M_TPQ_GERMAN_UPPER,                WDI_GERMAN+(3<<16)},
    {GUIIM_M_TPQ_GERMAN_ABC_LEAD_UPPER,            WDI_GERMAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_GERMAN_ABC_LOWER,            WDI_GERMAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_GERMAN_ABC_UPPER,            WDI_GERMAN_ABC+(3<<16)},
#endif

#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_TP_GREEK_LEAD_UPPER,             WDI_GREEK+(2<<16)},
    {GUIIM_M_TP_GREEK_LOWER,                  WDI_GREEK+(1<<16)},
    {GUIIM_M_TP_GREEK_UPPER,                  WDI_GREEK+(3<<16)},
    {GUIIM_M_TP_GREEK_ABC_LEAD_UPPER,              WDI_GREEK_ABC+(2<<16)},
    {GUIIM_M_TP_GREEK_ABC_LOWER,              WDI_GREEK_ABC+(1<<16)},
    {GUIIM_M_TP_GREEK_ABC_UPPER,              WDI_GREEK_ABC+(3<<16)},

    {GUIIM_M_TPQ_GREEK_LEAD_UPPER,           WDI_GREEK+(2<<16)},
    {GUIIM_M_TPQ_GREEK_LOWER,                WDI_GREEK+(1<<16)},
    {GUIIM_M_TPQ_GREEK_UPPER,                WDI_GREEK+(3<<16)},
    {GUIIM_M_TPQ_GREEK_ABC_LEAD_UPPER,            WDI_GREEK_ABC+(2<<16)},    
    {GUIIM_M_TPQ_GREEK_ABC_LOWER,            WDI_GREEK_ABC+(1<<16)},
    {GUIIM_M_TPQ_GREEK_ABC_UPPER,            WDI_GREEK_ABC+(3<<16)},
#endif

#if defined (IM_HAUSA_SUPPORT)
    {GUIIM_M_TP_HAUSA_LEAD_UPPER,             WDI_HAUSA+(2<<16)},
    {GUIIM_M_TP_HAUSA_LOWER,                  WDI_HAUSA+(1<<16)},
    {GUIIM_M_TP_HAUSA_UPPER,                  WDI_HAUSA+(3<<16)},
    {GUIIM_M_TP_HAUSA_ABC_LEAD_UPPER,              WDI_HAUSA_ABC+(2<<16)},
    {GUIIM_M_TP_HAUSA_ABC_LOWER,              WDI_HAUSA_ABC+(1<<16)},
    {GUIIM_M_TP_HAUSA_ABC_UPPER,              WDI_HAUSA_ABC+(3<<16)},

    {GUIIM_M_TPQ_HAUSA_LEAD_UPPER,           WDI_HAUSA+(2<<16)},
    {GUIIM_M_TPQ_HAUSA_LOWER,                WDI_HAUSA+(1<<16)},
    {GUIIM_M_TPQ_HAUSA_UPPER,                WDI_HAUSA+(3<<16)},
    {GUIIM_M_TPQ_HAUSA_ABC_LEAD_UPPER,            WDI_HAUSA_ABC+(2<<16)},    
    {GUIIM_M_TPQ_HAUSA_ABC_LOWER,            WDI_HAUSA_ABC+(1<<16)},
    {GUIIM_M_TPQ_HAUSA_ABC_UPPER,            WDI_HAUSA_ABC+(3<<16)},
#endif

#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_TP_HUNGARIAN_LEAD_UPPER,             WDI_HUNGARIAN+(2<<16)},
    {GUIIM_M_TP_HUNGARIAN_LOWER,                  WDI_HUNGARIAN+(1<<16)},
    {GUIIM_M_TP_HUNGARIAN_UPPER,                  WDI_HUNGARIAN+(3<<16)},
    {GUIIM_M_TP_HUNGARIAN_ABC_LEAD_UPPER,              WDI_HUNGARIAN_ABC+(2<<16)},
    {GUIIM_M_TP_HUNGARIAN_ABC_LOWER,              WDI_HUNGARIAN_ABC+(1<<16)},
    {GUIIM_M_TP_HUNGARIAN_ABC_UPPER,              WDI_HUNGARIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_HUNGARIAN_LEAD_UPPER,           WDI_HUNGARIAN+(2<<16)},
    {GUIIM_M_TPQ_HUNGARIAN_LOWER,                WDI_HUNGARIAN+(1<<16)},
    {GUIIM_M_TPQ_HUNGARIAN_UPPER,                WDI_HUNGARIAN+(3<<16)},
    {GUIIM_M_TPQ_HUNGARIAN_ABC_LEAD_UPPER,            WDI_HUNGARIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_HUNGARIAN_ABC_LOWER,            WDI_HUNGARIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_HUNGARIAN_ABC_UPPER,            WDI_HUNGARIAN_ABC+(3<<16)},
#endif

#if defined (IM_ICELANDIC_SUPPORT)
    {GUIIM_M_TP_ICELANDIC_LEAD_UPPER,             WDI_ICELANDIC+(2<<16)},
    {GUIIM_M_TP_ICELANDIC_LOWER,                  WDI_ICELANDIC+(1<<16)},
    {GUIIM_M_TP_ICELANDIC_UPPER,                  WDI_ICELANDIC+(3<<16)},
    {GUIIM_M_TP_ICELANDIC_ABC_LEAD_UPPER,              WDI_ICELANDIC_ABC+(2<<16)},
    {GUIIM_M_TP_ICELANDIC_ABC_LOWER,              WDI_ICELANDIC_ABC+(1<<16)},
    {GUIIM_M_TP_ICELANDIC_ABC_UPPER,              WDI_ICELANDIC_ABC+(3<<16)},

    {GUIIM_M_TPQ_ICELANDIC_LEAD_UPPER,           WDI_ICELANDIC+(2<<16)},
    {GUIIM_M_TPQ_ICELANDIC_LOWER,                WDI_ICELANDIC+(1<<16)},
    {GUIIM_M_TPQ_ICELANDIC_UPPER,                WDI_ICELANDIC+(3<<16)},
    {GUIIM_M_TPQ_ICELANDIC_ABC_LEAD_UPPER,            WDI_ICELANDIC_ABC+(2<<16)},    
    {GUIIM_M_TPQ_ICELANDIC_ABC_LOWER,            WDI_ICELANDIC_ABC+(1<<16)},
    {GUIIM_M_TPQ_ICELANDIC_ABC_UPPER,            WDI_ICELANDIC_ABC+(3<<16)},
#endif

#if defined (IM_IGBO_SUPPORT)
    {GUIIM_M_TP_IGBO_LEAD_UPPER,             WDI_IGBO+(2<<16)},
    {GUIIM_M_TP_IGBO_LOWER,                  WDI_IGBO+(1<<16)},
    {GUIIM_M_TP_IGBO_UPPER,                  WDI_IGBO+(3<<16)},
    {GUIIM_M_TP_IGBO_ABC_LEAD_UPPER,              WDI_IGBO_ABC+(2<<16)},
    {GUIIM_M_TP_IGBO_ABC_LOWER,              WDI_IGBO_ABC+(1<<16)},
    {GUIIM_M_TP_IGBO_ABC_UPPER,              WDI_IGBO_ABC+(3<<16)},

    {GUIIM_M_TPQ_IGBO_LEAD_UPPER,           WDI_IGBO+(2<<16)},
    {GUIIM_M_TPQ_IGBO_LOWER,                WDI_IGBO+(1<<16)},
    {GUIIM_M_TPQ_IGBO_UPPER,                WDI_IGBO+(3<<16)},
    {GUIIM_M_TPQ_IGBO_ABC_LEAD_UPPER,            WDI_IGBO_ABC+(2<<16)},    
    {GUIIM_M_TPQ_IGBO_ABC_LOWER,            WDI_IGBO_ABC+(1<<16)},
    {GUIIM_M_TPQ_IGBO_ABC_UPPER,            WDI_IGBO_ABC+(3<<16)},
#endif

#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_TP_INDONESIAN_LEAD_UPPER,             WDI_INDONESIAN+(2<<16)},
    {GUIIM_M_TP_INDONESIAN_LOWER,                  WDI_INDONESIAN+(1<<16)},
    {GUIIM_M_TP_INDONESIAN_UPPER,                  WDI_INDONESIAN+(3<<16)},
    {GUIIM_M_TP_INDONESIAN_ABC_LEAD_UPPER,              WDI_INDONESIAN_ABC+(2<<16)},
    {GUIIM_M_TP_INDONESIAN_ABC_LOWER,              WDI_INDONESIAN_ABC+(1<<16)},
    {GUIIM_M_TP_INDONESIAN_ABC_UPPER,              WDI_INDONESIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_INDONESIAN_LEAD_UPPER,           WDI_INDONESIAN+(2<<16)},
    {GUIIM_M_TPQ_INDONESIAN_LOWER,                WDI_INDONESIAN+(1<<16)},
    {GUIIM_M_TPQ_INDONESIAN_UPPER,                WDI_INDONESIAN+(3<<16)},
    {GUIIM_M_TPQ_INDONESIAN_ABC_LEAD_UPPER,            WDI_INDONESIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_INDONESIAN_ABC_LOWER,            WDI_INDONESIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_INDONESIAN_ABC_UPPER,            WDI_INDONESIAN_ABC+(3<<16)},
#endif

#if defined (IM_IRISH_SUPPORT)
    {GUIIM_M_TP_IRISH_LEAD_UPPER,             WDI_IRISH+(2<<16)},
    {GUIIM_M_TP_IRISH_LOWER,                  WDI_IRISH+(1<<16)},
    {GUIIM_M_TP_IRISH_UPPER,                  WDI_IRISH+(3<<16)},
    {GUIIM_M_TP_IRISH_ABC_LEAD_UPPER,              WDI_IRISH_ABC+(2<<16)},
    {GUIIM_M_TP_IRISH_ABC_LOWER,              WDI_IRISH_ABC+(1<<16)},
    {GUIIM_M_TP_IRISH_ABC_UPPER,              WDI_IRISH_ABC+(3<<16)},

    {GUIIM_M_TPQ_IRISH_LEAD_UPPER,           WDI_IRISH+(2<<16)},
    {GUIIM_M_TPQ_IRISH_LOWER,                WDI_IRISH+(1<<16)},
    {GUIIM_M_TPQ_IRISH_UPPER,                WDI_IRISH+(3<<16)},
    {GUIIM_M_TPQ_IRISH_ABC_LEAD_UPPER,            WDI_IRISH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_IRISH_ABC_LOWER,            WDI_IRISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_IRISH_ABC_UPPER,            WDI_IRISH_ABC+(3<<16)},
#endif

#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_TP_ITALIAN_LEAD_UPPER,            WDI_ITALIAN+(2<<16)},
    {GUIIM_M_TP_ITALIAN_LOWER,                 WDI_ITALIAN+(1<<16)},
    {GUIIM_M_TP_ITALIAN_UPPER,                 WDI_ITALIAN+(3<<16)},
    {GUIIM_M_TP_ITALIAN_ABC_LOWER,             WDI_ITALIAN_ABC+(1<<16)},
    {GUIIM_M_TP_ITALIAN_ABC_UPPER,             WDI_ITALIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_ITALIAN_LEAD_UPPER,          WDI_ITALIAN+(2<<16)},
    {GUIIM_M_TPQ_ITALIAN_LOWER,               WDI_ITALIAN+(1<<16)},
    {GUIIM_M_TPQ_ITALIAN_UPPER,               WDI_ITALIAN+(3<<16)},
    {GUIIM_M_TPQ_ITALIAN_ABC_LOWER,           WDI_ITALIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_ITALIAN_ABC_UPPER,           WDI_ITALIAN_ABC+(3<<16)},
#endif

#if defined (IM_KAZAKH_SUPPORT)
    {GUIIM_M_TP_KAZAKH_LEAD_UPPER,            WDI_KAZAKH+(2<<16)},
    {GUIIM_M_TP_KAZAKH_LOWER,                 WDI_KAZAKH+(1<<16)},
    {GUIIM_M_TP_KAZAKH_UPPER,                 WDI_KAZAKH+(3<<16)},
    {GUIIM_M_TP_KAZAKH_ABC_LOWER,             WDI_KAZAKH_ABC+(1<<16)},
    {GUIIM_M_TP_KAZAKH_ABC_UPPER,             WDI_KAZAKH_ABC+(3<<16)},

    {GUIIM_M_TPQ_KAZAKH_LEAD_UPPER,          WDI_KAZAKH+(2<<16)},
    {GUIIM_M_TPQ_KAZAKH_LOWER,               WDI_KAZAKH+(1<<16)},
    {GUIIM_M_TPQ_KAZAKH_UPPER,               WDI_KAZAKH+(3<<16)},
    {GUIIM_M_TPQ_KAZAKH_ABC_LOWER,           WDI_KAZAKH_ABC+(1<<16)},
    {GUIIM_M_TPQ_KAZAKH_ABC_UPPER,           WDI_KAZAKH_ABC+(3<<16)},
#endif

#if defined (IM_LATVIAN_SUPPORT)
    {GUIIM_M_TP_LATVIAN_LEAD_UPPER,             WDI_LATVIAN+(2<<16)},
    {GUIIM_M_TP_LATVIAN_LOWER,                  WDI_LATVIAN+(1<<16)},
    {GUIIM_M_TP_LATVIAN_UPPER,                  WDI_LATVIAN+(3<<16)},
    {GUIIM_M_TP_LATVIAN_ABC_LEAD_UPPER,              WDI_LATVIAN_ABC+(2<<16)},
    {GUIIM_M_TP_LATVIAN_ABC_LOWER,              WDI_LATVIAN_ABC+(1<<16)},
    {GUIIM_M_TP_LATVIAN_ABC_UPPER,              WDI_LATVIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_LATVIAN_LEAD_UPPER,           WDI_LATVIAN+(2<<16)},
    {GUIIM_M_TPQ_LATVIAN_LOWER,                WDI_LATVIAN+(1<<16)},
    {GUIIM_M_TPQ_LATVIAN_UPPER,                WDI_LATVIAN+(3<<16)},
    {GUIIM_M_TPQ_LATVIAN_ABC_LEAD_UPPER,            WDI_LATVIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_LATVIAN_ABC_LOWER,            WDI_LATVIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_LATVIAN_ABC_UPPER,            WDI_LATVIAN_ABC+(3<<16)},
#endif

#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_M_TP_LITHUANIAN_LEAD_UPPER,             WDI_LITHUANIAN+(2<<16)},
    {GUIIM_M_TP_LITHUANIAN_LOWER,                  WDI_LITHUANIAN+(1<<16)},
    {GUIIM_M_TP_LITHUANIAN_UPPER,                  WDI_LITHUANIAN+(3<<16)},

    {GUIIM_M_TPQ_LITHUANIAN_LEAD_UPPER,           WDI_LITHUANIAN+(2<<16)},
    {GUIIM_M_TPQ_LITHUANIAN_LOWER,                WDI_LITHUANIAN+(1<<16)},
    {GUIIM_M_TPQ_LITHUANIAN_UPPER,                WDI_LITHUANIAN+(3<<16)},
#endif

#if defined (IM_MACEDONIAN_SUPPORT)
    {GUIIM_M_TP_MACEDONIAN_LEAD_UPPER,             WDI_MACEDONIAN+(2<<16)},
    {GUIIM_M_TP_MACEDONIAN_LOWER,                  WDI_MACEDONIAN+(1<<16)},
    {GUIIM_M_TP_MACEDONIAN_UPPER,                  WDI_MACEDONIAN+(3<<16)},
    {GUIIM_M_TP_MACEDONIAN_ABC_LEAD_UPPER,              WDI_MACEDONIAN_ABC+(2<<16)},
    {GUIIM_M_TP_MACEDONIAN_ABC_LOWER,              WDI_MACEDONIAN_ABC+(1<<16)},
    {GUIIM_M_TP_MACEDONIAN_ABC_UPPER,              WDI_MACEDONIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_MACEDONIAN_LEAD_UPPER,           WDI_MACEDONIAN+(2<<16)},
    {GUIIM_M_TPQ_MACEDONIAN_LOWER,                WDI_MACEDONIAN+(1<<16)},
    {GUIIM_M_TPQ_MACEDONIAN_UPPER,                WDI_MACEDONIAN+(3<<16)},
    {GUIIM_M_TPQ_MACEDONIAN_ABC_LEAD_UPPER,            WDI_MACEDONIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_MACEDONIAN_ABC_LOWER,            WDI_MACEDONIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_MACEDONIAN_ABC_UPPER,            WDI_MACEDONIAN_ABC+(3<<16)},
#endif

#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_TP_MALAY_LEAD_UPPER,             WDI_MALAY+(2<<16)},
    {GUIIM_M_TP_MALAY_LOWER,                  WDI_MALAY+(1<<16)},
    {GUIIM_M_TP_MALAY_UPPER,                  WDI_MALAY+(3<<16)},
    {GUIIM_M_TP_MALAY_ABC_LEAD_UPPER,              WDI_MALAY_ABC+(2<<16)},
    {GUIIM_M_TP_MALAY_ABC_LOWER,              WDI_MALAY_ABC+(1<<16)},
    {GUIIM_M_TP_MALAY_ABC_UPPER,              WDI_MALAY_ABC+(3<<16)},

    {GUIIM_M_TPQ_MALAY_LEAD_UPPER,           WDI_MALAY+(2<<16)},
    {GUIIM_M_TPQ_MALAY_LOWER,                WDI_MALAY+(1<<16)},
    {GUIIM_M_TPQ_MALAY_UPPER,                WDI_MALAY+(3<<16)},
    {GUIIM_M_TPQ_MALAY_ABC_LEAD_UPPER,            WDI_MALAY_ABC+(2<<16)},    
    {GUIIM_M_TPQ_MALAY_ABC_LOWER,            WDI_MALAY_ABC+(1<<16)},
    {GUIIM_M_TPQ_MALAY_ABC_UPPER,            WDI_MALAY_ABC+(3<<16)},
#endif

#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_M_TP_NORWEGIAN_LEAD_UPPER,             WDI_NORWEGIAN+(2<<16)},
    {GUIIM_M_TP_NORWEGIAN_LOWER,                  WDI_NORWEGIAN+(1<<16)},
    {GUIIM_M_TP_NORWEGIAN_UPPER,                  WDI_NORWEGIAN+(3<<16)},
    {GUIIM_M_TP_NORWEGIAN_ABC_LEAD_UPPER,              WDI_NORWEGIAN_ABC+(2<<16)},
    {GUIIM_M_TP_NORWEGIAN_ABC_LOWER,              WDI_NORWEGIAN_ABC+(1<<16)},
    {GUIIM_M_TP_NORWEGIAN_ABC_UPPER,              WDI_NORWEGIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_NORWEGIAN_LEAD_UPPER,           WDI_NORWEGIAN+(2<<16)},
    {GUIIM_M_TPQ_NORWEGIAN_LOWER,                WDI_NORWEGIAN+(1<<16)},
    {GUIIM_M_TPQ_NORWEGIAN_UPPER,                WDI_NORWEGIAN+(3<<16)},
    {GUIIM_M_TPQ_NORWEGIAN_ABC_LEAD_UPPER,            WDI_NORWEGIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_NORWEGIAN_ABC_LOWER,            WDI_NORWEGIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_NORWEGIAN_ABC_UPPER,            WDI_NORWEGIAN_ABC+(3<<16)},
#endif

#ifdef IM_POLISH_SUPPORT
    {GUIIM_M_TP_POLISH_LEAD_UPPER,             WDI_POLISH+(2<<16)},
    {GUIIM_M_TP_POLISH_LOWER,                  WDI_POLISH+(1<<16)},
    {GUIIM_M_TP_POLISH_UPPER,                  WDI_POLISH+(3<<16)},
    {GUIIM_M_TP_POLISH_ABC_LEAD_UPPER,              WDI_POLISH_ABC+(2<<16)},
    {GUIIM_M_TP_POLISH_ABC_LOWER,              WDI_POLISH_ABC+(1<<16)},
    {GUIIM_M_TP_POLISH_ABC_UPPER,              WDI_POLISH_ABC+(3<<16)},

    {GUIIM_M_TPQ_POLISH_LEAD_UPPER,           WDI_POLISH+(2<<16)},
    {GUIIM_M_TPQ_POLISH_LOWER,                WDI_POLISH+(1<<16)},
    {GUIIM_M_TPQ_POLISH_UPPER,                WDI_POLISH+(3<<16)},
    {GUIIM_M_TPQ_POLISH_ABC_LEAD_UPPER,            WDI_POLISH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_POLISH_ABC_LOWER,            WDI_POLISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_POLISH_ABC_UPPER,            WDI_POLISH_ABC+(3<<16)},
#endif

#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_TP_PORTUGUESE_LEAD_UPPER,             WDI_PORTUGUESE+(2<<16)},
    {GUIIM_M_TP_PORTUGUESE_LOWER,                  WDI_PORTUGUESE+(1<<16)},
    {GUIIM_M_TP_PORTUGUESE_UPPER,                  WDI_PORTUGUESE+(3<<16)},
    {GUIIM_M_TP_PORTUGUESE_ABC_LEAD_UPPER,              WDI_PORTUGUESE_ABC+(2<<16)},
    {GUIIM_M_TP_PORTUGUESE_ABC_LOWER,              WDI_PORTUGUESE_ABC+(1<<16)},
    {GUIIM_M_TP_PORTUGUESE_ABC_UPPER,              WDI_PORTUGUESE_ABC+(3<<16)},

    {GUIIM_M_TPQ_PORTUGUESE_LEAD_UPPER,           WDI_PORTUGUESE+(2<<16)},
    {GUIIM_M_TPQ_PORTUGUESE_LOWER,                WDI_PORTUGUESE+(1<<16)},
    {GUIIM_M_TPQ_PORTUGUESE_UPPER,                WDI_PORTUGUESE+(3<<16)},
    {GUIIM_M_TPQ_PORTUGUESE_ABC_LEAD_UPPER,            WDI_PORTUGUESE_ABC+(2<<16)},    
    {GUIIM_M_TPQ_PORTUGUESE_ABC_LOWER,            WDI_PORTUGUESE_ABC+(1<<16)},
    {GUIIM_M_TPQ_PORTUGUESE_ABC_UPPER,            WDI_PORTUGUESE_ABC+(3<<16)},
#endif

#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_TP_ROMANIAN_LEAD_UPPER,             WDI_ROMANIAN+(2<<16)},
    {GUIIM_M_TP_ROMANIAN_LOWER,                  WDI_ROMANIAN+(1<<16)},
    {GUIIM_M_TP_ROMANIAN_UPPER,                  WDI_ROMANIAN+(3<<16)},
    {GUIIM_M_TP_ROMANIAN_ABC_LEAD_UPPER,              WDI_ROMANIAN_ABC+(2<<16)},
    {GUIIM_M_TP_ROMANIAN_ABC_LOWER,              WDI_ROMANIAN_ABC+(1<<16)},
    {GUIIM_M_TP_ROMANIAN_ABC_UPPER,              WDI_ROMANIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_ROMANIAN_LEAD_UPPER,           WDI_ROMANIAN+(2<<16)},
    {GUIIM_M_TPQ_ROMANIAN_LOWER,                WDI_ROMANIAN+(1<<16)},
    {GUIIM_M_TPQ_ROMANIAN_UPPER,                WDI_ROMANIAN+(3<<16)},
    {GUIIM_M_TPQ_ROMANIAN_ABC_LEAD_UPPER,            WDI_ROMANIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_ROMANIAN_ABC_LOWER,            WDI_ROMANIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_ROMANIAN_ABC_UPPER,            WDI_ROMANIAN_ABC+(3<<16)},
#endif

#if defined (IM_SERBIAN_SUPPORT)
    {GUIIM_M_TP_SERBIAN_LEAD_UPPER,             WDI_SERBIAN+(2<<16)},
    {GUIIM_M_TP_SERBIAN_LOWER,                  WDI_SERBIAN+(1<<16)},
    {GUIIM_M_TP_SERBIAN_UPPER,                  WDI_SERBIAN+(3<<16)},
    {GUIIM_M_TP_SERBIAN_ABC_LEAD_UPPER,              WDI_SERBIAN_ABC+(2<<16)},
    {GUIIM_M_TP_SERBIAN_ABC_LOWER,              WDI_SERBIAN_ABC+(1<<16)},
    {GUIIM_M_TP_SERBIAN_ABC_UPPER,              WDI_SERBIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_SERBIAN_LEAD_UPPER,           WDI_SERBIAN+(2<<16)},
    {GUIIM_M_TPQ_SERBIAN_LOWER,                WDI_SERBIAN+(1<<16)},
    {GUIIM_M_TPQ_SERBIAN_UPPER,                WDI_SERBIAN+(3<<16)},
    {GUIIM_M_TPQ_SERBIAN_ABC_LEAD_UPPER,            WDI_SERBIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_SERBIAN_ABC_LOWER,            WDI_SERBIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_SERBIAN_ABC_UPPER,            WDI_SERBIAN_ABC+(3<<16)},
#endif

#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_TP_SLOVAK_LEAD_UPPER,             WDI_SLOVAK+(2<<16)},
    {GUIIM_M_TP_SLOVAK_LOWER,                  WDI_SLOVAK+(1<<16)},
    {GUIIM_M_TP_SLOVAK_UPPER,                  WDI_SLOVAK+(3<<16)},
    {GUIIM_M_TP_SLOVAK_ABC_LEAD_UPPER,              WDI_SLOVAK_ABC+(2<<16)},
    {GUIIM_M_TP_SLOVAK_ABC_LOWER,              WDI_SLOVAK_ABC+(1<<16)},
    {GUIIM_M_TP_SLOVAK_ABC_UPPER,              WDI_SLOVAK_ABC+(3<<16)},

    {GUIIM_M_TPQ_SLOVAK_LEAD_UPPER,           WDI_SLOVAK+(2<<16)},
    {GUIIM_M_TPQ_SLOVAK_LOWER,                WDI_SLOVAK+(1<<16)},
    {GUIIM_M_TPQ_SLOVAK_UPPER,                WDI_SLOVAK+(3<<16)},
    {GUIIM_M_TPQ_SLOVAK_ABC_LEAD_UPPER,            WDI_SLOVAK_ABC+(2<<16)},    
    {GUIIM_M_TPQ_SLOVAK_ABC_LOWER,            WDI_SLOVAK_ABC+(1<<16)},
    {GUIIM_M_TPQ_SLOVAK_ABC_UPPER,            WDI_SLOVAK_ABC+(3<<16)},
#endif

#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_TP_SLOVENIAN_LEAD_UPPER,             WDI_SLOVENIAN+(2<<16)},
    {GUIIM_M_TP_SLOVENIAN_LOWER,                  WDI_SLOVENIAN+(1<<16)},
    {GUIIM_M_TP_SLOVENIAN_UPPER,                  WDI_SLOVENIAN+(3<<16)},
    {GUIIM_M_TP_SLOVENIAN_ABC_LEAD_UPPER,              WDI_SLOVENIAN_ABC+(2<<16)},
    {GUIIM_M_TP_SLOVENIAN_ABC_LOWER,              WDI_SLOVENIAN_ABC+(1<<16)},
    {GUIIM_M_TP_SLOVENIAN_ABC_UPPER,              WDI_SLOVENIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_SLOVENIAN_LEAD_UPPER,           WDI_SLOVENIAN+(2<<16)},
    {GUIIM_M_TPQ_SLOVENIAN_LOWER,                WDI_SLOVENIAN+(1<<16)},
    {GUIIM_M_TPQ_SLOVENIAN_UPPER,                WDI_SLOVENIAN+(3<<16)},
    {GUIIM_M_TPQ_SLOVENIAN_ABC_LEAD_UPPER,            WDI_SLOVENIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_SLOVENIAN_ABC_LOWER,            WDI_SLOVENIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_SLOVENIAN_ABC_UPPER,            WDI_SLOVENIAN_ABC+(3<<16)},
#endif

#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_TP_SPANISH_LEAD_UPPER,             WDI_SPANISH+(2<<16)},
    {GUIIM_M_TP_SPANISH_LOWER,                  WDI_SPANISH+(1<<16)},
    {GUIIM_M_TP_SPANISH_UPPER,                  WDI_SPANISH+(3<<16)},
    {GUIIM_M_TP_SPANISH_ABC_LEAD_UPPER,              WDI_SPANISH_ABC+(2<<16)},
    {GUIIM_M_TP_SPANISH_ABC_LOWER,              WDI_SPANISH_ABC+(1<<16)},
    {GUIIM_M_TP_SPANISH_ABC_UPPER,              WDI_SPANISH_ABC+(3<<16)},

    {GUIIM_M_TPQ_SPANISH_LEAD_UPPER,           WDI_SPANISH+(2<<16)},
    {GUIIM_M_TPQ_SPANISH_LOWER,                WDI_SPANISH+(1<<16)},
    {GUIIM_M_TPQ_SPANISH_UPPER,                WDI_SPANISH+(3<<16)},
    {GUIIM_M_TPQ_SPANISH_ABC_LEAD_UPPER,            WDI_SPANISH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_SPANISH_ABC_LOWER,            WDI_SPANISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_SPANISH_ABC_UPPER,            WDI_SPANISH_ABC+(3<<16)},
#endif

#if defined (IM_SWAHILI_SUPPORT)
    {GUIIM_M_TP_SWAHILI_LEAD_UPPER,             WDI_SWAHILI+(2<<16)},
    {GUIIM_M_TP_SWAHILI_LOWER,                  WDI_SWAHILI+(1<<16)},
    {GUIIM_M_TP_SWAHILI_UPPER,                  WDI_SWAHILI+(3<<16)},
    {GUIIM_M_TP_SWAHILI_ABC_LEAD_UPPER,              WDI_SWAHILI_ABC+(2<<16)},
    {GUIIM_M_TP_SWAHILI_ABC_LOWER,              WDI_SWAHILI_ABC+(1<<16)},
    {GUIIM_M_TP_SWAHILI_ABC_UPPER,              WDI_SWAHILI_ABC+(3<<16)},

    {GUIIM_M_TPQ_SWAHILI_LEAD_UPPER,           WDI_SWAHILI+(2<<16)},
    {GUIIM_M_TPQ_SWAHILI_LOWER,                WDI_SWAHILI+(1<<16)},
    {GUIIM_M_TPQ_SWAHILI_UPPER,                WDI_SWAHILI+(3<<16)},
    {GUIIM_M_TPQ_SWAHILI_ABC_LEAD_UPPER,            WDI_SWAHILI_ABC+(2<<16)},    
    {GUIIM_M_TPQ_SWAHILI_ABC_LOWER,            WDI_SWAHILI_ABC+(1<<16)},
    {GUIIM_M_TPQ_SWAHILI_ABC_UPPER,            WDI_SWAHILI_ABC+(3<<16)},
#endif

#if defined (IM_SWEDISH_SUPPORT)
    {GUIIM_M_TP_SWEDISH_LEAD_UPPER,             WDI_SWEDISH+(2<<16)},
    {GUIIM_M_TP_SWEDISH_LOWER,                  WDI_SWEDISH+(1<<16)},
    {GUIIM_M_TP_SWEDISH_UPPER,                  WDI_SWEDISH+(3<<16)},
    {GUIIM_M_TP_SWEDISH_ABC_LEAD_UPPER,              WDI_SWEDISH_ABC+(2<<16)},
    {GUIIM_M_TP_SWEDISH_ABC_LOWER,              WDI_SWEDISH_ABC+(1<<16)},
    {GUIIM_M_TP_SWEDISH_ABC_UPPER,              WDI_SWEDISH_ABC+(3<<16)},

    {GUIIM_M_TPQ_SWEDISH_LEAD_UPPER,           WDI_SWEDISH+(2<<16)},
    {GUIIM_M_TPQ_SWEDISH_LOWER,                WDI_SWEDISH+(1<<16)},
    {GUIIM_M_TPQ_SWEDISH_UPPER,                WDI_SWEDISH+(3<<16)},
    {GUIIM_M_TPQ_SWEDISH_ABC_LEAD_UPPER,            WDI_SWEDISH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_SWEDISH_ABC_LOWER,            WDI_SWEDISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_SWEDISH_ABC_UPPER,            WDI_SWEDISH_ABC+(3<<16)},
#endif

#if defined (IM_TURKISH_SUPPORT)
    {GUIIM_M_TP_TURKISH_LEAD_UPPER,             WDI_TURKISH+(2<<16)},
    {GUIIM_M_TP_TURKISH_LOWER,                  WDI_TURKISH+(1<<16)},
    {GUIIM_M_TP_TURKISH_UPPER,                  WDI_TURKISH+(3<<16)},
    {GUIIM_M_TP_TURKISH_ABC_LEAD_UPPER,              WDI_TURKISH_ABC+(2<<16)},
    {GUIIM_M_TP_TURKISH_ABC_LOWER,              WDI_TURKISH_ABC+(1<<16)},
    {GUIIM_M_TP_TURKISH_ABC_UPPER,              WDI_TURKISH_ABC+(3<<16)},

    {GUIIM_M_TPQ_TURKISH_LEAD_UPPER,           WDI_TURKISH+(2<<16)},
    {GUIIM_M_TPQ_TURKISH_LOWER,                WDI_TURKISH+(1<<16)},
    {GUIIM_M_TPQ_TURKISH_UPPER,                WDI_TURKISH+(3<<16)},
    {GUIIM_M_TPQ_TURKISH_ABC_LEAD_UPPER,            WDI_TURKISH_ABC+(2<<16)},    
    {GUIIM_M_TPQ_TURKISH_ABC_LOWER,            WDI_TURKISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_TURKISH_ABC_UPPER,            WDI_TURKISH_ABC+(3<<16)},
#endif

#if defined (IM_UKRAINIAN_SUPPORT)
    {GUIIM_M_TP_UKRAINIAN_LEAD_UPPER,             WDI_UKRAINIAN+(2<<16)},
    {GUIIM_M_TP_UKRAINIAN_LOWER,                  WDI_UKRAINIAN+(1<<16)},
    {GUIIM_M_TP_UKRAINIAN_UPPER,                  WDI_UKRAINIAN+(3<<16)},
    {GUIIM_M_TP_UKRAINIAN_ABC_LEAD_UPPER,              WDI_UKRAINIAN_ABC+(2<<16)},
    {GUIIM_M_TP_UKRAINIAN_ABC_LOWER,              WDI_UKRAINIAN_ABC+(1<<16)},
    {GUIIM_M_TP_UKRAINIAN_ABC_UPPER,              WDI_UKRAINIAN_ABC+(3<<16)},

    {GUIIM_M_TPQ_UKRAINIAN_LEAD_UPPER,           WDI_UKRAINIAN+(2<<16)},
    {GUIIM_M_TPQ_UKRAINIAN_LOWER,                WDI_UKRAINIAN+(1<<16)},
    {GUIIM_M_TPQ_UKRAINIAN_UPPER,                WDI_UKRAINIAN+(3<<16)},
    {GUIIM_M_TPQ_UKRAINIAN_ABC_LEAD_UPPER,            WDI_UKRAINIAN_ABC+(2<<16)},    
    {GUIIM_M_TPQ_UKRAINIAN_ABC_LOWER,            WDI_UKRAINIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_UKRAINIAN_ABC_UPPER,            WDI_UKRAINIAN_ABC+(3<<16)},
#endif

#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_TP_VIETNAMESE_LEAD_UPPER,             WDI_VIETNAMESE+(2<<16)},
    {GUIIM_M_TP_VIETNAMESE_LOWER,                  WDI_VIETNAMESE+(1<<16)},
    {GUIIM_M_TP_VIETNAMESE_UPPER,                  WDI_VIETNAMESE+(3<<16)},
    {GUIIM_M_TP_VIETNAMESE_ABC_LEAD_UPPER,              WDI_VIETNAMESE_ABC+(2<<16)},
    {GUIIM_M_TP_VIETNAMESE_ABC_LOWER,              WDI_VIETNAMESE_ABC+(1<<16)},
    {GUIIM_M_TP_VIETNAMESE_ABC_UPPER,              WDI_VIETNAMESE_ABC+(3<<16)},

    {GUIIM_M_TPQ_VIETNAMESE_LEAD_UPPER,           WDI_VIETNAMESE+(2<<16)},
    {GUIIM_M_TPQ_VIETNAMESE_LOWER,                WDI_VIETNAMESE+(1<<16)},
    {GUIIM_M_TPQ_VIETNAMESE_UPPER,                WDI_VIETNAMESE+(3<<16)},
    {GUIIM_M_TPQ_VIETNAMESE_ABC_LEAD_UPPER,            WDI_VIETNAMESE_ABC+(2<<16)},    
    {GUIIM_M_TPQ_VIETNAMESE_ABC_LOWER,            WDI_VIETNAMESE_ABC+(1<<16)},
    {GUIIM_M_TPQ_VIETNAMESE_ABC_UPPER,            WDI_VIETNAMESE_ABC+(3<<16)},
#endif

#if defined (IM_ZULU_SUPPORT)
    {GUIIM_M_TP_ZULU_LEAD_UPPER,             WDI_ZULU+(2<<16)},
    {GUIIM_M_TP_ZULU_LOWER,                  WDI_ZULU+(1<<16)},
    {GUIIM_M_TP_ZULU_UPPER,                  WDI_ZULU+(3<<16)},
    {GUIIM_M_TP_ZULU_ABC_LEAD_UPPER,              WDI_ZULU_ABC+(2<<16)},
    {GUIIM_M_TP_ZULU_ABC_LOWER,              WDI_ZULU_ABC+(1<<16)},
    {GUIIM_M_TP_ZULU_ABC_UPPER,              WDI_ZULU_ABC+(3<<16)},

    {GUIIM_M_TPQ_ZULU_LEAD_UPPER,           WDI_ZULU+(2<<16)},
    {GUIIM_M_TPQ_ZULU_LOWER,                WDI_ZULU+(1<<16)},
    {GUIIM_M_TPQ_ZULU_UPPER,                WDI_ZULU+(3<<16)},
    {GUIIM_M_TPQ_ZULU_ABC_LEAD_UPPER,            WDI_ZULU_ABC+(2<<16)},    
    {GUIIM_M_TPQ_ZULU_ABC_LOWER,            WDI_ZULU_ABC+(1<<16)},
    {GUIIM_M_TPQ_ZULU_ABC_UPPER,            WDI_ZULU_ABC+(3<<16)},
#endif

#if defined (IM_KHMER_SUPPORT)
    {GUIIM_M_TP_KHMER,              WDI_KHMER + (1<<16)},
    {GUIIM_M_TP_KHMER_ABC,              WDI_KHMER_ABC+(3<<16)},

    {GUIIM_M_TPQ_KHMER,                WDI_KHMER+(1<<16)},
    {GUIIM_M_TPQ_KHMER_ABC,            WDI_KHMER_ABC+(1 << 16)},
#endif

#if defined (IM_LAO_SUPPORT)
    {GUIIM_M_TP_LAO,              WDI_LAO + (1<<16)},
    {GUIIM_M_TP_LAO_ABC,              WDI_LAO_ABC+(3<<16)},

    {GUIIM_M_TPQ_LAO,                WDI_LAO+(1<<16)},
    {GUIIM_M_TPQ_LAO_ABC,            WDI_LAO_ABC+(1 << 16)},
#endif

#if defined (IM_UYGHUR_SUPPORT)
    {GUIIM_M_TP_UYGHUR,              WDI_UIGHUR + (1<<16)},
    {GUIIM_M_TP_UYGHUR_ABC,              WDI_UIGHUR_ABC+(3<<16)},

    {GUIIM_M_TPQ_UYGHUR,                WDI_UIGHUR+(1<<16)},
    {GUIIM_M_TPQ_UYGHUR_ABC,            WDI_UIGHUR_ABC+(1 << 16)},
#endif

#if defined (IM_SINHALESE_SUPPORT)
    {GUIIM_M_TP_SINHALESE,              WDI_SINHALESE + (1<<16)},
    {GUIIM_M_TP_SINHALESE_ABC,              WDI_SINHALESE_ABC+(3<<16)},

    {GUIIM_M_TPQ_SINHALESE,                WDI_SINHALESE+(1<<16)},
    {GUIIM_M_TPQ_SINHALESE_ABC,            WDI_SINHALESE_ABC+(1 << 16)},
#endif

};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/******************************************************************************/
// Description: Initialize
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr)
{
    MMIIM_SOGOU_DATA_T *data_ptr = PNULL;

	SCI_TRACE_LOW ("[MMIIM] [sogou] Initialize: param_ptr=%08X", param_ptr);
    //SCI_ASSERT(PNULL != param_ptr);  /*assert verified*/
    if(PNULL == param_ptr)
    {
        return  data_ptr;
    }
    data_ptr = (MMIIM_SOGOU_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SOGOU_DATA_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_SOGOU_DATA_T));

    //data
    data_ptr->init_param = *param_ptr;
    data_ptr->lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;
    data_ptr->win_id = param_ptr->win_id;

    SOGOU_ImeInit(data_ptr);
    SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_set.method);

    return data_ptr;
}

/******************************************************************************/
// Description: Terminate
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL BOOLEAN Terminate(void *handle)
{
    MMIIM_SOGOU_DATA_T *data_ptr = (MMIIM_SOGOU_DATA_T*)handle;
	
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    if(PNULL == data_ptr)
    {
        return FALSE;
    }

	//加个监测，否则这里老是脱不了关系
	MMI_CheckAllocatedMemInfo();	
    SG_Quit();
	MMI_CheckAllocatedMemInfo();
	
    SCI_FREE(data_ptr);
#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_SGFreeImeSkinBuffer();
#endif
    return TRUE; /*lint !e438*/
}

/******************************************************************************/
// Description: HandleMsg
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleMsg(void *handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *event_data_ptr)
{
    MMIIM_SOGOU_DATA_T *data_ptr = (MMIIM_SOGOU_DATA_T*)handle;
    BOOLEAN result = FALSE;

    if (PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleMsg: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SOGOU_1876_112_2_18_2_25_16_47,(uint8*)"");
        return FALSE;
    }

    switch (event)
    {
    case GUIIM_EVENT_SYS_MSG:
        result = HandleSysMsg(data_ptr, event_data_ptr);
        break;

    case GUIIM_EVENT_SET_METHOD:
        result = SetMethod(handle, event_data_ptr->set_method);
        break;

    case GUIIM_EVENT_GET_METHOD:
        event_data_ptr->get_method = data_ptr->method;
        result = TRUE;
        break;

    case GUIIM_EVENT_PAINT:
        if (!IsPlatfromNativeIME(data_ptr->method)
            && (0 != data_ptr->edit_win.state
            || 0 != data_ptr->letter_win.state
            || 0 != data_ptr->cand_win.state
            ))
        {
#ifdef SOGOU_USE_PDA_SKIN
            SG_DisplaySkin(data_ptr);
#else
            SG_DisplayBg (data_ptr);
#endif
            if (data_ptr->preeditwin_height > 0)
            {
                MMIIM_DrawPreEditWin(data_ptr->lcd_info_ptr, &(data_ptr->edit_win), data_ptr->win_id);
            }
            if (data_ptr->letterwin_height > 0)
            {
                MMIIM_DrawLetterWin(data_ptr->lcd_info_ptr, &(data_ptr->letter_win), &data_ptr->lastdisp,
                    &data_ptr->imeResult, data_ptr->win_id);
            }
            if (data_ptr->candwin_height > 0)
            {
                MMIIM_DrawCandWin(data_ptr->lcd_info_ptr, &(data_ptr->cand_win), &data_ptr->lastdisp,
                    data_ptr->win_id, &data_ptr->imeResult);
            }
        }
        result = TRUE;
        break;

    case GUIIM_EVENT_SLEEP:
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_CTRL_RECT:
    case GUIIM_EVENT_GET_RECT:
        event_data_ptr->get_rect = data_ptr->rect;
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_EDIT_STATE:
        /*
        9keys逻辑比较清晰
        26keys
            1 当无联想字的时候，统一就是全程delete
            2 有联想字的时候，联想字改成cancel，其他编辑过程，都是inputing
        */

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        if (GUIIM_INPUT_TYPE_QWERT == data_ptr->init_param.method_param_ptr->im_def_ptr->input)
        {
            if (0 != data_ptr->cand_win.count)
            {
                if ((GUIIM_LANG_CHS == data_ptr->init_param.method_param_ptr->im_def_ptr->lang) ||
                    (GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
                {
                    if (0 != data_ptr->edit_win.count)
                    {
                        //event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                        if (data_ptr->cand_win.has_lable)
		                {
		                    event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
		                }
		                else
		                {
		                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
		                }
                    }
                    else
                    {
                        event_data_ptr->edit_state = GUIIM_EDIT_CANDITATE;
                    }
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_DEALING;
                }
            }
            else
            {
                event_data_ptr->edit_state = GUIIM_EDIT_NONE;
            }
        }
        else
#endif
        {
            if (0 != data_ptr->edit_win.count)
            {
                if (data_ptr->cand_win.has_lable)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                }
            }
            else if (0 != data_ptr->cand_win.count)
            {
                //edit 没有内容，也可能是在编写
                if (MMIIM_WIN_STATE_EDIT == data_ptr->cand_win.state)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                }
                else if (data_ptr->cand_win.has_lable)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_CANDITATE;
                }
            }
            else
            {
                event_data_ptr->edit_state = GUIIM_EDIT_NONE;
            }
        }

        result = TRUE;
        break;

    default:
        break;
    }

    return result;
}

/******************************************************************************/
// Description: 处理TP消息
// Global resource dependence: NONE
// Author:
// Note:
/******************************************************************************/
LOCAL void HandleTpKeySelect(MMIIM_SOGOU_DATA_T *data_ptr,uint16 tp_index)
{
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    SG_UINT input_method;

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    input_method = SOGOU_GetInputMethod(data_ptr);

    if (WDI_CN_BIHUA != input_method)
    {
        switch (SOGOU_GetImeResult(data_ptr)->imeStatus)
        {
        case WD_STATUS_CHINESE_INPUT:
        case WD_STATUS_CHINESE_LEGEND:
        case WD_STATUS_FOREIGN_INPUT:
        case WD_STATUS_FOREIGN_LEGEND:
            SOGOU_HandleIMEOnKeyEvent(data_ptr,  WDK_DOWN);
            break;
        default:
            break;
        }
        SOGOU_HandleIMEOnKeyEvent(data_ptr,WDK_NORMAL_1 + tp_index);
    }
    else
    {
        if (MMIIM_WIN_STATE_CAND != data_ptr->cand_win.state)
        {
            SOGOU_HandleIMEOnKeyEvent(data_ptr,  WDK_DOWN);
        }

        SOGOU_HandleIMEOnKeyEvent(data_ptr,  WDK_NORMAL_1 + tp_index);
    }
#else
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    WD_SetVariable(1, tp_index + 1, &(data_ptr->imeResult));
    SOGOU_HandleIMEOnKeyEvent(data_ptr,  WDK_OK);
#endif
}

/******************************************************************************/
// Description: 处理系统消息
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsg(MMIIM_SOGOU_DATA_T *data_ptr, GUIIM_EVENT_DATA_U *event_data_ptr)
{
    LOCAL BOOLEAN s_is_down_pass = FALSE;
    uint32 sg_vk = SOGOU_VK_NONE;
    GUI_POINT_T tp_point = {0};
    BOOLEAN is_tp_cand = FALSE;
    uint16 tp_index = 0;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    SG_UINT event = SOGOU_EVENT_KEYBOARD;
    GUI_BOTH_RECT_T pop_menu_rect = {0};
    unsigned short commit_str[GUIIM_COMMIT_BUFFER_MAX + 1] = { 0 };
#ifndef KEYPAD_TYPE_QWERTY_KEYPAD
    wchar num_char = 0;
#endif
    WD_UINT8 old_ime_status = SOGOU_GetImeResult(data_ptr)->imeStatus;

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    if(PNULL == data_ptr)
    {
        return FALSE;
    }
    
    switch(event_data_ptr->sys_msg.msg_id)
    {
    case MSG_TIMER:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        tp_point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        tp_point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        if(GUI_PointIsInRect(tp_point, data_ptr->cand_win.rect_cand))
        {

            is_tp_cand = MMIIM_TpDownCandAsKey(&data_ptr->cand_win,
                tp_point,
                &tp_index,
                SOGOU_GetLastDisp(data_ptr));

            if(is_tp_cand)
            {
                HandleTpKeySelect(data_ptr, tp_index);
            }
            else if (!SOGOU_GetImeResult(data_ptr)->selecting[0][0])
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        else if(GUI_PointIsInRect(tp_point, data_ptr->cand_win.rect_idc_left))
        {
            if(data_ptr->cand_win.idc_flag & MMIIM_IDC_FLAG_LEFT)
            {
                SOGOU_HandleIMEOnKeyEvent(data_ptr,  (data_ptr->cand_win.draw_direction == MMIIM_DRAW_RIGHT2LEFT) ? WDK_RIGHT : WDK_LEFT);
            }
            else
            {
                return TRUE;
            }
        }
        else if(GUI_PointIsInRect(tp_point, data_ptr->cand_win.rect_idc_right))
        {
            if(data_ptr->cand_win.idc_flag & MMIIM_IDC_FLAG_RIGHT)
            {
                if (data_ptr->imeResult.imeStatus != WD_STATUS_FOREIGN_SELECTLETTER &&
                    data_ptr->imeResult.imeStatus != WD_STATUS_CHINESE_SELECTPINYIN)
                {
                    SOGOU_HandleIMEOnKeyEvent(data_ptr,  (data_ptr->cand_win.draw_direction == MMIIM_DRAW_RIGHT2LEFT) ? WDK_LEFT : WDK_RIGHT);
                }
            }
            else
            {
                return TRUE;
            }
        }
        else
        {
            return FALSE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    //NEWMS00116735 [6800h][openphone]搜狗输入法 按键输入 点击候选栏字符 即调出软键盘
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
    {
        tp_point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        tp_point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        if(GUI_PointIsInRect(tp_point, data_ptr->cand_win.rect_cand))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
        break;  /*lint !e527 */
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
    case MSG_KEYLONG_CANCEL:        /*lint !e835 !e616*/
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYLONG_DEL:       /*lint !e835 !e616*/
#endif
        SOGOU_HandleIMEOnKeyEvent(data_ptr,WDK_LONG_C);
        break;

    default:
        sg_vk = ConvertKey(data_ptr, event_data_ptr, &event);

        if(SOGOU_VK_NONE == sg_vk)
        {
            return FALSE;
        }

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        if (KEY_CTRL == sg_vk)
        {
            OpenSwitchIMWin(data_ptr, &pop_menu_rect);
            return TRUE;
        }

        if (WDK_FUNC == sg_vk && (SOGOU_VK_NONE != event) &&
            (SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_CHINESE_NORMAL ||
             SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_CHINESE_LEGEND ||
             SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_NORMAL ||
             SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_LEGEND))
        {
            OpenSysbolWin(data_ptr);
            return TRUE;
        }
        // 26键情况下,在外文输入状态下,或选字状态下在第一页时,屏蔽向上键
        if (WDK_UP == sg_vk && 
            ((SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_INPUT) ||
             (SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_SELECT && SOGOU_GetImeResult(data_ptr)->curPage == 0)))
        {
            return TRUE;
        }
#else
        if (WDK_SHARP == sg_vk)
        {
            OpenSwitchIMWin(data_ptr, &pop_menu_rect);
            return TRUE;
        }

        //* 键用于弹出符号窗口
        if (WDK_STAR == sg_vk && !SOGOU_GetImeResult(data_ptr)->selecting[0][0])
        {
            OpenSysbolWin(data_ptr);
            return TRUE;
        }
#endif // #ifdef KEYPAD_TYPE_QWERTY_KEYPAD

#ifndef KEYPAD_TYPE_QWERTY_KEYPAD
        switch (event_data_ptr->sys_msg.msg_id & MMIIM_SOGOU_KEY_MASK)
        {
        /* key */
        case KEY_0:
            num_char = '0';
            break;

        case KEY_1:
        case KEY_2:
        case KEY_3:
        case KEY_4:
        case KEY_5:
        case KEY_6:
        case KEY_7:
        case KEY_8:
        case KEY_9:
            num_char = '1' + (event_data_ptr->sys_msg.msg_id & MMIIM_SOGOU_KEY_MASK) - KEY_1;
            break;

        default:
            break;
        }

        //按中的是数字键
        if (0 != num_char)
        {
            //非输入情况下长按数字键上数字
            if((sg_vk & 0xffff0000) == SOGOU_VK_LONGCLICK && s_is_down_pass)
            {
                switch (event_data_ptr->sys_msg.msg_id & MMIIM_SOGOU_KEY_MASK)
                {
                case KEY_0:
                    sg_vk = 0x25;
                    break;

                case KEY_1:
                case KEY_2:
                case KEY_3:
                case KEY_4:
                case KEY_5:
                case KEY_6:
                case KEY_7:
                case KEY_8:
                case KEY_9:
                    sg_vk = 0x26 + (event_data_ptr->sys_msg.msg_id & MMIIM_SOGOU_KEY_MASK) - KEY_1;
                    break;

                default:
                    return TRUE;
                }
            }
            else if((sg_vk & 0xffff0000) == MMIIM_SOGOU_VKEY_PRESSSED)
            {
                s_is_down_pass = TRUE;
                return TRUE;
            }
            else if((sg_vk & 0xffff0000) == MMIIM_SOGOU_VKEY_RELEASED && s_is_down_pass)
            {
                //不返回，修改键值
                sg_vk &= 0x0000ffff;
                sg_vk |= MMIIM_SOGOU_VKEY_PRESSSED; /*lint !e835*/
            }
        }
        else if ((event_data_ptr->sys_msg.msg_id & MMIIM_SOGOU_KEY_MASK) == KEY_STAR)
        {
            if ((sg_vk & 0xffff0000) == MMIIM_SOGOU_VKEY_PRESSSED)
            {
                s_is_down_pass = TRUE;
                return TRUE;
            }
            else if((sg_vk & 0xffff0000) == MMIIM_SOGOU_VKEY_RELEASED && s_is_down_pass)
            {
                sg_vk &= 0x0000ffff;
                sg_vk |= MMIIM_SOGOU_VKEY_PRESSSED; /*lint !e835*/
            }
            else if (((sg_vk & 0xffff0000) == SOGOU_VK_LONGCLICK && s_is_down_pass))
            {
                sg_vk = WDK_LONG_STAR;
            }
        }
#else
        switch (event_data_ptr->sys_msg.msg_id & MMIIM_SOGOU_KEY_MASK)
        {
        case KEY_DEL:
        case KEY_WEB:
        case KEY_OK:
        case KEY_CANCEL:
        case KEY_ENTER:
        case KEY_UP:
        case KEY_LEFT:
        case KEY_RIGHT:
        case KEY_DOWN:
            break;

        default:
            if ((sg_vk & 0xffff0000) == MMIIM_SOGOU_VKEY_PRESSSED)
            {
                s_is_down_pass = TRUE;
                return TRUE;
            }
            else if((sg_vk & 0xffff0000) == MMIIM_SOGOU_VKEY_RELEASED && s_is_down_pass)
            {
                //不返回，修改键值
                sg_vk &= 0x0000ffff;
                sg_vk |= MMIIM_SOGOU_VKEY_PRESSSED; /*lint !e835*/
            }
            break;
        }
#endif

        //不处理up消息
        if((sg_vk & MMIIM_SOGOU_VKEY_RELEASED))
        {
            return FALSE;
        }

        s_is_down_pass = FALSE;

        if (SOGOU_VK_NONE == event)
        {
            commit_str[0] = sg_vk;
        }
        else
        {
            SOGOU_HandleIMEOnKeyEvent(data_ptr,sg_vk);
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
            if (sg_vk == WDK_C && SOGOU_GetImeResult(data_ptr)->returnKey)
            {
                data_ptr->isHandleLongDel = TRUE;
            }
            else
            {
                data_ptr->isHandleLongDel = FALSE;
            }
#endif
        }
    }

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    if (SOGOU_VK_NONE == event)
    {
        if (0 == data_ptr->cand_win.count)
        {
            notify_pack.data.commit_data.commit[0] = (wchar)(commit_str[0]);
            notify_pack.data.commit_data.commit_len = 1;
            notify_pack.notify = GUIIM_NOTIFY_COMMIT;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }
    }
    else
#endif
    if (SOGOU_GetImeResult(data_ptr)->output[0])
    {
        uint32 len_1 = sizeof(notify_pack.data.commit_data.commit);
        uint32 len_2 = MMIAPICOM_Wstrlen(SOGOU_GetImeResult(data_ptr)->output);

        notify_pack.data.commit_data.commit_len = MIN(len_1, len_2);
        SCI_MEMCPY(notify_pack.data.commit_data.commit,
                   SOGOU_GetImeResult(data_ptr)->output,
                   notify_pack.data.commit_data.commit_len * sizeof(WD_UINT16));

 //TK版本最后一个空格不上屏，纯物理输入，空格还是需要
#if defined MMI_IM_PDA_SUPPORT   
        if (notify_pack.data.commit_data.commit_len > 1 && notify_pack.data.commit_data.commit[notify_pack.data.commit_data.commit_len - 1] == ' ')
        {
            notify_pack.data.commit_data.commit_len --;   
        }
#endif

        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    }

    if ((!IsPlatfromNativeIME(data_ptr->method)) && (SOGOU_VK_NONE != event))
    {
        // 前后状态不同则更改编辑区域大小
        if (old_ime_status != SOGOU_GetImeResult(data_ptr)->imeStatus)
        {
            SetImeDisplayArea(data_ptr);
        }

#ifdef SOGOU_USE_PDA_SKIN
        SG_DisplaySkin(data_ptr);
#else
        SG_DisplayBg (data_ptr);
#endif
        // 更新编辑窗口内容
        DrawPreEditWin(data_ptr, TRUE);

        // 更新编辑窗口内容
        DrawLetterWin(data_ptr, TRUE);

        // 更新候选词窗口内容
        DrawCandWin(data_ptr, TRUE);
    }

    if (SOGOU_GetImeResult(data_ptr)->returnKey)
    {
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        if (event_data_ptr->sys_msg.msg_id == MSG_KEYLONG_DEL && data_ptr->isHandleLongDel)
        {
            data_ptr->isHandleLongDel = FALSE;
            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            return TRUE;
        }
        if (event_data_ptr->sys_msg.msg_id == MSG_KEYDOWN_DEL)
        {
            notify_pack.data.commit_data.commit[0] = (wchar)'\b';
            notify_pack.data.commit_data.commit_len = 1;
            notify_pack.notify = GUIIM_NOTIFY_COMMIT;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            return TRUE;
        }
        if (SOGOU_VK_NONE == event && 0 == data_ptr->cand_win.count)
        {
            return TRUE;
        }
#endif
        if ((sg_vk == WDK_DOWN || sg_vk == WDK_UP || sg_vk == WDK_LEFT || sg_vk == WDK_RIGHT) &&
            SOGOU_GetImeResult(data_ptr)->imeStatus != WD_STATUS_CHINESE_NORMAL &&
            SOGOU_GetImeResult(data_ptr)->imeStatus != WD_STATUS_FOREIGN_NORMAL &&
            SOGOU_GetImeResult(data_ptr)->imeStatus != WD_STATUS_ABC_NORMAL)
        {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

/******************************************************************************/
// Description: SetMethod
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL BOOLEAN SetMethod(void *handle, GUIIM_METHOD_T method)
{
    MMIIM_SOGOU_DATA_T *data_ptr = (MMIIM_SOGOU_DATA_T*)handle;
    SG_UINT     input_method;

    //SCI_ASSERT(PNULL != handle);  /*assert verified*/
    if(PNULL == data_ptr)
    {
        return FALSE;
    }
    
    data_ptr->method = method;

    input_method = MMIIM_Sogou_ConvertMethod (method); //此函数中会做完是否物理全键盘的输入法转换操作

    WD_SwitchIme(input_method&0x0000FFFF);

    switch(input_method>>16)
    {
    case 2:
        WD_SwitchForeignCapslock(1);
        break;

    case 3:
        WD_SwitchForeignCapslock(2);
        break;

    case 1:
        WD_SwitchForeignCapslock(0);
        break;

    default:
        break;
    }

    //set the drawing direction of candidate bar and edit bar.
    switch (input_method)
    {
    case WDI_ARABIC:
    case WDI_ARABIC_ABC:
    case WDI_URDU:
    case WDI_URDU_ABC:
    case WDI_PERSIAN:
    case WDI_PERSIAN_ABC:
    case WDI_HEBREW:
    case WDI_HEBREW_ABC:
        data_ptr->edit_win.draw_direction = MMIIM_DRAW_RIGHT2LEFT;
        data_ptr->cand_win.draw_direction = MMIIM_DRAW_RIGHT2LEFT;
        break;
    default:
        data_ptr->edit_win.draw_direction = MMIIM_DRAW_LEFT2RIGHT;
        data_ptr->cand_win.draw_direction = MMIIM_DRAW_LEFT2RIGHT;
        break;
    }

    //下面这修改必须注意：目前的物理UI都是会在下方画两条栏的，不论是什么输入法
    //现在需求要求在abc, 123下，不要画这些栏，由于没有UI状态值，所有暂时如下修改

    SetImeDisplayArea(data_ptr);

    return TRUE;
}

/******************************************************************************/
// Description: 初始化输入法相关的数据
// Global resource dependence: NONE
// Author: Ming.Song
// Note: 多处的+1/-1是边界处理
/******************************************************************************/
LOCAL void SetImeDisplayArea(MMIIM_SOGOU_DATA_T *data_ptr)
{
    GUI_RECT_T win_rect = {0};
    GUIIM_NOTIFY_PACK_T notify_pack = { 0 };

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }

    if (SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_CHINESE_NORMAL ||
        SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_NORMAL ||
        SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_ABC_NORMAL)
    {
        data_ptr->preeditwin_height = 0;
        data_ptr->candwin_height = 0;
        data_ptr->letterwin_height = 0;
    }
    else
    {
        data_ptr->preeditwin_height = SG_SKIN_HEIGHT / 3;
        data_ptr->candwin_height = SG_SKIN_HEIGHT / 3;
        data_ptr->letterwin_height = SG_SKIN_HEIGHT / 3;
    }

    data_ptr->cand_win.lable_width = GUI_GetFontWidth(g_mmiim_ui_config.font_lable, UNICODE_ASCII);
    data_ptr->letter_win.lable_width = 3;
    win_rect = MMITHEME_GetClientRectEx(data_ptr->win_id);

#if defined MMI_IM_PDA_SUPPORT
//下面的这段只可能成立一个条件，如果简繁同时开，宏使用#if, #elif 则有问题
#if defined IM_SIMP_CHINESE_SUPPORT
    if (data_ptr->imeResult.imeType == WDI_CN_BIHUA)
    {
        data_ptr->preeditwin_height = 0;
    }
#endif

#if defined IM_TRAD_CHINESE_SUPPORT    
    if (data_ptr->imeResult.imeType == WDI_CN_BIHUAF)
    {
        data_ptr->preeditwin_height = 0;
    }
#endif
#else   //MMI_PDA_SUPPORT
    if (data_ptr->imeResult.outputKeys[0] == 0)
    {
        data_ptr->preeditwin_height = 0;
    }
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    data_ptr->letterwin_height = 0;
#else
    if (data_ptr->imeResult.imeType == WDI_CN_BIHUA)
    {
        data_ptr->preeditwin_height = 0;
    }
    if (data_ptr->imeResult.letterSelecting[0][0] == 0)
    {
        data_ptr->letterwin_height = 0;
    }    
#endif
    if (data_ptr->imeResult.selecting[0][0] == 0)
    {
        data_ptr->candwin_height = 0;
    }
#endif  //MMI_PDA_SUPPORT

    if (data_ptr->candwin_height > 0)
    {
        MMIIM_InitCandWin(&data_ptr->cand_win, &win_rect, data_ptr->candwin_height);
        win_rect.bottom = data_ptr->cand_win.rect_cand.top - MMIIM_EDIT_WIN_SPACE;
    }
    if (data_ptr->letterwin_height > 0)
    {
        MMIIM_InitLetterWin(&data_ptr->letter_win, &win_rect, data_ptr->candwin_height);
        win_rect.bottom = data_ptr->letter_win.rect_cand.top - MMIIM_EDIT_WIN_SPACE + 1;
    }
    if (data_ptr->preeditwin_height > 0)
    {
        MMIIM_InitPreEditWin(&data_ptr->edit_win, &win_rect, data_ptr->preeditwin_height);
    }

    //这个区域有误
    // @pb and other application request, the bars can dynamic adjust the height in the bottom of windows.
    // so we hide the three bars when there is no content need to display.
    //if (data_ptr->preeditwin_height + data_ptr->candwin_height + data_ptr->letterwin_height > 0)
    {
        win_rect = MMITHEME_GetClientRectEx(data_ptr->win_id);
        data_ptr->rect.bottom = win_rect.bottom;
        data_ptr->rect.top = win_rect.bottom - (data_ptr->preeditwin_height + data_ptr->candwin_height + data_ptr->letterwin_height);
        data_ptr->rect.left = data_ptr->cand_win.rect_idc_left.left;
        data_ptr->rect.right = data_ptr->cand_win.rect_idc_right.right;
    }
    
    /*
    else
    {
        data_ptr->rect = MMITHEME_GetClientRectEx(data_ptr->win_id);
#if defined KEYPAD_TYPE_TYPICAL_KEYPAD      //9键存在prev字符串，而26键没有
        data_ptr->rect.top = data_ptr->rect.bottom - SG_SKIN_HEIGHT;
#else
        data_ptr->rect.top = data_ptr->rect.bottom - (SG_SKIN_HEIGHT * 2 / 3);
#endif
    }
    */

    notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
    GUIIM_CommitData (data_ptr->init_param.im_handle, &notify_pack);
}

/******************************************************************************/
// Description: 转化按键信息
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL uint32 ConvertKey(MMIIM_SOGOU_DATA_T *data_ptr, GUIIM_EVENT_DATA_U *event_data_ptr,SG_UINT *pEvent)
{
    uint32 sg_vk = SOGOU_VK_NONE;
    uint32 sg_vk_type = 0xffff0000;
    SG_UINT event = SOGOU_EVENT_KEYBOARD;
    MMI_MESSAGE_ID_E msg;
    GUIIM_METHOD_T  input_method;
    MMIIM_SP_VKEY_T sp_vk = MMIIM_SP_KEY_NONE;

    if (PNULL == data_ptr || PNULL == event_data_ptr || PNULL == pEvent )
    {
        //SCI_TRACE_LOW:"MMI: ConvertKey PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SOGOU_2611_112_2_18_2_25_18_48,(uint8*)"");
        return SOGOU_VK_NONE;
    }

    msg = event_data_ptr->sys_msg.msg_id;
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    if((MSG_KEYLONG_COMMA<=msg && msg<=MSG_KEYLONG_PERIOD) || (MSG_KEYLONG_COMMA<=msg && msg<=MSG_KEYLONG_PERIOD)
        || (MSG_KEYLONG_UP<=msg && msg<=MSG_KEYLONG_RIGHT) || (MSG_KEYPRESSUP_UP<=msg && msg<=MSG_KEYPRESSUP_RIGHT)
        || (MSG_KEYLONG_AND == msg) || (MSG_KEYLONG_QUESTION == msg) || (MSG_KEYLONG_EXCLAMATION == msg) || (MSG_KEYLONG_SAND == msg))
#else
    if ((MSG_KEYLONG_UP<=msg && msg<=MSG_KEYLONG_RIGHT) || (MSG_KEYPRESSUP_UP<=msg && msg<=MSG_KEYPRESSUP_RIGHT))
#endif
    {
        return SOGOU_VK_NONE;
    }

    // 不处理长按OK键消息
    if (MSG_KEYLONG_OK == msg || MSG_KEYLONG_WEB == msg)
    {
        //SCI_TRACE_LOW:"mmiim_sogou.c ConvertKey: doesn't process MSG_KEYLONG_OK, return 1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SOGOU_2645_112_2_18_2_25_18_49,(uint8*)"");
        return SOGOU_VK_NONE;
    }

    input_method = data_ptr->init_param.method_param_ptr->im_set.method;
    switch(msg & MMIIM_SOGOU_KEY_TYPE_MASK)
    {
    case KEY_PRESSED:
        sg_vk_type = MMIIM_SOGOU_VKEY_PRESSSED;
        break;

    case KEY_RELEASED:
        sg_vk_type = MMIIM_SOGOU_VKEY_RELEASED;
        break;

    case KEY_LONG_PRESSED:
        sg_vk_type = SOGOU_VK_LONGCLICK;
        break;

    default:
        return SOGOU_VK_NONE;
    }

    /* 键值转换 */
    sp_vk = MMIIM_PhysicalKeyMsgToImKey (msg);
    switch(sp_vk)
    {
#ifndef KEYPAD_TYPE_QWERTY_KEYPAD           //9键
        /* key */
    case MMIIM_SP_KEY_0:
        sg_vk = WDK_NORMAL_0;
        break;
    case MMIIM_SP_KEY_1:
        sg_vk = WDK_NORMAL_1;
        break;
    case MMIIM_SP_KEY_2:
        sg_vk = WDK_NORMAL_2;
        break;
    case MMIIM_SP_KEY_3:
        sg_vk = WDK_NORMAL_3;
        break;
    case MMIIM_SP_KEY_4:
        sg_vk = WDK_NORMAL_4;
        break;
    case MMIIM_SP_KEY_5:
        sg_vk = WDK_NORMAL_5;
        break;
    case MMIIM_SP_KEY_6:
        sg_vk = WDK_NORMAL_6;
        break;
    case MMIIM_SP_KEY_7:
        sg_vk = WDK_NORMAL_7;
        break;
    case MMIIM_SP_KEY_8:
        sg_vk = WDK_NORMAL_8;
        break;
    case MMIIM_SP_KEY_9:
        sg_vk = WDK_NORMAL_9;
        break;
    case MMIIM_SP_KEY_STAR:
        sg_vk = WDK_STAR;
        break;
    case MMIIM_SP_KEY_HASH:
        sg_vk = WDK_SHARP;
        break;
#endif
    //case MSG_CTL_CANCEL:
    case MMIIM_SP_KEY_CANCEL:
        sg_vk = WDK_C;
        break;
    case MMIIM_SP_KEY_LEFT:
        sg_vk = (data_ptr->cand_win.draw_direction == MMIIM_DRAW_RIGHT2LEFT) ? WDK_RIGHT : WDK_LEFT;
        break;
    case MMIIM_SP_KEY_UP:
        sg_vk = WDK_UP;
        break;
    case MMIIM_SP_KEY_RIGHT:
        sg_vk = (data_ptr->cand_win.draw_direction == MMIIM_DRAW_RIGHT2LEFT) ? WDK_LEFT : WDK_RIGHT;
        break;
    case MMIIM_SP_KEY_DOWN:
        sg_vk = WDK_DOWN;
        break;

    case MMIIM_SP_KEY_OK:
        sg_vk = WDK_OK;
        break;

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MMIIM_SP_KEY_Q:
    case MMIIM_SP_KEY_W:
    case MMIIM_SP_KEY_E:
    case MMIIM_SP_KEY_R:
    case MMIIM_SP_KEY_T:
    case MMIIM_SP_KEY_Y:
    case MMIIM_SP_KEY_U:
    case MMIIM_SP_KEY_I:
    case MMIIM_SP_KEY_O:
    case MMIIM_SP_KEY_P:
    case MMIIM_SP_KEY_A:
    case MMIIM_SP_KEY_S:
    case MMIIM_SP_KEY_D:
    case MMIIM_SP_KEY_F:
    case MMIIM_SP_KEY_G:
    case MMIIM_SP_KEY_H:
    case MMIIM_SP_KEY_K:
    case MMIIM_SP_KEY_L:
    case MMIIM_SP_KEY_J:
    case MMIIM_SP_KEY_Z:
    case MMIIM_SP_KEY_X:
    case MMIIM_SP_KEY_C:
    case MMIIM_SP_KEY_V:
    case MMIIM_SP_KEY_B:
    case MMIIM_SP_KEY_N:
    case MMIIM_SP_KEY_M:
        if (sg_vk_type == MMIIM_SOGOU_VKEY_RELEASED)
        {
            msg -= 0x100;
        }
        else if (sg_vk_type == SOGOU_VK_LONGCLICK)
        {
            msg -= 0x200;
        }
        if (GUIIM_M_Q_STROKE == input_method ||GUIIM_M_Q_123 == input_method)
        {
            sg_vk = s_stroke_app_keymap[msg - MSG_APP_Q];
        }
        else
        {
            sg_vk = s_common_app_keymap[msg - MSG_APP_Q];
            if (sg_vk_type == SOGOU_VK_LONGCLICK)
            {
                sg_vk = sg_vk + WDK_LONG_LETTER - WDK_NORMAL_LETTER;
            }
            event = SOGOU_EVENT_CHAR;
        }
        break;
    case MMIIM_SP_KEY_FN:
        sg_vk = WDK_FUNC;
        break;

    case MMIIM_SP_KEY_0:
        sg_vk = (uint16)'0';       //这个值要传到编辑器输入字符
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_1:
    case MMIIM_SP_KEY_2:
    case MMIIM_SP_KEY_3:
    case MMIIM_SP_KEY_4:
    case MMIIM_SP_KEY_5:
    case MMIIM_SP_KEY_6:
    case MMIIM_SP_KEY_7:
    case MMIIM_SP_KEY_8:
    case MMIIM_SP_KEY_9:
        if ((SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_CHINESE_SELECT)    
            || (SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_SELECT)
            )
        {
            sg_vk = (sp_vk) - MMIIM_SP_KEY_1 + WDK_NORMAL_1;
            event = SOGOU_EVENT_CHAR;
        }
        else
        {
            sg_vk = (uint16)'1' + ((sp_vk) - MMIIM_SP_KEY_1);       //这个值要传到编辑器输入字符
            event = SOGOU_VK_NONE;
        }    
        break;
    case MMIIM_SP_KEY_AT_QWERTY:
        sg_vk = (uint16)'@';
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_AND:
        sg_vk = (uint16)'&';
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_SPACE:
        sg_vk = WDK_SPACE;
        if ((msg & KEY_LONG_PRESSED) == KEY_LONG_PRESSED)
        {
            sg_vk = SOGOU_VK_NONE;
            event = SOGOU_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_ENTER:
        //sogou非输入状态，直接将enter键上屏
        if (SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_CHINESE_NORMAL
            || SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_NORMAL
            || SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_ABC_NORMAL)
        {
            sg_vk = (uint16)'\n';
            event = SOGOU_VK_NONE;
        }
        else
        {
            sg_vk = WDK_ENTER;
            if ((msg & KEY_LONG_PRESSED) == KEY_LONG_PRESSED)
            {
                sg_vk = SOGOU_VK_NONE;
                event = SOGOU_VK_NONE;
            }
        }

        break;
    case MMIIM_SP_KEY_SHIFT:
        sg_vk = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_CTRL:
        sg_vk = KEY_CTRL;
        break;
    case MMIIM_SP_KEY_PLUS:
        sg_vk = (uint16)'+';
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_MINUS:
        sg_vk = (uint16)'-';
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_DOUBLE_QUOTES:
        sg_vk = (uint16)'"';
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_SLASH:
        sg_vk = (uint16)'/';
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_EXCLAMATION:
        sg_vk = (uint16)'!';
        event = SOGOU_VK_NONE;
        break;
#if 0   //不再与MMK系统shift相关，前面的physicaltoim已经做了转换      
    case MMIIM_SP_KEY_SHIFT_AND:
        if (0 == data_ptr->cand_win.count)
        {
            sg_vk = (uint16)'\'';
            event = SOGOU_VK_NONE;
        }
        else
        {
            sg_vk = WDK_SEPERATER;
        }
        break;
#endif        
    case MMIIM_SP_KEY_STAR:
        sg_vk = (uint16)'*';
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_HASH:
        sg_vk = (uint16)'#';
        event = SOGOU_VK_NONE;
        break;
    //MMK不支持的按键
    case MMIIM_SP_KEY_QUOTES:
        sg_vk = (uint16)'\'';
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_EQUAL:
        sg_vk = (uint16)'=';//注意sogou的WDK_FUNC值与'='值相同，因此在使用的WDK_FUNC需注意
        event = SOGOU_VK_NONE;
        break;
    case MMIIM_SP_KEY_RIGHT_SLASH:
        sg_vk = (uint16)'\\';
        event = SOGOU_VK_NONE;
        break;

    case MMIIM_SP_KEY_DEL:
        sg_vk = WDK_C;
        break;

    //下面这些符号存在全角和半角之分
    case MMIIM_SP_KEY_COMMA:
        sg_vk = (uint16)',';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            sg_vk = MMIIM_ConvertToFullSymbol (sg_vk);
        }
#endif
        event = SOGOU_VK_NONE;
        break;
        
    case MMIIM_SP_KEY_PERIOD:
        sg_vk = (uint16)'.';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            sg_vk = MMIIM_ConvertToFullSymbol (sg_vk);
        }
#endif
        event = SOGOU_VK_NONE;
        break;
        
    case MMIIM_SP_KEY_QUESTION:
        sg_vk = (uint16)'?';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            sg_vk = MMIIM_ConvertToFullSymbol (sg_vk);
        }
#endif
        event = SOGOU_VK_NONE;
        break;
        
    case MMIIM_SP_KEY_LEFT_PARENTHESIS:
        sg_vk = (uint16)'(';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            sg_vk = MMIIM_ConvertToFullSymbol (sg_vk);
        }
#endif
        event = SOGOU_VK_NONE;
        break;
        
    case MMIIM_SP_KEY_RIGHT_PARENTHESIS:
        sg_vk = (uint16)')';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            sg_vk = MMIIM_ConvertToFullSymbol (sg_vk);
        }
#endif
        event = SOGOU_VK_NONE;
        break;
        
    case MMIIM_SP_KEY_SEMICOLON:
        sg_vk = (uint16)';';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            sg_vk = MMIIM_ConvertToFullSymbol (sg_vk);
        }
#endif
        event = SOGOU_VK_NONE;
        break;
        
    case MMIIM_SP_KEY_COLON:
        sg_vk = (uint16)':';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            sg_vk = MMIIM_ConvertToFullSymbol (sg_vk);
        }
#endif
        event = SOGOU_VK_NONE;
        break;
        
#endif
    default:
        break;
    }

    if(SOGOU_VK_NONE != sg_vk)
    {
        sg_vk |= sg_vk_type;
    }
    *pEvent = event;

    return sg_vk; /*lint !e438*/
}

/******************************************************************************/
// Description: 转化输入法类型
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
PUBLIC SG_UINT MMIIM_Sogou_ConvertMethod (GUIIM_METHOD_T method)
{
    uint16 i;
    uint16 im_cnt = 0;
    SG_UINT sg_im = 0;

    if(GUIIM_M_NONE == method)
        return sg_im;

    im_cnt = ARR_SIZE(s_sogou_method_relation_list);
    for(i = 0; i < im_cnt; i++)
    {
        if(s_sogou_method_relation_list[i].guiim_method == method)
        {
            sg_im = s_sogou_method_relation_list[i].sogou_method;
            return sg_im;
        }
    }

    im_cnt = ARR_SIZE(s_sogou_tp_method_relation_list);
    for(i = 0; i < im_cnt; i++)
    {
        if (s_sogou_tp_method_relation_list[i].guiim_method == method)
        {
            sg_im = s_sogou_tp_method_relation_list[i].sogou_method;
            return sg_im;
        }
    }

    return sg_im;
}


/*****************************************************************************/
//  Description : ime init
//  Global resource dependence : none
//  Author:
//  Note:imeType 窗口的ID号
/*****************************************************************************/
PUBLIC uint8 WD_ImeInit(uint8 allowed_input, GUIIM_LANGUAGE_T language_type)
{
    uint8 default_ime;
    uint8 v_capLock;
    //uint32 i = 0;
    //GUIIM_LANGUAGE_T language_type = MMIAPIIM_GetLanguage();
    WD_UINT8 ImeList[IME_LIST_MAX] = { 0 };

    default_ime = g_imeResult.imeType;
    v_capLock = g_imeResult.capLock;

    // 英文一定要初始化
    ImeList[0] = WDI_ENGLISH;
    ImeList[1] = WDI_ENGLISH_ABC;
    ImeList[2] = WDI_DIGIT;
    switch (language_type)
    {
#ifdef IM_SIMP_CHINESE_SUPPORT
    case GUIIM_LANG_CHS:
        ImeList[3] = WDI_CN_FULLPINYIN;
        ImeList[4] = WDI_CN_CPINYIN;
        ImeList[5] = WDI_CN_BIHUA;
        ImeList[6] = 0;

        if (default_ime != WDI_ENGLISH  && default_ime != WDI_ENGLISH_ABC && default_ime != WDI_DIGIT && 
            default_ime != WDI_CN_BIHUA && default_ime != WDI_CN_CPINYIN && default_ime != WDI_CN_FULLPINYIN)
        {
            default_ime = WDI_CN_CPINYIN;
        }
        break;
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
    case GUIIM_LANG_CHT:
        ImeList[3] = WDI_CN_FULLZHUYIN;
        ImeList[4] = WDI_CN_ZHUYIN;
        ImeList[5] = 0;

        if (default_ime != WDI_ENGLISH  && default_ime != WDI_ENGLISH_ABC && default_ime != WDI_DIGIT && 
            default_ime != WDI_CN_ZHUYIN && default_ime != WDI_CN_FULLZHUYIN)
        {
            default_ime = WDI_CN_ZHUYIN;
        }
        break;
#endif

#ifdef IM_ARABIC_SUPPORT
    case GUIIM_LANG_ARABIC:
        ImeList[3] = WDI_ARABIC;
        ImeList[4] = WDI_ARABIC_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_ARABIC && default_ime != WDI_ARABIC_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_ARABIC;
        }
        break;
#endif

#ifdef IM_FRENCH_SUPPORT
    case GUIIM_LANG_FRENCH:
        ImeList[3] = WDI_FRENCH;
        ImeList[4] = WDI_FRENCH_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_FRENCH && default_ime != WDI_FRENCH_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_FRENCH;
        }
        break;
#endif

#ifdef IM_ITALIAN_SUPPORT
    case GUIIM_LANG_ITALIAN:
        ImeList[3] = WDI_ITALIAN;
        ImeList[4] = WDI_ITALIAN_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_ITALIAN && default_ime != WDI_ITALIAN_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_ITALIAN;
        }
        break;
#endif

#ifdef IM_RUSSIAN_SUPPORT
    case GUIIM_LANG_RUSSIAN:
        ImeList[3] = WDI_RUSSIAN;
        ImeList[4] = WDI_RUSSIAN_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_RUSSIAN && default_ime != WDI_RUSSIAN_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_RUSSIAN;
        }
        break;
#endif

#ifdef IM_BENGALI_SUPPORT
    case GUIIM_LANG_BENGALI:
        ImeList[3] = WDI_BENGALI;
        ImeList[4] = WDI_BENGALI_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_BENGALI && default_ime != WDI_BENGALI_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_BENGALI;
        }
        break;
#endif

#ifdef IM_HINDI_SUPPORT
    case GUIIM_LANG_HINDI:
        ImeList[3] = WDI_HINDI;
        ImeList[4] = WDI_HINDI_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_HINDI && default_ime != WDI_HINDI_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_HINDI;
        }
        break;
#endif

#ifdef IM_MARATHI_SUPPORT
    case GUIIM_LANG_MARATHI:
        ImeList[3] = WDI_MARATHI;
        ImeList[4] = WDI_MARATHI_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_MARATHI && default_ime != WDI_MARATHI_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_MARATHI;
        }
        break;
#endif

#ifdef IM_PERSIAN_SUPPORT
    case GUIIM_LANG_PERSIAN:
        ImeList[3] = WDI_PERSIAN;
        ImeList[4] = WDI_PERSIAN_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_PERSIAN && default_ime != WDI_PERSIAN_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_PERSIAN;
        }
        break;
#endif

#ifdef IM_TAGALOG_SUPPORT
    case GUIIM_LANG_TAGALOG:
        ImeList[3] = WDI_TAGALOG;
        ImeList[4] = WDI_TAGALOG_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_TAGALOG && default_ime != WDI_TAGALOG_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_TAGALOG;
        }
        break;
#endif

#ifdef IM_TAMIL_SUPPORT
    case GUIIM_LANG_TAMIL:
        ImeList[3] = WDI_TAMIL;
        ImeList[4] = WDI_TAMIL_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_TAMIL && default_ime != WDI_TAMIL_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_TAMIL;
        }
        break;
#endif

#ifdef IM_TELUGU_SUPPORT
    case GUIIM_LANG_TELUGU:
        ImeList[3] = WDI_TELUGU;
        ImeList[4] = WDI_TELUGU_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_TELUGU && default_ime != WDI_TELUGU_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_TELUGU;
        }
        break;
#endif

#ifdef IM_THAI_SUPPORT
    case GUIIM_LANG_THAI:
        ImeList[3] = WDI_THAI;
        ImeList[4] = WDI_THAI_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_THAI && default_ime != WDI_THAI_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_THAI;
        }
        break;
#endif

#ifdef IM_URDU_SUPPORT
    case GUIIM_LANG_URDU:
        ImeList[3] = WDI_URDU;
        ImeList[4] = WDI_URDU_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_URDU && default_ime != WDI_URDU_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_URDU;
        }
        break;
#endif

#ifdef IM_GERMAN_SUPPORT
    case GUIIM_LANG_GERMAN:
        ImeList[3] = WDI_GERMAN;
        ImeList[4] = WDI_GERMAN_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_GERMAN && default_ime != WDI_GERMAN_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_GERMAN;
        }
        break;
#endif

#ifdef IM_GREEK_SUPPORT
    case GUIIM_LANG_GREEK:
        ImeList[3] = WDI_GREEK;
        ImeList[4] = WDI_GREEK_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_GREEK && default_ime != WDI_GREEK_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_GREEK;
        }
        break;
#endif

#ifdef IM_INDONESIAN_SUPPORT
    case GUIIM_LANG_INDONESIAN:
        ImeList[3] = WDI_INDONESIAN;
        ImeList[4] = WDI_INDONESIAN_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_INDONESIAN && default_ime != WDI_INDONESIAN_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_INDONESIAN;
        }
        break;
#endif


#ifdef IM_PORTUGUESE_SUPPORT
    case GUIIM_LANG_PORTUGUESE:
        ImeList[3] = WDI_PORTUGUESE;
        ImeList[4] = WDI_PORTUGUESE_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_PORTUGUESE && default_ime != WDI_PORTUGUESE_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_PORTUGUESE;
        }
        break;
#endif


#ifdef IM_SPANISH_SUPPORT
    case GUIIM_LANG_SPANISH:
        ImeList[3] = WDI_SPANISH;
        ImeList[4] = WDI_SPANISH_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_SPANISH && default_ime != WDI_SPANISH_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_SPANISH;
        }
        break;
#endif

#ifdef IM_VIETNAMESE_SUPPORT
    case GUIIM_LANG_VIETNAMESE:
        ImeList[3] = WDI_VIETNAMESE;
        ImeList[4] = WDI_VIETNAMESE_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_VIETNAMESE && default_ime != WDI_VIETNAMESE_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_VIETNAMESE;
        }
        break;
#endif

#ifdef IM_TURKISH_SUPPORT
    case GUIIM_LANG_TURKISH:
        ImeList[3] = WDI_TURKISH;
        ImeList[4] = WDI_TURKISH_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_TURKISH && default_ime != WDI_TURKISH_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_TURKISH;
        }
        break;
#endif

#ifdef IM_SLOVAK_SUPPORT
    case GUIIM_LANG_SLOVAK:
        ImeList[3] = WDI_SLOVAK;
        ImeList[4] = WDI_SLOVAK_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_SLOVAK && default_ime != WDI_SLOVAK_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_SLOVAK;
        }
        break;
#endif

#ifdef IM_UYGHUR_SUPPORT
    case GUIIM_LANG_UYGHUR:
        ImeList[3] = WDI_UIGHUR;
        ImeList[4] = WDI_UIGHUR_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_UIGHUR && default_ime != WDI_UIGHUR_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_UIGHUR;
        }
        break;
#endif

#ifdef IM_SINHALESE_SUPPORT
    case GUIIM_LANG_SINHALESE:
        ImeList[3] = WDI_SINHALESE;
        ImeList[4] = WDI_SINHALESE_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_SINHALESE && default_ime != WDI_SINHALESE_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_SINHALESE;
        }
        break;
#endif

    case GUIIM_LANG_ENGLISH:
    default:
        ImeList[3] = 0;
        if (default_ime != WDI_ENGLISH && default_ime != WDI_ENGLISH_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_ENGLISH;
        }
        break;
    }
#if defined IM_SIMP_CHINESE_SUPPORT
    if(1 == SG_Check_D_Space_Is_Enough())
    {
        SG_setBaseFolderPath(L"D:\\@sogou\\");
		g_sg_save_position = 1;//皮肤放到E盘//2012_11_6
    }
    else if (1 == SG_Check_E_Space_Is_Enough())
    {
        SG_setBaseFolderPath(L"E:\\@sogou\\");
		g_sg_save_position = 1;
    }
	else
	{
		g_sg_save_position = 2;
	}
#endif
    IME_Initialize(ImeList, default_ime, g_FlashData, &g_imeResult);
    SG_Check_Build_Time();//6_19_1(2012_12_21)
    //引擎初始化
    WD_SwitchIme(default_ime);

    return 1;
}


/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void DrawPreEditWin(MMIIM_SOGOU_DATA_T *data_ptr, BOOLEAN is_draw_immediately)
{
    uint16 i = 0;
    uint16 *poutputKeys = SOGOU_GetImeResult(data_ptr)->outputKeys;

    MMIIM_ResetPreEditWin(&(data_ptr->edit_win));

    /*
    flagState 当前输入法状态：
    1 编辑状态，候选词标签不用显示
    2 选择状态，显示候选词标签
    3 英文自造词状态，显示自造词窗口
    */
    if (SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_CHINESE_NORMAL ||
        SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_NORMAL ||
        SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_ABC_NORMAL)
    {
        data_ptr->edit_win.state = MMIIM_WIN_STATE_NONE;
    }
    else if (SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_CHINESE_SELECT ||
             SOGOU_GetImeResult(data_ptr)->imeStatus == WD_STATUS_FOREIGN_SELECT)
    {
        data_ptr->edit_win.state = MMIIM_WIN_STATE_CAND;
    }
    else
    {
        data_ptr->edit_win.state = MMIIM_WIN_STATE_EDIT;
    }

    /*
    pPreBuffer  需要在Pre-Edit窗口显示的字符串，使用'\0'分割多个项
    nPreEdit    Pre-Edit共有多少项
    iHotPreEdit 需要显示光标的项id，从0开始计算
    */
    for ( ; poutputKeys[i] > 0x4DFF && (i < MMIIM_WIN_BUFFER_LEN); i++)
    {
    }

    if (poutputKeys[0] > 0x4DFF && SOGOU_GetImeResult(data_ptr)->imeType == WDI_CN_CPINYIN)
    {
        SCI_MEMCPY(data_ptr->edit_win.buffer, SOGOU_GetImeResult(data_ptr)->outputKeys, i * sizeof(uint16));
        if (i < MMIIM_WIN_BUFFER_LEN)
        {
            SCI_MEMCPY(data_ptr->edit_win.buffer + i, 
                       SOGOU_GetImeResult(data_ptr)->outputKeys + i, 
                       sizeof(data_ptr->edit_win.buffer) - i * sizeof(uint16));
        }
    }
    else
    {
        SCI_MEMCPY(data_ptr->edit_win.buffer, SOGOU_GetImeResult(data_ptr)->outputKeys, sizeof(data_ptr->edit_win.buffer));
    }

    data_ptr->edit_win.count = MMIAPICOM_Wstrlen (data_ptr->edit_win.buffer);
//    data_ptr->edit_win.has_cursor = FALSE;

    if (is_draw_immediately && data_ptr->preeditwin_height > 0)
    {
        MMIIM_DrawPreEditWin(data_ptr->lcd_info_ptr, &(data_ptr->edit_win), data_ptr->win_id);
    }
}

/******************************************************************************/
// Description: 选字窗口
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void DrawLetterWin(MMIIM_SOGOU_DATA_T *data_ptr, BOOLEAN is_draw_immediately)
{
    int i = 0, j = 0, l = 0;

    MMIIM_ResetCandWin(&data_ptr->letter_win);

    /*
    pCandBuffer 候选词窗口的字符串，使用'\0'分割多个候选词
    nCand 当前候选词的个数
    iHotCand 被选中的候选词id，从0开始计算，最少总有一个被选中
    */
    if (MMIAPICOM_Wstrlen((const wchar*)SOGOU_GetImeResult(data_ptr)->letterSelecting) == 0)
    {
        data_ptr->letter_win.state = MMIIM_WIN_STATE_NONE;
    }
    else
    {
        do
        {
            l = 0;
            do
            {
                data_ptr->letter_win.buffer[i] = SOGOU_GetImeResult(data_ptr)->letterSelecting[j][l];
                l++;
                i++;
            } while (SOGOU_GetImeResult(data_ptr)->letterSelecting[j][l] && i < (MMIIM_WIN_BUFFER_LEN - 1));
            data_ptr->letter_win.buffer[i] = '\0';
            i++;
            j++;
        } while (SOGOU_GetImeResult(data_ptr)->letterSelecting[j][0] && /*i <MAX_WORD_LEN*2 && */i < (MMIIM_WIN_BUFFER_LEN - 1));
        data_ptr->letter_win.count = j;
        if (SOGOU_GetImeResult(data_ptr)->cnMix)
        {
            data_ptr->letter_win.has_hot = TRUE;
        }
        else
        {
            data_ptr->letter_win.has_hot = FALSE;
        }
        data_ptr->letter_win.hot_index = SOGOU_GetImeResult(data_ptr)->idxLetterSelecting;
        if (data_ptr->letter_win.hot_index > 4)
        {
            data_ptr->letter_win.disp_first_index += data_ptr->letter_win.hot_index-3;//6;//     .first_ = 8;
        }
        else
        {
            data_ptr->letter_win.disp_first_index = 0;//6;//     .first_ = 8;
        }

        if(SOGOU_GetImeResult(data_ptr)->curPage != 0 || SOGOU_GetImeResult(data_ptr)->idxLetterSelecting != 0)
        {
            if (MMIIM_DRAW_RIGHT2LEFT == data_ptr->letter_win.draw_direction)
            {
                data_ptr->letter_win.idc_flag |= MMIIM_IDC_FLAG_RIGHT;
            }
            else
            {
                data_ptr->letter_win.idc_flag |= MMIIM_IDC_FLAG_LEFT;
            }
        }
    }

    if (is_draw_immediately && data_ptr->letterwin_height > 0)
    {
        MMIIM_DrawLetterWin(data_ptr->lcd_info_ptr,
                            &(data_ptr->letter_win),
                            &data_ptr->lastletterdisp,
                            &data_ptr->imeResult,
                            data_ptr->win_id);
    }
}

/******************************************************************************/
// Description: 选字窗口
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
LOCAL void DrawCandWin(MMIIM_SOGOU_DATA_T *data_ptr, BOOLEAN is_draw_immediately)
{
    int i = 0, j = 0, l = 0;

    MMIIM_ResetCandWin(&data_ptr->cand_win);

    /*
    pCandBuffer 候选词窗口的字符串，使用'\0'分割多个候选词
    nCand 当前候选词的个数
    iHotCand 被选中的候选词id，从0开始计算，最少总有一个被选中
    */
    if (MMIAPICOM_Wstrlen((const wchar*)(SOGOU_GetImeResult(data_ptr)->selecting)) == 0)
    {
        data_ptr->cand_win.state = MMIIM_WIN_STATE_NONE;
    }
    else
    {
        do
        {
            l = 0;
            do
            {
                data_ptr->cand_win.buffer[i] = SOGOU_GetImeResult(data_ptr)->selecting[j][l];
                l++;
                i++;
            } while (SOGOU_GetImeResult(data_ptr)->selecting[j][l] && i < (MMIIM_WIN_BUFFER_LEN - 1));
            data_ptr->cand_win.buffer[i] = '\0';
            i++;
            j++;
        } while (SOGOU_GetImeResult(data_ptr)->selecting[j][0] && i <MAX_WORD_LEN*2 && i < (MMIIM_WIN_BUFFER_LEN - 1));
        data_ptr->cand_win.count = j;
        data_ptr->cand_win.has_hot = TRUE;
        data_ptr->cand_win.hot_index = SOGOU_GetImeResult(data_ptr)->idxSelecting;

        if (WD_STATUS_CHINESE_INPUT == SOGOU_GetImeResult(data_ptr)->imeStatus
            || WD_STATUS_FOREIGN_INPUT == SOGOU_GetImeResult(data_ptr)->imeStatus)
        {
            data_ptr->cand_win.state = MMIIM_WIN_STATE_EDIT;
        }
        else if(WD_STATUS_CHINESE_NORMAL != SOGOU_GetImeResult(data_ptr)->imeStatus
            && WD_STATUS_FOREIGN_NORMAL != SOGOU_GetImeResult(data_ptr)->imeStatus
            && WD_STATUS_CHINESE_LEGEND != SOGOU_GetImeResult(data_ptr)->imeStatus)
        {
            data_ptr->cand_win.state = MMIIM_WIN_STATE_CAND;
            data_ptr->cand_win.has_lable = TRUE;
        }
        else
        {
            data_ptr->cand_win.state = MMIIM_WIN_STATE_NONE;
        }

        if(SOGOU_GetImeResult(data_ptr)->curPage != 0 || SOGOU_GetImeResult(data_ptr)->idxSelecting != 0)
        {
            if (MMIIM_DRAW_RIGHT2LEFT == data_ptr->cand_win.draw_direction)
            {
                data_ptr->cand_win.idc_flag |= MMIIM_IDC_FLAG_RIGHT;
            }
            else
            {
                data_ptr->cand_win.idc_flag |= MMIIM_IDC_FLAG_LEFT;
            }
        }
    }

    if (is_draw_immediately && data_ptr->candwin_height > 0)
    {
        MMIIM_DrawCandWin(data_ptr->lcd_info_ptr,
                          &(data_ptr->cand_win),
                          &data_ptr->lastdisp,
                          data_ptr->win_id,
                          &data_ptr->imeResult);
    }
}

/*****************************************************************************/
//  Description : 根据输入法确定是否需要画候选区或者编辑区
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPlatfromNativeIME(GUIIM_METHOD_T method)
{
    if (GUIIM_M_ENGLISH_ABC_LEAD_UPPER == method ||
        GUIIM_M_ENGLISH_ABC_LOWER == method ||
        GUIIM_M_ENGLISH_ABC_UPPER == method ||
        GUIIM_M_123 == method ||
        GUIIM_M_Q_ENGLISH_ABC_LEAD_UPPER == method ||
        GUIIM_M_Q_ENGLISH_ABC_LOWER == method ||
        GUIIM_M_Q_ENGLISH_ABC_UPPER == method ||
        GUIIM_M_Q_123 == method)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 打开切换输入法窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenSwitchIMWin(MMIIM_SOGOU_DATA_T *data_ptr, GUI_BOTH_RECT_T *pop_menu_rect_ptr)
{
    if (PNULL == data_ptr || PNULL == pop_menu_rect_ptr)
    {
        return;
    }

    //输入法控件统一处理切换输入法
    MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, pop_menu_rect_ptr);
    MMIIM_SwitchingCreateWin(&(data_ptr->init_param), *pop_menu_rect_ptr, MMIIM_EP_TOP_LEFT);
}

/*****************************************************************************/
//  Description : 打开符号窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenSysbolWin(MMIIM_SOGOU_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr->im_set.method);  /*assert verified*/
    if (PNULL == data_ptr||PNULL == data_ptr->init_param.method_param_ptr->im_set.method)
    {
        return;
    }

    //输入法控件统一处理切换输入法
    MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle,
                           MMIIM_GetSymbType(data_ptr->init_param.method_param_ptr->im_set.method));
}

/******************************************************************************/
// Description: 获取当前输入法库控制指针
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
LOCAL WD_imeResult* SOGOU_GetImeResult(MMIIM_SOGOU_DATA_T *data_ptr)
{
    return &(data_ptr->imeResult);
}

/*******************************************************************************************/
//  Description : get last disp
//  Global resource dependence :
//  Author:
//  Note:
/*********************************************************************************************/
LOCAL uint8 SOGOU_GetLastDisp(MMIIM_SOGOU_DATA_T *data_ptr)
{
    return data_ptr->lastdisp;
}

/*****************************************************************************/
//  Description : ime init
//  Global resource dependence : none
//  Author:
//  Note:imeType 窗口的ID号
/*****************************************************************************/
LOCAL uint8 SOGOU_ImeInit(MMIIM_SOGOU_DATA_T *data_ptr)
{
    uint8 default_ime;
    uint8   v_capLock;
    static GUIIM_LANGUAGE_T s_prelanguage_type = GUIIM_INPUT_TYPE_ALL;
    WD_UINT8 ImeList[IME_LIST_MAX] = { 0 };
    GUIIM_LANGUAGE_T language_type = MMIAPIIM_GetLanguage();
    uint8 ret_value = 0;
    
    default_ime = SOGOU_GetImeResult(data_ptr)->imeType;
    v_capLock = SOGOU_GetImeResult(data_ptr)->capLock;

    // 英文一定要初始化
    ImeList[0] = WDI_ENGLISH;
    ImeList[1] = WDI_ENGLISH_ABC;
    ImeList[2] = WDI_DIGIT;
    // 中文输入法
    switch (language_type)
    {
    case GUIIM_LANG_CHS:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        ImeList[3] = WDI_CN_FULLPINYIN;
#else
        ImeList[3] = WDI_CN_CPINYIN;
#endif
        ImeList[4] = WDI_CN_BIHUA;
        ImeList[5] = 0;

        if (default_ime != WDI_ENGLISH && default_ime != WDI_ENGLISH_ABC &&
            default_ime != WDI_DIGIT   && default_ime != WDI_CN_BIHUA    &&
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
            default_ime != WDI_CN_FULLPINYIN
#else
            default_ime != WDI_CN_CPINYIN
#endif
            )
        {
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
            default_ime = WDI_CN_FULLPINYIN;
#else
            default_ime = WDI_CN_CPINYIN;
#endif
        }
        break;

    //繁体中文
    case GUIIM_LANG_CHT:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        ImeList[3] = WDI_CN_FULLZHUYIN;
#else
        ImeList[3] = WDI_CN_ZHUYIN;
#endif
        ImeList[4] = WDI_CN_BIHUAF;
        ImeList[5] = 0;

        if (default_ime != WDI_ENGLISH && default_ime != WDI_ENGLISH_ABC &&
            default_ime != WDI_DIGIT   && default_ime != WDI_CN_BIHUAF    &&
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
            default_ime != WDI_CN_FULLZHUYIN
#else
            default_ime != WDI_CN_ZHUYIN
#endif
            )
        {
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
            default_ime = WDI_CN_FULLZHUYIN;
#else
            default_ime = WDI_CN_ZHUYIN;
#endif
        }
        break;

#ifdef IM_ARABIC_SUPPORT
    case GUIIM_LANG_ARABIC:
        ImeList[3] = WDI_ARABIC;
        ImeList[4] = WDI_ARABIC_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_ARABIC && default_ime != WDI_ARABIC_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_ARABIC;
        }
        break;
#endif

#ifdef IM_FRENCH_SUPPORT
    case GUIIM_LANG_FRENCH:
        ImeList[3] = WDI_FRENCH;
        ImeList[4] = WDI_FRENCH_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_FRENCH && default_ime != WDI_FRENCH_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_FRENCH;
        }
        break;
#endif

#ifdef IM_ITALIAN_SUPPORT
    case GUIIM_LANG_ITALIAN:
        ImeList[3] = WDI_ITALIAN;
        ImeList[4] = WDI_ITALIAN_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_ITALIAN && default_ime != WDI_ITALIAN_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_ITALIAN;
        }
        break;
#endif

#ifdef IM_TURKISH_SUPPORT
    case GUIIM_LANG_TURKISH:
        ImeList[3] = WDI_TURKISH;
        ImeList[4] = WDI_TURKISH_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_TURKISH && default_ime != WDI_TURKISH_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_TURKISH;
        }
        break;
#endif

#ifdef IM_SLOVAK_SUPPORT
    case GUIIM_LANG_SLOVAK:
        ImeList[3] = WDI_SLOVAK;
        ImeList[4] = WDI_SLOVAK_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_SLOVAK && default_ime != WDI_SLOVAK_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_SLOVAK;
        }
        break;
#endif

#ifdef IM_UYGHUR_SUPPORT
    case GUIIM_LANG_UYGHUR:
        ImeList[3] = WDI_UIGHUR;
        ImeList[4] = WDI_UIGHUR_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_UIGHUR && default_ime != WDI_UIGHUR_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_UIGHUR;
        }
        break;
#endif

#ifdef IM_SINHALESE_SUPPORT
    case GUIIM_LANG_SINHALESE:
        ImeList[3] = WDI_SINHALESE;
        ImeList[4] = WDI_SINHALESE_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_SINHALESE && default_ime != WDI_SINHALESE_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_SINHALESE;
        }
        break;
#endif

    case GUIIM_LANG_ENGLISH:
    default:
        ImeList[3] = 0;
        if (default_ime != WDI_ENGLISH && default_ime != WDI_ENGLISH_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_ENGLISH;
        }
        break;
    }    
#if defined IM_SIMP_CHINESE_SUPPORT
    if(1 == SG_Check_D_Space_Is_Enough())
    {
        SG_setBaseFolderPath(L"D:\\@sogou\\");
		g_sg_save_position = 1;//皮肤放到E盘//2012_11_6
    }
    else if (1 == SG_Check_E_Space_Is_Enough())
    {
        SG_setBaseFolderPath(L"E:\\@sogou\\");
		g_sg_save_position = 1;
    }
	else
	{
		g_sg_save_position = 2;
	}
	
	SCI_TraceLow("Sogou_ImeInit 1 g_sg_save_position=%d",g_sg_save_position);//6_4
#endif
    {
        ret_value = IME_Initialize(ImeList, default_ime, g_FlashData, SOGOU_GetImeResult(data_ptr));
        SG_Check_Build_Time();//6_19_1(2012_12_21)
        if (ret_value)
        {
            //SCI_TraceLow:"Sogou_ImeInit"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1233_112_2_18_2_28_4_161,(uint8*)"");
            return ret_value;
        }
    }

#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_SGGetImeSkin();
#endif
    if (v_capLock && s_prelanguage_type == language_type)
    {
        if(SOGOU_GetImeResult(data_ptr)->imeType > WDI_FOREIGN_BEGIN && SOGOU_GetImeResult(data_ptr)->imeType < WDI_MULTIABC_BEGIN)
        {
            if(v_capLock==1)
            {
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
                WD_OnKey(WDK_NORMAL_A,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
                WD_OnKey(WDK_SHIFT,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
#else
                WD_OnKey(WDK_NORMAL_2,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
                WD_OnKey(WDK_SHARP,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
#endif
                WD_OnKey(WDK_C,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
            }
            else if(v_capLock==2)
            {
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
                WD_OnKey(WDK_NORMAL_A,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
                WD_OnKey(WDK_SHIFT,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
                WD_OnKey(WDK_SHIFT,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
#else
                WD_OnKey(WDK_NORMAL_2,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
                WD_OnKey(WDK_SHARP,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
                WD_OnKey(WDK_SHARP,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
#endif
                WD_OnKey(WDK_C,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
            }
        }
        else if(SOGOU_GetImeResult(data_ptr)->imeType > WDI_MULTIABC_BEGIN)
        {
            WD_OnKey(WDK_NORMAL_2,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
            WD_OnKey(WDK_SHARP,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
            WD_OnKey(WDK_C,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
        }
    }
    s_prelanguage_type = language_type;

    return 1;
}

/*****************************************************************************/
//  Description : 获取当前输入法ID号
//  Global resource dependence : none
//  Author:
//  Note:imeType 窗口的ID号
/*****************************************************************************/
LOCAL uint8 SOGOU_GetInputMethod(MMIIM_SOGOU_DATA_T *data_ptr)
{
    return data_ptr->imeResult.imeType;
}

/*****************************************************************************/
//  Description : 处理输入法按键消息
//  Global resource dependence : none
//  Author:
//  Note:event 事件的ID， param  触笔的坐标
/*****************************************************************************/
LOCAL uint8 SOGOU_HandleIMEOnKeyEvent(MMIIM_SOGOU_DATA_T *data_ptr,uint16  sg_vk)
{
    if (sg_vk == WDK_SHARP && SOGOU_GetImeResult(data_ptr)->imeType == WDI_CN_FULLPINYIN)
        {
            if (SOGOU_GetImeResult(data_ptr)->selecting[0][0])
            {
                WD_OnKey(WDK_LONG_C,0,MMI_DEFAULT_LASTDISPLAY_NUM,SOGOU_GetImeResult(data_ptr));
            }
            WD_OnKey(WDK_SHARP,0,SOGOU_GetLastDisp(data_ptr),SOGOU_GetImeResult(data_ptr));
        }

    if (sg_vk)
    {
        WD_OnKey(sg_vk, 0, SOGOU_GetLastDisp(data_ptr), SOGOU_GetImeResult(data_ptr));
    }

    return 0;
}

#ifdef SOGOU_USE_PDA_SKIN
/******************************************************************************/
// Description: 设置皮肤为默认皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
LOCAL void SG_SetupDefaultSkin(void)
{
    MMI_STRING_T str_default= {0};
    MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_DEFAULT, &str_default);

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)    
    MMIAPIIM_SGSetupSelectedSkinFile((uint16*)str_default.wstr_ptr,str_default.wstr_len);
#endif
    s_is_use_default_skin = TRUE;
}

/******************************************************************************/
// Description: 获取当前设置的皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGGetImeSkin(void)
{
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    WD_UINT16 pic_filename[MMIFILE_FULL_PATH_MAX_LEN + 1] = { 0 };
    WD_UINT32 full_path_len;
    WD_UINT32 file_size;
    WD_UINT32 data_size = 0;
    WD_UINT8 *pic_data_ptr=NULL;
    JINF_GET_INFO_IN_T jpg_input;
    JINF_INFO_T jpg_info;
    WD_UINT16 dest_width = 0;
    WD_UINT16 dest_height = 0;
    MMIFILE_HANDLE filehandle;
    uint32 readed = 0;
    GUIIMG_SRC_T        img_src = {0};
    GUIIMG_DISPLAY_T    img_display = {0};
    GUIIMG_DEC_OUT_T    img_output = {0};
    uint32              buf_size = 0;

    MMIAPIIM_SGFreeImeSkinBuffer();
	if(0 == g_sg_save_position)
	{
	    MMIAPICOM_Wstrncpy(pic_filename, (const wchar*)SG_D_SKIN_CONFIG_PATH, MMIFILE_FULL_PATH_MAX_LEN);
	    filehandle = MMIAPIFMM_CreateFile((const wchar*)SG_D_SKIN_CONFIG_FILE, SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);
	}
	else if(1 == g_sg_save_position)
	{
	    MMIAPICOM_Wstrncpy(pic_filename, (const wchar*)SG_E_SKIN_CONFIG_PATH, MMIFILE_FULL_PATH_MAX_LEN);
	    filehandle = MMIAPIFMM_CreateFile((const wchar*)SG_E_SKIN_CONFIG_FILE, SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);
	}
	else
	{
		return FALSE;
	}
	
    if (filehandle)
    {
        MMIAPIFMM_ReadFile(filehandle, (void*)(pic_filename + SKIN_FILEPATH_LEN), 
                         2 * (MMIFILE_FULL_PATH_MAX_LEN - SKIN_FILEPATH_LEN), &readed, PNULL);
        MMIAPIFMM_CloseFile(filehandle);
    }

    if (filehandle == 0 || FALSE == MMIAPIFMM_IsFileExist(pic_filename,MMIAPICOM_Wstrlen(pic_filename)))
    {
        s_is_use_default_skin = TRUE;
		if(0 == g_sg_save_position)
		{
        	MMIAPICOM_Wstrncpy(pic_filename, (const wchar*)SG_D_SKIN_DEFAULT_FILE, MMIFILE_FULL_PATH_MAX_LEN);
		}
		else if(1 == g_sg_save_position)
		{
        	MMIAPICOM_Wstrncpy(pic_filename, (const wchar*)SG_E_SKIN_DEFAULT_FILE, MMIFILE_FULL_PATH_MAX_LEN);
		}
		else
		{
			return FALSE;
		}
    }
	
    if (TRUE==MMIAPIFMM_IsFileExist(pic_filename, MMIAPICOM_Wstrlen(pic_filename)))
    {
        if(MMIAPIFMM_GetFileInfo(pic_filename, MMIAPICOM_Wstrlen(pic_filename), &file_size, NULL, NULL))
        {
            full_path_len = MMIAPICOM_Wstrlen(pic_filename);
            if((file_size > 0) && (file_size < MIN_SKIN_FILE_BUF_SIZE))
            {
                //读图片文件头70，足以得到其长和宽
                data_size = file_size;

                pic_data_ptr = (uint8*)MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);/*lint !e64*/

                if (pic_data_ptr)
                {
                    if(MMIAPIFMM_ReadFilesDataSyn(pic_filename,full_path_len,pic_data_ptr,data_size))
                    {
                        jpg_input.jpeg_buf_ptr = pic_data_ptr;
                        jpg_input.jpeg_buf_size = data_size;
                        jpg_input.read_file_func = 0;
                        if (JINF_SUCCESS == IMGJPEG_GetInfo(&jpg_input,&jpg_info))
                        {
                            dest_width = jpg_info.image_width;
                            dest_height = jpg_info.image_height;
                        }
                        else
                        {
                             MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                             SG_SetupDefaultSkin();
                             return FALSE;
                        }
                    }
                }
                if (dest_width == SG_SKIN_SCREEN_P_WIDTH && dest_height == SG_SKIN_HEIGHT)
                {
                    img_src.src_buf_ptr = pic_data_ptr;
                    img_src.src_data_size = data_size;
                    //set image dest width and height
                    img_display.dest_width = dest_width;
                    img_display.dest_height = dest_height;
                    //set image decode output data buffer
                    img_output.decode_data_ptr = (uint8*)MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                    if (PNULL == img_output.decode_data_ptr)
                    {
                        if (PNULL != pic_data_ptr)
                        {
                            MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                        }
                        SG_SetupDefaultSkin();
                        return FALSE;
                    }
                    img_output.decode_data_size = BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                    //decode jpg
                    if (GUIIMG_Decode(&img_src, &img_display, &img_output))
                    {
                        //alloc bitmap
                        s_sg_skin_bitmap_ptr = SCI_ALLOCA(sizeof(GUIIMG_BITMAP_T));
                        if (PNULL == s_sg_skin_bitmap_ptr)
                        {
                            if (PNULL != pic_data_ptr)
                            {
                                MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                            }
                            if (PNULL != img_output.decode_data_ptr)
                            {
                                MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                                img_output.decode_data_ptr = PNULL;
                            }
                            SG_SetupDefaultSkin();
                            return FALSE;
                        }
                        SCI_MEMSET(s_sg_skin_bitmap_ptr, 0, sizeof(GUIIMG_BITMAP_T));
                        //alloc bitmap vpbits
                        //s_sg_skin_bitmap_ptr->bit_ptr = (uint8*)MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_SOURCE);//2011-7-27
                        s_sg_skin_bitmap_ptr->bit_ptr = (uint8*)SCI_ALLOCA(SG_BLOCK_MEM_POOL_PIC_SOURCE_SIZE);//2011-7-27
                        if (PNULL == s_sg_skin_bitmap_ptr->bit_ptr)
                        {
                            if (PNULL != pic_data_ptr)
                            {
                                MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                            }
                            if (PNULL != img_output.decode_data_ptr)
                            {
                                MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                                img_output.decode_data_ptr = PNULL;
                            }
                            if (PNULL != s_sg_skin_bitmap_ptr)
                            {
                                SCI_FREE(s_sg_skin_bitmap_ptr);
                            }
                            SG_SetupDefaultSkin();
                            return FALSE;
                        }
                        //buf_size = BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_SOURCE);//2011-7-27
                        buf_size = SG_BLOCK_MEM_POOL_PIC_SOURCE_SIZE;//2011-7-27
                        SCI_MEMSET(s_sg_skin_bitmap_ptr->bit_ptr, 0, buf_size);

                        //copy vpbits
                        MMI_MEMCPY(s_sg_skin_bitmap_ptr->bit_ptr, buf_size,
                                   img_output.decode_data_ptr, img_output.decode_data_size,
                                   img_output.decode_data_size);
                        s_sg_skin_bitmap_ptr->img_width = img_output.actual_width;
                        s_sg_skin_bitmap_ptr->img_height = img_output.actual_height;
                        s_is_use_default_skin = FALSE;
                    }
                    //free file data buffer
                    if (PNULL != pic_data_ptr)
                    {
                        MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                    }
                    //free vpbits
                    if (PNULL != img_output.decode_data_ptr)
                    {
                        MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                        img_output.decode_data_ptr = PNULL;
                    }
                    return TRUE;
                }
                else
                {
                    if (PNULL != pic_data_ptr)
                    {
                        MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                    }
                    if (PNULL != img_output.decode_data_ptr)
                    {
                        MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                        img_output.decode_data_ptr = PNULL;
                    }
                    SG_SetupDefaultSkin();
                    return FALSE;
                }
            }
            else
            {
                SG_SetupDefaultSkin();
                return FALSE;
            }
        }
        else
        {
            SG_SetupDefaultSkin();
            return FALSE;
        }
    }
    else
    {
        SG_SetupDefaultSkin();
        return FALSE;
    }
#else
    s_is_use_default_skin = TRUE;
    return TRUE;
#endif
}

/******************************************************************************/
// Description: 释放当前皮肤占用的内存空间
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGFreeImeSkinBuffer(void)
{
    if (s_sg_skin_bitmap_ptr && s_sg_skin_bitmap_ptr->bit_ptr)
    {
        //MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_SOURCE); //释放block mem//2011-7-27
        SCI_FREE(s_sg_skin_bitmap_ptr->bit_ptr);//2011-7-27
    }
    if (s_sg_skin_bitmap_ptr)
    {
        SCI_FREE(s_sg_skin_bitmap_ptr);
    }
    return TRUE;
}

/******************************************************************************/
// Description: 显示搜狗皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
LOCAL BOOLEAN SG_DisplaySkin(MMIIM_SOGOU_DATA_T *data_ptr)
{
    GUI_POINT_T dis_point;
    GUI_RECT_T rect = MMITHEME_GetClientRectEx(data_ptr->win_id);
    GUI_RECT_T img_rect;
    uint8 draw_line = 0;

    if (data_ptr->candwin_height > 0)
    {
        draw_line++;
    }
    if (data_ptr->letterwin_height > 0)
    {
        draw_line++;
    }
    if (data_ptr->preeditwin_height > 0)
    {
        draw_line++;
    }

    if (draw_line == 0)
    {
        return TRUE;
    }

    rect.top = rect.bottom - SG_SKIN_HEIGHT / 3 * draw_line;

    dis_point.x = rect.left;
    dis_point.y = rect.top;

    if (s_is_use_default_skin)
    {
        img_rect.left = 0;
        img_rect.right = MMITHEME_IsMainScreenLandscape() ? SG_SKIN_SCREEN_L_WIDTH - 1: SG_SKIN_SCREEN_P_WIDTH - 1;
        if (draw_line == 1)
        {
            img_rect.bottom = SG_SKIN_HEIGHT / 3;
            img_rect.top = 0;
        }
        else if (draw_line == 2)
        {
            img_rect.bottom = rect.bottom;
            img_rect.top = SG_SKIN_HEIGHT / 3;
        }
        else
        {
            img_rect.bottom = rect.bottom;
            img_rect.top = 0;
        }

        GUIRES_DisplayImg(&dis_point, &rect, &img_rect, data_ptr->win_id, IMAGE_SOGOU_DEFAULT_SKIN, data_ptr->lcd_info_ptr);
    }
    else if(s_sg_skin_bitmap_ptr && s_sg_skin_bitmap_ptr->bit_ptr)
    {
        if (FALSE == MMITHEME_IsMainScreenLandscape())
        {
            GUIIMG_DisplayBmp(FALSE, &rect, &dis_point, s_sg_skin_bitmap_ptr, data_ptr->lcd_info_ptr);
        }
        else
        {
            int16 repeat_times = (SG_SKIN_SCREEN_L_WIDTH - SG_SKIN_SCREEN_P_WIDTH) / SG_SKIN_REPEAT_AREA_WIDTH;
            int16 i;

            rect.right = SG_SKIN_LEFT_WIDTH;
            // first step: display left area
            GUIIMG_DisplayBmp(FALSE, &rect, &dis_point, s_sg_skin_bitmap_ptr, data_ptr->lcd_info_ptr);
            // second step: display repeat area
            for (i = 0; i < repeat_times; i++)
            {
                rect.left = SG_SKIN_LEFT_WIDTH + i * SG_SKIN_REPEAT_AREA_WIDTH;
                rect.right = rect.left + SG_SKIN_REPEAT_AREA_WIDTH - 1;
                dis_point.x = i * SG_SKIN_REPEAT_AREA_WIDTH;
                GUIIMG_DisplayBmp(FALSE, &rect, &dis_point, s_sg_skin_bitmap_ptr, data_ptr->lcd_info_ptr);
            }
            // third step: display right area;
            rect.left = SG_SKIN_LEFT_WIDTH + SG_SKIN_REPEAT_AREA_WIDTH * repeat_times;
            rect.right = SG_SKIN_SCREEN_L_WIDTH - 1;
            dis_point.x = SG_SKIN_SCREEN_L_WIDTH - SG_SKIN_SCREEN_P_WIDTH;
            GUIIMG_DisplayBmp(FALSE, &rect, &dis_point, s_sg_skin_bitmap_ptr, data_ptr->lcd_info_ptr);
        }
    }
//  else
//  {
//      LCD_FillRect(data_ptr->lcd_info_ptr, pShowRect, g_mmiim_ui_config.color_bg_pre_edit);
//  }

    return TRUE;
}

/******************************************************************************/
// Description: 是否使用的是默认皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGIsUseDefaultSkin(void)
{
    return s_is_use_default_skin;
}

#else
/******************************************************************************/
// Description: 如果不存在皮肤，则需要绘制背景，否则字符刷新时，
//                    候选字等会出现重叠
// Global resource dependence: NONE
// Author: yaoguang.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN SG_DisplayBg (MMIIM_SOGOU_DATA_T *data_ptr)
{
    int32 line_cnt = 0, iloop = 0;
    int16 line_top = 0;
    
    if (data_ptr == PNULL)
    {
        SCI_TRACE_LOW ("MMIIM:SG_DisplayBg");
        return FALSE;
    }
    
    //背景
    LCD_FillRect (data_ptr->lcd_info_ptr, data_ptr->rect, g_mmiim_ui_config.color_bg_default);

    if (data_ptr->preeditwin_height)
    {
        line_cnt ++;
    }

    if (data_ptr->letterwin_height)
    {
        line_cnt ++;
    }

    if (data_ptr->candwin_height)
    {
        line_cnt ++;
    }

#if defined KEYPAD_TYPE_QWERTY_KEYPAD
    if (data_ptr->rect.bottom != data_ptr->rect.top && line_cnt == 0)   //初始化状态
    {
        line_cnt = 2;       //26键输入时，没有preedit窗口，故只需要2行显示
    }
#else
    if (data_ptr->rect.bottom != data_ptr->rect.top && line_cnt == 0)   //初始化状态
    {
        line_cnt = 3;
    }
#endif
    
    //需要分割线
    if(line_cnt!=0)
    {
        for (iloop = 0; iloop < line_cnt; iloop ++)
        {
            
            line_top = data_ptr->rect.top + (data_ptr->rect.bottom - data_ptr->rect.top) * iloop / line_cnt;
            LCD_DrawLine (data_ptr->lcd_info_ptr, data_ptr->rect.left, line_top, data_ptr->rect.right, line_top, g_mmiim_ui_config.color_border);
        }
    }

    return TRUE;
}
#endif
/******************************************************************************/
// Description: 是否需要绘制皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGIsDrawSkin(void)
{
    if ((s_sg_skin_bitmap_ptr && s_sg_skin_bitmap_ptr->bit_ptr) || s_is_use_default_skin)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef SOGOU_USE_PDA_SKIN
/*lint -save -e574 -e737*/

/******************************************************************************/
// Description: 在指定区域内绘制皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_DrawSkinInRect(MMI_WIN_ID_T win_id, GUI_RECT_T *rect_ptr, 
                                       GUILCD_DRAW_DC_T *dc_data_ptr)
{
    uint32 i = 0;
    //uint32 j = 0;
    uint8 *buffer = PNULL;
    uint8 *first_line_buffer = PNULL;
    GUI_RECT_T disp_rect = {0};
    uint16 skin_width = 0, skin_height = 0;
    GUI_RECT_T img_rect;
	uint32 bytes_per_pixel = 0;

    //参数校验
    if (rect_ptr == PNULL || dc_data_ptr == PNULL || dc_data_ptr->lcd_buffer_ptr == PNULL)
    {
        SCI_TraceLow ("MMIAPIIM_DrawSkinInRect Err param");
        return FALSE;
    }

    //显示区域设置
    disp_rect = *rect_ptr;
    if (disp_rect.right <= disp_rect.left || disp_rect.bottom <= disp_rect.top)
    {
        SCI_TraceLow ("MMIAPIIM_DrawSkinInRect Err disprect");
        return FALSE;
    }

    if (DATA_TYPE_ARGB888 == dc_data_ptr->data_type)		//GetBytesPerPixel
    {
        bytes_per_pixel = 4;
    }
    else		//不管怎样，这么处理，总不至于导致内存越界覆盖
    {
        bytes_per_pixel = 2;
    }

    if (disp_rect.bottom >= dc_data_ptr->height)     //超过dc显示区域
    {
        disp_rect.bottom = dc_data_ptr->height -1;
    }
    if (disp_rect.right >= dc_data_ptr->mem_width)
    {
        disp_rect.right = dc_data_ptr->mem_width -1;
    }

    buffer = dc_data_ptr->lcd_buffer_ptr;
    if (s_is_use_default_skin)
    {
        //图片大小
        GUIRES_GetImgWidthHeight(&skin_width, &skin_height, IMAGE_SOGOU_DEFAULT_SKIN, win_id);
        if (skin_width == 0 || skin_height == 0)
        {
            SCI_TraceLow ("MMIAPIIM_DrawSkinInRect");
            return FALSE;
        }
        img_rect.left = 0;
        img_rect.right = disp_rect.right - disp_rect.left;
        img_rect.top = SG_SKIN_HEIGHT / 3;
        img_rect.bottom = SG_SKIN_HEIGHT / 3;
        GUIRES_DisplayImgDst(NULL, &disp_rect, &img_rect, win_id, IMAGE_SOGOU_DEFAULT_SKIN, dc_data_ptr);       //从距离顶部1/3的地方开始显示

        if (skin_height >= disp_rect.bottom - disp_rect.top)        //图片比显示区域大，取1/3处的一行数据填充
        {
            first_line_buffer = buffer + (disp_rect.top * dc_data_ptr->mem_width * bytes_per_pixel);
            for (i = disp_rect.top + 1; i <= disp_rect.bottom; i++)
            {
                SCI_MEMCPY (buffer + i * (dc_data_ptr->mem_width * bytes_per_pixel), first_line_buffer, (disp_rect.right - disp_rect.left) * bytes_per_pixel);
            }
#if 0
			//绘制
            img_rect.left = 0;
            img_rect.right = skin_width;
            img_rect.top = 0;
            img_rect.bottom = disp_rect.bottom - disp_rect.top;

            GUIRES_DisplayImgDst(NULL, &disp_rect, &img_rect, win_id, IMAGE_SOGOU_DEFAULT_SKIN, dc_data_ptr);
#endif
        }
        else
        {
            first_line_buffer = buffer + (disp_rect.top * dc_data_ptr->mem_width * bytes_per_pixel);
            for (i = disp_rect.top + 1; i <= disp_rect.bottom - SG_SKIN_HEIGHT * 2 / 3; i++)
            {
                SCI_MEMCPY (buffer + i * (dc_data_ptr->mem_width * bytes_per_pixel), first_line_buffer, (disp_rect.right - disp_rect.left) * bytes_per_pixel);
            }
            img_rect.top = SG_SKIN_HEIGHT / 3;
            img_rect.bottom = SG_SKIN_HEIGHT - 1;
            disp_rect.top = disp_rect.bottom - SG_SKIN_HEIGHT * 2 / 3 + 1;
            GUIRES_DisplayImgDst(NULL, &disp_rect, &img_rect, win_id, IMAGE_SOGOU_DEFAULT_SKIN, dc_data_ptr);
#if 0			
            //显示图片部分
            disp_rect.top = disp_rect.bottom - skin_height;

            img_rect.left = 0;
            img_rect.right = skin_width;
            img_rect.top = 0;
            img_rect.bottom = skin_height;
            
            GUIRES_DisplayImgDst (NULL, &disp_rect, &img_rect, win_id, IMAGE_SOGOU_DEFAULT_SKIN, dc_data_ptr);   

            
            //填充多出的部分
            first_line_buffer = buffer + ((disp_rect.top + 1) * dc_data_ptr->mem_width * bytes_per_pixel);
            for (i = rect_ptr->top; i <= disp_rect.top; i++)
            {
                SCI_MEMCPY (buffer + i * (dc_data_ptr->mem_width * bytes_per_pixel), first_line_buffer, (disp_rect.right - disp_rect.left) * bytes_per_pixel);
            }
#endif
        }
    }
    else if(s_sg_skin_bitmap_ptr && s_sg_skin_bitmap_ptr->bit_ptr)
    {
        GUIIMG_DISPLAYBMP_PARAM_T display_param = { 0 };
        GUI_LCD_DEV_INFO lcd_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
        BOOLEAN is_landscape = MMITHEME_IsMainScreenLandscape();
        GUI_POINT_T dis_point;
        dis_point.x = rect_ptr->left;
#ifdef WIN32
        dis_point.y = rect_ptr->top - SG_SKIN_HEIGHT / 3;        
#else
        dis_point.y = rect_ptr->top - SG_SKIN_HEIGHT / 3 - 1;
#endif
        disp_rect.bottom = rect_ptr->top;
        display_param.is_horizontal = FALSE;
        display_param.rect_ptr = &disp_rect;
        display_param.point_ptr = &dis_point;
        display_param.bitmap_ptr = s_sg_skin_bitmap_ptr;
        display_param.lcd_dev_ptr = &lcd_info;
        display_param.is_transparent_mode = FALSE;
        display_param.transparent_color = 0;
        if (is_landscape)
        {
            if (s_sg_skin_bitmap_ptr->img_width < disp_rect.right - disp_rect.left)
            {
                int16 repeat_times = (disp_rect.right - disp_rect.left - s_sg_skin_bitmap_ptr->img_width) / SG_SKIN_REPEAT_AREA_WIDTH + 1;
                int16 k;
                disp_rect.right = SG_SKIN_LEFT_WIDTH + disp_rect.left;
                GUIIMG_DisplayBmpDst(&display_param, dc_data_ptr);
                for (k = 0; k < repeat_times; k++)
                {
                    disp_rect.left = SG_SKIN_LEFT_WIDTH + k * SG_SKIN_REPEAT_AREA_WIDTH;
                    disp_rect.right = disp_rect.left + SG_SKIN_REPEAT_AREA_WIDTH - 1;
                    dis_point.x = SG_SKIN_LEFT_WIDTH * i;
                    GUIIMG_DisplayBmpDst(&display_param, dc_data_ptr);
                }
                disp_rect.right = rect_ptr->right;
                disp_rect.left = disp_rect.right - SG_SKIN_SCREEN_P_WIDTH + SG_SKIN_LEFT_WIDTH;
                dis_point.x = disp_rect.left - SG_SKIN_LEFT_WIDTH;
                GUIIMG_DisplayBmpDst(&display_param, dc_data_ptr);
            }
        }
        else
        {
            GUIIMG_DisplayBmpDst(&display_param, dc_data_ptr);
        }
        disp_rect = *rect_ptr;

        buffer = dc_data_ptr->lcd_buffer_ptr;
        if (s_sg_skin_bitmap_ptr->img_height > disp_rect.bottom - disp_rect.top)
        {
            first_line_buffer = buffer + (disp_rect.top * dc_data_ptr->mem_width * bytes_per_pixel);
            for (i = disp_rect.top + 1; i <= disp_rect.bottom; i++)
            {
                SCI_MEMCPY (buffer + i * (dc_data_ptr->mem_width * bytes_per_pixel), first_line_buffer, (disp_rect.right - disp_rect.left) * bytes_per_pixel);
            }
#if 0
            for (i = disp_rect.top + 1; i <= disp_rect.bottom; i++)
            {
                for (j = disp_rect.left; j <= disp_rect.right; j++)
                {
                    buffer[dc_data_ptr->mem_width * i + j] = buffer[dc_data_ptr->mem_width * disp_rect.top + j];
                }
            }
#endif
        }
        else
        {
			first_line_buffer = buffer + (disp_rect.top * dc_data_ptr->mem_width * bytes_per_pixel);
			for (i = disp_rect.top + 1; i <= disp_rect.bottom - SG_SKIN_HEIGHT * 2 / 3; i++)
			{
				SCI_MEMCPY (buffer + i * (dc_data_ptr->mem_width * bytes_per_pixel), first_line_buffer, (disp_rect.right - disp_rect.left) * bytes_per_pixel);
			}
#if 0
            for (i = disp_rect.top + 1; i <= disp_rect.bottom - SG_SKIN_HEIGHT * 2 / 3; i++)
            {
                for (j = disp_rect.left; j <= disp_rect.right; j++)
                {
                    buffer[dc_data_ptr->mem_width * i + j] = buffer[dc_data_ptr->mem_width * disp_rect.top + j];
                }
            }
#endif

            if (is_landscape)
            {
                int16 repeat_times = (disp_rect.right - disp_rect.left - s_sg_skin_bitmap_ptr->img_width) / SG_SKIN_REPEAT_AREA_WIDTH + 1;
                int16 k;
                
                disp_rect.top = disp_rect.bottom - SG_SKIN_HEIGHT * 2 / 3;
                dis_point.x = disp_rect.left;
#ifdef WIN32
                dis_point.y = disp_rect.bottom - SG_SKIN_HEIGHT;
#else
                dis_point.y = disp_rect.bottom - SG_SKIN_HEIGHT - 1;
#endif

                disp_rect.right = SG_SKIN_LEFT_WIDTH + disp_rect.left;
                // first step: display left area
                GUIIMG_DisplayBmpDst(&display_param, dc_data_ptr);
                // second step: display repeat area
                for (k = 0; k < repeat_times; k++)
                {
                    disp_rect.left = SG_SKIN_LEFT_WIDTH + k * SG_SKIN_REPEAT_AREA_WIDTH;
                    disp_rect.right = disp_rect.left + SG_SKIN_REPEAT_AREA_WIDTH - 1;
                    dis_point.x = SG_SKIN_REPEAT_AREA_WIDTH * k;
                    GUIIMG_DisplayBmpDst(&display_param, dc_data_ptr);
                }
                // third step: display right area;
                disp_rect.right = rect_ptr->right;
                disp_rect.left = disp_rect.right - SG_SKIN_SCREEN_P_WIDTH + SG_SKIN_LEFT_WIDTH;
                dis_point.x = disp_rect.left - SG_SKIN_LEFT_WIDTH;
                GUIIMG_DisplayBmpDst(&display_param, dc_data_ptr);
            }
            else
            {
                disp_rect.top = disp_rect.bottom - SG_SKIN_HEIGHT * 2 / 3;
#ifdef WIN32
                dis_point.y = disp_rect.bottom - SG_SKIN_HEIGHT;
#else
                dis_point.y = disp_rect.bottom - SG_SKIN_HEIGHT - 1;
#endif
                GUIIMG_DisplayBmpDst(&display_param, dc_data_ptr);
            }
        }
    }
    //  else
    //  {
    //      LCD_FillRect(data_ptr->lcd_info_ptr, pShowRect, g_mmiim_ui_config.color_bg_pre_edit);
    //  }
    
    return TRUE;
}
/*lint -restore */ 
#endif
/******************************************************************************/
// Description: 恢复搜狗输入法设置到出厂值
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
#ifdef IM_SIMP_CHINESE_SUPPORT
extern SYSTEM_UPDATE_SETTINGS_DATA_T g_sogou_update_settings_data;
extern VOCABULARY_UPDATE_FREQUENCY_T g_sogou_vocabulary_update_frequency_data;
#endif

PUBLIC BOOLEAN MMIAPIIM_SGResetNv (void)
{
    // nv items
#ifdef IM_SIMP_CHINESE_SUPPORT
    g_sogou_update_settings_data.is_need_connect_remind = TRUE;
    g_sogou_update_settings_data.is_need_update_notif = TRUE;
    SCI_MEMSET(&g_sogou_vocabulary_update_frequency_data.time, 0, sizeof(g_sogou_vocabulary_update_frequency_data.time));
    g_sogou_vocabulary_update_frequency_data.frequency = VOCABULARY_UPDATE_FREQUENCY_E_SEVEN;

    MMI_WriteNVItem(IMNV_SOGOU_UPDATE_FREQUENCY, &g_sogou_vocabulary_update_frequency_data.frequency);
    MMI_WriteNVItem(IMNV_SOGOU_UPDATE_SETTING, &g_sogou_update_settings_data);
    MMI_WriteNVItem(IMNV_SOGOU_UPDATE_TIME, &g_sogou_vocabulary_update_frequency_data.time);

	MMIAPIIM_SGDelAutoEvent();//6_12_1
#endif
    return TRUE;
}

PUBLIC BOOLEAN MMIAPIIM_SGResetDefault(void)
{
#if defined IM_SIMP_CHINESE_SUPPORT
    // 删除系统BIN
    WD_WordBinRestore();
#endif

	if(0 == g_sg_save_position)
	{
		SFS_DeleteFile(L"D:\\@sogou\\worddata",PNULL);
	}
	else if(1 == g_sg_save_position)
	{
		SFS_DeleteFile(L"E:\\@sogou\\worddata",PNULL);
	}
	
    // 恢复默认皮肤设置
#ifdef SOGOU_USE_PDA_SKIN
    SG_SetupDefaultSkin();
#endif

    return TRUE;
}

//这个是sogou输入法的准备工作，与输入法实例不相关，仅仅在开机时初始化一次
//因速度较慢，在收到初始化消息后初始化
PUBLIC void MMIAPIIM_SGInit (void)
{
	SG_Check_Build_Time ();
	
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
    SG_InitImeFile();
    SG_QuitImeFile();
#endif    
#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_SGGetImeSkin();
#endif
}

#else
//define a fake variable
PUBLIC GUIIM_KERNEL_APIS_T const g_guiim_sogou_apis = { 0 };
#endif //MMIIM_USE_KB_SOGOU


/*Edit by script, ignore 2 case. Thu Apr 26 19:00:56 2012*/ //IGNORE9527
