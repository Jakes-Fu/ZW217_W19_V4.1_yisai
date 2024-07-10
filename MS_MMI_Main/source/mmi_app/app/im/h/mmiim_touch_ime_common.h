/*==============================================================================
File Name: mmiim_touch_ime_common.h
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/

#ifndef _MMIIM_TOUCH_IME_COMMON_H_
#define _MMIIM_TOUCH_IME_COMMON_H_



/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "mmiim_touch_common.h"


#if defined (MMI_IM_PDA_SUPPORT)


#define SYMBOL_PAGES (4)
#define SYMBOL_PAGE_NUM (0)
#define SYMBOL_PAGE_SYM (1)
#define SYMBOL_PAGE_FACE (3)
#define SYMBOL_COL (5)
#define SYMBOL_ROW (4)
#define SYMBOL_PAGE_SIZE (SYMBOL_COL*SYMBOL_ROW)

#define SYS_PAGES (5)
#define SYS_PAGE_NUM (0)
#define SYS_PAGE_SYM (4)
#define SYS_COL (3)
#define SYS_ROW (4)
#define SYS_CTRL_CNT (6)
#define SYS_PAGE_SIZE (SYS_COL*SYS_ROW)

typedef struct SYS_DATA_TAG
{
    uint8 cur_page;
} SYS_DATA_T;

typedef struct KERNEL_DATA_ABC_TAG
{
    KEY_INFO_T      *last_key;
    uint8           last_key_index;
    uint8           commit_timer;
} KERNEL_DATA_ABC_T;

typedef struct MULTI_TAP_DATA_TAG
{
    KEY_INFO_T      *last_key;
    uint8           last_key_index;
    uint8           commit_timer;
} MULTI_TAP_DATA_T;



typedef struct KERNEL_DATA_DIGIT_TAG
{
    uint8 is_expand;
} KERNEL_DATA_DIGIT_T;


enum SYMBOL_TYPE_TAG
{
    SYMBOL_TYPE_HALF,
    SYMBOL_TYPE_FULL,
};
typedef uint8 SYMBOL_TYPE_T;

enum SYMBOL_INIT_TAG
{
    SYMBOL_INIT_NUM,
    SYMBOL_INIT_SYM,
    SYMBOL_INIT_FACE,
};
typedef uint8 SYMBOL_INIT_T;

typedef struct
{
    SYMBOL_TYPE_T type;
    uint8 is_lock;
    uint8 cur_page;
} SYMBOL_DATA_T;





#ifdef __cplusplus
extern "C"
{
#endif

    /* symbol */
    PUBLIC BOOLEAN SymInit(IM_DATA_T *data_ptr, SYMBOL_TYPE_T type, SYMBOL_INIT_T init);
    PUBLIC BOOLEAN SymTerm(IM_DATA_T *data_ptr);
    PUBLIC void SymDraw(IM_DATA_T * data_ptr);
    PUBLIC void SymInitKb(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN SymCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN SymCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC void SymUpdate(IM_DATA_T *data_ptr);


    /* digit */
    PUBLIC BOOLEAN DigitInit(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN DigitTerm(IM_DATA_T *data_ptr);
    PUBLIC void DigitDraw(IM_DATA_T * data_ptr);
    PUBLIC void DigitInitKb(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN DigitCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN DigitCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    /*  system  */
    PUBLIC void SysDraw(IM_DATA_T * data_ptr);
    PUBLIC BOOLEAN SysTerm(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN SysInit(IM_DATA_T *data_ptr, uint8 page_index);

    /* thumb abc */
    PUBLIC BOOLEAN ThumbAbcInit(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN ThumbAbcTerm(IM_DATA_T *data_ptr);
    PUBLIC void ThumbAbcDraw(IM_DATA_T * data_ptr);
    PUBLIC void ThumbAbcInitKb(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN ThumbAbcCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAbcCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
#if 0   //火拼功能去掉
    PUBLIC void ThumbAbcInitKbPbHuoPin(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN ThumbAbcCharUpPbHuoPin(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
#endif
    PUBLIC BOOLEAN ThumbAbcCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAbcCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAbcCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC wchar ThumbAbcGetChar(CAPITTAL_MODE_T cap, uint16 key_code, uint8 key_idx);
    PUBLIC BOOLEAN ThumbAbcCommitLast(IM_DATA_T *data_ptr);
    PUBLIC void ThumbAbcCommitCB(uint8 timer_id, uint32 param);

    /* qwert abc */
    PUBLIC BOOLEAN QwertAbcInit(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN QwertAbcTerm(IM_DATA_T *data_ptr);
    PUBLIC void QwertAbcDraw(IM_DATA_T * data_ptr);
    PUBLIC void QwertAbcInitKb(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN QwertAbcCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN QwertAbcCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


    PUBLIC BOOLEAN MultiTapCommitLast(IM_DATA_T *data_ptr);
    PUBLIC void MultiTapCommitCB(uint8 timer_id, uint32 param);
    PUBLIC BOOLEAN MultiTapCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN MultitapVkeyUpDeal(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ClearMultitapTimer (IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN ThumbMultiTapInit(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN ThumbMultiTapTerm(IM_DATA_T *data_ptr);
    PUBLIC void ThumbMultiTapDraw(IM_DATA_T * data_ptr);
    PUBLIC BOOLEAN ThumbMultiTapCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbMultiTapCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbMultiTapCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbMultiTapCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbMultiTapCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC void ThumbMultiTapInitKb(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN QwertMultiTapInit(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN QwertMultiTapTerm(IM_DATA_T *data_ptr);
    PUBLIC void QwertMultiTapDraw(IM_DATA_T * data_ptr);
    PUBLIC void QwertMultiTapInitKb(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN QwertMultiTapCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN QwertMultiTapCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

    //特殊字符配置重定义键
    PUBLIC int32 ThumbSmartRedefineKey0 (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr);
    PUBLIC int32 ThumbSmartRedefineKey1 (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr);
    PUBLIC int32 ThumbSmartRedefineCaps (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr);
    PUBLIC int32 ThumbMultitapRedefineKey0(GUIIM_METHOD_T method, KEY_INFO_T *key_ptr);
    PUBLIC int32 ThumbMultitapRedefineCaps (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr);
    PUBLIC int32 QwertSmartRedefineCaps (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr);
	PUBLIC BOOLEAN QwertMultiTapSwitchVkb (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC int32 QwertMultitapRedefineCaps (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr);
    PUBLIC int32 MultiTapCharKeyInit(KEY_INFO_T *key_ptr, const wchar *const text_ptr);
    PUBLIC int32 QwertMultitapConfigPeriod (KEY_INFO_T *key_ptr, MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr);
#ifdef __cplusplus
}
#endif



#endif //defined (MMI_IM_PDA_SUPPORT) 

#endif /*_MMIIM_TOUCH_IME_COMMON_H_*/



