/*==============================================================================
File Name: mmiim_sp_hw.c
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
#if (!defined MMI_IM_PDA_SUPPORT)
#include "mmi_app_im_trc.h"
#include "mmiim_sp_hw.h"

#if defined(MMIIM_USE_HANDWRITING)


#include "mmk_msg.h"
#include "mmi_custom_define.h"
#include "mmidisplay_data.h"
#include "guifont.h"

#include "mmi_theme.h"
#include "mmiim_internal.h"
#include "mmiim_sp_symbol.h"
#include "mmiim.h"
#include "mmiim_tp_cstar_position.h"
#include "guires.h"
#include "mmiim_im_switching.h"
#include "spml_api.h"

#include "mmk_app.h"
#if !defined (MMI_IM_PDA_SUPPORT)
#include "guiform.h"
#endif
#if defined(MMIIM_CORE_USE_SPIM)
#include "spim_ch_asso.h"
#elif defined(MMIIM_CORE_USE_CSTAR)     //--cstar begin
#include "csk2api.h"
#include "mmiim_im_switching.h"
#elif defined(MMIIM_CORE_USE_SOGOU) //--sogou begin
#include "WD_Def.h"//6_12_2
#include "iHCR.h"
#include "WD_HwDef.h"
#include "mmiim_sogou.h"

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

extern uint8 WD_ImeInit(uint8 allowed_input, GUIIM_LANGUAGE_T language_type);       
#else       //--others
#error No associate function!
#endif

#define MMIIM_SP_HW_LESS_POINT      4       //小于多少个点，则不响应手写

extern MMIIM_SP_TEXT_BAR_THEME_T g_text_bar_theme;

/*==============================================================================
Function Name: Initialize
Description: Initialize
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr);

/*==============================================================================
Function Name: InitData
Description: InitData
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void InitData(MMIIM_SP_HW_DATA_T *data_ptr);

/*==============================================================================
Function Name: Terminate
Description: Terminate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(void *handle);

/*==============================================================================
Function Name: HandleMsg
Description: HandleMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMsg(void *handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *event_data_ptr);

/*==============================================================================
Function Name: HandleSysMsgNoInput
Description: HandleSysMsgNoInput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*==============================================================================
Function Name: HandleSysMsgInputing
Description: HandleSysMsgInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*==============================================================================
Function Name: HandleSysMsgSelecting
Description: HandleSysMsgSelecting
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgSelecting(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*==============================================================================
Function Name: HandleSysMsgAssociate
Description: HandleSysMsgAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgAssociate(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*==============================================================================
Function Name: TransMsg
Description: TransMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TransMsg(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_MSG_T *msg_ptr
);

/*==============================================================================
Description: IsValidPoint
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN IsValidPoint(GUI_POINT_T *ponit_ptr, GUI_RECT_T full_rect);

/*==============================================================================
Function Name: TpDownNormal
Description: TpDownNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpDownNormal(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: TpMoveNormal
Description: TpMoveNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpMoveNormal(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: TpLongNormal
Description: TpLongNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpLongNormal(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);


/*==============================================================================
Function Name: TpUpVkey
Description: TpUpVkey
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpVkey(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: TpUpSelecting
Description: TpUpSelecting
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpSelecting(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: TpUpAssociate
Description: TpUpAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpAssociate(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
);

/*==============================================================================
Function Name: AddStrokeFlag
Description: AddStrokeFlag
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void AddStrokeFlag(MMIIM_SP_HW_DATA_T *data_ptr);

/*==============================================================================
Function Name: AddTraceFlag
Description: AddTraceFlag
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void AddTraceFlag(MMIIM_SP_HW_DATA_T *data_ptr);

/*==============================================================================
Function Name: AddTrace
Description: AddTrace
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN AddTrace(MMIIM_SP_HW_DATA_T *data_ptr, GUI_POINT_T *tp_point_ptr);

/*==============================================================================
Function Name: RecognizeTrace
Description: RecognizeTrace
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void RecognizeTrace(MMIIM_SP_HW_DATA_T *data_ptr);

/*==============================================================================
Function Name: MarkTrace
Description: MarkTrace
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void MarkTrace(MMIIM_SP_HW_DATA_T *data_ptr);

/*==============================================================================
Function Name: SetBarCand
Description: SetBarCand
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetBarCand(MMIIM_SP_HW_DATA_T *data_ptr);


/*==============================================================================
Function Name: SetBarAsso
Description: SetBarAsso
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetBarAsso(MMIIM_SP_HW_DATA_T *data_ptr, wchar chr);


/*==============================================================================
Function Name: DrawUI
Description: DrawUI
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawUI(MMIIM_SP_HW_DATA_T *data_ptr);

/*==============================================================================
Function Name: ReDrawUI
Description: ReDrawUI
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ReDrawUI(MMIIM_SP_HW_DATA_T *data_ptr);


/*==============================================================================
Function Name: CommitTransMsg
Description: CommitChar
Author: haiwu.chen
Global resource dependence: NONE
Note: 应用通过GUIIM直传消息报给GUIIM的上一级
==============================================================================*/
LOCAL void CommitTransMsg(
    MMIIM_SP_HW_DATA_T *data_ptr, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param
);

/*==============================================================================
Function Name: CommitChar
Description: CommitChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitChar(MMIIM_SP_HW_DATA_T *data_ptr, wchar c);


/*==============================================================================
Function Name: CommitUnderlineChar
Description: CommitUnderlineChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitUnderlineChar(MMIIM_SP_HW_DATA_T *data_ptr, wchar c);

/*==============================================================================
Function Name: GetHotChar
Description: GetHotChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL wchar GetHotChar(MMIIM_SP_BAR_DATA_T *bar_data_ptr, uint hot_idx);

/*==============================================================================
Function Name: LimitSet
Description: LimitSet
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void LimitSet(MMIIM_SP_HW_DATA_T *data_ptr, GUIIM_LIMIT_T limit);

/*==============================================================================
Function Name: RePaint
Description: RePaint
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN RePaint(MMIIM_SP_HW_DATA_T *data_ptr);

/*==============================================================================
Function Name: ChangeRect
Description: ChangeRect
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN ChangeRect(MMIIM_SP_HW_DATA_T *data_ptr, BOOLEAN is_refresh);

/*==============================================================================
Description: PopSymbPanel
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void PopSymbPanel(MMIIM_SP_HW_DATA_T *data_ptr);

/*****************************************************************************/
//  Description :init attribute info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void InitAttributeInfo(MMIIM_SP_HW_DATA_T *data_ptr);

/*==============================================================================
Description: SetToNoInput
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetToNoInput(MMIIM_SP_HW_DATA_T *data_ptr);


/*==============================================================================
Description: GetCurPageCnt
Global resource dependence: NONE
Author: Haiwu.Chen
Note: get the total char count in current page
==============================================================================*/
LOCAL uint8 GetCurPageCnt(MMIIM_SP_BAR_DATA_T *bar_data_ptr);


/*==============================================================================
Description: IsCanMoveLeft
Global resource dependence: NONE
Author: Haiwu.Chen
Note: it can move left
==============================================================================*/
LOCAL BOOLEAN IsCanMoveLeft(MMIIM_SP_BAR_DATA_T *bar_data_ptr, uint8 hot_index);


/*==============================================================================
Description: IsCanMoveRight
Global resource dependence: NONE
Author: Haiwu.Chen
Note: it can move right
==============================================================================*/
LOCAL BOOLEAN IsCanMoveRight(MMIIM_SP_BAR_DATA_T *bar_data_ptr, uint8 hot_index);


GUIIM_KERNEL_APIS_T const g_guiim_sp_hw_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};


