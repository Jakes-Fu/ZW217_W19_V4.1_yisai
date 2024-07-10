/*==============================================================================
File Name: mmiim_sp_hw.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/

#ifndef _MMIIM_SP_HW_H_
#define _MMIIM_SP_HW_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_sp_tp_basic.h"

#if defined(MMIIM_USE_HANDWRITING)
#if defined(IM_HANDWRITING_HANWANG)
#include "hwapi.h"
#include "HWAPI_ml.h"
#include "HWLANG.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if (defined(MMIIM_CORE_USE_SOGOU))
#define MAXCANDNUM              10          // 可输出的最大侯选字个数
#define HWRERAMSIZE             (14*1024)   // 引擎需要的临时运算空间大小
#define STROKEENDMARK           (-1)        // 定义笔画结束标记值

#endif

enum
{
    MMIIM_SP_HW_TRACE_LENGTH_MAX = 512,
    MMIIM_SP_HW_DEFAULT_TRACE_COLOR = 0x001f,
    MMIIM_SP_HW_DEFAULT_TRACE_WIDTH = 3,
    MMIIM_SP_HW_DEFAULT_LABLE_WIDTH = 3,
};

typedef struct tagThw_HWFrame{
    short left;     // 书写框左上角 X 坐标
    short top;      // 书写框左上角 Y 坐标
    short width;    // 书写框宽度
    short height;   // 书写框高度
} T_hw_HWFrame;
typedef struct
{
    unsigned short  wMode;          // 识别模式。分为单字识别和魔格识别。
    unsigned short  wLanguage;      // 识别语言，限英语、越南语。
    unsigned long   dwRange;        // 识别范围
    int             iEnableSlant;   // 是否开启倾斜字识别。（预留）
    int             iCandNum;       // 候选字个数
    int             iBoxWidth;      // 书写框宽度，不需要则设成 <= 0
    int             iBoxHeight;     // 书写框高度，不需要则设成 <= 0
    unsigned char*  pRam;           // 运算空间，要求起始地址4字节对齐
    unsigned char*  pRom;           // 识别字典。如果字典集成，则为NULL。
    unsigned short  *pCharSet;      // 语言扩展字符集
    T_hw_HWFrame*       pFrame;         // 书写框，如果为NULL，则为全屏书写
} MMI_HW_ATTRIBUTE_T;
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

typedef struct tagThw_HWPoint
{
    short x; // 笔迹 X 坐标
    short y; // 笔迹 Y 坐标
} Thw_HWPoint;

//ui data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUIIM_INIT_KERNEL_T init_param;

    //用丰富的状态来判定某消息的不同反应
    MMIIM_SP_STATE_T state;

    GUI_RECT_T rect_full;
    GUI_RECT_T rect_write;
    GUI_RECT_T rect_draw;

    //显示相关的
    MMIIM_SP_TP_KB_INFO_T kb;
    MMIIM_SP_TEXT_BAR_T bar_cand;
    MMIIM_SP_TEXT_BAR_T bar_asso;

    Thw_HWPoint trace[MMIIM_SP_HW_TRACE_LENGTH_MAX];
    uint32 trace_len;
    MMI_HW_ATTRIBUTE_T attr;
    wchar result[MAXCANDNUM];
    int32 result_len;

    BOOLEAN is_asso;
    wchar asso_chr[MMIIM_SP_ASSO_CNT_MAX];

    MMIIM_SP_BAR_DATA_T cand;
    MMIIM_SP_BAR_DATA_T asso;

    BOOLEAN is_timer_active;

    uint32 recognize_tick;
    GUI_COLOR_T trace_color;
    uint16 trace_width;

    //draw flag
    BOOLEAN need_paint;
    BOOLEAN is_rect_change;
    BOOLEAN is_kb_dirty;
    BOOLEAN is_bar_cand_dirty;
    BOOLEAN is_bar_asso_dirty;

    //本应用是否把按键音给关了
    BOOLEAN is_tp_ring_close;

    BOOLEAN is_regonize;
    BOOLEAN is_first_rect_reg;
    
} MMIIM_SP_HW_DATA_T;


#if defined IM_ENGINE_SOGOU
/*==============================================================================
Description: SOGOU_HwInitialize
Global resource dependence: NONE
Author: guopengzhang@sogou
Note: 
==============================================================================*/
PUBLIC BOOLEAN SOGOU_HwInitialize(GUIIM_LANGUAGE_T im_lang);
#endif

#endif //MMIIM_USE_HANDWRITING

#endif //_MMIIM_SP_HW_H_



