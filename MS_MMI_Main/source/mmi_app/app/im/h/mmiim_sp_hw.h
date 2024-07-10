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
#define MAXCANDNUM              10          // �����������ѡ�ָ���
#define HWRERAMSIZE             (14*1024)   // ������Ҫ����ʱ����ռ��С
#define STROKEENDMARK           (-1)        // ����ʻ��������ֵ

#endif

enum
{
    MMIIM_SP_HW_TRACE_LENGTH_MAX = 512,
    MMIIM_SP_HW_DEFAULT_TRACE_COLOR = 0x001f,
    MMIIM_SP_HW_DEFAULT_TRACE_WIDTH = 3,
    MMIIM_SP_HW_DEFAULT_LABLE_WIDTH = 3,
};

typedef struct tagThw_HWFrame{
    short left;     // ��д�����Ͻ� X ����
    short top;      // ��д�����Ͻ� Y ����
    short width;    // ��д����
    short height;   // ��д��߶�
} T_hw_HWFrame;
typedef struct
{
    unsigned short  wMode;          // ʶ��ģʽ����Ϊ����ʶ���ħ��ʶ��
    unsigned short  wLanguage;      // ʶ�����ԣ���Ӣ�Խ���
    unsigned long   dwRange;        // ʶ��Χ
    int             iEnableSlant;   // �Ƿ�����б��ʶ�𡣣�Ԥ����
    int             iCandNum;       // ��ѡ�ָ���
    int             iBoxWidth;      // ��д���ȣ�����Ҫ����� <= 0
    int             iBoxHeight;     // ��д��߶ȣ�����Ҫ����� <= 0
    unsigned char*  pRam;           // ����ռ䣬Ҫ����ʼ��ַ4�ֽڶ���
    unsigned char*  pRom;           // ʶ���ֵ䡣����ֵ伯�ɣ���ΪNULL��
    unsigned short  *pCharSet;      // ������չ�ַ���
    T_hw_HWFrame*       pFrame;         // ��д�����ΪNULL����Ϊȫ����д
} MMI_HW_ATTRIBUTE_T;
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

typedef struct tagThw_HWPoint
{
    short x; // �ʼ� X ����
    short y; // �ʼ� Y ����
} Thw_HWPoint;

//ui data
typedef struct
{
    CAF_HANDLE_T ctrl_handle;
    GUIIM_INIT_KERNEL_T init_param;

    //�÷ḻ��״̬���ж�ĳ��Ϣ�Ĳ�ͬ��Ӧ
    MMIIM_SP_STATE_T state;

    GUI_RECT_T rect_full;
    GUI_RECT_T rect_write;
    GUI_RECT_T rect_draw;

    //��ʾ��ص�
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

    //��Ӧ���Ƿ�Ѱ�����������
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