/*==============================================================================
Function Name: Initialize
Description: Initialize
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr)
{
    MMIIM_SP_HW_DATA_T *data_ptr = PNULL;

    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    if (PNULL == param_ptr)
    {
        return data_ptr;
    }
    
    //SCI_TRACE_LOW:"[MMIIM] Initialize hw: win_id=%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_676_112_2_18_2_25_30_58,(uint8*)"d", param_ptr->win_id);

#if !defined (MMI_IM_PDA_SUPPORT)
    GUIFORM_IsSlide(MMK_GetRootFormCtrlHandle(param_ptr->win_id), FALSE);
#endif

    data_ptr = (MMIIM_SP_HW_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SP_HW_DATA_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_SP_HW_DATA_T));

    data_ptr->ctrl_handle = param_ptr->im_handle;
    data_ptr->init_param = *param_ptr;

    data_ptr->state = MMIIM_SP_STATE_NOINPUT;

    InitData(data_ptr);

    //if tp press ring is on, then close and record.
    data_ptr->is_tp_ring_close = FALSE;
    if(MMIDEFAULT_IsEnableTpRing())
    {
        MMIDEFAULT_EnableTpRing("SP HW", FALSE);
        data_ptr->is_tp_ring_close = TRUE;
    }

#if defined(MMIIM_CORE_USE_SOGOU)
    SOGOU_HwInitialize(MMIAPIIM_GetLanguage());
#endif
    return data_ptr;
}

/*==============================================================================
Function Name: InitData
Description: InitData
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void InitData(MMIIM_SP_HW_DATA_T *data_ptr)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint16 img_width = 0;
    uint16 img_height = 0;
    uint16 bar_img_width = 0;
    uint16 bar_img_height = 0;
    GUI_RECT_T win_rect = {0};
    uint8 width_char = GUI_GetFontWidth(MMIIM_SP_TEXT_BAR_FONT_TEXT, UNICODE_HANZI);
    uint8 width_spc = MMIIM_SP_TEXT_BAR_SPACE;
    GUI_POINT_T offset = {0};
    uint16 scroll_bar_width = MMITHEME_GetScrollBarWidth();
    uint16 HW_TEXT_BAR_CAND_WIDTH = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return ;
    }
    
	win_rect = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle);

    //set offset
    offset.x = win_rect.left;
    offset.y = win_rect.bottom + 1;
    //创建新的键盘
    MMIIM_SP_TP_KbInit(&(data_ptr->kb), MMIIM_SP_TP_KB_TYPE_HW, &offset);

    data_ptr->kb.win_id = data_ptr->init_param.win_id;
    data_ptr->kb.lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;

    HW_TEXT_BAR_CAND_WIDTH = MMITHEME_IsMainScreenLandscape() ? 
        (MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H - MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H) : (MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V - MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V);

    data_ptr->asso.per_page = HW_TEXT_BAR_CAND_WIDTH / (width_char + width_spc);
    data_ptr->cand.per_page = data_ptr->asso.per_page;

    LimitSet(data_ptr, data_ptr->init_param.method_param_ptr->limit);

    //alter g_text_bar_theme by screen landcape
    if (MMITHEME_IsMainScreenLandscape())
    {
        GUIRES_GetImgWidthHeight(&img_width, &img_height, 
            g_text_bar_theme.img_idc_left[MMIIM_SP_IDC_ACTIVE], data_ptr->init_param.win_id);
        g_text_bar_theme.rect_idc_left.left = MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H;
        g_text_bar_theme.rect_idc_left.right = g_text_bar_theme.rect_idc_left.left + img_width;
        g_text_bar_theme.rect_idc_left.top = MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_H;
        g_text_bar_theme.rect_idc_left.bottom = g_text_bar_theme.rect_idc_left.top + img_height;
        
        g_text_bar_theme.rect_idc_right.left = MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_H;
        g_text_bar_theme.rect_idc_right.right = g_text_bar_theme.rect_idc_right.left + img_width;
        g_text_bar_theme.rect_idc_right.top = MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_H;
        g_text_bar_theme.rect_idc_right.bottom = g_text_bar_theme.rect_idc_right.top + img_height;        
    }
    else
    {
        GUIRES_GetImgWidthHeight(&img_width, &img_height, 
            g_text_bar_theme.img_idc_left[MMIIM_SP_IDC_ACTIVE], data_ptr->init_param.win_id);
        g_text_bar_theme.rect_idc_left.left = MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V;
        g_text_bar_theme.rect_idc_left.right = g_text_bar_theme.rect_idc_left.left + img_width;
        g_text_bar_theme.rect_idc_left.top = MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_V;
        g_text_bar_theme.rect_idc_left.bottom = g_text_bar_theme.rect_idc_left.top + img_height;
        
        g_text_bar_theme.rect_idc_right.left = MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_V;
        g_text_bar_theme.rect_idc_right.right = g_text_bar_theme.rect_idc_right.left + img_width;
        g_text_bar_theme.rect_idc_right.top = MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_V;
        g_text_bar_theme.rect_idc_right.bottom = g_text_bar_theme.rect_idc_right.top + img_height;
    }
    
    //bar
    //cand
    GUIRES_GetImgWidthHeight(&bar_img_width, &bar_img_height,
        g_text_bar_theme.img_bg, data_ptr->init_param.win_id);
    
    MMIIM_SP_TextBarInit(&(data_ptr->bar_cand));
    data_ptr->bar_cand.win_id = data_ptr->init_param.win_id;
    data_ptr->bar_cand.lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;

    data_ptr->bar_cand.width_label = 0;
    data_ptr->bar_cand.width_space = MMIIM_SP_TEXT_BAR_SPACE;
    data_ptr->bar_cand.has_hot = 1;

    data_ptr->bar_cand.rect = win_rect;

    if (data_ptr->is_asso)
    {
        //asso
        MMIIM_SP_TextBarInit(&(data_ptr->bar_asso));
        data_ptr->bar_asso.win_id = data_ptr->init_param.win_id;
        data_ptr->bar_asso.lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;

        data_ptr->bar_asso.width_label = 0;
        data_ptr->bar_asso.width_space = MMIIM_SP_TEXT_BAR_SPACE;
        data_ptr->bar_asso.has_hot = 1;

        data_ptr->bar_asso.rect = win_rect;
        data_ptr->bar_asso.rect.bottom = data_ptr->kb.rect.top - 1;
        data_ptr->bar_asso.rect.top = data_ptr->bar_asso.rect.bottom - bar_img_height + 1;

        //cand
        data_ptr->bar_cand.rect.bottom = data_ptr->bar_asso.rect.top - 1;
        data_ptr->bar_cand.rect.top = data_ptr->bar_cand.rect.bottom - bar_img_height + 1;

    }
    else
    {
        //cand
        data_ptr->bar_cand.rect.bottom = data_ptr->kb.rect.top - 1;
        data_ptr->bar_cand.rect.top = data_ptr->bar_cand.rect.bottom - bar_img_height + 1;
    }

    //rect
    GUILCD_GetLogicWidthHeight(data_ptr->init_param.lcd_info_ptr->lcd_id, &lcd_width, &lcd_height);
    data_ptr->rect_full.right = lcd_width - 1;
    data_ptr->rect_full.bottom = lcd_height - 1;

    data_ptr->rect_write = win_rect;

    //rewrite write rect [0, calculate value]
    //data_ptr->rect_write.top = 0;
    data_ptr->rect_write.bottom = data_ptr->kb.rect.top - 1;

    //修正rect_write, 留出滚动条的位置
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        data_ptr->rect_write.left += scroll_bar_width;
    }
    else
    {
        data_ptr->rect_write.right -= scroll_bar_width;
    }

    data_ptr->rect_draw = data_ptr->kb.rect;

    //custom variable
    switch (MMIIM_GetHwSpeed())
    {
    case MMIIM_HW_SPEED_FAST:
        data_ptr->recognize_tick = MMIIM_EXPL_TIMER_OUT_SHORT;
        break;

    case MMIIM_HW_SPEED_NORMAL:
        data_ptr->recognize_tick = MMIIM_EXPL_TIMER_OUT_NORMAL;
        break;

    case MMIIM_HW_SPEED_SLOW:
        data_ptr->recognize_tick = MMIIM_EXPL_TIMER_OUT_LONG;
        break;

    default:
        data_ptr->recognize_tick = MMIIM_EXPL_TIMER_OUT_DEFAULT;
        break;

    }

    data_ptr->trace_color = MMIIM_GetHwColor();

    switch (MMIIM_GetHwThickness())
    {
    case MMIIM_HW_THICKNESS_THICK:
        data_ptr->trace_width = MMIIM_SP_HW_DEFAULT_TRACE_WIDTH + 2;
        break;

    case MMIIM_HW_THICKNESS_NORMAL:
        data_ptr->trace_width = MMIIM_SP_HW_DEFAULT_TRACE_WIDTH;
        break;

    case MMIIM_HW_THICKNESS_THIN:
        data_ptr->trace_width = 1;
        break;

    default:
        data_ptr->trace_width = MMIIM_SP_HW_DEFAULT_TRACE_WIDTH;
        break;

    }

    return;
}


/*==============================================================================
Function Name: Terminate
Description: Terminate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(void *handle)
{
    MMIIM_SP_HW_DATA_T *data_ptr = (MMIIM_SP_HW_DATA_T*)handle;

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] Terminate hw: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_906_112_2_18_2_25_31_59,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMIIM] Terminate hw: win_id=%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_910_112_2_18_2_25_31_60,(uint8*)"d", data_ptr->init_param.win_id);

    //if tp ring press is set
    if(data_ptr->is_tp_ring_close)
    {
        MMIDEFAULT_EnableTpRing("SP HW", TRUE);
        data_ptr->is_tp_ring_close = FALSE;
    }

    MMIIM_SP_TP_KbTerm(&(data_ptr->kb));

#if defined(MMIIM_CORE_USE_SOGOU)
#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)
    iHCR_Exit();
#endif
#endif

#if defined(MMIIM_CORE_USE_SOGOU)   //可能使用cstar物理输入和sogou手写组合输入
    SG_Quit();
#endif

#if !defined (MMI_IM_PDA_SUPPORT)
    GUIFORM_IsSlide(MMK_GetRootFormCtrlHandle(data_ptr->init_param.win_id), TRUE);
#endif

    SCI_FREE(data_ptr->attr.pRam);

    SCI_FREE(data_ptr);

    return TRUE;
}


/*==============================================================================
Function Name: HandleMsg
Description: HandleMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMsg(void *handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *event_data_ptr)
{
    MMIIM_SP_HW_DATA_T *data_ptr = (MMIIM_SP_HW_DATA_T*)handle;
    BOOLEAN is_dealt = FALSE;
    BOOLEAN result = FALSE;

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleMsg: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_957_112_2_18_2_25_31_61,(uint8*)"");
        return FALSE;
    }

    switch (event)
    {
    case GUIIM_EVENT_SYS_MSG:

        switch (data_ptr->state)
        {
        case MMIIM_SP_STATE_NOINPUT:
            is_dealt = HandleSysMsgNoInput(data_ptr, event_data_ptr);
            break;

        case MMIIM_SP_STATE_INPUTING:
            is_dealt = HandleSysMsgInputing(data_ptr, event_data_ptr);
            break;

        case MMIIM_SP_STATE_SELECTING:
            is_dealt = HandleSysMsgSelecting(data_ptr, event_data_ptr);
            break;

        case MMIIM_SP_STATE_ASSOCIATE:
            is_dealt = HandleSysMsgAssociate(data_ptr, event_data_ptr);
            break;

        default:
            break;
        }

        if (!is_dealt)
        {
            break;
        }

        ReDrawUI(data_ptr);

        result = is_dealt;

        break;

    case GUIIM_EVENT_SET_METHOD:
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_METHOD:
        event_data_ptr->get_method = GUIIM_M_HANDWRITING;
        result = TRUE;
        break;

    case GUIIM_EVENT_PAINT:
        DrawUI(data_ptr);
        result = TRUE;
        break;

    case GUIIM_EVENT_SLEEP:
    case GUIIM_EVENT_FORCE_COMMIT:
        
        if (SetToNoInput(data_ptr))
        {
            //...redraw
            ReDrawUI(data_ptr);
        }
        
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_CTRL_RECT:
        event_data_ptr->get_ctrl_rect = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle);
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_RECT:
        event_data_ptr->get_rect = data_ptr->rect_draw;
        result = TRUE;
        break;

    case GUIIM_EVENT_SET_LIMIT:
        LimitSet(data_ptr, event_data_ptr->set_limit);
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_EDIT_STATE:

        if (MMIIM_SP_STATE_INPUTING == data_ptr->state)
        {
            event_data_ptr->edit_state = GUIIM_EDIT_HANDWRITING;
        }
        else if (MMIIM_SP_STATE_SELECTING == data_ptr->state)
        {
            event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
        }
        else if (MMIIM_SP_STATE_ASSOCIATE == data_ptr->state)
        {
            event_data_ptr->edit_state = GUIIM_EDIT_CANDITATE;
        }
        else
        {
            event_data_ptr->edit_state = GUIIM_EDIT_NONE;
        }

        result = TRUE;
        break;

    default:
        break;
    }

    return result;
}


/*==============================================================================
Function Name: HandleSysMsgNoInput
Description: HandleSysMsgNoInput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    GUI_POINT_T tp_point = {0};
    BOOLEAN is_tp_in_rect_full = FALSE;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == event_data_ptr)
    {
        return FALSE;
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_TPDOWN:

        if (MMIIM_SP_MSG_TP_AREA0 == msg.para.tp.area)
        {
            //重置参数
            data_ptr->trace_len = 0;
            AddTrace(data_ptr, &msg.para.tp.point);
            is_dealt = FALSE;
            data_ptr->is_first_rect_reg = TRUE;
        }
        else
        {
            is_dealt = TpDownNormal(data_ptr, &msg.para.tp);
            data_ptr->is_first_rect_reg = FALSE;
        }

        break;

    case MMIIM_SP_MSG_TPMOVE:
        if (data_ptr->is_first_rect_reg)
        {
            data_ptr->is_regonize = TRUE;
            data_ptr->state = MMIIM_SP_STATE_INPUTING;

            tp_point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
            tp_point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

            is_tp_in_rect_full = GUI_PointIsInRect(tp_point, data_ptr->rect_full);

            if (is_tp_in_rect_full)
            {
                MMIIM_StopTimer(&(data_ptr->is_timer_active));
                if (AddTrace(data_ptr, &tp_point))
                {
                    MarkTrace(data_ptr);
                }
            }
            else
            {
                AddStrokeFlag(data_ptr);
                MMIIM_StartTimerByTick(&(data_ptr->is_timer_active), data_ptr->ctrl_handle, data_ptr->recognize_tick);
            }

            is_dealt = TRUE;
        }
        else
        {
            is_dealt = TpMoveNormal(data_ptr, &msg.para.tp);
        }
        break;

    case MMIIM_SP_MSG_TPUP:
        if (data_ptr->is_first_rect_reg
            && !data_ptr->is_regonize)
        {
            //这个消息需要模拟回传给edit
            
            CommitTransMsg(data_ptr, MSG_TP_PRESS_DOWN, event_data_ptr->sys_msg.param);
            CommitTransMsg(data_ptr, MSG_TP_PRESS_UP, event_data_ptr->sys_msg.param);
            
            is_dealt = FALSE;
        }
        else
        {
            is_dealt = TpUpVkey(data_ptr, &msg.para.tp);

            if (MMIIM_SP_TP_KbRelease(&data_ptr->kb))
            {
                data_ptr->is_kb_dirty = TRUE;
                is_dealt = TRUE;
            }
        }
        break;

    case MMIIM_SP_MSG_TPLONG:
        is_dealt = TpLongNormal(data_ptr, &msg.para.tp);
        break;

    default:
        break;
    }


    return is_dealt;
}


/*==============================================================================
Function Name: HandleSysMsgInputing
Description: HandleSysMsgInputing
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    GUI_POINT_T tp_point = {0};
    BOOLEAN is_tp_in_rect_full = FALSE;


    //消息转换
    switch (event_data_ptr->sys_msg.msg_id)
    {
    case MSG_CTL_LOSE_FOCUS:
        SetToNoInput(data_ptr);
        break;
        
    case MSG_TIMER:

        if (MMIIM_EXPL_TIMER_ID == *(uint8*)(event_data_ptr->sys_msg.param)
                || data_ptr->is_timer_active)
        {
            AddTraceFlag(data_ptr);
            RecognizeTrace(data_ptr);

            if (data_ptr->result_len > 0)
            {
                data_ptr->state = MMIIM_SP_STATE_SELECTING;
                data_ptr->need_paint = TRUE;
                //set bar
                SetBarCand(data_ptr);
                SetBarAsso(data_ptr, data_ptr->result[0]);
#ifndef IM_ENGINE_SOGOU
                CommitUnderlineChar(data_ptr, data_ptr->result[0]);
#endif
                data_ptr->is_rect_change = TRUE;
            }
            else
            {
                SetToNoInput(data_ptr);
            }

            is_dealt = TRUE;
        }

        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        tp_point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        tp_point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        AddTrace(data_ptr, &tp_point);

        is_dealt = TRUE;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        tp_point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        tp_point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        is_tp_in_rect_full = GUI_PointIsInRect(tp_point, data_ptr->rect_full);

        if (is_tp_in_rect_full)
        {
            MMIIM_StopTimer(&(data_ptr->is_timer_active));
            if (AddTrace(data_ptr, &tp_point))
            {
                MarkTrace(data_ptr);
            }
        }
        else
        {
            AddStrokeFlag(data_ptr);
            MMIIM_StartTimerByTick(&(data_ptr->is_timer_active), data_ptr->ctrl_handle, data_ptr->recognize_tick);
        }

        is_dealt = TRUE;

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        tp_point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        tp_point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);
        
        if (data_ptr->trace_len == 1)
        {
            MMI_HANDLE_T win_handle = 0;
            MMI_HANDLE_T ctrl_handle = 0;
            MMI_HANDLE_T parent_handle = 0;

            MMK_GetHandleByPos(tp_point, TRUE, &win_handle, &ctrl_handle, data_ptr->ctrl_handle);
            parent_handle = MMK_GetParentCtrlHandle(data_ptr->ctrl_handle);

            //非空、非父控件
            if (0 != ctrl_handle && parent_handle != ctrl_handle)
            {
                SetToNoInput(data_ptr);

                MMK_SetActiveCtrl(ctrl_handle, FALSE);

                break;
            }
        }

        AddStrokeFlag(data_ptr);
        MMIIM_StartTimerByTick(&(data_ptr->is_timer_active), data_ptr->ctrl_handle, data_ptr->recognize_tick);

        is_dealt = TRUE;

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:

        if (KEY_PRESSED == (event_data_ptr->sys_msg.msg_id & MMIIM_SP_KEY_TYPE_MASK)
                || KEY_RELEASED == (event_data_ptr->sys_msg.msg_id & MMIIM_SP_KEY_TYPE_MASK)
                || KEY_LONG_PRESSED == (event_data_ptr->sys_msg.msg_id & MMIIM_SP_KEY_TYPE_MASK)
                || KEY_LONG_RELEASED == (event_data_ptr->sys_msg.msg_id & MMIIM_SP_KEY_TYPE_MASK)
                || KEY_REPEATED == (event_data_ptr->sys_msg.msg_id & MMIIM_SP_KEY_TYPE_MASK)
           )
        {
            is_dealt = TRUE;
        }

        break;
    }


    return is_dealt;
}


/*==============================================================================
Function Name: HandleSysMsgSelecting
Description: HandleSysMsgSelecting
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgSelecting(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    wchar cand_hot_chr = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == event_data_ptr)
    {
        return FALSE;
    }
    
    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        //commit last char
        //CommitChar(data_ptr, GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index));
        
        //SetToNoInput(data_ptr);
        return FALSE;
    }

    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_TPDOWN:

        if (MMIIM_SP_MSG_TP_AREA0 == msg.para.tp.area)
        {
            CommitChar(data_ptr, GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index));

            data_ptr->state = MMIIM_SP_STATE_INPUTING;

            //重置参数
            data_ptr->trace_len = 0;
            AddTrace(data_ptr, &msg.para.tp.point);
            is_dealt = TRUE;
        }
        else
        {
            is_dealt = TpDownNormal(data_ptr, &msg.para.tp);
        }

        break;

    case MMIIM_SP_MSG_TPMOVE:
        is_dealt = TpMoveNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_TPUP:
        is_dealt = TpUpSelecting(data_ptr, &msg.para.tp);

        if (MMIIM_SP_TP_KbRelease(&data_ptr->kb))
        {
            data_ptr->is_kb_dirty = TRUE;
            is_dealt = TRUE;
        }

        break;

    case MMIIM_SP_MSG_TPLONG:
        is_dealt = TpLongNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_KEY_PRESS:

        switch (msg.para.key)
        {         
        case MMIIM_SP_KEY_OK:
            CommitChar(data_ptr, GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index));

            //asso
            if (data_ptr->is_asso)
            {
                MMIIM_SP_TextBarClear(&(data_ptr->bar_cand));
                data_ptr->state = MMIIM_SP_STATE_ASSOCIATE;
                data_ptr->is_rect_change = TRUE;
            }
            else
            {
                SetToNoInput(data_ptr);
            }

            is_dealt = TRUE;
            
            break;
            
        //we deal with up,down,left,right
        case MMIIM_SP_KEY_UP:
            is_dealt = TRUE;
            if (MMIIM_SP_IDC_ACTIVE == data_ptr->bar_cand.idc_left)
            {
                data_ptr->cand.cur_page -= 1;
                MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->cand);

                //cand
                cand_hot_chr = GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index);
                SetBarAsso(data_ptr, cand_hot_chr);
        
                data_ptr->is_bar_cand_dirty = TRUE;
                data_ptr->is_bar_asso_dirty = TRUE;
            }
            break;
            
        case MMIIM_SP_KEY_DOWN:
            is_dealt = TRUE;
            if (MMIIM_SP_IDC_ACTIVE == data_ptr->bar_cand.idc_right)
            {
                data_ptr->cand.cur_page += 1;
                MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->cand);

                //cand
                cand_hot_chr = GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index);
                SetBarAsso(data_ptr, cand_hot_chr);
        
                data_ptr->is_bar_cand_dirty = TRUE;
                data_ptr->is_bar_asso_dirty = TRUE;
            }
            break;
            
        case MMIIM_SP_KEY_LEFT:
            is_dealt = TRUE;
            if (IsCanMoveLeft(&data_ptr->cand, data_ptr->bar_cand.hot_index))
            {
                data_ptr->bar_cand.hot_index--;

                //cand
                cand_hot_chr = GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index);
                SetBarAsso(data_ptr, cand_hot_chr);
        
                data_ptr->is_bar_cand_dirty = TRUE;
                data_ptr->is_bar_asso_dirty = TRUE;
            }
            break;
            
        case MMIIM_SP_KEY_RIGHT:
            is_dealt = TRUE;
            if (IsCanMoveRight(&data_ptr->cand, data_ptr->bar_cand.hot_index))
            {
                data_ptr->bar_cand.hot_index++;

                //cand
                cand_hot_chr = GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index);
                SetBarAsso(data_ptr, cand_hot_chr);
        
                data_ptr->is_bar_cand_dirty = TRUE;
                data_ptr->is_bar_asso_dirty = TRUE;
            }
            break;
            
        case MMIIM_SP_KEY_CANCEL:
            //先提交未确认字符
#ifndef IM_ENGINE_SOGOU
            CommitChar(data_ptr, GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index));
#endif
            SetToNoInput(data_ptr);
            
            is_dealt = TRUE;
            break;

        default:
            break;
        }
        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        {
            if (MMIIM_SP_KEY_CANCEL == msg.para.key)
            {
#ifndef IM_ENGINE_SOGOU
                //先提交未确认字符
                CommitChar(data_ptr, GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index));
#endif
                SetToNoInput(data_ptr);
                
                is_dealt = TRUE;
            }
        }
        break;

    default:
        break;
    }


    return is_dealt;
}


/*==============================================================================
Function Name: HandleSysMsgAssociate
Description: HandleSysMsgAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsgAssociate(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    wchar asso_hot_chr = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == event_data_ptr)
    {
        return FALSE;
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_TPDOWN:

        if (MMIIM_SP_MSG_TP_AREA0 == msg.para.tp.area)
        {
            data_ptr->state = MMIIM_SP_STATE_INPUTING;

            //重置参数
            data_ptr->trace_len = 0;
            AddTrace(data_ptr, &msg.para.tp.point);
            is_dealt = TRUE;
        }
        else
        {
            is_dealt = TpDownNormal(data_ptr, &msg.para.tp);
        }

        break;

    case MMIIM_SP_MSG_TPMOVE:
        is_dealt = TpMoveNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_TPUP:
        is_dealt = TpUpAssociate(data_ptr, &msg.para.tp);

        if (MMIIM_SP_TP_KbRelease(&data_ptr->kb))
        {
            data_ptr->is_kb_dirty = TRUE;
            is_dealt = TRUE;
        }

        break;

    case MMIIM_SP_MSG_TPLONG:
        is_dealt = TpLongNormal(data_ptr, &msg.para.tp);
        break;

    case MMIIM_SP_MSG_KEY_PRESS:

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_OK:
            //cand
            asso_hot_chr = GetHotChar(&data_ptr->asso, data_ptr->bar_asso.hot_index);
            CommitChar(data_ptr, asso_hot_chr);
            SetBarAsso(data_ptr, asso_hot_chr);
            is_dealt = TRUE;
            break;
            
        //we deal with up,down,left,right
        case MMIIM_SP_KEY_UP:
            is_dealt = TRUE;
            if (MMIIM_SP_IDC_ACTIVE == data_ptr->bar_asso.idc_left)
            {
                data_ptr->asso.cur_page -= 1;
                MMIIM_SP_TextBarUpdate(&data_ptr->bar_asso, &data_ptr->asso);
                data_ptr->is_bar_asso_dirty = TRUE;
            }
            break;
            
        case MMIIM_SP_KEY_DOWN:
            is_dealt = TRUE;
            if (MMIIM_SP_IDC_ACTIVE == data_ptr->bar_asso.idc_right)
            {
                data_ptr->asso.cur_page += 1;
                MMIIM_SP_TextBarUpdate(&data_ptr->bar_asso, &data_ptr->asso);
                data_ptr->is_bar_asso_dirty = TRUE;
            }
            break;
            
        case MMIIM_SP_KEY_LEFT:
            is_dealt = TRUE;
            if (IsCanMoveLeft(&data_ptr->asso, data_ptr->bar_asso.hot_index))
            {
                data_ptr->bar_asso.hot_index--;
                data_ptr->is_bar_asso_dirty = TRUE;
            }
            break;
            
        case MMIIM_SP_KEY_RIGHT:
            is_dealt = TRUE;
            if (IsCanMoveRight(&data_ptr->asso, data_ptr->bar_asso.hot_index))
            {
                data_ptr->bar_asso.hot_index++;
                data_ptr->is_bar_asso_dirty = TRUE;
            }
            break;
            
        case MMIIM_SP_KEY_CANCEL:
            SetToNoInput(data_ptr);
            is_dealt = TRUE;
            break;

        default:
            break;
        }
        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        {
            if (MMIIM_SP_KEY_CANCEL == msg.para.key)
            {    
                SetToNoInput(data_ptr);
                is_dealt = TRUE;
            }
        }
        break;

    default:
        break;
    }


    return is_dealt;
}


/*==============================================================================
Function Name: TransMsg
Description: TransMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TransMsg(
    MMIIM_SP_HW_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_MSG_T *msg_ptr
)
{
    MMIIM_SP_TP_AREA_INFO_T tp_area_info = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == event_data_ptr||PNULL == msg_ptr)
    {
        return;
    }
    
    tp_area_info.kb_ptr = &data_ptr->kb;

    if (MMIIM_SP_STATE_SELECTING == data_ptr->state)
    {
        tp_area_info.bar_ptr[0] = &data_ptr->bar_asso;
        tp_area_info.bar_ptr[1] = &data_ptr->bar_cand;
    }
    else if (MMIIM_SP_STATE_ASSOCIATE == data_ptr->state)
    {
        tp_area_info.bar_ptr[0] = &data_ptr->bar_asso;
    }

    tp_area_info.area_rect_ptr[0] = &data_ptr->rect_write;

    MMIIM_SP_TransMsg(event_data_ptr, &tp_area_info, msg_ptr);

    return;
}


/*==============================================================================
Description: IsValidPoint
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN IsValidPoint(GUI_POINT_T *ponit_ptr, GUI_RECT_T full_rect)
{
    //return (ponit_ptr->x >= 0 || ponit_ptr->y >= 0);
    
    if (ponit_ptr->x < 0 || ponit_ptr->y < 0)
        return FALSE;

    if (ponit_ptr->x > (full_rect.right - full_rect.left) ||
        ponit_ptr->y > (full_rect.bottom - full_rect.top))
        return FALSE;
    
    return TRUE;
}


/*==============================================================================
Function Name: TpDownNormal
Description: TpDownNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpDownNormal(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    BOOLEAN is_dealt = TRUE;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    switch (para_ptr->area)
    {
    case MMIIM_SP_MSG_TP_KB:

        if (para_ptr->idx < 0 || para_ptr->idx >= data_ptr->kb.key_cnt) break;

        //按下
        data_ptr->kb.keys_ptr[para_ptr->idx].state = MMIIM_SP_TP_KEY_STATE_PRESS;

        //设置最后被按下的键，用于移动之后按键的校验
        data_ptr->kb.last_key_idx = para_ptr->idx;

        data_ptr->is_kb_dirty = TRUE;

        break;

    case MMIIM_SP_MSG_TP_BAR0:
        MMIIM_SP_TextBarTPON(&data_ptr->bar_asso,
                             para_ptr->idx,
                             &data_ptr->is_bar_asso_dirty);

        break;

    case MMIIM_SP_MSG_TP_BAR1:
        MMIIM_SP_TextBarTPON(&data_ptr->bar_cand,
                             para_ptr->idx,
                             &data_ptr->is_bar_cand_dirty);

        break;

    default:
        is_dealt = FALSE;
        break;
    }


    return is_dealt;
}



/*==============================================================================
Function Name: TpMoveNormal
Description: TpMoveNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpMoveNormal(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    data_ptr->is_bar_asso_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_asso);
    data_ptr->is_bar_cand_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_cand);

    switch (para_ptr->area)
    {
    case MMIIM_SP_MSG_TP_KB:

        if (-1 == data_ptr->kb.last_key_idx)
        {
            break;
        }

        if (-1 == para_ptr->idx)
        {
            break;
        }

        //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/
        if (para_ptr->idx < 0 || para_ptr->idx >= data_ptr->kb.key_cnt)
        {
            break;
        }
        
        //仍然在同一个键上
        if (para_ptr->idx == data_ptr->kb.last_key_idx)
        {
            break;
        }

        //释放
        data_ptr->kb.keys_ptr[data_ptr->kb.last_key_idx].state = MMIIM_SP_TP_KEY_STATE_ACTIVE;

        //按下
        data_ptr->kb.keys_ptr[para_ptr->idx].state = MMIIM_SP_TP_KEY_STATE_PRESS;

        //设置最后被按下的键，用于移动之后按键的校验
        data_ptr->kb.last_key_idx = para_ptr->idx;

        data_ptr->is_kb_dirty = TRUE;

        break;

    case MMIIM_SP_MSG_TP_BAR0:
        MMIIM_SP_TextBarTPON(&data_ptr->bar_asso,
                             para_ptr->idx,
                             &data_ptr->is_bar_asso_dirty);

        break;

    case MMIIM_SP_MSG_TP_BAR1:
        MMIIM_SP_TextBarTPON(&data_ptr->bar_cand,
                             para_ptr->idx,
                             &data_ptr->is_bar_cand_dirty);

        break;

    default:
        break;
    }

    return TRUE;
}




/*==============================================================================
Function Name: TpLongNormal
Description: TpLongNormal
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 仅处理长按删除键
==============================================================================*/
LOCAL BOOLEAN TpLongNormal(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == para_ptr)
    {
        return TRUE;
    }
    
    if (MMIIM_SP_MSG_TP_KB != para_ptr->area
            || -1 == para_ptr->idx
            || para_ptr->idx != data_ptr->kb.last_key_idx)
    {
        return TRUE;
    }

    //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/
    if (para_ptr->idx < 0 || para_ptr->idx >= data_ptr->kb.key_cnt)
    {
        return TRUE;
    }
    
    if ('\b' != data_ptr->kb.keys_ptr[para_ptr->idx].value)
    {
        return TRUE;
    }

    notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    MMIIM_SP_TP_KbRelease(&data_ptr->kb);
    
    SetToNoInput(data_ptr);

    data_ptr->is_kb_dirty = TRUE;

    return TRUE;
}




