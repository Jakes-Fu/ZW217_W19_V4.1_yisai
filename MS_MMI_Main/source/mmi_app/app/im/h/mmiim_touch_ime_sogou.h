/*==============================================================================
File Name: mmiim_touch_ime_sogou.h
Author: Lianxiang.Zhou
Date: 2011/12/14
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/14   Lianxiang.Zhou  Create.
==============================================================================*/


#ifndef _MMIIM_TOUCH_IME_SOGOU_H_
#define _MMIIM_TOUCH_IME_SOGOU_H_



/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "mmiim_touch_ime_common.h"

#if defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_SOGOU)

#include "WD_def.h"
#include "iHCR.h"
#include "WD_HwDef.h"

#include "mmiim_sogou.h"



//========================sogou特殊的定义
#define IME_LIST_MAX        10

#define SOGOU_PINYIN_MATCH_INDEX_NONE 0xFFFF

//仅仅检测KERNEL_DATA_SOGOU_T；小心hw等结构体不同引起异常
#define CHECK_ENGINE_DATA(data_ptr) \
    do {\
        CHECK_DATA(data_ptr); \
        SCI_ASSERT(PNULL != ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))); /*assert verified*/ \
        SCI_ASSERT(PNULL != ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->pime_result); /*assert verified*/ \
    } while (0)

typedef enum _SOGOU_CAPLOCK_E
{
    E_CAPLOCK_LOWER,
    E_CAPLOCK_LEAD_UPPER,
    E_CAPLOCK_UPPER,
    E_CAPLOCK_MAX,
} SOGOU_CAPLOCK_E;

typedef enum _SOGOU_SELECT_E
{
    E_SELECT_NONE,
    E_SELECT_CAND,
    E_SELECT_LETTER,
    E_SELECT_MAX,
} SOGOU_SELECT_E;


// 候选字词最大页数
#define MAX_SOGOU_CANDIDATE_PAGE_CHINESE MAX_PAGE_FOR_SOGOU
#define MAX_SOGOU_CANDIDATE_PAGE_ENGLISH 20

#define MAX_SOGOU_CANDIDATE_PAGE (20)       //候选字词最大页数,这个是测试出来的，暂时没有查找到相关文档



typedef struct KERNEL_DATA_SOGOU_TAG
{
    //输入法引擎相关
    WD_imeResult         *pime_result;
    WD_UINT8             lastdisp;

    //各种输入状态单独使用的数据
    uint8 choice_pinyin_page;     //9键拼音输入时，choice的页码
} KERNEL_DATA_SOGOU_T;


//sogou输入法引擎相关联函数

// ==============搜狗输入法指针变量
//extern uint8 g_FlashData[FLASH_CAPACITY];


/* thumb en */
PUBLIC void ThumbLatinSwitchExpandKey(IM_DATA_T * data_ptr);
PUBLIC void ThumbLatinDispEdit(IM_DATA_T *data_ptr);
PUBLIC WD_imeResult* MMIIM_TOUCH_Sogou_GetImeResult(IM_DATA_T *data_ptr);

/* qwert en */
PUBLIC void QwertLatinClear(IM_DATA_T *data_ptr);
PUBLIC void QwertSmartSwitchExpandKey(IM_DATA_T * data_ptr);



