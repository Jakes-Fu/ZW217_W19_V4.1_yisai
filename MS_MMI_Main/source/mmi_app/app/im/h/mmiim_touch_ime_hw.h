/*==============================================================================
File Name: mmiim_touch_ime_hw.h
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

#ifndef _MMIIM_TOUCH_IME_HW_H_
#define _MMIIM_TOUCH_IME_HW_H_

#if defined (MMI_IM_PDA_SUPPORT) 

#if ((defined IM_HANDWRITING_HANWANG) || (defined IM_HANDWRITING_SOGOU))        //开启手写
/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#if defined IM_ENGINE_CSTAR
#include "mmiim_touch_ime_cstar.h"
#endif

#if defined IM_ENGINE_SOGOU
#include "mmiim_touch_ime_sogou.h"
#endif


#if defined(IM_HANDWRITING_HANWANG)  && defined(IM_ENGINE_CSTAR)
#include "hwapi.h"
#include "HWAPI_ml.h"
#include "HWLANG.h"

//联想字
#include "kmx_all_ldb.h"
#include "kmxime.h"
#endif

#if (defined(IM_HANDWRITING_SOGOU)) && defined(IM_ENGINE_SOGOU)
#include "WD_def.h"
#include "iHCR.h"
#include "WD_HwDef.h"
#include "mmiim_sogou.h"

//中文头文件
#if defined IM_SIMP_CHINESE_SUPPORT
#if defined(WIN32) || defined(CHIP_ENDIAN_LITTLE)
#if defined LOW_MEMORY_SUPPORT
#include "GB2312_LE.h"      //使用较小的识别字符集
#else
#include "GBK_AG_LE.h"
#endif
#else       //big endian begin
#if defined LOW_MEMORY_SUPPORT
#include "GB2312_BI.h"
#else
#include "GBK_AG_BI.h"
#endif
#endif
#endif

#if defined IM_TRAD_CHINESE_SUPPORT
#if defined(WIN32) || defined(CHIP_ENDIAN_LITTLE)
#include "BIG5_AG_LE.h"
#else
#include "BIG5_AG_BI.h"
#endif
#endif

#endif

#if defined IM_HANDWRITING_HANWANG
#define HW_MAXCANDNUM (MAXCANDNUM)
#define HW_MAXWORDLENGTH (KMX_MAX_LATIN_WORD_LENGTH)        //候选字词最大长度
#elif defined IM_HANDWRITING_SOGOU
#define HW_MAXCANDNUM (SOGOU_HW_MAXCANDNUM)
#define HW_MAXWORDLENGTH (MAX_WORD_LEN)
#define SOGOU_STROKEENDMARK (-1)
#else
#define HW_MAXWORDLENGTH (4)
#define HW_MAXCANDNUM (10)
#endif

typedef struct
{
    short   x;
    short   y;
} TRACE_POINT_T;

#if defined IM_HANDWRITING_HANWANG
typedef struct HW_HANWANG_DATA_TAG
{
    THWAttribute attr;
    uint16 engine_lang;    //
}HW_HANWANG_DATA_T;
#endif

typedef struct KERNEL_DATA_HW_TAG
{
    TRACE_POINT_T trace[HW_TRACE_CNT_LIMIT];
    uint32 trace_len;
    void *engine_data;      //输入法引擎相关数据
    uint8 recognize_timer_id;
    uint32 recognize_tick;
    uint32 trace_color;
    uint16 trace_width;
    BOOLEAN is_tp_ring_close;           /*!< 是否把按键音给关了 */
    wchar cand[CH_CANDIDATE_COUNT_E][HW_MAXWORDLENGTH];
    uint8 cand_cnt;
    uint8 cur_page;
    uint8 has_next_page;
    EDIT_STATE_T edit_state;
    wchar last_char;
} KERNEL_DATA_HW_T;


/* handwriting */
PUBLIC BOOLEAN HwInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HwTerm(IM_DATA_T *data_ptr);
PUBLIC void HwDraw(IM_DATA_T * data_ptr);
PUBLIC void HwInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HwWriteDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN HwWriteMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN HwWriteUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN HwCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN HwCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN HwSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN HwCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void HwRecognizeCB(uint8 timer_id, uint32 param);
PUBLIC void HwSetCand(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HwAddTrace (IM_DATA_T *data_ptr, int x, int y);
PUBLIC void HwClear(IM_DATA_T *data_ptr);
PUBLIC void HwAsso(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HwCommitDefault(IM_DATA_T *data_ptr);

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
PUBLIC BOOLEAN HwFullScreenGetDisplayPanelRect (IM_DATA_T *data_ptr, GUI_RECT_T *prect);
PUBLIC BOOLEAN HWFullScreenHandleTpMsg (IM_DATA_T *data_ptr, GUIIM_EVENT_DATA_U const *event_data_ptr);
PUBLIC BOOLEAN HwFullScreenInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HwFullScreenTerm(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HWFullScreenPreProc_HandleTpMsg (
    IM_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    BOOLEAN *is_pre_proc
);
#endif


PUBLIC BOOLEAN HwInitEngine (IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HwTermEngine (IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HwAddStrokeMark (IM_DATA_T *data_ptr);
PUBLIC BOOLEAN HwAddCharMark (IM_DATA_T *data_ptr);
PUBLIC int32 HwEngineRecognize (IM_DATA_T *data_ptr, wchar *result_ptr);


#if defined IM_HANDWRITING_SOGOU
PUBLIC int32 Sogou_CandToImeResult(WD_imeResult *pime_result, wchar *pcand);
PUBLIC BOOLEAN SOGOU_HwInitialize(GUIIM_LANGUAGE_T im_lang);
#endif

#endif      //#if ((defined IM_HANDWRITING_HANWANG) || (defined IM_HANDWRITING_SOGOU))        //开启手写
#endif //defined (MMI_IM_PDA_SUPPORT) 

#endif /*_MMIIM_TOUCH_IME_HW_H_*/