/*==============================================================================
Function Name: TpUpVkey
Description: TpUpVkey
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpVkey(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    GUI_BOTH_RECT_T pop_menu_rect = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == para_ptr)
    {
        return TRUE;
    }
    
    if (MMIIM_SP_MSG_TP_KB != para_ptr->area
            || -1 == para_ptr->idx
            || para_ptr->idx != data_ptr->kb.last_key_idx)
    {
        return TRUE;
    }

    //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/
    if (para_ptr->idx < 0 || para_ptr->idx >= data_ptr->kb.key_cnt)
    {
        return TRUE;
    }
    //按键处理
    //...
    // 1 切换输入法
    if (MMIIM_SP_TP_TAG_SHIFT_IM == data_ptr->kb.keys_ptr[para_ptr->idx].value)
    {
        MMIIM_SetTpPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
        MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_BOTTOM_LEFT);
    }
    // 符号
    else if (MMIIM_SP_TP_TAG_SYMBOL == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {
        PopSymbPanel(data_ptr);
    }
    // 数字
    else if (MMIIM_SP_TP_TAG_DIGITAL == data_ptr->kb.keys_ptr[para_ptr->idx].value)
    {
        GUIIM_NOTIFY_PACK_T notify_pack = {0};

        notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
        notify_pack.data.set_method = GUIIM_M_TP_123;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    }
    // 直接提交的按键
    else
    {
        CommitChar(data_ptr, data_ptr->kb.keys_ptr[para_ptr->idx].value);
    }


    return TRUE;
}





/*==============================================================================
Function Name: TpUpSelecting
Description: TpUpSelecting
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpSelecting(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    wchar cand_hot_chr = 0;
    wchar asso_hot_chr = 0;
    GUI_BOTH_RECT_T pop_menu_rect = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == para_ptr)
    {
        return TRUE;
    }
    
    data_ptr->is_bar_asso_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_asso);
    data_ptr->is_bar_cand_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_cand);

    if (-1 == para_ptr->idx)
    {
        return TRUE;
    }

    switch (para_ptr->area)
    {
    case MMIIM_SP_MSG_TP_KB:

        //抬起之前未击中键盘
        if (-1 == data_ptr->kb.last_key_idx)
        {
            break;
        }

        //不在同一个键上
        if (para_ptr->idx != data_ptr->kb.last_key_idx)
        {
            break;
        }

        //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/
        if (para_ptr->idx < 0 || para_ptr->idx >= data_ptr->kb.key_cnt)
        {
            break;
        }
        //按键处理
        //...
        // 1 切换输入法
        if (MMIIM_SP_TP_TAG_SHIFT_IM == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {
            MMIIM_SetTpPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
            MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_BOTTOM_LEFT);
        }
        // 符号
        else if (MMIIM_SP_TP_TAG_SYMBOL == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {            
            PopSymbPanel(data_ptr);
        }
        // 数字
        else if (MMIIM_SP_TP_TAG_DIGITAL == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            //先提交未确认字符
            CommitChar(data_ptr, GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index));

            notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
            notify_pack.data.set_method = GUIIM_M_TP_123;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }
        // 删除
        else if ('\b' == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {
#ifndef IM_ENGINE_SOGOU
            //先提交未确认字符
            CommitChar(data_ptr, GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index));
#endif

            SetToNoInput(data_ptr);
        }
        // 直接提交的按键
        else
        {
            //先提交未确认字符
            CommitChar(data_ptr, GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index));

            CommitChar(data_ptr, data_ptr->kb.keys_ptr[para_ptr->idx].value);
            SetToNoInput(data_ptr);
        }

        break;

    case MMIIM_SP_MSG_TP_BAR0:      //associate

        if (MMIIM_SP_TEXT_BAR_TP_IDC_LEFT == para_ptr->idx)
        {
            data_ptr->asso.cur_page -= 1;
            MMIIM_SP_TextBarUpdate(&data_ptr->bar_asso, &data_ptr->asso);
        }
        else if (MMIIM_SP_TEXT_BAR_TP_IDC_RIGHT == para_ptr->idx)
        {
            data_ptr->asso.cur_page += 1;
            MMIIM_SP_TextBarUpdate(&data_ptr->bar_asso, &data_ptr->asso);
        }
        else if (-1 < para_ptr->idx && para_ptr->idx < data_ptr->bar_asso.seg_cnt)
        {
            //asso
            cand_hot_chr = GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index);
            CommitChar(data_ptr, cand_hot_chr);
            asso_hot_chr = GetHotChar(&data_ptr->asso, data_ptr->bar_asso.hot_index);
            CommitChar(data_ptr, asso_hot_chr);
            //to asso
            MMIIM_SP_TextBarClear(&(data_ptr->bar_cand));
            data_ptr->state = MMIIM_SP_STATE_ASSOCIATE;
            SetBarAsso(data_ptr, asso_hot_chr);
            data_ptr->is_rect_change = TRUE;
        }

        break;

    case MMIIM_SP_MSG_TP_BAR1:      //candidate

        if (MMIIM_SP_TEXT_BAR_TP_IDC_LEFT == para_ptr->idx)
        {
            data_ptr->cand.cur_page -= 1;
            MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->cand);
        }
        else if (MMIIM_SP_TEXT_BAR_TP_IDC_RIGHT == para_ptr->idx)
        {
            data_ptr->cand.cur_page += 1;
            MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->cand);
        }
        else if (-1 < para_ptr->idx && para_ptr->idx < data_ptr->bar_cand.seg_cnt)
        {
            //commit candidate
            cand_hot_chr = GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index);
            CommitChar(data_ptr, cand_hot_chr);
            
            //switch to assotiate
            MMIIM_SP_TextBarClear(&(data_ptr->bar_cand));
            data_ptr->state = MMIIM_SP_STATE_ASSOCIATE;
            SetBarAsso(data_ptr, cand_hot_chr);
            data_ptr->is_rect_change = TRUE;

            if(data_ptr->asso.cnt == 0)     //hasn't any associates
            {
                data_ptr->state = MMIIM_SP_STATE_NOINPUT;
            }

            return TRUE;
        }
        else
        {
            break;
        }

        //cand
        cand_hot_chr = GetHotChar(&data_ptr->cand, data_ptr->bar_cand.hot_index);
        CommitUnderlineChar(data_ptr, cand_hot_chr);
        SetBarAsso(data_ptr, cand_hot_chr);

        break;

    default:
        break;
    }



    return TRUE;
}



/*==============================================================================
Function Name: TpUpAssociate
Description: TpUpAssociate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpUpAssociate(
    MMIIM_SP_HW_DATA_T *data_ptr,
    MMIIM_SP_MSG_PARA_TP_T const *para_ptr
)
{
    wchar asso_hot_chr = 0;
    GUI_BOTH_RECT_T pop_menu_rect = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == para_ptr)
    {
        return TRUE;
    }
    
    data_ptr->is_bar_asso_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_asso);
    data_ptr->is_bar_cand_dirty = MMIIM_SP_TextBarClearIDC(&data_ptr->bar_cand);

    if (-1 == para_ptr->idx)
    {
        return TRUE;
    }

    switch (para_ptr->area)
    {
    case MMIIM_SP_MSG_TP_KB:

        //抬起之前未击中键盘
        if (-1 == data_ptr->kb.last_key_idx)
        {
            break;
        }

        //不在同一个键上
        if (para_ptr->idx != data_ptr->kb.last_key_idx)
        {
            break;
        }

        //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/
        if (para_ptr->idx < 0 || para_ptr->idx >= data_ptr->kb.key_cnt)
        {
            break;
        }
        //按键处理
        //...
        // 1 切换输入法
        if (MMIIM_SP_TP_TAG_SHIFT_IM == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {
            MMIIM_SetTpPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
            MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_BOTTOM_LEFT);
        }
        // 符号
        else if (MMIIM_SP_TP_TAG_SYMBOL == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {
            PopSymbPanel(data_ptr);
        }
        // 数字
        else if (MMIIM_SP_TP_TAG_DIGITAL == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
            notify_pack.data.set_method = GUIIM_M_TP_123;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }
        // 删除
        else if ('\b' == data_ptr->kb.keys_ptr[para_ptr->idx].value)
        {
            SetToNoInput(data_ptr);
        }
        // 直接提交的按键
        else
        {
            CommitChar(data_ptr, data_ptr->kb.keys_ptr[para_ptr->idx].value);
            SetToNoInput(data_ptr);
        }

        break;

    case MMIIM_SP_MSG_TP_BAR0:

        if (MMIIM_SP_TEXT_BAR_TP_IDC_LEFT == para_ptr->idx)
        {
            data_ptr->asso.cur_page -= 1;
            MMIIM_SP_TextBarUpdate(&data_ptr->bar_asso, &data_ptr->asso);
        }
        else if (MMIIM_SP_TEXT_BAR_TP_IDC_RIGHT == para_ptr->idx)
        {
            data_ptr->asso.cur_page += 1;
            MMIIM_SP_TextBarUpdate(&data_ptr->bar_asso, &data_ptr->asso);
        }
        else if (-1 < para_ptr->idx && para_ptr->idx < data_ptr->bar_asso.seg_cnt)
        {
            //asso
            asso_hot_chr = GetHotChar(&data_ptr->asso, data_ptr->bar_asso.hot_index);
            CommitChar(data_ptr, asso_hot_chr);
            SetBarAsso(data_ptr, asso_hot_chr);
        }

        break;

    default:
        break;
    }



    return TRUE;
}


/*==============================================================================
Function Name: AddStrokeFlag
Description: AddStrokeFlag
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void AddStrokeFlag(MMIIM_SP_HW_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return;
    }
    
    if (data_ptr->trace_len < MMIIM_SP_HW_TRACE_LENGTH_MAX - 1)
    {
#if defined(MMIIM_CORE_USE_SOGOU)//use 讯飞手写
		GUIIM_LANGUAGE_T lang = MMIAPIIM_GetLanguage();
		if (lang == GUIIM_LANG_CHS || lang == GUIIM_LANG_CHT
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
            || lang == GUIIM_LANG_ENGLISH
#endif
            )
		{
			data_ptr->trace[data_ptr->trace_len].x = STROKEENDMARK;
			data_ptr->trace[data_ptr->trace_len].y = STROKEENDMARK;
		}
		else
		{
			data_ptr->trace[data_ptr->trace_len].x = STROKEENDMARK;
			data_ptr->trace[data_ptr->trace_len].y = 0;
		}
#else
        data_ptr->trace[data_ptr->trace_len].x = STROKEENDMARK;
        data_ptr->trace[data_ptr->trace_len].y = 0;
#endif

        data_ptr->trace_len += 1;
    }

    return;
}


/*==============================================================================
Function Name: AddTraceFlag
Description: AddTraceFlag
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void AddTraceFlag(MMIIM_SP_HW_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->trace_len < MMIIM_SP_HW_TRACE_LENGTH_MAX); /*assert verified*/
    if (PNULL == data_ptr ||data_ptr->trace_len >= MMIIM_SP_HW_TRACE_LENGTH_MAX)
    {
        return;
    }
    
    data_ptr->trace[data_ptr->trace_len].x = STROKEENDMARK;
    data_ptr->trace[data_ptr->trace_len].y = STROKEENDMARK;

    data_ptr->trace_len += 1;

    return;
}