#if defined (IM_SIMP_CHINESE_SUPPORT)
/* thumb pinyin */
PUBLIC BOOLEAN ThumbPinyinInit(IM_DATA_T *data_ptr, WD_UINT8 method);
PUBLIC BOOLEAN ThumbPinyinTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinDraw(IM_DATA_T * data_ptr);
PUBLIC void ThumbPinyinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbPinyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void ThumbPinyinClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinUpdate(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinDispEdit(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinSetCust(IM_DATA_T *data_ptr);
#endif

#if defined (IM_SIMP_CHINESE_SUPPORT)
/* Qwert pinyin */
PUBLIC BOOLEAN QwertPinyinInit(IM_DATA_T *data_ptr, WD_UINT8 method);
PUBLIC BOOLEAN QwertPinyinTerm(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinDraw(IM_DATA_T * data_ptr);
PUBLIC void QwertPinyinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void QwertPinyinClear(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinUpdate(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinSetCand(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinDispEdit(IM_DATA_T *data_ptr);
#endif

#if (defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT))	//繁体中文复用简体中文的笔画输入法
/* thumb stroke */
PUBLIC BOOLEAN ThumbStrokeInit(IM_DATA_T *data_ptr, WD_UINT8 method);
PUBLIC BOOLEAN ThumbStrokeTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeDraw(IM_DATA_T * data_ptr);
PUBLIC void ThumbStrokeInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbStrokeCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void ThumbStrokeClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeUpdate(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeDispEdit(IM_DATA_T *data_ptr);
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
/*==============================================================================
Description: ThumbZhuyinInit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinInit(IM_DATA_T *data_ptr, WD_UINT8 method);


/*==============================================================================
Description: ThumbZhuyinTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinTerm(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ThumbAbcDraw
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinDraw(IM_DATA_T * data_ptr);


/*==============================================================================
Description: 初始化键盘为拇指英文字母输入
Global resource dependence: NONE
Author: yaoguang.chen
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void ThumbZhuyinInitKb(IM_DATA_T *data_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入长按 - character键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - character键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入长按 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - custom键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: ThumbZhuyinSetLetterCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetLetterCand(IM_DATA_T *data_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
我们有两种候选按钮
1 候选字
2 候选拼音
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: ThumbZhuyinClear
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinClear(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ThumbZhuyinUpdate
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinUpdate(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ThumbZhuyinSetCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetCand(IM_DATA_T *data_ptr);


/*==============================================================================
Description: 显示pre edit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinDispEdit(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ThumbZhuyinSetCust
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetCust(IM_DATA_T *data_ptr);


/*==============================================================================
Description: QwertZhuyinInit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinInit(IM_DATA_T *data_ptr, WD_UINT8 method);


/*==============================================================================
Description: QwertZhuyinTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinTerm(IM_DATA_T *data_ptr);


/*==============================================================================
Description: QwertAbcDraw
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinDraw(IM_DATA_T * data_ptr);


/*==============================================================================
Description: 初始化键盘为拇指英文字母输入
Global resource dependence: NONE
Author: yaoguang.chen
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void QwertZhuyinInitKb(IM_DATA_T *data_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - character键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入长按 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: 拇指键盘Zhuyin输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
我们有两种候选按钮
1 候选字
2 候选拼音
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: QwertZhuyinClear
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinClear(IM_DATA_T *data_ptr);


/*==============================================================================
Description: QwertZhuyinUpdate
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinUpdate(IM_DATA_T *data_ptr);


/*==============================================================================
Description: QwertZhuyinSetCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinSetCand(IM_DATA_T *data_ptr);


/*==============================================================================
Description: 显示pre edit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinDispEdit(IM_DATA_T *data_ptr);
#endif

/*----------------------------------------------------------------------------------------------------------------------
Name:   MultiCharKeyInit

Func:   初始化一个多字符按键

Input:
            key_ptr:    操作的按键
            text_ptr:   按键中的字符数组

Output:

Return:
            err: < 0  succ: == 0

Thoughtway:

Create: yaoguangchen@spread. 2011-10-11

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MultiCharKeyInit(KEY_INFO_T *key_ptr, const wchar *const text_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartInit

Func:   智能输入法初始化

Input:
            data_ptr:    输入法指针

Output:

Return:
            TRUE:成功，FALSE:参数不合法或失败

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartInit(IM_DATA_T *data_ptr, WD_UINT8 sogou_method);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartTerm

Func:   9键智能输入法关闭

Input:
            data_ptr:    输入法指针

Output:

Return:
            TRUE:成功，FALSE:参数不合法或失败

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartTerm(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartDraw

Func:   9键智能输入法绘制

Input:
            data_ptr:    输入法指针

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartDraw(IM_DATA_T * data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartInitKb

Func:   9键智能输入法键盘初始化

Input:
            data_ptr:    输入法指针

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartInitKb(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCharLong

Func:   9键智能输入法，长按字符键0 ~ 9消息处理，如果为输入首个字符时，则将数字直接上屏

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的按键，会先检测该按键是否为字符按键，否则不做处理

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCharUp

Func:   9键智能输入法，字符键0 ~ 9消息处理

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的按键，会先检测该按键是否为字符按键，否则不做处理

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCtrlLong

Func:   9键智能输入法，控制键长按消息处理,此处暂时直接调用

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的控制按键

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCtrlUp

Func:   9键智能输入法，控制键消息处理

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的控制按键

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCustUp

Func:   9键智能输入法，用户自定义按键消息处理，此处默认定义为符号输入按键

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的控制按键

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartSysUp

Func:   9键智能输入法，系统按键消息处理

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的控制按键

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCandUp

Func:   9键智能输入法，候选字点击消息处理

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的候选字按键

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartClear

Func:   9键智能输入法，清除输入内容

Input:
            data_ptr:    输入法指针

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartClear(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartUpdate

Func:   9键智能输入法，内容刷新

Input:
            data_ptr:    输入法指针

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartUpdate(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   SmartSetCand

Func:   智能输入法，设置候选字

Input:
            data_ptr:    输入法指针

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:
        1.From ThumbSmartSetCand to SmartSetCand
        2.不再区分9键，26键，latin设置候选字，减少维护工作量
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void SmartSetCand (IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartDispEdit

Func:   9键智能输入法，当前已输入的字符串显示

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartDispEdit(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartInit

Func:   26键智能输入法初始化

Input:
            data_ptr:    输入法指针
            method:     sogou引擎对应的输入法

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartInit(IM_DATA_T *data_ptr, WD_UINT8 sogou_method);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartTerm

Func:   9键智能输入法关闭

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartTerm(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartDraw

Func:   26键智能输入法绘制

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartDraw(IM_DATA_T * data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartInitKb

Func:   26键智能输入法初始化虚拟键盘

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartInitKb(IM_DATA_T *data_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartUpdate

Func:   26键智能输入法刷新

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartUpdate(IM_DATA_T *data_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCharUp

Func:   26键智能输入法绘制

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCtrlLong

Func:   26键智能输入法控制按键长按消息处理

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCtrlUp

Func:   26键智能输入法控制按键消息处理

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartSysUp

Func:   26键智能输入法系统按键消息处理

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的系统按键

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCandUp

Func:   26键智能输入法候选字消息处理

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的系统按键

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartClear

Func:   26键智能输入法清空所有输入

Input:
            data_ptr:    输入法指针

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartClear(IM_DATA_T *data_ptr);


PUBLIC int32 Sogou_AssociateTo2DemensionArray(wchar *array, wchar *sogou_cand, uint16 array_len);

/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_ImeInit

Func:   sogou输入法引擎初始化

Input:  g_imeResult: 静态变量，输入法引擎的输入输出数据结构
           g_FlashData: 输入法需要的数据库
           MMIAPIIM_GetLanguage():  当前输入语言

Output:
            g_imeResult:静态变量，输入法引擎的输入输出数据结构

Return:
            == 0成功， > 0失败

Thoughtway:
            1. sogou输入法引擎初始化，需要传入语言参数，故对当前语言有一个依赖关系

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_ImeInit(IM_DATA_T *data_ptr, WD_UINT8 method);

/*==============================================================================
Description: if a key in a keyboard bmp, it will need to paint the bg, so caused 
	need to repaint all other keys.
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_BmpBgKeyCausedRepaint (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ConvertSpCapToSogou

Func:   将我们平台使用的大小写转换为sogou输入法大小写设置，这个类似中间层的概念，
            便于后期可能维护的其他平台的输入法引擎

Input:  CAPITTAL_MODE_T cap:平台大小写值

Output:

Return:
            SOGOU_CAPLOCK_E:    sogou输入法引擎的大小写值

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC SOGOU_CAPLOCK_E ConvertSpCapToSogou(CAPITTAL_MODE_T cap);

/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SetCapsLock

Func:   设置sogou输入法大小写

Input:
            set_caps: sogou 引擎对应的大小写

Output:

Return:
            < 0 err, else 0

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SetCapsLock(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ConvetIndexToVKey

Func:   26键虚拟键盘的按键布局是通过索引值传到按键回调函数的，而该按键索引与
            输入法引擎之间的按键索引顺序自然不对，需要做一个转换

Input:
            index:  26键虚拟键盘中的按钮索引值

Output:

Return:
            err:  0
            > 0 虚拟键盘对应按键对应的按键码

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint16 ConvetIndexToVKey(uint16 index);


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatCandPage

Func:   测试sogou输入法是否存在下一页候选字

Input:
            data_ptr:  当前输入法管理句柄

Output:

Return:

Thoughtway:
            1. sogou输入法没有在一次给出有多少页候选字，需要自己模拟测试是否有后一页

Notes:

Create: yaoguangchen@spread. 2011-09-21

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN Sogou_TestCandNextPage(IM_DATA_T *data_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatCandPage

Func:   模拟sogou输入法候选字翻页功能

Input:
            data_ptr:  当前输入法管理句柄
            pageup: 向上翻页or向下翻页

Output:

Return:
            err:  0
            > 0 虚拟键盘对应按键对应的按键码

Thoughtway:
            1.  本来想通过函数WD_SetVariable直接将翻页前的状态转换为选择状态的，但似乎无效，
                故只能通过按向下键的方式，从输入状态转换到选择状态

Notes:  该函数有概率性引起死机问题，会死在sogou库函数中，待sogou那边处理

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SimulatCandPage(IM_DATA_T *data_ptr, BOOLEAN pageup);



/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatLetterPage

Func:   模拟sogou输入法拼音翻页功能

Input:
            data_ptr:  当前输入法管理句柄
            pageup: 向上翻页or向下翻页

Return:
            err: < 0
            succ:  0

Thoughtway:
            候选拼音翻页

Notes:  该函数有概率性引起死机问题，会死在sogou库函数中，待sogou那边处理

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SimulatLetterPage(IM_DATA_T *data_ptr, BOOLEAN pageup);


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SetLastDisp

Func:   设置输入法上一次显示的候选字或词组个数

Input:
            data_ptr:  当前输入法管理句柄
            lastdisp:   上一次显示的候选字或词组个数

Output:
            将上一次的候选字或词组个数保存到输入法管理句柄中

Return:
            PNULL:参数无效或数据指针为空

Thoughtway:
            1.  该函数主要是因为输入法引擎在每次按键之前，需要知道当前一次显示的候选字
                个数，否则获取的候选字会不准确或其他情况发生
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void Sogou_SetLastDisp(IM_DATA_T *data_ptr, uint8 lastdisp);


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetLastDisp

Func:   获取输入法上一次显示的候选字或词组个数

Input:
            data_ptr:  当前输入法管理句柄

Output:
            将上一次的候选字或词组个数传出

Return:
            0: 参数无效或首次显示时的候选字个数为0

Thoughtway:
            1.  该函数主要是因为输入法引擎在每次按键之前，需要知道当前一次显示的候选字
                个数，否则获取的候选字会不准确或其他情况发生
            2.  该函数每次调用时，会重新将lastdisp值写掉，原因是，我们的UI有时候会将候选字
                展开或收起，导致显示的候选字个数不同
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetLastDisp(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetCandDisplayCnt

Func:   获取输入法当前显示的候选字或词组个数

Input:
            data_ptr:  当前输入法管理句柄

Output:
            根据候选字个数和候选按钮个数来判断当前显示的候选字个数

Return:
            输入法当前显示的候选字或词组个数

Thoughtway:
            1.  该函数主要是因为输入法引擎在每次按键之前，需要知道当前一次显示的候选字
                个数，否则获取的候选字会不准确或其他情况发生
            2. 函数与Sogou_GetLastDisp不同的地方是Sogou_GetLastDisp将获取的值做了一个保存并返回
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetCandDisplayCnt(IM_DATA_T *data_ptr);



PUBLIC int32 Sogou_DrawVkbSkin (IM_DATA_T *data_ptr, GUILCD_DRAW_DC_T *pdc_data);

/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetLetterTotalCnt

Func:   获取输入法当前候选拼音的总个数

Input:
            data_ptr:  当前输入法管理句柄

Return:
            返回候选拼音总个数

Thoughtway:
            1.候选拼音总个数，便于显示当前候选拼音字符及是否存在下一页
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetLetterTotalCnt(IM_DATA_T *data_ptr);


//孟加拉语特殊处理
#if defined IM_BENGALI_SUPPORT
PUBLIC BOOLEAN ThumbBengaliInit (IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbBengaliInitKb

Func:   9键智能输入法键盘初始化

Input:
            data_ptr:    输入法指针

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-07-17

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbBengaliInitKb(IM_DATA_T *data_ptr);


PUBLIC BOOLEAN ThumbBengaliTerm (IM_DATA_T *data_ptr);

#endif		//bengali end


#endif //defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_SOGOU)

#endif /*_MMIIM_TOUCH_IME_SOGOU_H_*/