/*==============================================================================
Function Name: AddTrace
Description: AddTrace
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN AddTrace(MMIIM_SP_HW_DATA_T *data_ptr, GUI_POINT_T *tp_point_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != tp_point_ptr); /*assert verified*/
    if (PNULL == data_ptr ||PNULL == tp_point_ptr)
    {
        return FALSE;
    }
    
    if (!IsValidPoint(tp_point_ptr, data_ptr->rect_full))
    {
        return FALSE;
    }

    if (data_ptr->trace_len <  MMIIM_SP_HW_TRACE_LENGTH_MAX - 2)
    {
        data_ptr->trace[data_ptr->trace_len].x = tp_point_ptr->x;
        data_ptr->trace[data_ptr->trace_len].y = tp_point_ptr->y;
        data_ptr->trace_len += 1;
    }

    return TRUE;
}

/*==============================================================================
Function Name:
Description:
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void RecognizeTrace(MMIIM_SP_HW_DATA_T *data_ptr)
{
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr )
    {
        return;
    }
    
    if (GUIIM_LANG_NONE != data_ptr->init_param.method_param_ptr->input_lang)
    {
        lang = data_ptr->init_param.method_param_ptr->input_lang;
    }
    else
    {
        lang = MMIAPIIM_GetLanguage();
    }

    //由于Initattribute时，如果local被限就return，未给设置语言，所以这里协助设置为English
    if (GUIIM_LIMIT_LOCAL & data_ptr->init_param.method_param_ptr->limit)
    {
        lang = GUIIM_LANG_ENGLISH;
    }

    MMIIM_StopTimer(&(data_ptr->is_timer_active));

#if 0
    {
        int ilogtrace = 0;
        //SCI_TRACE_LOW:"[MMIIM] RecognizeTrace log:"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_2444_112_2_18_2_25_34_62,(uint8*)"");
        //SCI_TRACE_LOW:"("
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_2445_112_2_18_2_25_34_63,(uint8*)"");
        for (ilogtrace = 0; ilogtrace < MMIIM_SP_HW_TRACE_LENGTH_MAX; ilogtrace++)
        {
            //SCI_TRACE_LOW:"%d,%d,"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_2448_112_2_18_2_25_34_64,(uint8*)"dd", data_ptr->trace[ilogtrace].x, data_ptr->trace[ilogtrace].y);

            if (-1 == data_ptr->trace[ilogtrace].x && -1 == data_ptr->trace[ilogtrace].y)
            {
                break;
            }
        }
        //SCI_TRACE_LOW:")"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_2455_112_2_18_2_25_34_65,(uint8*)"");
    }
#endif

    /*
    int HWRecognize( const THWPoint* pTrace, const THWAttribute* pAttr, unsigned short* pResult );
    功能：笔迹识别
    参数说明：
        pTrace：笔迹数据，其格式为(x1,y1),(x2,y2),...,(STROKEENDMARK,0),...,
                (STROKEENDMARK,0),(STROKEENDMARK,STROKEENDMARK)，
                其中(STROKEENDMARK,0)为笔画结束标志，
                (STROKEENDMARK,STROKEENDMARK)为字结束标志。
        pAttr：识别属性。
        pResult：识别结果存放区。大小为 2*MAXCANDNUM。
    返回值：
         -1:参数错误
        = 0:拒识
        > 0:候选字个数
    */
#if defined (MMIIM_CORE_USE_SOGOU)
    switch (lang)
    {
#if defined (IM_SIMP_CHINESE_SUPPORT)
    case GUIIM_LANG_CHS:
        iHCR_SetParam( iHCR_PARAM_LANGUAGE,  iHCR_LANGUAGE_Chinese_GB | iHCR_LANGUAGE_Numerals | iHCR_LANGUAGE_Symbols );
		data_ptr->result_len = iHCR_Recognize((iHCR_POINT_T*)data_ptr->trace, data_ptr->trace_len-1 , data_ptr->result, data_ptr->attr.iCandNum );	  
        break;
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
    case GUIIM_LANG_CHT:
        iHCR_SetParam( iHCR_PARAM_LANGUAGE,  iHCR_LANGUAGE_Chinese_Big5 | iHCR_LANGUAGE_Numerals | iHCR_LANGUAGE_Symbols );
		data_ptr->result_len = iHCR_Recognize((iHCR_POINT_T*)data_ptr->trace, data_ptr->trace_len-1 , data_ptr->result, data_ptr->attr.iCandNum );	  
        break;
#endif

#if defined (IM_ARABIC_SUPPORT)
    case GUIIM_LANG_ARABIC:
        {
            WD_HwSwitchIme(LANGUAGE_HW_ARABIC);
            data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
            data_ptr->result_len = MAXCANDNUM;
        }
        break;
#endif
#if defined (IM_VIETNAMESE_SUPPORT)
    case GUIIM_LANG_VIETNAMESE:
        {
            WD_HwSwitchIme(LANGUAGE_HW_VIETNAMESE);
            data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
            data_ptr->result_len = MAXCANDNUM;
        }
        break;
#endif
#if defined (IM_THAI_SUPPORT)
    case GUIIM_LANG_THAI:
        {
            WD_HwSwitchIme(LANGUAGE_HW_THAI);
            data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
            data_ptr->result_len = MAXCANDNUM;
        }
        break;
#endif
#if defined (IM_FRENCH_SUPPORT)
    case GUIIM_LANG_FRENCH:
		WD_HwSwitchIme(LANGUAGE_HW_FRENCH);
		data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
		data_ptr->result_len = MAXCANDNUM;
		break;
#endif
#if defined (IM_ITALIAN_SUPPORT)
    case GUIIM_LANG_ITALIAN:
		WD_HwSwitchIme(LANGUAGE_HW_ITALIAN);
		data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
		data_ptr->result_len = MAXCANDNUM;
		break;
#endif
#ifdef IM_PERSIAN_SUPPORT
    case GUIIM_LANG_PERSIAN:
        {
            WD_HwSwitchIme(LANGUAGE_HW_PERSIAN);
            data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
            data_ptr->result_len = MAXCANDNUM;
        }
        break;
#endif
#ifdef IM_URDU_SUPPORT
    case GUIIM_LANG_URDU:
        {
            WD_HwSwitchIme(LANGUAGE_HW_URDU);
            data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
            data_ptr->result_len = MAXCANDNUM;
        }
        break;
#endif
#ifdef IM_HEBREW_SUPPORT
    case GUIIM_LANG_HEBREW:
        {
            WD_HwSwitchIme(LANGUAGE_HW_HEBREW);
            data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
            data_ptr->result_len = MAXCANDNUM;
         }
        break;
#endif
    case GUIIM_LANG_ENGLISH:
    default:
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
        iHCR_SetParam( iHCR_PARAM_LANGUAGE,   iHCR_LANGUAGE_English | iHCR_LANGUAGE_Numerals | iHCR_LANGUAGE_Symbols );
        data_ptr->result_len = iHCR_Recognize((iHCR_POINT_T*)data_ptr->trace, data_ptr->trace_len-1 , data_ptr->result, data_ptr->attr.iCandNum );
#else
        WD_HwSwitchIme(LANGUAGE_HW_ENGLISH);
        data_ptr->result_len = WD_HwOnIdentify_Ex((uint16*)data_ptr->trace,(uint16*)data_ptr->result,0);
        data_ptr->result_len = MAXCANDNUM;
#endif
        break;
    }
#else
    switch (lang)
    {
    case GUIIM_LANG_ENGLISH:
#if !defined (IM_SIMP_CHINESE_SUPPORT)
        {
            M_THWAttribute m_attribute = {0};
            m_attribute.wMode = data_ptr->attr.wMode;
            m_attribute.wLanguage = data_ptr->attr.wLanguage;
            m_attribute.dwRange = data_ptr->attr.dwRange;
            m_attribute.iCandidateNum = data_ptr->attr.iCandNum;
            m_attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = MTL_HWRecognize((const THWPoint *)data_ptr->trace, &(m_attribute), data_ptr->result);
        }
#else
    {
        THWAttribute        attribute = {0};        //pclint告警消除，放置在这里
        
        attribute.dwRange = data_ptr->attr.dwRange;
        attribute.iCandidateNum = data_ptr->attr.iCandNum;
        attribute.pRam = data_ptr->attr.pRam;
        data_ptr->result_len = HWRecognize((const THWPoint *)data_ptr->trace, &(attribute), data_ptr->result);
    }
#endif
        break;
        
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
    case GUIIM_LANG_CHS:
    case GUIIM_LANG_CHT:
    {
        THWAttribute        attribute = {0};        //pclint告警消除，放置在这里
        
        attribute.dwRange = data_ptr->attr.dwRange;
        attribute.iCandidateNum = data_ptr->attr.iCandNum;
        attribute.pRam = data_ptr->attr.pRam;
        data_ptr->result_len = HWRecognize((const THWPoint *)data_ptr->trace, &(attribute), data_ptr->result);
        break;
    }
#endif

#if defined (IM_ARABIC_SUPPORT)
    case GUIIM_LANG_ARABIC:
        {
            ARABIC_THWAttribute arb_attribute = {0};
            arb_attribute.dwRange = data_ptr->attr.dwRange;
            arb_attribute.iCandidateNum = data_ptr->attr.iCandNum;
            arb_attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = HWRecognizeArabic((const THWPoint *)data_ptr->trace, &(arb_attribute), data_ptr->result);
        }
        break;
#endif

#if defined (IM_VIETNAMESE_SUPPORT)
    case GUIIM_LANG_VIETNAMESE:
        {
            VIE_THWAttribute    vie_attribute = {0};
            vie_attribute.wMode = data_ptr->attr.wMode;
            vie_attribute.wLanguage = data_ptr->attr.wLanguage;
            vie_attribute.dwRange = data_ptr->attr.dwRange;
            vie_attribute.iCandNum = data_ptr->attr.iCandNum;
            vie_attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = HWRecognizeVietnamese((const THWPoint *)data_ptr->trace, &(vie_attribute), data_ptr->result);
        }
        break;
#endif

#if defined (IM_THAI_SUPPORT)
    case GUIIM_LANG_THAI:
        {
            THAI_THWAttribute   thai_attribute = {0};
            thai_attribute.dwRange = data_ptr->attr.dwRange;
            thai_attribute.iCandidateNum = data_ptr->attr.iCandNum;
            thai_attribute.pRam = data_ptr->attr.pRam;
            thai_attribute.iBoxHeight = 0;
            thai_attribute.iBoxWidth = 0;
            //泰语的库无法与其他库共存,暂时注释掉
            data_ptr->result_len = HWRecognizeThai((const THWPoint *)data_ptr->trace, &(thai_attribute), data_ptr->result);
        }
        break;
#endif

#if defined (IM_PORTUGUESE_SUPPORT) || defined (IM_RUSSIAN_SUPPORT) \
        || defined (IM_GREEK_SUPPORT) || defined (IM_FRENCH_SUPPORT) \
        || defined (IM_HUNGARIAN_SUPPORT) || defined (IM_ITALIAN_SUPPORT) \
        || defined (IM_TURKISH_SUPPORT) || defined (IM_SPANISH_SUPPORT) \
        || defined (IM_GERMAN_SUPPORT) || defined (IM_ROMANIAN_SUPPORT) \
        || defined (IM_SLOVENIAN_SUPPORT) || defined (IM_CZECH_SUPPORT) \
        || defined (IM_MALAY_SUPPORT) || defined (IM_TAGALOG_SUPPORT) \
        || defined (IM_INDONESIAN_SUPPORT) || defined (IM_DUTCH_SUPPORT) \
        || defined (IM_BULGARIAN_SUPPORT) || defined (IM_POLISH_SUPPORT) \
        || defined (IM_DANISH_SUPPORT) || defined (IM_NORWEGIAN_SUPPORT) \
        || defined (IM_SLOVAK_SUPPORT) || defined (IM_SWEDISH_SUPPORT) \
        || defined (IM_CROATIAN_SUPPORT) || defined (IM_LITHUANIAN_SUPPORT) \
        || defined (IM_UKRAINIAN_SUPPORT) || defined (IM_FINNISH_SUPPORT)
    case GUIIM_LANG_FRENCH:
    case GUIIM_LANG_GERMAN:
    case GUIIM_LANG_ITALIAN:
    case GUIIM_LANG_GREEK:
    case GUIIM_LANG_PORTUGUESE:
    case GUIIM_LANG_HUNGARIAN:
    case GUIIM_LANG_RUSSIAN:
    case GUIIM_LANG_SPANISH:
    case GUIIM_LANG_TURKISH:
    case GUIIM_LANG_CZECH:
    case GUIIM_LANG_SLOVENIAN:
    case GUIIM_LANG_ROMANIAN:
    case GUIIM_LANG_MALAY:
    case GUIIM_LANG_INDONESIAN:
    case GUIIM_LANG_TAGALOG:
    case GUIIM_LANG_CROATIAN:
    case GUIIM_LANG_DUTCH:
    case GUIIM_LANG_BULGARIAN:
    case GUIIM_LANG_POLISH:
    case GUIIM_LANG_DANISH:
    case GUIIM_LANG_LITHUANIAN:
    case GUIIM_LANG_NORWEGIAN:
    case GUIIM_LANG_SLOVAK:
    case GUIIM_LANG_SWEDISH:
    case GUIIM_LANG_UKRAINIAN:
    case GUIIM_LANG_FINNISH:
        {
            M_THWAttribute      m_attribute = {0};
            m_attribute.wMode = data_ptr->attr.wMode;
            m_attribute.wLanguage = data_ptr->attr.wLanguage;
            m_attribute.dwRange = data_ptr->attr.dwRange;
            m_attribute.iCandidateNum = data_ptr->attr.iCandNum;
            m_attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = MTL_HWRecognize((const THWPoint *)data_ptr->trace, &(m_attribute), data_ptr->result);
        }
        break;
#endif

#ifdef IM_PERSIAN_SUPPORT
    case GUIIM_LANG_PERSIAN:
        {
            P_THWAttribute p_attribute = {0};
            p_attribute.wMode = data_ptr->attr.wMode;
            p_attribute.wLanguage = data_ptr->attr.wLanguage;
            p_attribute.dwRange = data_ptr->attr.dwRange;
            p_attribute.iCandNum  = data_ptr->attr.iCandNum;
            p_attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = P_HWRE_Recognize((const THWPoint *)data_ptr->trace, &(p_attribute), data_ptr->result);
        }
        break;
#endif

#ifdef IM_HINDI_SUPPORT
    case GUIIM_LANG_HINDI:
        {
            THWAttribute_HINDI p_attribute = {0};
            p_attribute.wMode = data_ptr->attr.wMode;
            p_attribute.wLanguage = data_ptr->attr.wLanguage;
            p_attribute.dwRange = data_ptr->attr.dwRange;
            p_attribute.iCandidateNum  = data_ptr->attr.iCandNum;
            p_attribute.pRam = data_ptr->attr.pRam;

            data_ptr->result_len = HWRecognize_HINDI((const THWPoint *)data_ptr->trace, &(p_attribute), (unsigned char*)data_ptr->result);
        }
        break;
#endif

#ifdef IM_BENGALI_SUPPORT
    case GUIIM_LANG_BENGALI:
        {
            THWAttribute_Bengala p_attribute = {0};
            p_attribute.wMode = data_ptr->attr.wMode;
            p_attribute.wLanguage = data_ptr->attr.wLanguage;
            p_attribute.dwRange = data_ptr->attr.dwRange;
            p_attribute.iCandidateNum  = data_ptr->attr.iCandNum;
            p_attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = HWRecognize_Bengala((const THWPoint *)data_ptr->trace, &(p_attribute), (unsigned char*)data_ptr->result);
        }
        break;
#endif

#ifdef IM_URDU_SUPPORT
    case GUIIM_LANG_URDU:
        {
            P_THWAttribute p_attribute = {0};
            p_attribute.wMode = data_ptr->attr.wMode;
            p_attribute.wLanguage = data_ptr->attr.wLanguage;
            p_attribute.dwRange = data_ptr->attr.dwRange;
            p_attribute.iCandNum  = data_ptr->attr.iCandNum;
            p_attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = P_HWRE_Recognize((const THWPoint *)data_ptr->trace, &(p_attribute), data_ptr->result);
        }
        break;
#endif

#ifdef IM_HEBREW_SUPPORT
    case GUIIM_LANG_HEBREW:
        {
            H_THWAttribute h_attribute = {0};
            h_attribute.dwLanguage = data_ptr->attr.wLanguage;
            h_attribute.dwRange = data_ptr->attr.dwRange;
            h_attribute.lCandidateNum = data_ptr->attr.iCandNum;
            h_attribute.pRam = data_ptr->attr.pRam;
            h_attribute.dwRamSize = HWRERAMSIZE;
            data_ptr->result_len = H_HWRE_Recognize((const THWPoint *)data_ptr->trace, &(h_attribute), data_ptr->result); 
         }
        break;
#endif

    default:
#if !defined (IM_SIMP_CHINESE_SUPPORT)
        {
            M_THWAttribute m_attribute = {0};
            m_attribute.wMode = data_ptr->attr.wMode;
            m_attribute.wLanguage = data_ptr->attr.wLanguage;
            m_attribute.dwRange = data_ptr->attr.dwRange;
            m_attribute.iCandidateNum = data_ptr->attr.iCandNum;
            m_attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = MTL_HWRecognize((const THWPoint *)data_ptr->trace, &(m_attribute), data_ptr->result);
        }
#else
        {
            THWAttribute        attribute = {0};        //pclint告警消除，放置在这里
        
            attribute.dwRange = data_ptr->attr.dwRange;
            attribute.iCandidateNum = data_ptr->attr.iCandNum;
            attribute.pRam = data_ptr->attr.pRam;
            data_ptr->result_len = HWRecognize((const THWPoint *)data_ptr->trace, &(attribute), data_ptr->result);
        }
#endif
        break;
    }
#endif
    //如果汉王给出的字，我们显示不了，则必须过滤掉，
    //为了尽量少调测试字符的显示与否函数，故多费一些空间
    if (0 < data_ptr->result_len)
    {
        int32 i = 0;
        int32 j = 0;
        uint16 disp_count = 0;
        wchar disp_label[MAXCANDNUM] = {0};
        for (i = 0, j = 0; i < data_ptr->result_len; i++)
        {
            if (SPMLAPI_TestChar(data_ptr->result[i]))
            {
                disp_label[j++] = data_ptr->result[i];
                disp_count++;
            }
        }
        if (disp_count != MAXCANDNUM)
        {
            for (i = 0; i < data_ptr->result_len; i++)
            {
                data_ptr->result[i] = disp_label[i];
            }
        }
        data_ptr->result_len = disp_count;
    }    

    return;
}


/*==============================================================================
Function Name: MarkTrace
Description: MarkTrace
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void MarkTrace(MMIIM_SP_HW_DATA_T *data_ptr)
{
    int16 x0 = 0, y0 = 0;
    int16 x1 = 0, y1 = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr )
    {
        return;
    }
    
    if (data_ptr->trace_len < 2)
    {
        return;
    }

    x0 = data_ptr->trace[data_ptr->trace_len - 2].x;
    y0 = data_ptr->trace[data_ptr->trace_len - 2].y;
    x1 = data_ptr->trace[data_ptr->trace_len - 1].x;
    y1 = data_ptr->trace[data_ptr->trace_len - 1].y;

    if (STROKEENDMARK == x0 || STROKEENDMARK == y0
        || STROKEENDMARK == x1 || STROKEENDMARK == y1)
    {
        return;
    }

    LCD_DrawThickLine(data_ptr->init_param.lcd_info_ptr,
                      x0, y0, x1, y1,
                      data_ptr->trace_color,
                      data_ptr->trace_width);

    return;
}


/*==============================================================================
Function Name: SetBarCand
Description: SetBarCand
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetBarCand(MMIIM_SP_HW_DATA_T *data_ptr)
{
    data_ptr->cand.list_ptr = data_ptr->result;
    data_ptr->cand.cnt = data_ptr->result_len;
    data_ptr->cand.cur_page = 0;

    MMIIM_SP_TextBarClear(&(data_ptr->bar_cand));
    MMIIM_SP_TextBarUpdate(&data_ptr->bar_cand, &data_ptr->cand);

    data_ptr->is_bar_cand_dirty = TRUE;

    return;
}



/*==============================================================================
Function Name: SetBarAsso
Description: SetBarAsso
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void SetBarAsso(MMIIM_SP_HW_DATA_T *data_ptr, wchar chr)
{    
#if defined(MMIIM_CORE_USE_SOGOU)
    uint8 asso_cnt = 0;
    wchar unicode[2] = {0};
    wchar ass[MMIIM_SP_ASSO_CNT_MAX<<2] = {0};
    int32 i = 0, k = 0;
#else
    int32 asso_cnt = 0;
#endif
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;

    if (!data_ptr->is_asso)
    {
        data_ptr->asso.cnt = 0;
        return;
    }

    lang = data_ptr->init_param.method_param_ptr->im_set.lang;

    asso_cnt = MMIIM_SP_ASSO_CNT_MAX;
#if defined(MMIIM_CORE_USE_SPIM)
    SPIM_CH_QueryAsso(chr, data_ptr->asso_chr, &asso_cnt);
#elif defined(MMIIM_CORE_USE_CSTAR) && (defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT))
    if (GUIIM_LANG_CHT == lang)
    {
        (void)HanziAsso_QueryAsso(MMIIM_ASSO_CHT_TYPE, chr, data_ptr->asso_chr, (CS_INT*)(&asso_cnt));
    }
    else
    {
        (void)HanziAsso_QueryAsso(MMIIM_ASSO_CHS_TYPE, chr, data_ptr->asso_chr, (CS_INT*)(&asso_cnt));
    }
#elif defined(MMIIM_CORE_USE_SOGOU)
    unicode[0] = chr;
    WD_SwitchIme(WDI_CN_CPINYIN);
    WD_GetAssociationalCandidate(unicode, 2, ass, asso_cnt);        /*lint !e718 !e746 !e628*/
    k=1;
    data_ptr->asso_chr[0] = ass[0];
    for (i=2;i<MMIIM_SP_ASSO_CNT_MAX<<2;i++)
    {
        if (!ass[i])
        {
            break;
        }
        if (ass[i-1]==0x002c)
        {
            data_ptr->asso_chr[k++] = ass[i];
        } 
    }
    asso_cnt = k;
#else
//#error No associate function!
#endif

    MMIIM_SP_TextBarClear(&(data_ptr->bar_asso));

#if defined(MMIIM_CORE_USE_CSTAR) && (defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT))
    if (asso_cnt == 0)      //asso_cnt 为unsigned型，不可能为负，pclint告警消除
    {
        return;
    }
#endif

    data_ptr->asso.list_ptr = data_ptr->asso_chr;
    data_ptr->asso.cnt = asso_cnt;
    data_ptr->asso.cur_page = 0;

    MMIIM_SP_TextBarUpdate(&data_ptr->bar_asso, &data_ptr->asso);

    data_ptr->is_bar_asso_dirty = TRUE;


    return;
}

/*==============================================================================
Function Name: DrawUI
Description: DrawUI
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawUI(MMIIM_SP_HW_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr )
    {
        return;
    }
    
    if (ChangeRect(data_ptr, TRUE))
    {
        return;
    }

    data_ptr->need_paint = FALSE;
    data_ptr->is_kb_dirty = FALSE;
    data_ptr->is_bar_asso_dirty = FALSE;
    data_ptr->is_bar_cand_dirty = FALSE;

    switch (data_ptr->state)
    {
    case MMIIM_SP_STATE_SELECTING:
        MMIIM_SP_TextBarDraw(&(data_ptr->bar_cand));
        //lint -fallthrough
    case MMIIM_SP_STATE_ASSOCIATE:
        MMIIM_SP_TextBarDraw(&(data_ptr->bar_asso));
        //lint -fallthrough
    case MMIIM_SP_STATE_INPUTING:
    case MMIIM_SP_STATE_NOINPUT:
        MMIIM_SP_TP_KbDraw(&(data_ptr->kb));
        break;

    default:
        break;
    }

    return;
}


/*==============================================================================
Function Name: ReDrawUI
Description: ReDrawUI
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ReDrawUI(MMIIM_SP_HW_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr )
    {
        return;
    }
    
    //change rect 的优先级高于 re paint
    if (ChangeRect(data_ptr, FALSE))
    {
        return;
    }

    if (RePaint(data_ptr))
    {
        return;
    }

    switch (data_ptr->state)
    {
    case MMIIM_SP_STATE_SELECTING:

        if (data_ptr->is_bar_cand_dirty)
        {
            MMIIM_SP_TextBarDraw(&(data_ptr->bar_cand));
        }

        //lint -fallthrough
    case MMIIM_SP_STATE_ASSOCIATE:

        if (data_ptr->is_bar_asso_dirty)
        {
            MMIIM_SP_TextBarDraw(&(data_ptr->bar_asso));
        }

        //lint -fallthrough
    case MMIIM_SP_STATE_NOINPUT:

        if (data_ptr->is_kb_dirty)
        {
            MMIIM_SP_TP_KbDraw(&(data_ptr->kb));
        }

        break;

    case MMIIM_SP_STATE_INPUTING:
    default:
        break;
    }

    data_ptr->is_kb_dirty = FALSE;
    data_ptr->is_bar_asso_dirty = FALSE;
    data_ptr->is_bar_cand_dirty = FALSE;

    return;
}


/*==============================================================================
Function Name: CommitTransMsg
Description: CommitChar
Author: haiwu.chen
Global resource dependence: NONE
Note: 应用通过GUIIM直传消息报给GUIIM的上一级
==============================================================================*/
LOCAL void CommitTransMsg(
    MMIIM_SP_HW_DATA_T *data_ptr, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] CommitChar: data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_3099_112_2_18_2_25_35_66,(uint8*)"");
        return;
    }

    notify_pack.notify = GUIIM_NOTIFY_TRANS_MSG;
    notify_pack.data.trans_msg.msg_id = msg_id;
    notify_pack.data.trans_msg.param = param;

    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

    return;
}


/*==============================================================================
Function Name: CommitChar
Description: CommitChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitChar(MMIIM_SP_HW_DATA_T *data_ptr, wchar c)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] CommitChar: data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_3125_112_2_18_2_25_36_67,(uint8*)"");
        return;
    }

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data.commit[0] = c;
    notify_pack.data.commit_data.commit_len = 1;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return;
}


/*==============================================================================
Function Name: CommitUnderlineChar
Description: CommitUnderlineChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitUnderlineChar(MMIIM_SP_HW_DATA_T *data_ptr, wchar c)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] CommitUnderlineChar: data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_3151_112_2_18_2_25_36_68,(uint8*)"");
        return;
    }

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data.underline[0] = c;
    notify_pack.data.commit_data.underline_len = 1;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return;
}



/*==============================================================================
Function Name: GetHotChar
Description: GetHotChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL wchar GetHotChar(MMIIM_SP_BAR_DATA_T *bar_data_ptr, uint hot_idx)
{
    uint8 abs_idx = 0;
    wchar cmt_chr = 0;

    //SCI_ASSERT(PNULL != bar_data_ptr); /*assert verified*/
    if (PNULL == bar_data_ptr )
    {
        return cmt_chr;
    }
    
    abs_idx = bar_data_ptr->cur_page * bar_data_ptr->per_page + hot_idx;
    cmt_chr = bar_data_ptr->list_ptr[abs_idx];

    return cmt_chr;
}


/*==============================================================================
Function Name: LimitSet
Description: LimitSet
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
由输入语言和支持的输入法类型来确定识别的类型
1 当数字输入法的时候，仅识别数字
2 除1以外，总是识别半角符号和英文
3 当输入语言是中文的时候，识别中文字符和符号
4 多国语言部分暂时不在此支持之列
==============================================================================*/
LOCAL void LimitSet(MMIIM_SP_HW_DATA_T *data_ptr, GUIIM_LIMIT_T limit)
{
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] LimitSet: data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_3207_112_2_18_2_25_36_69,(uint8*)"");
        return;
    }

    if (GUIIM_LANG_NONE != data_ptr->init_param.method_param_ptr->input_lang)
    {
        lang = data_ptr->init_param.method_param_ptr->input_lang;
    }
    else
    {
        lang = MMIAPIIM_GetLanguage();
    }

    data_ptr->init_param.method_param_ptr->limit = limit;

    InitAttributeInfo(data_ptr);

    //置联想初值，首先条件是支持中文或者繁体字(以后如果其它语言也支持联想，则要扩展这里)
    //其次：要求 GUIIM_LIMIT_LOCAL，即要求本地输入法不是被限的
    data_ptr->is_asso = (GUIIM_LANG_CHS == lang || GUIIM_LANG_CHT == lang) 
        && (!(GUIIM_LIMIT_LOCAL & limit));

    return;
}



/*==============================================================================
Function Name: RePaint
Description: RePaint
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN RePaint(MMIIM_SP_HW_DATA_T *data_ptr)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr )
    {
        return FALSE;
    }
    
    if (data_ptr->need_paint)
    {
        notify_pack.notify = GUIIM_NOTIFY_PAINT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    }

    return data_ptr->need_paint;
}


/*==============================================================================
Function Name: ChangeRect
Description: ChangeRect
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN ChangeRect(MMIIM_SP_HW_DATA_T *data_ptr, BOOLEAN is_refresh)
{
    BOOLEAN is_rect_change = FALSE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr )
    {
        return FALSE;
    }
    
    is_rect_change = data_ptr->is_rect_change;

    if (data_ptr->is_rect_change)
    {
        data_ptr->is_rect_change = FALSE;
        
        switch (data_ptr->state)
        {
        case MMIIM_SP_STATE_NOINPUT:
            data_ptr->rect_write.bottom = data_ptr->kb.rect.top - 1;
            break;

        case MMIIM_SP_STATE_INPUTING:
            break;

        case MMIIM_SP_STATE_SELECTING:
            data_ptr->rect_write.bottom = data_ptr->bar_cand.rect.top - 1;
            break;

        case MMIIM_SP_STATE_ASSOCIATE:
            data_ptr->rect_write.bottom = data_ptr->bar_asso.rect.top - 1;
            break;

        default:
            break;
        }

        data_ptr->rect_draw.top = data_ptr->rect_write.bottom + 1;

        notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;

        if (is_refresh)
        {
            GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);
        }
        else
        {
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }
    }

    return is_rect_change;
}


/*==============================================================================
Description: PopSymbPanel
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void PopSymbPanel(MMIIM_SP_HW_DATA_T *data_ptr)
{
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] PopSymbPanel: data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_3333_112_2_18_2_25_36_70,(uint8*)"");
        return;
    }

    if (GUIIM_LANG_NONE != data_ptr->init_param.method_param_ptr->input_lang)
    {
        lang = data_ptr->init_param.method_param_ptr->input_lang;
    }
    else
    {
        lang = MMIAPIIM_GetLanguage();
    }

    MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle, lang);

    return;
}


/*****************************************************************************/
//  Description :init attribute info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void InitAttributeInfo(MMIIM_SP_HW_DATA_T *data_ptr)
{
#if !defined(MMIIM_CORE_USE_SOGOU)      //pclint告警消除
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
#endif

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] InitAttributeInfo: data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_3364_112_2_18_2_25_36_71,(uint8*)"");
        return;
    }

    data_ptr->attr.iCandNum = MAXCANDNUM;

    if (PNULL == data_ptr->attr.pRam)
    {
        data_ptr->attr.pRam = (uint8*)SCI_ALLOC_APP(HWRERAMSIZE);
    }

    data_ptr->attr.pFrame = PNULL;
#if !defined(MMIIM_CORE_USE_SOGOU)

    data_ptr->attr.dwRange = ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE | ALC_COMMON_PUNCTUATION | ALC_COMMON_NUMERALS;

    if (GUIIM_LIMIT_ENGLISH & data_ptr->init_param.method_param_ptr->limit)
    {
        data_ptr->attr.dwRange &= ~(ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE | ALC_COMMON_PUNCTUATION); 
    }

    if (GUIIM_LIMIT_DIGITAL & data_ptr->init_param.method_param_ptr->limit)
    {
        data_ptr->attr.dwRange &= ~(ALC_COMMON_NUMERALS); 
    }

    if (GUIIM_LIMIT_LOCAL & data_ptr->init_param.method_param_ptr->limit)
    {
        //这里不能简单地就return，而应该对纯数字进行一次检测
        data_ptr->attr.dwRange = 0;
        data_ptr->attr.wMode = MODE_SINGLECHAR;
        data_ptr->attr.wLanguage = HWLANG_English;

        if (!(GUIIM_LIMIT_ENGLISH & data_ptr->init_param.method_param_ptr->limit))
        {
            data_ptr->attr.dwRange |= ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE | ALC_COMMON_PUNCTUATION;
        }
        
        if (!(GUIIM_LIMIT_DIGITAL & data_ptr->init_param.method_param_ptr->limit))
        {
            data_ptr->attr.dwRange |= ALC_COMMON_NUMERALS;
        }
        
        return;
    }

    if (GUIIM_LANG_NONE != data_ptr->init_param.method_param_ptr->input_lang)
    {
        lang = data_ptr->init_param.method_param_ptr->input_lang;
    }
    else
    {
        lang = MMIAPIIM_GetLanguage();
    }
    
    switch (lang)
    {
    case GUIIM_LANG_NONE:
    case GUIIM_LANG_ENGLISH:
    case GUIIM_LANG_SWAHILI:
        data_ptr->attr.wMode = MODE_SINGLECHAR;
        data_ptr->attr.wLanguage = HWLANG_English;
        data_ptr->attr.dwRange = ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE  | ALC_COMMON_NUMERALS | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS;
        break;
        
    case GUIIM_LANG_CHS:
        data_ptr->attr.dwRange = CHARSET_CNDEFAULT | CHARSET_ENGLISH | ALC_NUMERIC | ALC_PUNC_COMMON_HALF | ALC_PUNC_RARE_HALF 
                    | ALC_SYM_COMMON_HALF | ALC_SYM_RARE_HALF | ALC_PUNC_COMMON_FULL | ALC_PUNC_RARE_FULL | ALC_SYM_COMMON_FULL | ALC_SYM_RARE_FULL;
        break;
        
    case GUIIM_LANG_CHT:
        data_ptr->attr.dwRange = ALC_TC_COMMON | ALC_TC_RARE | CHARSET_ENGLISH | ALC_NUMERIC | ALC_PUNC_COMMON_HALF 
                    | ALC_SYM_COMMON_HALF | ALC_SYM_RARE_HALF | ALC_PUNC_COMMON_FULL | ALC_PUNC_RARE_FULL | ALC_SYM_COMMON_FULL;
        break;
        
    case GUIIM_LANG_ARABIC:
        data_ptr->attr.dwRange = ALC_ARABIC_BASIC | ALC_ARABIC_EXTEND | ALC_ARABIC_NUMERIC | ALC_ARABIC_SYMBOLS;
        break;
        
    case GUIIM_LANG_VIETNAMESE:
        data_ptr->attr.wMode = MODE_SINGLECHAR;
        data_ptr->attr.wLanguage = HWLANG_Vietnamese;
        data_ptr->attr.dwRange = ALC_VIET_LATIN_LOWERCASE | ALC_VIET_LATIN_UPPERCASE | ALC_VIET_COMMON_PUNC | ALC_VIET_COMMON_SYMBOLS | ALC_VIET_NUMERALS;
        break;
        
    case GUIIM_LANG_THAI:
        data_ptr->attr.dwRange  = CHARSET_ENGLISH_THAI | CHARSET_THAILAND | ALC_THAI_NUMERIC | ALC_THAI_PUNCTUATION | ALC_THAI_SYMBOL;
        data_ptr->attr.iBoxWidth = 0;
        data_ptr->attr.iBoxHeight = 0;
        break;

    case GUIIM_LANG_FRENCH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_French;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_GERMAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_German;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_ITALIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Italian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
    case GUIIM_LANG_GREEK:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Greek;
        data_ptr->attr.dwRange = ALC_GREEK | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_PORTUGUESE:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Portuguese;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_HUNGARIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Hungarian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_INDONESIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Indonesian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_HINDI:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Hindi;
        data_ptr->attr.dwRange = ALC_HINDI_CHAR | ALC_HINDI_NUM | ALC_HINDI_PUNC | ALC_HINDI_COM_PUNC | ALC_HINDI_SYM | ALC_HINDI_NUMERIC;
        break;
        
    case GUIIM_LANG_MALAY:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Malay;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_PERSIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Persian;
        data_ptr->attr.dwRange = ALC_PERSIAN_ARABIC_BASIC | ALC_PERSIAN_ARABIC_EXTEND 
            | ALC_PERSIAN_ARABIC_NUMERALS | ALC_PERSIAN_ARABIC_PUNCSYMB 
            | ALC_PERSIAN_PUNCTUATION | ALC_PERSIAN_SYMBOLS;
        break;
        
    case GUIIM_LANG_RUSSIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Russian;
        data_ptr->attr.dwRange = ALC_CYRILLIC | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_SPANISH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Spanish;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_TAGALOG:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Tagalog;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_TURKISH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Turkish;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_URDU:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Urdu;
        data_ptr->attr.dwRange = ALC_PERSIAN_ARABIC_BASIC | ALC_PERSIAN_ARABIC_EXTEND 
            | ALC_PERSIAN_ARABIC_NUMERALS | ALC_PERSIAN_ARABIC_PUNCSYMB 
            | ALC_PERSIAN_PUNCTUATION | ALC_PERSIAN_SYMBOLS;
        break;

    case GUIIM_LANG_CZECH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Czech;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    case GUIIM_LANG_SLOVENIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Slovenian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
    case GUIIM_LANG_ROMANIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Romanian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_HEBREW:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Hebrew;
        data_ptr->attr.dwRange = ALC_HEBREW;
        break;
        
    case GUIIM_LANG_BENGALI:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Bengali;
        data_ptr->attr.dwRange = ALC_BENGALA_CHAR | ALC_BENGALA_NUMERIC | ALC_BENGALA_PUNC | ALC_BENGALA_SYM;
        break;
        
   case GUIIM_LANG_CROATIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Croatian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_DUTCH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Dutch;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_BULGARIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Bulgarian;
        data_ptr->attr.dwRange = ALC_CYRILLIC | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_POLISH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Polish;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_DANISH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Danish;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_SLOVAK:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Slovakian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_LITHUANIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Lithuanian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_NORWEGIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Norwegian;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_SWEDISH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Swedish;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
   case GUIIM_LANG_UKRAINIAN:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Ukrainian;
        data_ptr->attr.dwRange = ALC_CYRILLIC | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;   

    case GUIIM_LANG_FINNISH:
        data_ptr->attr.wMode = MODE_SINGLECHAR; // 单字识别
        data_ptr->attr.wLanguage = HWLANG_Finnish;
        data_ptr->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
        
    default:
        break;
    }
#endif
}


/*==============================================================================
Description: SetToNoInput
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetToNoInput(MMIIM_SP_HW_DATA_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr )
    {
        return FALSE;
    }
    
    if (MMIIM_SP_STATE_NOINPUT != data_ptr->state)
    {
        data_ptr->state = MMIIM_SP_STATE_NOINPUT;
        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        data_ptr->trace_len = 0;
        MMIIM_SP_TextBarClear(&(data_ptr->bar_cand));
        MMIIM_SP_TextBarClear(&(data_ptr->bar_asso));

        data_ptr->is_rect_change = TRUE;

        data_ptr->is_regonize = FALSE;
        data_ptr->is_first_rect_reg = FALSE;
    }

    return data_ptr->is_rect_change;
}


/*==============================================================================
Description: GetCurPageCnt
Global resource dependence: NONE
Author: Haiwu.Chen
Note: get the total char count in current page
==============================================================================*/
LOCAL uint8 GetCurPageCnt(MMIIM_SP_BAR_DATA_T *bar_data_ptr)
{
    uint8 result_len = 0;
    //SCI_ASSERT(PNULL != bar_data_ptr); /*assert verified*/
    if (PNULL == bar_data_ptr )
    {
        return result_len;
    }
    
    result_len = bar_data_ptr->per_page;
    
    //last page
    if (bar_data_ptr->cnt <= bar_data_ptr->per_page * (bar_data_ptr->cur_page + 1))
    {
        result_len = bar_data_ptr->cnt - bar_data_ptr->per_page * bar_data_ptr->cur_page;
    }

    return result_len;
}


/*==============================================================================
Description: IsCanMoveLeft
Global resource dependence: NONE
Author: Haiwu.Chen
Note: it can move left
==============================================================================*/
LOCAL BOOLEAN IsCanMoveLeft(MMIIM_SP_BAR_DATA_T *bar_data_ptr, uint8 hot_index)
{
    uint8 result_len = 0;
    //SCI_ASSERT(PNULL != bar_data_ptr); /*assert verified*/
    if (PNULL == bar_data_ptr )
    {
        return FALSE;
    }
    
    result_len = GetCurPageCnt(bar_data_ptr);

    return hot_index > 0;
}


/*==============================================================================
Description: IsCanMoveRight
Global resource dependence: NONE
Author: Haiwu.Chen
Note: it can move right
==============================================================================*/
LOCAL BOOLEAN IsCanMoveRight(MMIIM_SP_BAR_DATA_T *bar_data_ptr, uint8 hot_index)
{
    uint8 result_len = 0;
    //SCI_ASSERT(PNULL != bar_data_ptr); /*assert verified*/
    if (PNULL == bar_data_ptr )
    {
        return FALSE;
    }
    
    result_len = GetCurPageCnt(bar_data_ptr);

    return hot_index < result_len - 1;
}


#else
GUIIM_KERNEL_APIS_T const g_guiim_sp_hw_apis = {0};
#endif //MMIIM_USE_HANDWRITING

#endif  //MMI_IM_PDA_SUPPORT

/*Edit by script, ignore 3 case. Thu Apr 26 19:00:56 2012*/ //IGNORE9527
