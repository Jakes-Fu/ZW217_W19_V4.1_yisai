/*==============================================================================
File Name: mmiim_touch_ime_hw.c
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



#include "mmi_app_im_trc.h"
#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif


/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#if defined (MMI_IM_PDA_SUPPORT) 

#include "mmiim_touch_ime_hw.h"
#include "mmk_tp.h"


#if defined IM_HANDWRITING_SOGOU
#include "WD_Def.h"//6_12_2
extern WD_imeResult g_imeResult;

LOCAL WD_HwData s_hw_data = { 0 };
#define SOGOU_HW_BASE_WIDTH  64
#define SOGOU_HW_BASE_HEIGHT 64
#endif



//forbid use cstar + sogou_hw or sogou + hanwang_hw;
//if use hanwang_hw, may cue that use cstar ime; sogou + sogou_hw
#if (defined IM_HANDWRITING_HANWANG || defined IM_HANDWRITING_SOGOU)

#include "spml_api.h"

#if defined MMIIM_HW_FULLSCREEN_SUPPORT

#define HW_WRITE_BEGIN_INTERAL (72)    //(6*6 + 6 * 6)   //此处以乘方计算，两点之间间距
#define HwFullScreenMoveInteral(x0, y0, x1, y1) ((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1))

LOCAL BOOLEAN HwFullScreenPanelKeyDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenPanelKeyMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenPanelKeyUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenPanelKeyLong (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenWinDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenWinMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenWinUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL void HwFullScreenInitKb(IM_DATA_T *data_ptr);
LOCAL BOOLEAN HwFullScreenCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL BOOLEAN HwFullScreenCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
LOCAL void HwFullScreenRecognizeCB(uint8 timer_id, uint32 param);
LOCAL void HwFullScreenSetCand(IM_DATA_T *data_ptr);
LOCAL void HwFullScreenClear(IM_DATA_T *data_ptr);
LOCAL BOOLEAN HwFullScreenCommitDefault(IM_DATA_T *data_ptr);
LOCAL BOOLEAN HwFullScreenReDrawOmitMoveDots (IM_DATA_T *data_ptr, uint32 omit_count);
#endif


LOCAL BOOLEAN HwDrawStroke (IM_DATA_T *data_ptr, int16 x0, int16 y0, int16 x1, int16 y1);

/*==============================================================================
Description: HwInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 仅仅是汉字手写，不支持英文手写，没有意义
==============================================================================*/
PUBLIC BOOLEAN HwInit(IM_DATA_T *data_ptr)
{
    MMIIM_HW_THICKNESS_E width_fix = MMIIM_GetHwThickness();
    MMIIM_HW_SPEED_E tick_fix = MMIIM_GetHwSpeed();
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
        
    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_HW_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_HW_T));

    HwInitKb(data_ptr);

    CHECK_DATA_EX(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    hw_data_p->recognize_tick = HW_RECOGNIZE_TICK;
    hw_data_p->trace_width = HW_TRACE_WIDTH;
    hw_data_p->trace_color = RGB565TO888(MMIIM_GetHwColor());

    if (width_fix < MMIIM_HW_THICKNESS_MAX)
    {
        hw_data_p->trace_width -= 2 * width_fix;
    }

    switch (tick_fix)
    {
    case MMIIM_HW_SPEED_FAST:
        hw_data_p->recognize_tick = HW_RECOGNIZE_TICK / 2;
        break;
    case MMIIM_HW_SPEED_NORMAL:
        hw_data_p->recognize_tick = HW_RECOGNIZE_TICK - HW_RECOGNIZE_TICK / 4;
        break;
    case MMIIM_HW_SPEED_SLOW:
        hw_data_p->recognize_tick = HW_RECOGNIZE_TICK;
        break;
    default:
        hw_data_p->recognize_tick = HW_RECOGNIZE_TICK - HW_RECOGNIZE_TICK / 4;
        break;
    }

    //if tp press ring is on, then close and record.
    hw_data_p->is_tp_ring_close = FALSE;

    if (MMIDEFAULT_IsEnableTpRing())
    {
        MMIDEFAULT_EnableTpRing("HAND WRITEING", FALSE);
        hw_data_p->is_tp_ring_close = TRUE;
    }

    HwInitEngine (data_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwTerm(IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA(data_ptr);

    HwCommitDefault(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->recognize_timer_id > 0)
    {
        MMK_StopTimer(hw_data_p->recognize_timer_id);
        hw_data_p->recognize_timer_id = 0;
    }

    //if tp ring press is set
    if (hw_data_p->is_tp_ring_close)
    {
        MMIDEFAULT_EnableTpRing("HAND WRITEING", TRUE);
        hw_data_p->is_tp_ring_close = FALSE;
    }

    HwTermEngine (data_ptr);    //依赖于data数据

    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));
    
    return TRUE;
}


/*==============================================================================
Description: HwDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HwDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*==============================================================================
Description: 初始化键盘为区域手写键盘
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void HwInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CHAR_CNT = 3;
    uint8 const CTRL_CNT = 7;
    uint8 const SYS_CNT = 2;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT_E;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    GUI_RECT_T key_rect = {0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = 0;
    vkb_ptr->key_cnt += 1; //cand vessel
    vkb_ptr->key_cnt += 2; //hide and expand
    vkb_ptr->key_cnt += 4; //4 characters, 四个符号，逗号、句号、问号和空格
    vkb_ptr->key_cnt += 4; //4 controllers, 四个控制键，删除、数字符号、设置、回车
    vkb_ptr->key_cnt += 1; //write panel
    vkb_ptr->key_cnt += CAND_CNT; //candidate keys
    vkb_ptr->key_cnt += 2; //expand pgup & pgdn

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //write panel - 最常用的吧
    key_idx = 0;
    key_rect.left = g_thumb_panel_position[2].left;
    key_rect.top = g_thumb_panel_position[2].top;
    key_rect.right = g_thumb_panel_position[16].right;
    key_rect.bottom = g_thumb_panel_position[16].bottom;
    ConvertRect(panel_width, panel_height,
                &key_rect,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_WRITE_PANEL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].f_down = HwWriteDown;
    vkb_ptr->keys[key_idx].f_move = HwWriteMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = HwWriteUp;

    //4 characters
    key_idx++;
    char_begin = key_idx;

    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - char_begin;
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
        if (MMIAPIIM_GetLanguage() == GUIIM_LANG_CHS || MMIAPIIM_GetLanguage() == GUIIM_LANG_CHT)
            vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_cn_symb[key_idx - char_begin];
        else
            vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_symb[key_idx - char_begin];
#else
        vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_symb[key_idx - char_begin];
#endif
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_ALL_TEXT;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = HwCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //--逗号
    key_idx = char_begin;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[9]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    //--句号
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[12]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    //--空格
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[15]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].text_len = 0;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_CENTER;

    //4 controllers
    //expand pgup & pgdn (controllers)
    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key 的位置索引从16开始到21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = HwCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[17]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;
    //--123?!#
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[18]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--enter
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[19]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--set
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[21]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_HW;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh拦截处理
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //被MMIIM_HandleSettingIMUp拦截处理
    //--ime lang set
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[22]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--expand pageup
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[23]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[24]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //hide and expand
    //这两个按键在同一个位置，交替出现, 初始化隐藏expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = HwSysUp;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--expand
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    //cand vessel
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
#if defined SOGOU_USE_PDA_SKIN    
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //不画背景，防止将sogou皮肤给覆盖掉
#endif    
    vkb_ptr->keys[key_idx].is_inactive = 1;

    //candidate...
    //候选键的位置确定，整齐的排成矩阵
    //内容未定
    //candidate key 总是放在最后，因为其可能会覆盖在其他的部分之上，比如art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_thumb_panel_position[0].right - g_thumb_panel_position[0].left - 5;         //右侧有一条竖线
        int16 h = g_thumb_panel_position[0].bottom - g_thumb_panel_position[0].top - 2 * data_ptr->theme.key_ver_space - 5; //上下留些空白,顶部有一条横线
        GUI_RECT_T cand_rect = {0};

        //偏移量直接放在ConvertRect外面计算，因函数内对上下偏移量不是均等的
        cand_rect.top = g_thumb_panel_position[0].top + y * (h + 2 * data_ptr->theme.key_ver_space) + data_ptr->theme.key_ver_space + 5;
        cand_rect.bottom = cand_rect.top + h;
        cand_rect.left = g_thumb_panel_position[0].left + x * w / CH_CANDIDATE_COL + 1;
        cand_rect.right = cand_rect.left + w / CH_CANDIDATE_COL;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, 0);

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = HwCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: HwWriteDown
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwWriteDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }
    
    HwCommitDefault(data_ptr);

    //move点能够立即收到，而不用等一段时间过来，导致前面几个点没有立即显示
    MMK_DelayTpMove( FALSE );       

    data_ptr->vkb.last_key = key_ptr;
    data_ptr->vkb.is_key_block = 1;

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->recognize_timer_id > 0)
    {
        MMK_StopTimer(hw_data_p->recognize_timer_id);
        hw_data_p->recognize_timer_id = 0;
    }

    if (hw_data_p->edit_state != EDIT_STATE_WRITE)
    {
        hw_data_p->edit_state = EDIT_STATE_WRITE;
        
        hw_data_p->trace_len = 0;
        HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);

        //NEWMS00163926 [6530][8800g2][openphone_240x400PDA_rvct_le]手写中点无法手写出
        HwDrawStroke (data_ptr, 
            data_ptr->last_tp.x - data_ptr->rect.left, data_ptr->last_tp.y - data_ptr->rect.top, 
            data_ptr->last_tp.x - data_ptr->rect.left, data_ptr->last_tp.y - data_ptr->rect.top);
    }
    else
    {
        HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);
    }

    return TRUE;
}


/*==============================================================================
Description: HwWriteMove
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwWriteMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }
    if (data_ptr->vkb.first_key != key_ptr)
    {
        data_ptr->vkb.last_key->is_press = 0;
		MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
        data_ptr->vkb.last_key = key_ptr;

        HideTip(data_ptr);
        DrawVkb(data_ptr);
        return TRUE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->recognize_timer_id > 0)
    {
        MMK_StopTimer(hw_data_p->recognize_timer_id);
        hw_data_p->recognize_timer_id = 0;
    }

    if (hw_data_p->edit_state != EDIT_STATE_WRITE)
    {
        hw_data_p->edit_state = EDIT_STATE_WRITE;

        hw_data_p->trace_len = 0;
        HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);
    }
	else
    {
        int16 x0, y0, x1, y1;

        HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);

        x0 = hw_data_p->trace[hw_data_p->trace_len-2].x - data_ptr->rect.left;
        y0 = hw_data_p->trace[hw_data_p->trace_len-2].y - data_ptr->rect.top;
        x1 = hw_data_p->trace[hw_data_p->trace_len-1].x - data_ptr->rect.left;
        y1 = hw_data_p->trace[hw_data_p->trace_len-1].y - data_ptr->rect.top;

        HwDrawStroke (data_ptr, x0, y0, x1, y1);
    }

    return TRUE;
}


/*==============================================================================
Description: HwWriteUp
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwWriteUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }
    if (data_ptr->vkb.first_key != key_ptr)
    {
        return TRUE;
    }

    HwAddStrokeMark (data_ptr);
    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    //when waiting...
    if (hw_data_p->recognize_timer_id > 0)
    {
        return TRUE;
    }

    if (hw_data_p->trace_len > 0)
    {
        hw_data_p->recognize_timer_id = MMK_CreateTimerCallback(
                    hw_data_p->recognize_tick,
                    HwRecognizeCB,
                    (uint32)data_ptr,
                    FALSE);
    }
    else
    {
        data_ptr->vkb.is_key_block = 0;
    }

    return TRUE;
}


/*==============================================================================
Description: HwCharUp
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }
    HwCommitDefault(data_ptr);

    if (VKEY_SPACE == key_ptr->code)
    {
//如果进入中文手写
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
        if (MMIAPIIM_GetLanguage() == GUIIM_LANG_CHS || MMIAPIIM_GetLanguage() == GUIIM_LANG_CHT)
            CommitChar(data_ptr, 0x3000);
        else
            CommitChar(data_ptr, ' ');
#else
        CommitChar(data_ptr, ' ');
#endif
    }
    else
    {
        CommitChar(data_ptr, key_ptr->text[0]);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwCtrlUp
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    EDIT_STATE_T edit_state = EDIT_STATE_NONE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }
    
    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    edit_state = hw_data_p->edit_state;
    HwCommitDefault(data_ptr);

    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (hw_data_p->cand_cnt > 0)
        {
            if (edit_state == EDIT_STATE_COMMIT_DEFAULT)
            {
                CommitChar(data_ptr, '\b');
            }

            HwClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        CommitChar(data_ptr, '\n');
        break;

    case VKEY_123SYMBOL:
        if (MMIAPIIM_GetLanguage() == GUIIM_LANG_CHS || MMIAPIIM_GetLanguage() == GUIIM_LANG_CHT)
        {
            SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        }
        else
        {
            SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        }

        key_ptr = PNULL;
        break;

    case VKEY_PAGEUP:

        if (hw_data_p->edit_state == EDIT_STATE_ASSO)
        {
            //SCI_ASSERT(hw_data_p->cur_page > 0); /*assert verified*/
            if(hw_data_p->cur_page > 0)
            {
                hw_data_p->cur_page--;
            }
            HwAsso(data_ptr);
        }

        break;

    case VKEY_PAGEDOWN:

        if (hw_data_p->edit_state == EDIT_STATE_ASSO)
        {
            //SCI_ASSERT(1 == hw_data_p->has_next_page); /*assert verified*/
            if(1 != hw_data_p->has_next_page) break;
            hw_data_p->cur_page++;
            HwAsso(data_ptr);
        }

        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwSysUp
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);

    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, hw_data_p->cand_cnt);
        HwSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwRecognizeCB
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HwRecognizeCB(uint8 timer_id, uint32 param)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)param;
    wchar result[HW_MAXCANDNUM << 2] = {0};     //sogou输入法需要的buffer要大些
    int32 recognize_res = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;

    if (data_ptr == PNULL || (sizeof(IM_DATA_T) != data_ptr->check_info))
    {
        return;
    }

	hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);

    if(hw_data_p == PNULL ||hw_data_p->recognize_timer_id != timer_id)
    {
        return;
    }
    
    hw_data_p->recognize_timer_id = 0;

    data_ptr->vkb.is_key_block = 0;
    hw_data_p->edit_state = EDIT_STATE_NONE;

    HwAddCharMark (data_ptr);

    SCI_MEMSET (result, 0, sizeof (result));
    recognize_res = HwEngineRecognize(data_ptr, result);

    hw_data_p->cand_cnt = 0;

    //如果给出的字，我们显示不了，则必须过滤掉，将cand放置在cand buffer中
    if (recognize_res > 0)
    {
        int32 i = 0;

        for (i = 0; i < recognize_res; i++)
        {
            if (result[i] && SPMLAPI_TestChar(result[i]))       //函数对参数0返回值依然为True;
            {
                hw_data_p->cand[hw_data_p->cand_cnt++][0] = result[i];
            }
        }
    }

    //set candidate
    if (hw_data_p->cand_cnt > 0)
    {
        GUIIM_NOTIFY_PACK_T notify_pack = {0};

        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        notify_pack.data.commit_data.underline[0] = hw_data_p->cand[0][0];  //第一个字作为underline字符
        notify_pack.data.commit_data.underline_len = 1;

        GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

        hw_data_p->edit_state = EDIT_STATE_COMMIT_DEFAULT;
        hw_data_p->has_next_page = 0;

        SetHideKey(data_ptr, hw_data_p->cand_cnt);
        HwSetCand(data_ptr);
    }

    HwDraw(data_ptr);

    return;
}





/*==============================================================================
Description: HwClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HwClear(IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    data_ptr->vkb.is_key_block = FALSE;

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    hw_data_p->edit_state = EDIT_STATE_NONE;

    hw_data_p->trace_len = 0;
    hw_data_p->cand_cnt = 0;
    hw_data_p->cur_page = 0;
    hw_data_p->has_next_page = 0;

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, hw_data_p->cand_cnt);
    HwSetCand(data_ptr);

    return;
}


/*==============================================================================
Description: HWCommitDefault
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwCommitDefault(IM_DATA_T *data_ptr)
{
    BOOLEAN has_commit = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    has_commit = (EDIT_STATE_COMMIT_DEFAULT == hw_data_p->edit_state);

    if (has_commit)
    {
        CommitChar(data_ptr, hw_data_p->cand[0][0]);
        hw_data_p->edit_state = EDIT_STATE_NONE;
    }

    return has_commit;
}


/*==============================================================================
Description: 在函数内部对参数做一个校验纠正，防止外部纠正遗漏而引起异常
Global resource dependence: 
Author: 
Note:
==============================================================================*/
PUBLIC BOOLEAN HwAddTrace (IM_DATA_T *data_ptr, int x, int y)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA (data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->trace_len > HW_TRACE_CNT_LIMIT - 3)     //stroke mark, char mark保留
    {
        SCI_TRACE_LOW ("[MMIIM] HwAddTrace dots overflow");
        //hw_data_p->trace_len = HW_TRACE_CNT_LIMIT - 3;
        return FALSE;
    }
    
    hw_data_p->trace[hw_data_p->trace_len].x = x;
    hw_data_p->trace[hw_data_p->trace_len].y = y;
    hw_data_p->trace_len ++;
    
    return TRUE;
}


/*==============================================================================
Description: HwCandUp
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN HwCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 select_idx = 0, text_len = 0;
    wchar last_char = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }
    select_idx = key_ptr->code - VKEY_CANDIDATE;

    //SCI_ASSERT(select_idx >= 0); /*assert verified*/
    //SCI_ASSERT(select_idx < hw_data_p->cand_cnt); /*assert verified*/

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if(select_idx >= 0 && select_idx < hw_data_p->cand_cnt)
    {
        text_len = MMIAPICOM_Wstrlen (hw_data_p->cand[select_idx]);
        last_char = hw_data_p->cand[select_idx][text_len - 1];  //sogou可能存在输入多个字符的情况
        CommitStr (data_ptr, hw_data_p->cand[select_idx], text_len);
    }

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }
    
    HwClear(data_ptr);

#if defined IM_SIMP_CHINESE_SUPPORT
    //if(data_ptr->init_param.method_param_ptr->input_lang == GUIIM_LANG_CHS)     //手写时，该语言值为空
    if (MMIAPIIM_GetLanguage() == GUIIM_LANG_CHS)
    {
        hw_data_p->last_char = last_char;
        HwAsso(data_ptr);
    }
#endif

#if defined IM_TRAD_CHINESE_SUPPORT
    if (MMIAPIIM_GetLanguage()  == GUIIM_LANG_CHT)
    {
        hw_data_p->last_char = last_char;
        HwAsso(data_ptr);
    }
#endif

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 将通用的一些程序流程放到函数里，不使用内嵌函数，减少代码量，
                    虽然速度有损
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwDrawStroke (IM_DATA_T *data_ptr, int16 x0, int16 y0, int16 x1, int16 y1)
{
    BOOLEAN is_dev_ok = FALSE;
    GUILCD_DRAW_DC_T dc_data = {0}; 
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    GUI_RECT_T draw_rect = {0};

    CHECK_DATA_EX (data_ptr);
    
    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->ui_layer, &dc_data);
    if (!is_dev_ok)
    {
        SCI_TRACE_LOW ("[MMIIM] HwDrawStroke");
        return FALSE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    _DrawLine(&dc_data, x0, y0, x1, y1,
              hw_data_p->trace_width,
              hw_data_p->trace_color);

    if (x0 < x1)
    {
        draw_rect.left = x0 - 1;
        draw_rect.right = x1 + 1;
    }
    else
    {
        draw_rect.left = x1 - 1;
        draw_rect.right = x0 + 1;
    }

    if (y0 < y1)
    {
        draw_rect.top = y0 - 1;
        draw_rect.bottom = y1 + 1;
    }
    else
    {
        draw_rect.top = y1 - 1;
        draw_rect.bottom = y0 + 1;
    }

    MMITHEME_StoreUpdateRect(&data_ptr->ui_layer, draw_rect);

    return TRUE;
}


#if defined MMIIM_HW_FULLSCREEN_SUPPORT

/*==============================================================================
Description: 获取当前面板显示区域，而不是整个屏幕区域
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN HwFullScreenGetDisplayPanelRect (IM_DATA_T *data_ptr, GUI_RECT_T *prect)
{
    int16 panel_percent = MMIIM_HW_FS_PANEL_PERCENT_NORMAL;
    int16 panel_height = 0;
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRectEx(data_ptr->init_param.win_handle);
    GUI_RECT_T client_rect_ex = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle);

    CHECK_DATA(data_ptr);
    if (prect == PNULL)
    {
        //SCI_TRACE_LOW:"HwFullScreenGetPanelRect"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1012_112_2_18_2_27_55_119,(uint8*)"");
        return FALSE;
    }

    switch (data_ptr->kb_size)
    {
    case MMIIM_KEYBOARD_SIZE_BIG:
        panel_percent = MMIIM_HW_FS_PANEL_PERCENT_BIG;
        break;

    case MMIIM_KEYBOARD_SIZE_NORMAL:
        panel_percent = MMIIM_HW_FS_PANEL_PERCENT_NORMAL;
        break;
    case MMIIM_KEYBOARD_SIZE_SMALL:
        panel_percent = MMIIM_HW_FS_PANEL_PERCENT_SMALL;
        break;

    //case MMIIM_KEYBOARD_SIZE_NORMAL:
    default:
        break;
    }

    panel_height = full_rect.bottom - full_rect.top + 1;        //正常键盘为5行，而全屏手写的区域为3行
    panel_height = panel_height * panel_percent / 100;      //包含按键的面板实际显示区域，除开透明按键部分

    prect->bottom = client_rect_ex.bottom;
    prect->top = client_rect_ex.bottom - panel_height + 1;    //这里要考虑存在softkey的情况                               
    prect->left = client_rect_ex.left;
    prect->right = client_rect_ex.right;

    return TRUE;
}


/*==============================================================================
Description: 面板上的按键down消息处理，包括各种不同的输入状态
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenPanelKeyDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN msg_dealt = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    data_ptr->vkb.last_key = key_ptr;

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    switch (hw_data_p->edit_state)
    {
        case EDIT_STATE_NONE:
        {
            VkbDown(data_ptr, key_ptr);
            msg_dealt =  TRUE;
            break;
        }
        
        case EDIT_STATE_WRITE:
        {
            //写下一笔
            data_ptr->vkb.last_key = key_ptr;

            if (hw_data_p->recognize_timer_id > 0)
            {
                MMK_StopTimer(hw_data_p->recognize_timer_id);
                hw_data_p->recognize_timer_id = 0;
            }

            
            {
                HwAddStrokeMark (data_ptr);
                HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);
            }

            msg_dealt =  TRUE;   
            break;
        }
        
        case EDIT_STATE_COMMIT_DEFAULT:
        {
            VkbDown(data_ptr, key_ptr);
            msg_dealt =  TRUE;            
            break;
        }
        
        case EDIT_STATE_ASSO:
        {
            VkbDown(data_ptr, key_ptr);
            msg_dealt =  TRUE;            
            break;
        }
        
        default:
            break;
    }
    
    return msg_dealt;
}


/*==============================================================================
Description: 面板上的按键Move消息处理，包括各种不同的输入状态
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenPanelKeyMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN msg_dealt = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    switch (hw_data_p->edit_state)
    {
        case EDIT_STATE_NONE:
        case EDIT_STATE_COMMIT_DEFAULT:
        case EDIT_STATE_ASSO:
        {
            uint16 interal = HwFullScreenMoveInteral (data_ptr->first_tp.x, data_ptr->first_tp.y, data_ptr->last_tp.x, data_ptr->last_tp.y) ;

            //判断是否启动手写
            if (interal <= HW_WRITE_BEGIN_INTERAL)   
            {
                if (hw_data_p->trace_len == 0)   //首个down点存入笔迹
                {
                    HwAddTrace (data_ptr, data_ptr->first_tp.x, data_ptr->first_tp.y);
                }
                
                HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);
                msg_dealt = FALSE;
                break;
            }
            
            //上一个字符提交
            if (hw_data_p->edit_state == EDIT_STATE_COMMIT_DEFAULT)
            {
                HwFullScreenCommitDefault (data_ptr);
            }

            if (hw_data_p->trace_len == 0)   //首个down点存入笔迹
            {
                HwAddTrace (data_ptr, data_ptr->first_tp.x, data_ptr->first_tp.y);
            }

            HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);
            
            //高亮按键取消高亮
            HideTip(data_ptr);
            if (data_ptr->vkb.last_key && data_ptr->vkb.last_key->is_press)      //
            {
                data_ptr->vkb.last_key->is_press = 0;
            }
            HwDraw(data_ptr);

            hw_data_p->edit_state = EDIT_STATE_WRITE;     //启动手写
            HwFullScreenReDrawOmitMoveDots (data_ptr, hw_data_p->trace_len - 1);    //忽略点补上来画出

            msg_dealt = TRUE;
            break;
        }
        
        case EDIT_STATE_WRITE:
        {
            if (key_ptr == PNULL)
            {
                //SCI_TRACE_LOW:"HwFullScreenPanelKeyMove EDIT_STATE_WRITE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1211_112_2_18_2_27_55_121,(uint8*)"");
                return FALSE;
            }

            
            {
                int16 x0, y0, x1, y1;

                HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);
                
                x0 = hw_data_p->trace[hw_data_p->trace_len-2].x - data_ptr->rect.left;
                y0 = hw_data_p->trace[hw_data_p->trace_len-2].y - data_ptr->rect.top;
                x1 = hw_data_p->trace[hw_data_p->trace_len-1].x - data_ptr->rect.left;
                y1 = hw_data_p->trace[hw_data_p->trace_len-1].y - data_ptr->rect.top;

                HwDrawStroke (data_ptr, x0, y0, x1, y1);
            }

            msg_dealt = TRUE;
            break;
        }
        
        default:
            break;
    }
    
    return msg_dealt;
}


/*==============================================================================
Description: 面板上的按键Up消息处理，包括各种不同的输入状态
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenPanelKeyUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN msg_dealt = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    switch (hw_data_p->edit_state)
    {
        case EDIT_STATE_NONE:
        case EDIT_STATE_COMMIT_DEFAULT:
        case EDIT_STATE_ASSO:
        {
            switch (key_ptr->style)
            {
                /*!< 一般的字符按键 */
                case KEY_STYLE_CHARACTER:        
                {
                    HwFullScreenCharUp (data_ptr, key_ptr);
                    break;
                }

                /*!< 一般的控制按键 */
                case KEY_STYLE_CONTROL:
                {
                    HwFullScreenCtrlUp (data_ptr, key_ptr);
                    break;
                }

                 /*!< 定制的按键 */
                case KEY_STYLE_CUSTOM:
                {
                    break;
                }

                /*!< 候选按键 */
                case KEY_STYLE_CANDIDATE:
                {
                    HwFullScreenCandUp (data_ptr, key_ptr);
                    break;
                }

                /*!< 系统级的控制案件，比较特殊的控制按键 */
                case KEY_STYLE_SYSTEM:
                {
                    HwFullScreenSysUp (data_ptr, key_ptr);
                    break;
                }
                
                default:
                    break;
            }
            
            msg_dealt = TRUE;      
            break;
        }
        
        case EDIT_STATE_WRITE:
        {
            //when waiting...
            if (hw_data_p->recognize_timer_id > 0)
            {
                //SCI_TRACE_LOW:"HwFullScreenPanelKeyUp"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1342_112_2_18_2_27_55_123,(uint8*)"");
                msg_dealt = FALSE;
                break;
            }

            if (hw_data_p->trace_len > 0)
            {
                hw_data_p->recognize_timer_id = MMK_CreateTimerCallback(
                                                           hw_data_p->recognize_tick,
                                                           HwFullScreenRecognizeCB,
                                                           (uint32)data_ptr,
                                                           FALSE);
            }

            msg_dealt = TRUE;
            break;
        }
        
        default:
            break;
    }
    
    return msg_dealt;
}


/*==============================================================================
Description: 面板上的按键Long消息处理，包括各种不同的输入状态
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenPanelKeyLong (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN msg_dealt = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    if (key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"HwFullScreenPanelKeyLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1381_112_2_18_2_27_55_124,(uint8*)"");
        return FALSE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    switch (hw_data_p->edit_state)
    {
        case EDIT_STATE_NONE:
        {
            msg_dealt =  TRUE;
            break;
        }
        
        case EDIT_STATE_WRITE:
        case EDIT_STATE_COMMIT_DEFAULT:
        case EDIT_STATE_ASSO:
        default:
            break;
    }
    
    return msg_dealt;
}


/*==============================================================================
Description: 面板上的按键以外，挡住窗口部分的按键消息处理，包括各种不同的
                    输入状态
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenWinDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN msg_dealt = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    data_ptr->vkb.last_key = key_ptr;
    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    switch (hw_data_p->edit_state)
    {
        case EDIT_STATE_NONE:
        case EDIT_STATE_COMMIT_DEFAULT:     //此状态假设为选择候选字状态
        case EDIT_STATE_ASSO:
        {
            msg_dealt =  FALSE;     //不做什么事，消息继续下发
            break;
        }
        
        case EDIT_STATE_WRITE:
        {
            //写下一笔
            data_ptr->vkb.last_key = key_ptr;

            if (hw_data_p->recognize_timer_id > 0)
            {
                MMK_StopTimer(hw_data_p->recognize_timer_id);
                hw_data_p->recognize_timer_id = 0;
            }

            
            {
                HwAddStrokeMark (data_ptr);
                HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);
            }

            msg_dealt =  TRUE;   
            break;
        }
        
        default:
            break;
    }
    
    return msg_dealt;
}


/*==============================================================================
Description: 面板上的按键以外，挡住窗口部分的按键Move消息处理，包括各种不同的
                    输入状态
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenWinMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN msg_dealt = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    switch (hw_data_p->edit_state)
    {
        case EDIT_STATE_NONE:
        case EDIT_STATE_COMMIT_DEFAULT:
        case EDIT_STATE_ASSO:
        {
            uint16 interal = HwFullScreenMoveInteral (data_ptr->first_tp.x, data_ptr->first_tp.y, data_ptr->last_tp.x, data_ptr->last_tp.y) ;

            //判断是否启动手写
            if (interal <= HW_WRITE_BEGIN_INTERAL)   
            {
                if (hw_data_p->trace_len == 0)   //首个down点存入笔迹
                {
                    HwAddTrace (data_ptr, data_ptr->first_tp.x, data_ptr->first_tp.y);
                }
                
                HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);
                msg_dealt = FALSE;
                break;
            }

            //上一个字符提交
            if (hw_data_p->edit_state == EDIT_STATE_COMMIT_DEFAULT)
            {
                HwFullScreenCommitDefault (data_ptr);
            }

            if (hw_data_p->trace_len == 0)   //首个down点存入笔迹
            {
                HwAddTrace (data_ptr, data_ptr->first_tp.x, data_ptr->first_tp.y);
            }

            HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);

            //高亮按键取消高亮
            HideTip(data_ptr);
            if (data_ptr->vkb.last_key && data_ptr->vkb.last_key->is_press)      //
            {
                data_ptr->vkb.last_key->is_press = 0;
            }
            HwDraw(data_ptr);
            
            hw_data_p->edit_state = EDIT_STATE_WRITE;     //启动手写
            HwFullScreenReDrawOmitMoveDots (data_ptr, hw_data_p->trace_len - 1);    //忽略点补上来画出
            
            msg_dealt = TRUE;
            break;
        }
        
        case EDIT_STATE_WRITE:
        {
            if (key_ptr == PNULL)
            {
                //SCI_TRACE_LOW:"HwFullScreenWinMove EDIT_STATE_WRITE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1556_112_2_18_2_27_56_125,(uint8*)"");
                return FALSE;
            }

            
            {
                int16 x0, y0, x1, y1;

                HwAddTrace (data_ptr, data_ptr->last_tp.x, data_ptr->last_tp.y);

                x0 = hw_data_p->trace[hw_data_p->trace_len-2].x - data_ptr->rect.left;
                y0 = hw_data_p->trace[hw_data_p->trace_len-2].y - data_ptr->rect.top;
                x1 = hw_data_p->trace[hw_data_p->trace_len-1].x - data_ptr->rect.left;
                y1 = hw_data_p->trace[hw_data_p->trace_len-1].y - data_ptr->rect.top;

                HwDrawStroke (data_ptr, x0, y0, x1, y1);
            }

            msg_dealt = TRUE;
            break;
        }
        
        default:
            break;
    }

    return msg_dealt;
}


/*==============================================================================
Description: 面板上的按键以外，挡住窗口部分的按键Up消息处理，包括各种不同的
                    输入状态
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenWinUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN msg_dealt = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    if (key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"HwFullScreenWinUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1635_112_2_18_2_27_56_127,(uint8*)"");
        return FALSE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    switch (hw_data_p->edit_state)
    {
        case EDIT_STATE_NONE:
        case EDIT_STATE_COMMIT_DEFAULT:
        case EDIT_STATE_ASSO:
        {
            msg_dealt = FALSE;

            //这里理论上是可以不处理的，
            //但是某些烂屏会出现down, 没有move，但是up点和down点不一致的情况，整死哥了
            if (data_ptr->vkb.last_key && data_ptr->vkb.last_key->is_press)
            {
                //SCI_TRACE_LOW:"VkbUp touchpanel"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1651_112_2_18_2_27_56_128,(uint8*)"");
                data_ptr->vkb.last_key->is_press = 0;
				MMIIM_SetVKeyPaintStatus (data_ptr->vkb.last_key, TRUE);

				HideTip (data_ptr);
                DrawVkb(data_ptr);
            }
            
            break;
        }
        
        case EDIT_STATE_WRITE:
        {
            //when waiting...
            if (hw_data_p->recognize_timer_id > 0)
            {
                //SCI_TRACE_LOW:"HwFullScreenWinUp"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1664_112_2_18_2_27_56_129,(uint8*)"");
                msg_dealt = FALSE;
                break;
            }

            if (hw_data_p->trace_len > 0)
            {
                hw_data_p->recognize_timer_id = MMK_CreateTimerCallback(
                                                           hw_data_p->recognize_tick,
                                                           HwFullScreenRecognizeCB,
                                                           (uint32)data_ptr,
                                                           FALSE);
            }

            msg_dealt = TRUE;
            break;
        }
        
        default:
            break;
    }

    return msg_dealt;
}


/*==============================================================================
Description: 全屏手写触摸屏消息处理
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN HWFullScreenHandleTpMsg (
    IM_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN msg_dealt = TRUE;       //默认为TRUE, 如果没有处理，置为false
    KEY_INFO_T *key_ptr = PNULL;
    GUI_POINT_T point = {0};

    CHECK_DATA(data_ptr);

    if (event_data_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"HWFullScreenHandleTpMsg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_1708_112_2_18_2_27_56_130,(uint8*)"");
        return FALSE;
    }    

    switch (event_data_ptr->sys_msg.msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    {
        //move点能够立即收到，而不用等一段时间过来，导致前面几个点没有立即显示
        MMK_DelayTpMove( FALSE );       
        
        point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        data_ptr->first_tp = point;
        data_ptr->last_tp = point;
        data_ptr->vkb.down_point = point;

        key_ptr = FindKey(data_ptr, &point);

        if (PNULL != key_ptr && PNULL != key_ptr->f_down)
        {
            data_ptr->vkb.first_key = key_ptr;
            msg_dealt = key_ptr->f_down(data_ptr, key_ptr);
        }
        else if (data_ptr->vkb.bg_key && GUI_PointIsInRect(point, data_ptr->vkb.bg_key->rect))
        {
            //这里模拟窗口区域空白键处理消息
            //如果down消息继续下发到了窗口而被窗口处理了的话，则
            //只能表示IM控件所在窗口没有move到合适的区域，与此处的
            //消息下发无关
            msg_dealt = HwFullScreenWinDown (data_ptr, data_ptr->vkb.bg_key);       
        }

        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
    {
        //在失去焦点并且重新获得之后，可能会丢失last key信息
        if (PNULL == data_ptr->vkb.last_key) 
            break;

        point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);
        
        if (data_ptr->vkb.state & KEYBOARD_CHOOSE_MENU)
        {
            MMIIM_MethodSettingMenuRefresh (data_ptr, point, TRUE);
            break;
        }     
        
        data_ptr->last_tp = point;

        key_ptr = FindKey(data_ptr, &point);

        if (PNULL != key_ptr && PNULL != key_ptr->f_move)
        {
            msg_dealt = key_ptr->f_move(data_ptr, key_ptr);
        }
        else if (data_ptr->vkb.bg_key && GUI_PointIsInRect(point, data_ptr->vkb.bg_key->rect))
        {
            //这里模拟窗口区域空白键处理消息
            //如果down消息继续下发到了窗口而被窗口处理了的话，则
            //只能表示IM控件所在窗口没有move到合适的区域，与此处的
            //消息下发无关
            msg_dealt = HwFullScreenWinMove (data_ptr, data_ptr->vkb.bg_key);       
        }
   
        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
    {
        KERNEL_DATA_HW_T *hw_data_p = PNULL;
        GUI_RECT_T fullwin_rect = MMITHEME_GetFullScreenRectEx(data_ptr->init_param.win_handle);
        
        //在失去焦点并且重新获得之后，可能会丢失last key信息
        if (PNULL == data_ptr->vkb.last_key) 
            break;

        //非手写状态，move的点全部清空,防止抖动点被计入到下一次手写了
        hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
        if (hw_data_p->edit_state != EDIT_STATE_WRITE)
        {
            SCI_MEMSET (hw_data_p->trace, 0, sizeof (hw_data_p->trace));
            hw_data_p->trace_len = 0;
        }
        
        point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        //内部开发测试存在窗口比触摸屏小的情况，导致有些点在窗口外面
        //被设置到窗口边界值而无法处理，做一个容错处理
        if (point.x > fullwin_rect.right - fullwin_rect.left)
        {
            point.x = fullwin_rect.right - fullwin_rect.left;
        }
        if (point.y > fullwin_rect.bottom - fullwin_rect.top)
        {
            point.y = fullwin_rect.bottom - fullwin_rect.top;
        }
        
        if (data_ptr->vkb.state & KEYBOARD_CHOOSE_MENU)
        {
            MMIIM_MethodSettingMenuUp (data_ptr);
            break;
        }      
		
        data_ptr->last_tp = point;

        key_ptr = FindKey(data_ptr, &point);

        if (PNULL != key_ptr && PNULL != key_ptr->f_up)
        {
            msg_dealt = key_ptr->f_up (data_ptr, key_ptr);
        }
        else if (data_ptr->vkb.bg_key && GUI_PointIsInRect(point, data_ptr->vkb.bg_key->rect))
        {
            //这里模拟窗口区域空白键处理消息
            //如果down消息继续下发到了窗口而被窗口处理了的话，则
            //只能表示IM控件所在窗口没有move到合适的区域，与此处的
            //消息下发无关
            msg_dealt = HwFullScreenWinUp (data_ptr, data_ptr->vkb.bg_key);     
        }
        else if (data_ptr->vkb.last_key->is_press)  //
        {
            data_ptr->vkb.last_key->is_press = 0;
			MMIIM_SetVKeyPaintStatus (data_ptr->vkb.last_key, TRUE);
			
            HideTip(data_ptr);
            DrawVkb(data_ptr);
            msg_dealt = TRUE;
        }

        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
    {
        point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        key_ptr = FindKey(data_ptr, &point);

        if (PNULL != key_ptr && PNULL != key_ptr->f_long
                && key_ptr == data_ptr->vkb.last_key)
        {
            msg_dealt = key_ptr->f_long(data_ptr, key_ptr);
        }
        
        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        msg_dealt = FALSE;
        break;
    }

    return msg_dealt;
}

/*==============================================================================
Description: HwFullScreenInit
Global resource dependence: NONE
Author: yaoguang.chen
Note: 仅仅是汉字手写，不支持英文手写，没有意义
==============================================================================*/
PUBLIC BOOLEAN HwFullScreenInit(IM_DATA_T *data_ptr)
{
    MMIIM_HW_THICKNESS_E width_fix = MMIIM_GetHwThickness();
    MMIIM_HW_SPEED_E tick_fix = MMIIM_GetHwSpeed();
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    HwFullScreenInitKb(data_ptr);

    //Notes!! 从symbol切换回手写时，没有重新设置IM控件区域，导致有误
    if (data_ptr->is_symbol_panel)
    {
        GUIIM_NOTIFY_PACK_T notify_pack = {0};
        
        notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
        GUIIM_SetNotify (data_ptr->init_param.im_handle, &notify_pack);
    }

//关闭剪贴板功能
#if defined GUIF_CLIPBOARD
{
    MMI_HANDLE_T edit_handle = MMK_GetParentCtrlHandle (data_ptr->init_param.im_handle);
    GUIEDIT_SetClipboardEnabled (edit_handle, FALSE);
}
#endif

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_HW_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_HW_T));

    CHECK_DATA_EX(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    hw_data_p->recognize_tick = HW_RECOGNIZE_TICK;
    hw_data_p->trace_width = HW_TRACE_WIDTH;
    hw_data_p->trace_color = RGB565TO888(MMIIM_GetHwColor());

    if (width_fix < MMIIM_HW_THICKNESS_MAX)
    {
        hw_data_p->trace_width -= 2 * width_fix;
    }

    switch(tick_fix) 
    {
    case MMIIM_HW_SPEED_FAST:
        hw_data_p->recognize_tick = HW_RECOGNIZE_TICK / 3;  //HW_RECOGNIZE_TICK/2;
    	break;
    case MMIIM_HW_SPEED_NORMAL:
        hw_data_p->recognize_tick = HW_RECOGNIZE_TICK * 2 / 3;  //HW_RECOGNIZE_TICK - HW_RECOGNIZE_TICK/4;
    	break;
    case MMIIM_HW_SPEED_SLOW:
        hw_data_p->recognize_tick = HW_RECOGNIZE_TICK;
    	break;
    default:
        hw_data_p->recognize_tick = HW_RECOGNIZE_TICK - HW_RECOGNIZE_TICK/4;
        break;
    }

    //if tp press ring is on, then close and record.
    hw_data_p->is_tp_ring_close = FALSE;

    if (MMIDEFAULT_IsEnableTpRing())
    {
        MMIDEFAULT_EnableTpRing("HAND WRITEING", FALSE);
        hw_data_p->is_tp_ring_close = TRUE;
    }

    HwInitEngine (data_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwFullScreenTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN HwFullScreenTerm(IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA(data_ptr);

//恢复剪贴板功能
#if defined GUIF_CLIPBOARD
{
    MMI_HANDLE_T edit_handle = MMK_GetParentCtrlHandle (data_ptr->init_param.im_handle);
    GUIEDIT_SetClipboardEnabled (edit_handle, TRUE);
}
#endif

    HwFullScreenCommitDefault(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->recognize_timer_id > 0)
    {
        MMK_StopTimer(hw_data_p->recognize_timer_id);
        hw_data_p->recognize_timer_id = 0;
    }

    //if tp ring press is set
    if (hw_data_p->is_tp_ring_close)
    {
        MMIDEFAULT_EnableTpRing("HAND WRITEING", TRUE);
        hw_data_p->is_tp_ring_close = FALSE;
    }

    //ReleaseUILayer(data_ptr);
    UILAYER_RemoveBltLayer (&data_ptr->ui_layer);
    
    HwTermEngine (data_ptr);

    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));
    
    return TRUE;
}


/*==============================================================================
Description: 初始化键盘为区域手写键盘
Global resource dependence: NONE
Author: yaoguang.chen
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
LOCAL void HwFullScreenInitKb(IM_DATA_T *data_ptr)
{
    uint8 const SUNDRY_CNT = 4;
    uint8 const CHAR_CNT = 7;
    uint8 const CTRL_CNT = 7;   //sapce, backspace, enter, 123symb, ime set, lang set, pageup, page down
    uint8 const SYS_CNT = 2;
    uint8 const CAND_CNT = HW_FULLSCREEN_EXPAND_CAND_COUNT;
    int32 char_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width = 0, panel_height = 0, cand_width = 0, cand_height = 0;
    GUI_RECT_T key_rect = {0};
    int16 blank_height = 0;     //虚拟键盘上面部分空白部分高度
    
    CHECK_DATA(data_ptr);

    data_ptr->theme = g_hw_fullscreen_theme_default;

    ReleaseVkb(&data_ptr->vkb);
    
    //当前为手写
    HwFullScreenGetDisplayPanelRect (data_ptr, &data_ptr->rect);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;
    blank_height = data_ptr->rect.bottom - panel_height + 1;     //面板区域相对顶部的空白区域高度
    
    //tip rect是一个固定的高度，宽度是按键宽度的一个倍数
    data_ptr->tip_rect.left = 0;
    data_ptr->tip_rect.top = 0;
    data_ptr->tip_rect.right = panel_width;
    data_ptr->tip_rect.bottom = panel_height * TIP_EXPAND_MULTIPLE / GetPanelKeyRowNum (data_ptr);/*lint !e524*/

    //全屏手写初始化时，需要重新设置
    data_ptr->rect = MMITHEME_GetFullScreenRectEx(data_ptr->init_param.win_handle);
        
    vkb_ptr = &(data_ptr->vkb);
    vkb_ptr->key_cnt = 0;
    vkb_ptr->key_cnt += SUNDRY_CNT; //blank, blank, cand vessel
    vkb_ptr->key_cnt += CHAR_CNT;     //characters, '?', '.', '!', '...', '、', ':' , '.', 
    vkb_ptr->key_cnt += CTRL_CNT;       //backspace, enter, 123symb, ime switch
    vkb_ptr->key_cnt += SYS_CNT; //hide and expand, expand pgup & pgdn
    vkb_ptr->key_cnt += CAND_CNT;   //candidate keys
    
    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //blank  虚拟键盘上面的部分
    key_idx = 0;
    vkb_ptr->keys[key_idx].rect.top = 0;
    vkb_ptr->keys[key_idx].rect.bottom = blank_height - 1;
    vkb_ptr->keys[key_idx].rect.left = 0;
    vkb_ptr->keys[key_idx].rect.right = data_ptr->rect.right;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL | KEY_TAG_EXPAND;       /*lint !e655*/
    vkb_ptr->keys[key_idx].code = VKEY_WIN_PANEL;      //VKEY_WRITE_PANEL
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_TRANSPARENT_E;    //透明显示
    vkb_ptr->keys[key_idx].is_inactive = 0;
    vkb_ptr->keys[key_idx].f_down = HwFullScreenWinDown;      //消息下发到窗口
    vkb_ptr->keys[key_idx].f_move = HwFullScreenWinMove;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenWinUp;

    //blank 部分窗口存在softkey等控件，虚拟键盘的下面部分
    key_idx ++;
    if (blank_height + panel_height <= data_ptr->rect.bottom)   //存在softkey区域
    {
        vkb_ptr->keys[key_idx].rect.top = blank_height + panel_height;
        vkb_ptr->keys[key_idx].rect.bottom = data_ptr->rect.bottom;
        vkb_ptr->keys[key_idx].rect.left = 0;
        vkb_ptr->keys[key_idx].rect.right = data_ptr->rect.right;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL | KEY_TAG_EXPAND;   /*lint !e655*/
        vkb_ptr->keys[key_idx].code = VKEY_WIN_PANEL;      //VKEY_WRITE_PANEL
        vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
        vkb_ptr->keys[key_idx].add_style.art_disp = KASA_TRANSPARENT_E;    //透明显示
        vkb_ptr->keys[key_idx].is_inactive = 0;
        vkb_ptr->keys[key_idx].f_down = HwFullScreenWinDown;      //消息下发到窗口
        vkb_ptr->keys[key_idx].f_move = HwFullScreenWinMove;
        vkb_ptr->keys[key_idx].f_up = HwFullScreenWinUp;
    }
    else        //区域为0的按键，绘制时会引起绘制越界,内存覆盖assert
    {
        vkb_ptr->keys[key_idx].rect.top = data_ptr->rect.bottom;
        vkb_ptr->keys[key_idx].rect.bottom = vkb_ptr->keys[key_idx].rect.top;
        vkb_ptr->keys[key_idx].rect.left = 0;
        vkb_ptr->keys[key_idx].rect.right = data_ptr->rect.right;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL | KEY_TAG_EXPAND;   /*lint !e655*/
        vkb_ptr->keys[key_idx].code = VKEY_WIN_PANEL;      //VKEY_WRITE_PANEL
        vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
        vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //什么都不画
        vkb_ptr->keys[key_idx].is_inactive = 1;         //也不激活它
        vkb_ptr->keys[key_idx].f_down = HwFullScreenWinDown;      //消息下发到窗口
        vkb_ptr->keys[key_idx].f_move = HwFullScreenWinMove;
        vkb_ptr->keys[key_idx].f_up = HwFullScreenWinUp;
    }

    //background  这个键之所以存在，是因为按键间隔处没有消息处理
    key_idx ++;
    vkb_ptr->keys[key_idx].rect.top = blank_height;
    vkb_ptr->keys[key_idx].rect.bottom = blank_height + panel_height - 1;
    vkb_ptr->keys[key_idx].rect.left = data_ptr->rect.left;
    vkb_ptr->keys[key_idx].rect.right = data_ptr->rect.right;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL | KEY_TAG_EXPAND;   /*lint !e655*/
#if defined SOGOU_USE_PDA_SKIN
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //不画背景，防止将sogou皮肤给覆盖掉
#else
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_DRAW_E;    //该装饰按键绘制面板背景色
#endif    
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    vkb_ptr->keys[key_idx].f_down = HwFullScreenWinDown;       //按键间隔需要消息处理
    vkb_ptr->keys[key_idx].f_move = HwFullScreenWinMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenWinUp;
    vkb_ptr->bg_key = vkb_ptr->keys + key_idx;  //还是使用一个键值记住方便些
    
    //cand vessel
    key_idx ++;
    HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
#if defined SOGOU_USE_PDA_SKIN    
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //不画背景，防止将sogou皮肤给覆盖掉
#endif    
    vkb_ptr->keys[key_idx].is_inactive = 1;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    
    //characters
    key_idx ++;
    char_begin = key_idx;
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - char_begin;
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
        if(MMIAPIIM_GetLanguage() == GUIIM_LANG_CHS ||MMIAPIIM_GetLanguage() == GUIIM_LANG_CHT)     
            vkb_ptr->keys[key_idx].text[0] = g_hw_fullscreen_cust_cn_symb[key_idx - char_begin];
        else
            vkb_ptr->keys[key_idx].text[0] = g_hw_fullscreen_cust_symb[key_idx - char_begin];    
#else
        vkb_ptr->keys[key_idx].text[0] = g_hw_fullscreen_cust_symb[key_idx - char_begin];
#endif

        HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_ALL_TEXT; 
        vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
        vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //space 空格
    key_idx --;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_CENTER;

    //DEL
    key_idx ++;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
    vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
    vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    vkb_ptr->keys[key_idx].has_tip = FALSE;    
    HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].text_len = 0;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;

    //enter
    key_idx ++;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
    vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
    vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    vkb_ptr->keys[key_idx].has_tip = FALSE;    
    HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].text_len = 0;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;

    //--123?!#
    key_idx ++;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
    vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
    vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    vkb_ptr->keys[key_idx].has_tip = FALSE;    
    HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    
     //  ime set 
    key_idx ++;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    vkb_ptr->keys[key_idx].has_tip = FALSE;    
    HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].text_len = 0;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;

    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
    vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--expand pageup
    key_idx++;
    HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
    vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
    vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    
    //--expand pagedown
    key_idx++;
    HwFsConvertRect(panel_width, panel_height,
                &(g_hw_fullscreen_position[key_idx]),
                blank_height,
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
    vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
    vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    vkb_ptr->keys[key_idx].is_hide = 1;    
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    
    //第一行  sys hide, expand
    //hide and expand
    //这两个按键在同一个位置，交替出现, 初始化隐藏expand
    key_idx ++;
    sys_begin = key_idx;
	
    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        HwFsConvertRect(panel_width, panel_height,
                    &(g_hw_fullscreen_position[key_idx]),     
                    blank_height,
                    &(vkb_ptr->keys[key_idx].rect),
                    0,
                    0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);         
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
        vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
    //--expand
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    // - expand - cand panel(art)
    //candidate...
    //候选键的位置确定，整齐的排成矩阵
    //内容未定
    //candidate key 总是放在最后，因为其可能会覆盖在其他的部分之上，比如art
    key_rect.left = g_hw_fullscreen_position[3].left;
    key_rect.right = g_hw_fullscreen_position[3].right;
    //key_rect.top = g_hw_fullscreen_position[3].top;
    //key_rect.bottom = g_hw_fullscreen_position[3].bottom;

    cand_width = (key_rect.right - key_rect.left) / HW_FULLSCREEN_EXPAND_CAND_COL;
    cand_height = g_hw_fullscreen_position[3].bottom - g_hw_fullscreen_position[3].top;
    
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % HW_FULLSCREEN_EXPAND_CAND_COL;
        int16 y = (key_idx - cand_begin) / HW_FULLSCREEN_EXPAND_CAND_COL;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = key_rect.top + y * cand_height;
        cand_rect.bottom = cand_rect.top + cand_height;
        cand_rect.left = key_rect.left + x * cand_width;
        cand_rect.right = cand_rect.left + cand_width;

        HwFsConvertRect(panel_width, panel_height,
                    &cand_rect,
                    blank_height,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = HwFullScreenPanelKeyDown;
        vkb_ptr->keys[key_idx].f_move = HwFullScreenPanelKeyMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = HwFullScreenPanelKeyUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}

/*==============================================================================
Description: HwFullScreenCharUp
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"HwFullScreenCharUp key_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2345_112_2_18_2_27_57_131,(uint8*)"");
        return FALSE;
    }

    HwFullScreenCommitDefault(data_ptr);

    if (VKEY_SPACE == key_ptr->code)
    {
//如果进入中文手写
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT    
        if(MMIAPIIM_GetLanguage() == GUIIM_LANG_CHS ||MMIAPIIM_GetLanguage() == GUIIM_LANG_CHT)     
            CommitChar(data_ptr, 0x3000);
        else
            CommitChar(data_ptr, ' ');
#else
        CommitChar(data_ptr, ' ');
#endif
    }
    else
    {
        CommitChar(data_ptr, key_ptr->text[0]);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwFullScreenCtrlUp
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    EDIT_STATE_T edit_state = EDIT_STATE_NONE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    if (key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"HwFullScreenCtrlUp key_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2387_112_2_18_2_27_58_132,(uint8*)"");
        return FALSE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    edit_state = hw_data_p->edit_state;
    HwFullScreenCommitDefault(data_ptr);    //此处把状态给写掉了

    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (hw_data_p->cand_cnt > 0)
        {
            if (edit_state == EDIT_STATE_COMMIT_DEFAULT)
            {
                CommitChar(data_ptr, '\b');
            }

            HwFullScreenClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        CommitChar(data_ptr, '\n');
        break;

    case VKEY_123SYMBOL:
        if(MMIAPIIM_GetLanguage() == GUIIM_LANG_CHS ||MMIAPIIM_GetLanguage() == GUIIM_LANG_CHT)
        {
            SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        }
        else
        {
            SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        }

        key_ptr = PNULL;
        break;

    case VKEY_SET:
        VkbUp(data_ptr, key_ptr);
        MMIIM_VkeyUpSet (data_ptr, key_ptr);
        return TRUE;
        
    case VKEY_LANGUAGE_SET:
        VkeyUpLangSet (data_ptr, key_ptr);      //输入法已经切换，不需要刷新vkbup
        return TRUE;
        
    case VKEY_PAGEUP:

        if (hw_data_p->edit_state == EDIT_STATE_ASSO)
        {
            if (hw_data_p->cur_page == 0)
            {
                //SCI_TRACE_LOW:"HwFullScreenCtrlUp VKEY_PAGEUP"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2446_112_2_18_2_27_58_133,(uint8*)"");
                return FALSE;
            }
            
            hw_data_p->cur_page--;
            HwAsso(data_ptr);
        }

        break;

    case VKEY_PAGEDOWN:

        if (hw_data_p->edit_state == EDIT_STATE_ASSO)
        {
            if (1 != hw_data_p->has_next_page)
            {
                //SCI_TRACE_LOW:"HwFullScreenCtrlUp VKEY_PAGEDOWN"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2462_112_2_18_2_27_58_134,(uint8*)"");
                return FALSE;
            }
            
            hw_data_p->cur_page++;
            HwAsso(data_ptr);
        }

        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwFullScreenSysUp
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (PNULL == key_ptr)
    {
        //SCI_TRACE_LOW:"HwFullScreenSysUp key_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2493_112_2_18_2_27_58_135,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, hw_data_p->cand_cnt);
        HwFullScreenSetCand(data_ptr);
        break;

    case VKEY_HIDE:
        HidePanel(data_ptr);
        return TRUE;
        //break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwFullScreenCandUp
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 select_idx = 0;
    wchar last_char = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    if (PNULL == key_ptr)
    {
        //SCI_TRACE_LOW:"HwFullScreenCandUp key_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2533_112_2_18_2_27_58_136,(uint8*)"");
        return FALSE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    select_idx = key_ptr->code - VKEY_CANDIDATE;
    if (select_idx < 0
        || select_idx >= hw_data_p->cand_cnt)
    {
        //SCI_TRACE_LOW:"HwFullScreenCandUp select_idx"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2541_112_2_18_2_27_58_137,(uint8*)"");
        return FALSE;
    }

    last_char = hw_data_p->cand[select_idx][0];
    CommitChar(data_ptr, last_char);

    HwFullScreenClear(data_ptr);

#if defined IM_SIMP_CHINESE_SUPPORT
    //if(data_ptr->init_param.method_param_ptr->input_lang == GUIIM_LANG_CHS)     //手写时，该语言值为空
    if(MMIAPIIM_GetLanguage() == GUIIM_LANG_CHS)
    {
        hw_data_p->last_char = last_char;
        HwAsso(data_ptr);
    } 
#endif

#if defined IM_TRAD_CHINESE_SUPPORT
    if(MMIAPIIM_GetLanguage()  == GUIIM_LANG_CHT)
    {
        hw_data_p->last_char = last_char;
        HwAsso(data_ptr);
    } 
#endif

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: HwFullScreenRecognizeCB
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL void HwFullScreenRecognizeCB(uint8 timer_id, uint32 param)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)param;
    wchar result[HW_MAXCANDNUM] = {0};
    int32 recognize_res = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    if( PNULL == data_ptr || (sizeof(IM_DATA_T) != data_ptr->check_info) )
    {
        return;    
    }
    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if ( PNULL == hw_data_p  || (hw_data_p->recognize_timer_id != timer_id) )
    {
        return;
    } 

    hw_data_p->recognize_timer_id = 0;

    hw_data_p->edit_state = EDIT_STATE_NONE;

#if defined IM_HANDWRITING_HANWANG
    HwAddCharMark (data_ptr);
#elif defined IM_HANDWRITING_SOGOU
    HwAddStrokeMark (data_ptr);
#endif

    recognize_res = HwEngineRecognize (data_ptr, result);
    hw_data_p->cand_cnt = 0;

    //如果给出的字，我们显示不了，则必须过滤掉，
    if (recognize_res > 0)
    {
        int32 i = 0;

        for (i = 0; i < recognize_res; i++)
        {
            if (result[i] && SPMLAPI_TestChar(result[i]))   //函数对参数0返回值依然为True;
            {
                hw_data_p->cand[hw_data_p->cand_cnt++][0] = result[i];
            }
        }
    }

    //清空笔迹
    SCI_MEMSET (hw_data_p->trace, 0, sizeof(hw_data_p->trace));
    hw_data_p->trace_len = 0;

    //set candidate
    if (hw_data_p->cand_cnt > 0)
    {
        GUIIM_NOTIFY_PACK_T notify_pack = {0};

        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        notify_pack.data.commit_data.underline[0] = hw_data_p->cand[0][0];
        notify_pack.data.commit_data.underline_len = 1;

        GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

        hw_data_p->edit_state = EDIT_STATE_COMMIT_DEFAULT;
        hw_data_p->has_next_page = 0;

        SetHideKey(data_ptr, hw_data_p->cand_cnt);
        HwFullScreenSetCand(data_ptr);
    }

    HwDraw(data_ptr);

    return;
}


/*==============================================================================
Description: HwFullScreenSetCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL void HwFullScreenSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}
	
    cand_key_idx = data_ptr->vkb.candidate_first;

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (0 == hw_data_p->cand_cnt)
    {
        goto HwFullScreenSetCandEnd;
    }

    //set normal candidate
    while (cand_cnt < hw_data_p->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= HW_FULLSCREEN_EXPAND_CAND_COL)
        {
            break;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = 1;
        data_ptr->vkb.keys[cand_key_idx].text[0] = hw_data_p->cand[cand_cnt][0];
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		//将text字段数据同步到display_text字段
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);

        cand_key_idx++;
        cand_cnt++;
    }

    if (1 == data_ptr->vkb.is_expand)
    {
        hw_data_p->cand_cnt = cand_cnt;

        if (hw_data_p->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);

        if (1 == hw_data_p->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
    }

HwFullScreenSetCandEnd:

    //隐藏剩下的候选按钮
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    return;
}


/*==============================================================================
Description: HwFullScreenClear
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL void HwFullScreenClear(IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    
    if (data_ptr->is_symbol_panel) return;

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    hw_data_p->edit_state = EDIT_STATE_NONE;

    hw_data_p->trace_len = 0;
    hw_data_p->cand_cnt = 0;
    hw_data_p->cur_page = 0;
    hw_data_p->has_next_page = 0;

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, hw_data_p->cand_cnt);
    HwFullScreenSetCand(data_ptr);

    return;
}


/*==============================================================================
Description: HwFullScreenCommitDefault
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenCommitDefault(IM_DATA_T *data_ptr)
{
    BOOLEAN has_commit = FALSE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    has_commit = (EDIT_STATE_COMMIT_DEFAULT == hw_data_p->edit_state);

    if (has_commit)
    {
        CommitChar(data_ptr, hw_data_p->cand[0][0]);
        hw_data_p->edit_state = EDIT_STATE_NONE;
    }

    return has_commit;
}


/*==============================================================================
Description: 
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN HWFullScreenPreProc_HandleTpMsg (
    IM_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    BOOLEAN *is_pre_proc
)
{
    BOOLEAN result = FALSE;
    
    CHECK_DATA (data_ptr);
    if (is_pre_proc == PNULL)
    {
        //SCI_TraceLow:"HWFullScreenPreProc_HandleTpMsg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2793_112_2_18_2_27_58_139,(uint8*)"");
        return FALSE;
    }
    
    if (!data_ptr->is_symbol_panel
        && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING
        ) //从手写切换到sym后，上述的输入法值依然为hw
    {
        result = HWFullScreenHandleTpMsg(data_ptr, event_data_ptr);
        *is_pre_proc = TRUE;
    }
    else
    {
        *is_pre_proc = FALSE;
    }

    return result;
}


/*==============================================================================
Description: 全屏手写，点击和手写容易混要，起始几个点如果move不够大，先不
                    画出来，在启动手写后，补上没有画出的点
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwFullScreenReDrawOmitMoveDots (IM_DATA_T *data_ptr, uint32 omit_count)
{
    uint32 dot_count = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    BOOLEAN is_dev_ok = FALSE;
    GUILCD_DRAW_DC_T dc_data = {0}; 
    GUI_RECT_T draw_rect = {999, 999, 0, 0};        //将left和top先置为最大值
 
    CHECK_DATA_EX(data_ptr);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->ui_layer, &dc_data);
    if (!is_dev_ok)
    {
        SCI_TRACE_LOW ("[MMIIM] HwDrawStroke");
        return FALSE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);

    omit_count = omit_count < hw_data_p->trace_len - 1 ? omit_count : hw_data_p->trace_len - 1;
    omit_count = omit_count < HW_TRACE_CNT_LIMIT - 1 ? omit_count : HW_TRACE_CNT_LIMIT - 1;
    for (dot_count = 0; dot_count < omit_count; dot_count ++)
    {   //此处不判断是否有一笔写完的标志，假设数据正确
        _DrawLine(&dc_data, 
                  hw_data_p->trace[dot_count].x, hw_data_p->trace[dot_count].y, hw_data_p->trace[dot_count + 1].x, hw_data_p->trace[dot_count + 1].y, 
                  hw_data_p->trace_width,
                  hw_data_p->trace_color);

        //计算刷新区域，left取所有点最小值，right取最大值
        draw_rect.left = draw_rect.left < hw_data_p->trace[dot_count].x ? draw_rect.left : hw_data_p->trace[dot_count].x;
        draw_rect.left = draw_rect.left < hw_data_p->trace[dot_count + 1].x ? draw_rect.left : hw_data_p->trace[dot_count + 1].x;

        draw_rect.right = draw_rect.right > hw_data_p->trace[dot_count].x ? draw_rect.right : hw_data_p->trace[dot_count].x;
        draw_rect.right = draw_rect.right > hw_data_p->trace[dot_count + 1].x ? draw_rect.right : hw_data_p->trace[dot_count + 1].x;

        draw_rect.top = draw_rect.top < hw_data_p->trace[dot_count].y ? draw_rect.top : hw_data_p->trace[dot_count].y;
        draw_rect.top = draw_rect.top < hw_data_p->trace[dot_count + 1].y ? draw_rect.top : hw_data_p->trace[dot_count + 1].y;

        draw_rect.bottom = draw_rect.bottom > hw_data_p->trace[dot_count].y ? draw_rect.bottom : hw_data_p->trace[dot_count].y;
        draw_rect.bottom = draw_rect.bottom > hw_data_p->trace[dot_count + 1].y ? draw_rect.bottom : hw_data_p->trace[dot_count + 1].y;
    }

    draw_rect.left -= 1;
    draw_rect.top -= 1;
    draw_rect.right += 1;
    draw_rect.bottom += 1;
    
    MMITHEME_StoreUpdateRect(&data_ptr->ui_layer, draw_rect);
    
    return TRUE;
}
#endif      //defined MMIIM_HW_FULLSCREEN_SUPPORT


#if defined IM_HANDWRITING_HANWANG

LOCAL void HwCstarAsso (IM_DATA_T *data_ptr)        //here cue use cstar + hanwang_hw
{
#if (defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT))
    wchar buffer[CH_CANDIDATE_COUNT_E*ASSO_PAGE_LIMIT + 1] = {0};
    int32 offset_asso_cnt = 0;
    int32 asso_cnt = 0;
    int32 asso_idx = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    GUIIM_LANGUAGE_T lang = MMIAPIIM_GetLanguage();     
    
    CHECK_DATA_EX(data_ptr);
    if (lang != GUIIM_LANG_CHS && lang != GUIIM_LANG_CHT)
    {
        SCI_TRACE_LOW ("[MMIIM] HwCstarAsso");
        return;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    hw_data_p->edit_state = EDIT_STATE_ASSO;

    offset_asso_cnt = hw_data_p->cur_page * CH_CANDIDATE_COUNT_E;

    asso_cnt = kmx_chinese_query_association(GetKmxLdb_ptr(lang, FALSE),
               hw_data_p->last_char, buffer,
               offset_asso_cnt + CH_CANDIDATE_COUNT_E + 1);

    if (asso_cnt <= offset_asso_cnt)
    {
        return;
    }

    hw_data_p->has_next_page = 0;

    if (asso_cnt > offset_asso_cnt + CH_CANDIDATE_COUNT_E)
    {
        if (hw_data_p->cur_page + 1 < ASSO_PAGE_LIMIT)
        {
            hw_data_p->has_next_page = 1;
        }

        asso_cnt = CH_CANDIDATE_COUNT_E;
    }
    else
    {
        asso_cnt -= offset_asso_cnt;
    }

    for (asso_idx = 0; asso_idx < asso_cnt; asso_idx++)
    {
        hw_data_p->cand[asso_idx][0] = buffer[offset_asso_cnt + asso_idx];
    }

    hw_data_p->cand_cnt = asso_cnt;

    SetHideKey(data_ptr, hw_data_p->cand_cnt);
    HwSetCand(data_ptr);
#endif

    return;
}
/*==============================================================================
Description: HwAsso
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HwAsso (IM_DATA_T *data_ptr)
{
    HwCstarAsso (data_ptr);
    return;
}


PUBLIC BOOLEAN HwAddStrokeMark (IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    CHECK_DATA (data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->trace_len > HW_TRACE_CNT_LIMIT - 2)     //char mark保留
    {
        SCI_TRACE_LOW ("[MMIIM] HwAddStrokeMark dots overflow");
        hw_data_p->trace_len = HW_TRACE_CNT_LIMIT - 2;
    }
    
    hw_data_p->trace[hw_data_p->trace_len].x = STROKEENDMARK;
    hw_data_p->trace[hw_data_p->trace_len].y = 0;
    hw_data_p->trace_len ++;

    return TRUE;
}


PUBLIC BOOLEAN HwAddCharMark (IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    CHECK_DATA (data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->trace_len > HW_TRACE_CNT_LIMIT - 1)     
    {
        SCI_TRACE_LOW ("[MMIIM] HwAddCharMark dots overflow");
        hw_data_p->trace_len = HW_TRACE_CNT_LIMIT - 1;
    }
    
    hw_data_p->trace[hw_data_p->trace_len].x = STROKEENDMARK;
    hw_data_p->trace[hw_data_p->trace_len].y = STROKEENDMARK;
    hw_data_p->trace_len ++;

    return TRUE;
}


/*==============================================================================
Description: HwInitEngineAttr
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN HwInitEngineAttr(IM_DATA_T *data_ptr)
{
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    KERNEL_DATA_HW_T *hw_data_p = PNULL; 
    HW_HANWANG_DATA_T *pengine_data = PNULL;

    if (PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"HwInitEngineAttr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2915_112_2_18_2_27_59_140,(uint8*)"");
        return FALSE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p == PNULL)
    {
        //SCI_TRACE_LOW:"HwHanWangRecognize"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2922_112_2_18_2_27_59_141,(uint8*)"");
        return FALSE;
    }

    pengine_data = (HW_HANWANG_DATA_T *)hw_data_p->engine_data;
    if (pengine_data == PNULL)
    {
        //SCI_TRACE_LOW:"HwHanWangRecognize"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2929_112_2_18_2_27_59_142,(uint8*)"");
        return FALSE;
    }

    pengine_data->attr.iCandidateNum = HW_MAXCANDNUM;
    pengine_data->attr.dwRange = CHARSET_ENGLISH | ALC_NUMERIC
            | ALC_PUNC_COMMON_HALF | ALC_PUNC_RARE_HALF | ALC_SYM_COMMON_HALF | ALC_SYM_RARE_HALF
            | ALC_PUNC_COMMON_FULL | ALC_PUNC_RARE_FULL | ALC_SYM_COMMON_FULL | ALC_SYM_RARE_FULL;
    pengine_data->attr.pRam = (uint8*)SCI_ALLOC_APP(HWRERAMSIZE);

    if (PNULL == pengine_data->attr.pRam)
    {
        //SCI_TRACE_LOW:"HwInitEngineAttr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_2941_112_2_18_2_27_59_143,(uint8*)"");
        return FALSE;
    }

    pengine_data->attr.dwRange = ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE | ALC_COMMON_PUNCTUATION | ALC_COMMON_NUMERALS;

    if (GUIIM_LIMIT_ENGLISH & data_ptr->init_param.method_param_ptr->limit)
    {
        pengine_data->attr.dwRange &= ~(ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE | ALC_COMMON_PUNCTUATION);
    }

    if (GUIIM_LIMIT_DIGITAL & data_ptr->init_param.method_param_ptr->limit)
    {
        pengine_data->attr.dwRange &= ~(ALC_COMMON_NUMERALS);
    }

    if (GUIIM_LIMIT_LOCAL & data_ptr->init_param.method_param_ptr->limit)
    {
        //这里不能简单地就return，而应该对纯数字进行一次检测
        pengine_data->attr.dwRange = 0;

        pengine_data->engine_lang = HWLANG_English;

        if (!(GUIIM_LIMIT_ENGLISH & data_ptr->init_param.method_param_ptr->limit))
        {
            pengine_data->attr.dwRange |= ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE | ALC_COMMON_PUNCTUATION;
        }

        if (!(GUIIM_LIMIT_DIGITAL & data_ptr->init_param.method_param_ptr->limit))
        {
            pengine_data->attr.dwRange |= ALC_COMMON_NUMERALS;
        }

        return TRUE;
    }

    if (GUIIM_LANG_NONE != data_ptr->init_param.method_param_ptr->input_lang)
    {
        lang = data_ptr->init_param.method_param_ptr->input_lang;
    }
    else
    {
        //lang = MMIAPIIM_GetLanguage();
        lang = MMIAPIIM_GetLanguage();      //采用前一种输入语言，如果已经设置了该语言
    }

    switch (lang)
    {
    case GUIIM_LANG_NONE:
    case GUIIM_LANG_ENGLISH:
    case GUIIM_LANG_SWAHILI:

        pengine_data->engine_lang = HWLANG_English;
        pengine_data->attr.dwRange = ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE  | ALC_COMMON_NUMERALS | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS;
        break;

    case GUIIM_LANG_CHS:
        pengine_data->attr.dwRange = CHARSET_CNDEFAULT | CHARSET_ENGLISH | ALC_NUMERIC | ALC_PUNC_COMMON_HALF | ALC_PUNC_RARE_HALF
                | ALC_SYM_COMMON_HALF | ALC_SYM_RARE_HALF | ALC_PUNC_COMMON_FULL | ALC_PUNC_RARE_FULL | ALC_SYM_COMMON_FULL | ALC_SYM_RARE_FULL;
        break;

    case GUIIM_LANG_CHT:
        pengine_data->attr.dwRange = ALC_TC_COMMON | ALC_TC_RARE | CHARSET_ENGLISH | ALC_NUMERIC | ALC_PUNC_COMMON_HALF
                | ALC_SYM_COMMON_HALF | ALC_SYM_RARE_HALF | ALC_PUNC_COMMON_FULL | ALC_PUNC_RARE_FULL | ALC_SYM_COMMON_FULL;
        break;

    case GUIIM_LANG_ARABIC:
        pengine_data->attr.dwRange = ALC_ARABIC_BASIC | ALC_ARABIC_EXTEND | ALC_ARABIC_NUMERIC | ALC_ARABIC_SYMBOLS | ALC_ARABIC_LATIN_NUMERIC;
        break;

    case GUIIM_LANG_VIETNAMESE:
        pengine_data->engine_lang = HWLANG_Vietnamese;
        pengine_data->attr.dwRange = ALC_VIET_LATIN_LOWERCASE | ALC_VIET_LATIN_UPPERCASE | ALC_VIET_COMMON_PUNC | ALC_VIET_COMMON_SYMBOLS | ALC_VIET_NUMERALS;
        break;

    case GUIIM_LANG_THAI:
        pengine_data->attr.dwRange  = CHARSET_ENGLISH_THAI | CHARSET_THAILAND | ALC_THAI_NUMERIC | ALC_THAI_PUNCTUATION | ALC_THAI_SYMBOL;
        break;

    case GUIIM_LANG_FRENCH:
        pengine_data->engine_lang = HWLANG_French;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_GERMAN:

        pengine_data->engine_lang = HWLANG_German;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_ITALIAN:

        pengine_data->engine_lang = HWLANG_Italian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
    case GUIIM_LANG_GREEK:

        pengine_data->engine_lang = HWLANG_Greek;
        pengine_data->attr.dwRange = ALC_GREEK | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_PORTUGUESE:

        pengine_data->engine_lang = HWLANG_Portuguese;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_HUNGARIAN:

        pengine_data->engine_lang = HWLANG_Hungarian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_INDONESIAN:

        pengine_data->engine_lang = HWLANG_Indonesian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_HINDI:

        pengine_data->engine_lang = HWLANG_Hindi;
        pengine_data->attr.dwRange = ALC_HINDI_CHAR | ALC_HINDI_NUM | ALC_HINDI_PUNC | ALC_HINDI_COM_PUNC | ALC_HINDI_SYM | ALC_HINDI_NUMERIC;
        break;

    case GUIIM_LANG_MALAY:

        pengine_data->engine_lang = HWLANG_Malay;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_PERSIAN:

        pengine_data->engine_lang = HWLANG_Persian;
        pengine_data->attr.dwRange = ALC_PERSIAN_ARABIC_BASIC | ALC_PERSIAN_ARABIC_EXTEND
                | ALC_PERSIAN_ARABIC_NUMERALS | ALC_PERSIAN_ARABIC_PUNCSYMB
                | ALC_PERSIAN_PUNCTUATION | ALC_PERSIAN_SYMBOLS;
        break;

    case GUIIM_LANG_RUSSIAN:

        pengine_data->engine_lang = HWLANG_Russian;
        pengine_data->attr.dwRange = ALC_CYRILLIC | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_SPANISH:

        pengine_data->engine_lang = HWLANG_Spanish;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_TAGALOG:

        pengine_data->engine_lang = HWLANG_Tagalog;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_TURKISH:

        pengine_data->engine_lang = HWLANG_Turkish;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_URDU:

        pengine_data->engine_lang = HWLANG_Urdu;
        pengine_data->attr.dwRange = ALC_PERSIAN_ARABIC_BASIC | ALC_PERSIAN_ARABIC_EXTEND
                | ALC_PERSIAN_ARABIC_NUMERALS | ALC_PERSIAN_ARABIC_PUNCSYMB
                | ALC_PERSIAN_PUNCTUATION | ALC_PERSIAN_SYMBOLS;
        break;

    case GUIIM_LANG_CZECH:

        pengine_data->engine_lang = HWLANG_Czech;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_SLOVENIAN:

        pengine_data->engine_lang = HWLANG_Slovenian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;
    case GUIIM_LANG_ROMANIAN:

        pengine_data->engine_lang = HWLANG_Romanian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_HEBREW:

        pengine_data->engine_lang = HWLANG_Hebrew;
        pengine_data->attr.dwRange = ALC_HEBREW;
        break;

    case GUIIM_LANG_BENGALI:

        pengine_data->engine_lang = HWLANG_Bengali;
        pengine_data->attr.dwRange = ALC_BENGALA_CHAR | ALC_BENGALA_NUMERIC | ALC_BENGALA_PUNC | ALC_BENGALA_SYM;
        break;

    case GUIIM_LANG_CROATIAN:

        pengine_data->engine_lang = HWLANG_Croatian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_DUTCH:

        pengine_data->engine_lang = HWLANG_Dutch;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_BULGARIAN:

        pengine_data->engine_lang = HWLANG_Bulgarian;
        pengine_data->attr.dwRange = ALC_CYRILLIC | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_POLISH:

        pengine_data->engine_lang = HWLANG_Polish;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_DANISH:

        pengine_data->engine_lang = HWLANG_Danish;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_SLOVAK:

        pengine_data->engine_lang = HWLANG_Slovakian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_LITHUANIAN:

        pengine_data->engine_lang = HWLANG_Lithuanian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_NORWEGIAN:

        pengine_data->engine_lang = HWLANG_Norwegian;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_SWEDISH:

        pengine_data->engine_lang = HWLANG_Swedish;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_UKRAINIAN:

        pengine_data->engine_lang = HWLANG_Ukrainian;
        pengine_data->attr.dwRange = ALC_CYRILLIC | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    case GUIIM_LANG_FINNISH:

        pengine_data->engine_lang = HWLANG_Finnish;
        pengine_data->attr.dwRange = ALC_LATIN | ALC_COMMON_PUNCTUATION | ALC_COMMON_SYMBOLS | ALC_COMMON_NUMERALS;
        break;

    default:
        break;
    }

    return TRUE;
}


PUBLIC BOOLEAN HwInitEngine (IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p == PNULL)
    {
        SCI_TRACE_LOW ("[MMIIM] HwInitEngine");
        return FALSE;
    }
    
    hw_data_p->engine_data = SCI_ALLOC_APP(sizeof(HW_HANWANG_DATA_T));
    SCI_MEMSET(hw_data_p->engine_data, 0, sizeof(HW_HANWANG_DATA_T));

    return HwInitEngineAttr (data_ptr);
}


PUBLIC BOOLEAN HwTermEngine (IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    HW_HANWANG_DATA_T * hw_data_data_ptr = PNULL;

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p == PNULL)
    {
        SCI_TRACE_LOW ("[MMIIM] HwInitEngine");
        return FALSE;
    }

    SCI_FREEIF(((HW_HANWANG_DATA_T *)hw_data_p->engine_data)->attr.pRam);
    hw_data_data_ptr = (HW_HANWANG_DATA_T *)hw_data_p->engine_data;
    SCI_FREEIF(hw_data_data_ptr);

    return TRUE;
}


LOCAL int32 HwHanWangRecognize (IM_DATA_T *data_ptr, wchar *result_ptr)
{
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    int32 result_len = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL; 
    HW_HANWANG_DATA_T *pengine_data = PNULL;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (result_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"HwHanWangRecognize"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_3248_112_2_18_2_27_59_144,(uint8*)"");
        return FALSE;
    }

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p == PNULL)
    {
        //SCI_TRACE_LOW:"HwHanWangRecognize"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_3255_112_2_18_2_27_59_145,(uint8*)"");
        return FALSE;
    }

    pengine_data = (HW_HANWANG_DATA_T *)hw_data_p->engine_data;
    if (pengine_data == PNULL)
    {
        //SCI_TRACE_LOW:"HwHanWangRecognize"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_3262_112_2_18_2_27_59_146,(uint8*)"");
        return FALSE;
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

    //MMIIM_StopTimer(&(data_ptr->is_timer_active));

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
    switch (lang)
    {
    case GUIIM_LANG_ENGLISH:
#if !defined (IM_SIMP_CHINESE_SUPPORT)        
        {
            M_THWAttribute m_attribute = {0};
            m_attribute.wMode = MODE_SINGLECHAR;
            m_attribute.wLanguage = pengine_data->engine_lang;
            m_attribute.dwRange = pengine_data->attr.dwRange;
            m_attribute.iCandidateNum = pengine_data->attr.iCandidateNum;
            m_attribute.pRam = pengine_data->attr.pRam;
            result_len = MTL_HWRecognize((const THWPoint *)hw_data_p->trace, &(m_attribute), result_ptr);
        }
#else
        {
            THWAttribute        attribute = {0};        //pclint告警消除，放置在这里
        
            attribute.dwRange = CHARSET_ENGLISH | ALC_NUMERIC | ALC_PUNC_COMMON_HALF | ALC_PUNC_RARE_HALF
                | ALC_SYM_COMMON_HALF | ALC_SYM_RARE_HALF;
            attribute.iCandidateNum = pengine_data->attr.iCandidateNum;
            attribute.pRam = pengine_data->attr.pRam;
            result_len = HWRecognize((const THWPoint *)hw_data_p->trace, &(attribute), result_ptr);
        }
#endif    
        break;

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
    case GUIIM_LANG_CHS:
    case GUIIM_LANG_CHT:
        {
            THWAttribute        attribute = {0};        //pclint告警消除，放置在这里

            attribute.dwRange = pengine_data->attr.dwRange;
            attribute.iCandidateNum = pengine_data->attr.iCandidateNum;
            attribute.pRam = pengine_data->attr.pRam;
            result_len = HWRecognize((const THWPoint *)hw_data_p->trace, &(attribute), result_ptr);
            break;
        }
#endif

#if defined (IM_ARABIC_SUPPORT)
    case GUIIM_LANG_ARABIC:
        {
            ARABIC_THWAttribute arb_attribute = {0};
            arb_attribute.dwRange = pengine_data->attr.dwRange;
            arb_attribute.iCandidateNum = pengine_data->attr.iCandidateNum;
            arb_attribute.pRam = pengine_data->attr.pRam;
            result_len = HWRecognizeArabic((const THWPoint *)hw_data_p->trace, &(arb_attribute), result_ptr);
        }
        break;
#endif

#if defined (IM_VIETNAMESE_SUPPORT)
    case GUIIM_LANG_VIETNAMESE:
        {
            VIE_THWAttribute    vie_attribute = {0};
            vie_attribute.wMode = MODE_SINGLECHAR;
            vie_attribute.wLanguage = pengine_data->engine_lang;
            vie_attribute.dwRange = pengine_data->attr.dwRange;
            vie_attribute.iCandNum = pengine_data->attr.iCandidateNum;
            vie_attribute.pRam = pengine_data->attr.pRam;
            result_len = HWRecognizeVietnamese((const THWPoint *)hw_data_p->trace, &(vie_attribute), result_ptr);
        }
        break;
#endif

#if defined (IM_THAI_SUPPORT)
    case GUIIM_LANG_THAI:
        {
            THAI_THWAttribute   thai_attribute = {0};
            thai_attribute.dwRange = pengine_data->attr.dwRange;
            thai_attribute.iCandidateNum = pengine_data->attr.iCandidateNum;
            thai_attribute.pRam = pengine_data->attr.pRam;
            thai_attribute.iBoxHeight = 0;
            thai_attribute.iBoxWidth = 0;
            //泰语的库无法与其他库共存,暂时注释掉
            result_len = HWRecognizeThai((const THWPoint *)hw_data_p->trace, &(thai_attribute), result_ptr);
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
            m_attribute.wMode = MODE_SINGLECHAR;
            m_attribute.wLanguage = pengine_data->engine_lang;
            m_attribute.dwRange = pengine_data->attr.dwRange;
            m_attribute.iCandidateNum = pengine_data->attr.iCandidateNum;
            m_attribute.pRam = pengine_data->attr.pRam;
            result_len = MTL_HWRecognize((const THWPoint *)hw_data_p->trace, &(m_attribute), result_ptr);
        }
        break;
#endif

#ifdef IM_PERSIAN_SUPPORT
    case GUIIM_LANG_PERSIAN:
        {
            P_THWAttribute p_attribute = {0};
            p_attribute.wMode = MODE_SINGLECHAR;
            p_attribute.wLanguage = pengine_data->engine_lang;
            p_attribute.dwRange = pengine_data->attr.dwRange;
            p_attribute.iCandNum  = pengine_data->attr.iCandidateNum;
            p_attribute.pRam = pengine_data->attr.pRam;
            result_len = P_HWRE_Recognize((const THWPoint *)hw_data_p->trace, &(p_attribute), result_ptr);
        }
        break;
#endif

#ifdef IM_HINDI_SUPPORT
    case GUIIM_LANG_HINDI:
        {
            THWAttribute_HINDI p_attribute = {0};
            p_attribute.wMode = MODE_SINGLECHAR;
            p_attribute.wLanguage = pengine_data->engine_lang;
            p_attribute.dwRange = pengine_data->attr.dwRange;
            p_attribute.iCandidateNum  = pengine_data->attr.iCandidateNum;
            p_attribute.pRam = pengine_data->attr.pRam;

            result_len = HWRecognize_HINDI((const THWPoint *)hw_data_p->trace, &(p_attribute), (unsigned char*)result_ptr);
        }
        break;
#endif

#ifdef IM_BENGALI_SUPPORT
    case GUIIM_LANG_BENGALI:
        {
            THWAttribute_Bengala p_attribute = {0};
            p_attribute.wMode = MODE_SINGLECHAR;
            p_attribute.wLanguage = pengine_data->engine_lang;
            p_attribute.dwRange = pengine_data->attr.dwRange;
            p_attribute.iCandidateNum  = pengine_data->attr.iCandidateNum;
            p_attribute.pRam = pengine_data->attr.pRam;
            result_len = HWRecognize_Bengala((const THWPoint *)hw_data_p->trace, &(p_attribute), (unsigned char*)result_ptr);
        }
        break;
#endif

#ifdef IM_URDU_SUPPORT
    case GUIIM_LANG_URDU:
        {
            P_THWAttribute p_attribute = {0};
            p_attribute.wMode = MODE_SINGLECHAR;
            p_attribute.wLanguage = pengine_data->engine_lang;
            p_attribute.dwRange = pengine_data->attr.dwRange;
            p_attribute.iCandNum  = pengine_data->attr.iCandidateNum;
            p_attribute.pRam = pengine_data->attr.pRam;
            result_len = P_HWRE_Recognize((const THWPoint *)hw_data_p->trace, &(p_attribute), result_ptr);
        }
        break;
#endif

#ifdef IM_HEBREW_SUPPORT
    case GUIIM_LANG_HEBREW:
        {
            H_THWAttribute h_attribute = {0};
            h_attribute.dwLanguage = pengine_data->engine_lang;
            h_attribute.dwRange = pengine_data->attr.dwRange;
            h_attribute.lCandidateNum = pengine_data->attr.iCandidateNum;
            h_attribute.pRam = pengine_data->attr.pRam;
            h_attribute.dwRamSize = HWRERAMSIZE;
            result_len = H_HWRE_Recognize((const THWPoint *)hw_data_p->trace, &(h_attribute), result_ptr);
        }
        break;
#endif

    default:
#if !defined (IM_SIMP_CHINESE_SUPPORT)    
        {
            M_THWAttribute m_attribute = {0};
            m_attribute.wMode = MODE_SINGLECHAR;
            m_attribute.wLanguage = pengine_data->engine_lang;
            m_attribute.dwRange = pengine_data->attr.dwRange;
            m_attribute.iCandidateNum = pengine_data->attr.iCandidateNum;
            m_attribute.pRam = pengine_data->attr.pRam;
            result_len = MTL_HWRecognize((const THWPoint *)hw_data_p->trace, &(m_attribute), result_ptr);
        }
#else
        {
            THWAttribute        attribute = {0};        //pclint告警消除，放置在这里
        
            attribute.dwRange = pengine_data->attr.dwRange;
            attribute.iCandidateNum = pengine_data->attr.iCandidateNum;
            attribute.pRam = pengine_data->attr.pRam;
            result_len = HWRecognize((const THWPoint *)hw_data_p->trace, &(attribute), result_ptr);
        }
#endif
        break;
    }

    //如果汉王给出的字，我们显示不了，则必须过滤掉，
    //为了尽量少调测试字符的显示与否函数，故多费一些空间
    if (0 < result_len)
    {
        int32 i = 0;
        int32 j = 0;
        uint16 disp_count = 0;
        wchar disp_label[HW_MAXCANDNUM] = {0};
        for (i = 0, j = 0; i < result_len; i++)
        {
            if (result_ptr[i] && SPMLAPI_TestChar(result_ptr[i]))    //函数对参数0返回值依然为True;
            {
                disp_label[j++] = result_ptr[i];
                disp_count++;
            }
        }
        if (disp_count != HW_MAXCANDNUM)
        {
            for (i = 0; i < result_len; i++)
            {
                result_ptr[i] = disp_label[i];
            }
        }
        result_len = disp_count;
    }

    return result_len;
}

/*==============================================================================
Description: HwEngineRecognize
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 HwEngineRecognize (IM_DATA_T *data_ptr, wchar *result_ptr)
{
    return HwHanWangRecognize (data_ptr, result_ptr);
}


/*==============================================================================
Description: HwSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HwSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}
	
    cand_key_idx = data_ptr->vkb.candidate_first;

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (0 == hw_data_p->cand_cnt)
    {
        goto HwSetCandEnd;
    }

    //set normal candidate
    while (cand_cnt < hw_data_p->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = 1;
        data_ptr->vkb.keys[cand_key_idx].text[0] = hw_data_p->cand[cand_cnt][0];
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		//将text字段数据同步到display_text字段
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
		
        cand_key_idx++;
        cand_cnt++;
    }


    if (1 == data_ptr->vkb.is_expand)
    {
        hw_data_p->cand_cnt = cand_cnt;

        if (hw_data_p->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);

        if (1 == hw_data_p->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
    }

HwSetCandEnd:

    //隐藏剩下的候选按钮
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    return;
}


#elif defined IM_HANDWRITING_SOGOU

LOCAL void HwSogouAsso (IM_DATA_T *data_ptr)
{
    wchar associate_buffer[SOGOU_HW_MAXCANDNUM << 2] = {0};
    //wchar cand_buffer[SOGOU_HW_MAXCANDNUM] = {0};
    wchar char_value[2] = {0, 0};
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA (data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    hw_data_p->edit_state = EDIT_STATE_ASSO;
    
    SCI_MEMSET(associate_buffer, 0, sizeof(associate_buffer));
    char_value[0] = hw_data_p->last_char;
    if (MMIAPIIM_GetLanguage () == GUIIM_LANG_CHS)	
    {
        WD_SwitchIme(WDI_CN_CPINYIN);
    	 WD_GetAssociationalCandidate(char_value, 2, associate_buffer, SOGOU_HW_MAXCANDNUM);        /*lint !e628 !e718 !e746*/
    }
    else if (MMIAPIIM_GetLanguage () == GUIIM_LANG_CHT)	
    {
        WD_SwitchIme(WDI_CN_ZHUYIN);
    	 WD_GetAssociationalCandidate(char_value, 2, associate_buffer, SOGOU_HW_MAXCANDNUM);        /*lint !e628 !e718 !e746*/
    }

    SCI_MEMSET ((hw_data_p->cand), 0, sizeof((hw_data_p->cand)));
    hw_data_p->cand_cnt =
        Sogou_AssociateTo2DemensionArray ((wchar *)(hw_data_p->cand), associate_buffer, HW_MAXWORDLENGTH);
    //SCI_MEMCPY ((wchar *)(hw_data_p->cand), cand_buffer, sizeof (cand_buffer));
    SetHideKey(data_ptr, hw_data_p->cand_cnt);
    HwSetCand(data_ptr);

    data_ptr->vkb.state = KEYBOARD_ASSOCIATE;

    return;
}

/*==============================================================================
Description: HwAsso
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HwAsso (IM_DATA_T *data_ptr)
{
    HwSogouAsso (data_ptr);
    return;
}


PUBLIC BOOLEAN HwAddStrokeMark (IM_DATA_T *data_ptr)
{
    GUIIM_LANGUAGE_T lang = MMIAPIIM_GetLanguage();
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA (data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->trace_len > HW_TRACE_CNT_LIMIT - 2)     //char mark保留
    {
        SCI_TRACE_LOW ("[MMIIM] HwAddStrokeMark dots overflow");
        hw_data_p->trace_len = HW_TRACE_CNT_LIMIT - 2;
    }
    
    if (lang == GUIIM_LANG_CHS || lang == GUIIM_LANG_CHT
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
            || lang == GUIIM_LANG_ENGLISH       //英文复用中文手写
#endif
        )
    {
        hw_data_p->trace[hw_data_p->trace_len].x = SOGOU_STROKEENDMARK;
        hw_data_p->trace[hw_data_p->trace_len].y = SOGOU_STROKEENDMARK;
    }
    else
    {
        hw_data_p->trace[hw_data_p->trace_len].x = SOGOU_STROKEENDMARK;
        hw_data_p->trace[hw_data_p->trace_len].y = 0;
    }

    hw_data_p->trace_len ++;
    
    return TRUE;
}


PUBLIC BOOLEAN HwAddCharMark (IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    CHECK_DATA (data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p->trace_len > HW_TRACE_CNT_LIMIT - 1)     
    {
        SCI_TRACE_LOW ("[MMIIM] HwAddCharMark dots overflow");
        hw_data_p->trace_len = HW_TRACE_CNT_LIMIT - 1;
    }
    
    hw_data_p->trace[hw_data_p->trace_len].x = SOGOU_STROKEENDMARK;
    hw_data_p->trace[hw_data_p->trace_len].y = SOGOU_STROKEENDMARK;
    hw_data_p->trace_len ++;

    return TRUE;
}



/*==============================================================================
Description: SOGOU_HwInitialize
Global resource dependence: NONE
Author: guopengzhang@sogou
Note: 
==============================================================================*/
PUBLIC BOOLEAN SOGOU_HwInitialize(GUIIM_LANGUAGE_T im_lang)
{
#if defined MMIIM_CORE_USE_SOGOU    
    WD_ImeInit(0, im_lang);
#endif

    if (im_lang == GUIIM_LANG_CHS || im_lang == GUIIM_LANG_CHT
#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT) 
        || im_lang == GUIIM_LANG_ENGLISH
#endif
        )
    {
        switch (im_lang)
        {
#ifdef IM_SIMP_CHINESE_SUPPORT
        case GUIIM_LANG_CHS:
            if (iHCR_Init((char*)g_ResData, RES_DATA_SIZE) != iHCR_OK)
            {
                //SCI_TRACE_LOW:"mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_457_112_2_18_2_25_30_54,(uint8*)"");
                return FALSE;
            }
            break;
#endif
#ifdef IM_TRAD_CHINESE_SUPPORT
        case GUIIM_LANG_CHT:
            if (iHCR_Init((char*)g_TRAD_ResData, TRAD_RES_DATA_SIZE) != iHCR_OK)
            {
                //SCI_TRACE_LOW:"mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_466_112_2_18_2_25_30_55,(uint8*)"");
                return FALSE;
            }
            break;
#endif

        case GUIIM_LANG_ENGLISH:
        default:
#ifdef IM_SIMP_CHINESE_SUPPORT
            if (iHCR_Init((char*)g_ResData, RES_DATA_SIZE) != iHCR_OK)
            {
                //SCI_TRACE_LOW:"mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_477_112_2_18_2_25_30_56,(uint8*)"");
                return FALSE;
            }
#elif defined(IM_TRAD_CHINESE_SUPPORT)
            if (iHCR_Init((char*)g_TRAD_ResData, TRAD_RES_DATA_SIZE) != iHCR_OK)
            {
                //SCI_TRACE_LOW:"mmiim_sp_hw.c SOGOU_HwInitialize iHCR_Init error!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_HW_483_112_2_18_2_25_30_57,(uint8*)"");
                return FALSE;
            }
#endif
            break;
        }

        return TRUE;
    }

    s_hw_data.width = SOGOU_HW_BASE_WIDTH;
    s_hw_data.height = SOGOU_HW_BASE_HEIGHT;
    s_hw_data.flag = 0; 
    s_hw_data.maxsize = SOGOU_HW_TRACE_CNT_LIMIT;
    switch (im_lang)
    {
    case GUIIM_LANG_ARABIC:
        s_hw_data.imeType = LANGUAGE_HW_ARABIC;
        break;
    case GUIIM_LANG_ITALIAN:
        s_hw_data.imeType = LANGUAGE_HW_ITALIAN;
        break;
    case GUIIM_LANG_ENGLISH:
        s_hw_data.imeType = LANGUAGE_HW_ENGLISH;
        break;
    case GUIIM_LANG_BENGALI: 
        s_hw_data.imeType = LANGUAGE_HW_BENGALI;
        break;
    case GUIIM_LANG_HEBREW: 
        s_hw_data.imeType = LANGUAGE_HW_HEBREW;
        break;
    case GUIIM_LANG_HINDI: 
        s_hw_data.imeType = LANGUAGE_HW_HINDI;
        break;
    case GUIIM_LANG_PERSIAN: 
        s_hw_data.imeType = LANGUAGE_HW_PERSIAN;
        break;
    case GUIIM_LANG_PUNJABI: 
        s_hw_data.imeType = LANGUAGE_HW_PUNJABI;
        break;
    case GUIIM_LANG_TAGALOG: 
        s_hw_data.imeType = LANGUAGE_HW_TAGALOG;
        break;
    case GUIIM_LANG_TAMIL: 
        s_hw_data.imeType = LANGUAGE_HW_TAMIL;
        break;
    case GUIIM_LANG_TELUGU: 
        s_hw_data.imeType = LANGUAGE_HW_TELUGU;
        break;
    case GUIIM_LANG_THAI: 
        s_hw_data.imeType = LANGUAGE_HW_THAI;
        break;
    case GUIIM_LANG_URDU: 
        s_hw_data.imeType = LANGUAGE_HW_URDU;
        break;
    case GUIIM_LANG_AFRIKAANS: 
        s_hw_data.imeType = LANGUAGE_HW_AFRIKAANS;
        break;
    case GUIIM_LANG_BULGARIAN: 
        s_hw_data.imeType = LANGUAGE_HW_BULGARIAN;
        break;
    case GUIIM_LANG_CROATIAN: 
        s_hw_data.imeType = LANGUAGE_HW_CROATIAN;
        break;
    case GUIIM_LANG_CZECH: 
        s_hw_data.imeType = LANGUAGE_HW_CZECH;
        break;
    case GUIIM_LANG_DANISH: 
        s_hw_data.imeType = LANGUAGE_HW_DANISH;
        break;
    case GUIIM_LANG_DUTCH: 
        s_hw_data.imeType = LANGUAGE_HW_DUTCH;
        break;
    case GUIIM_LANG_ESTONIAN: 
        s_hw_data.imeType = LANGUAGE_HW_ESTONIAN;
        break;
    case GUIIM_LANG_FINNISH: 
        s_hw_data.imeType = LANGUAGE_HW_FINNISH;
        break;
    case GUIIM_LANG_FRENCH: 
        s_hw_data.imeType = LANGUAGE_HW_FRENCH;
        break;
    case GUIIM_LANG_GERMAN: 
        s_hw_data.imeType = LANGUAGE_HW_GERMAN;
        break;
    case GUIIM_LANG_GREEK: 
        s_hw_data.imeType = LANGUAGE_HW_GREEK;
        break;
    case GUIIM_LANG_HAUSA: 
        s_hw_data.imeType = LANGUAGE_HW_HAUSA;
        break;
    case GUIIM_LANG_HUNGARIAN: 
        s_hw_data.imeType = LANGUAGE_HW_HUNGARIAN;
        break;
    case GUIIM_LANG_ICELANDIC: 
        s_hw_data.imeType = LANGUAGE_HW_ICELANDIC;
        break;
    case GUIIM_LANG_IGBO: 
        s_hw_data.imeType = LANGUAGE_HW_IGBO;
        break;
    case GUIIM_LANG_INDONESIAN: 
        s_hw_data.imeType = LANGUAGE_HW_INDONESIAN;
        break;
    case GUIIM_LANG_KAZAKH: 
        s_hw_data.imeType = LANGUAGE_HW_KAZAKH;
        break;
    case GUIIM_LANG_LATVIAN: 
        s_hw_data.imeType = LANGUAGE_HW_LATVIAN;
        break;
    case GUIIM_LANG_LITHUANIAN: 
        s_hw_data.imeType = LANGUAGE_HW_LITHUANIAN;
        break;
    case GUIIM_LANG_MACEDONIAN: 
        s_hw_data.imeType = LANGUAGE_HW_MACEDONIAN;
        break;
    case GUIIM_LANG_MALAY: 
        s_hw_data.imeType = LANGUAGE_HW_MALAY;
        break;
    case GUIIM_LANG_NORWEGIAN: 
        s_hw_data.imeType = LANGUAGE_HW_NORWEGIAN;
        break;
    case GUIIM_LANG_POLISH: 
        s_hw_data.imeType = LANGUAGE_HW_POLISH;
        break;
    case GUIIM_LANG_PORTUGUESE: 
        s_hw_data.imeType = LANGUAGE_HW_PORTUGUESE;
        break;
    case GUIIM_LANG_ROMANIAN: 
        s_hw_data.imeType = LANGUAGE_HW_ROMANIAN;
        break;
    case GUIIM_LANG_RUSSIAN: 
        s_hw_data.imeType = LANGUAGE_HW_RUSSIAN;
        break;
    case GUIIM_LANG_SERBIAN: 
        s_hw_data.imeType = LANGUAGE_HW_SERBIAN;
        break;
    case GUIIM_LANG_SLOVAK: 
        s_hw_data.imeType = LANGUAGE_HW_SLOVAK;
        break;
    case GUIIM_LANG_SLOVENIAN: 
        s_hw_data.imeType = LANGUAGE_HW_SLOVENIAN;
        break;
    case GUIIM_LANG_SPANISH: 
        s_hw_data.imeType = LANGUAGE_HW_SPANISH;
        break;
    case GUIIM_LANG_SWAHILI: 
        s_hw_data.imeType = LANGUAGE_HW_SWAHILI;
        break;
    case GUIIM_LANG_SWEDISH: 
        s_hw_data.imeType = LANGUAGE_HW_SWEDISH;
        break;
    case GUIIM_LANG_TURKISH: 
        s_hw_data.imeType = LANGUAGE_HW_TURKISH;
        break;
    case GUIIM_LANG_UKRAINIAN: 
        s_hw_data.imeType = LANGUAGE_HW_UKRAINIAN;
        break;
    case GUIIM_LANG_VIETNAMESE: 
        s_hw_data.imeType = LANGUAGE_HW_VIETNAMESE;
        break;
    case GUIIM_LANG_MYANMAR:
        s_hw_data.imeType = LANGUAGE_HW_BURMESE;
        break;
        
    // 部分语言目前搜狗输入法不支持手写
    case GUIIM_LANG_MARATHI: 
    default:
        s_hw_data.imeType = LANGUAGE_HW_UNKNOWN;
        break;
    }

    WD_FoHwInitialize(&s_hw_data, PNULL);

    return TRUE;
}


PUBLIC BOOLEAN HwInitEngine (IM_DATA_T *data_ptr)
{
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (hw_data_p == PNULL)
    {
        SCI_TRACE_LOW ("[MMIIM] HwInitEngine");
        return FALSE;
    }
    
    SCI_MEMSET(&g_imeResult, 0, sizeof(WD_imeResult));
    hw_data_p->engine_data = &g_imeResult;

    SOGOU_HwInitialize(MMIAPIIM_GetLanguage());

    return TRUE;
}


PUBLIC BOOLEAN HwTermEngine (IM_DATA_T *data_ptr)
{
    switch (MMIAPIIM_GetLanguage())
    {
        case GUIIM_LANG_CHS:
        case GUIIM_LANG_CHT:
#if defined IM_SIMP_CHINESE_SUPPORT ||defined IM_TRAD_CHINESE_SUPPORT
        case GUIIM_LANG_ENGLISH:        //英文复用中文的手写
            iHCR_Exit();
#endif
            break;

        default:
            break;
    }

#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_SGFreeImeSkinBuffer();
#endif
    SG_Quit();

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_HwRecognize

Func:   sogou手写识别

Input:
            lang:   识别字符语言
            trace:  笔迹点数组
            trace_len:  笔迹点长度
            psave_result:   保存识别出的字符数组
            max_cand_num:   识别最多字符个数

Output:
            psave_result: 写掉该指针，将数据保存到该指针内

Return:
            recognize_res:识别出的字符个数，错误或没有识别出字符返回0
Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
LOCAL long Sogou_HwRecognize(
    GUIIM_LANGUAGE_T lang,
    iHCR_POINT_T* trace,
    long trace_len,
    unsigned short *psave_result,
    long max_cand_num)
{
    long recognize_res = 0;

    if (trace == PNULL)
    {
        //SCI_TRACE_LOW:"Sogou_HwRecognize"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_3779_112_2_18_2_28_0_147,(uint8*)"");
        return 0;
    }
    //SCI_TRACE_LOW:"mmiim_touch_sogou.c Sogou_HwRecognize lang = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_3782_112_2_18_2_28_0_148,(uint8*)"d", lang);

    if (trace_len > HW_TRACE_CNT_LIMIT)
    {
        SCI_TRACE_LOW ("[MMIIM] Sogou_HwRecognize err trace_len");
        return 0;
    }
    
    switch (lang)
    {
#if defined (IM_SIMP_CHINESE_SUPPORT)
    case GUIIM_LANG_CHS:
        iHCR_SetParam(iHCR_PARAM_LANGUAGE,  iHCR_LANGUAGE_Chinese_GB | iHCR_LANGUAGE_Numerals | iHCR_LANGUAGE_Symbols | iHCR_LANGUAGE_English);
        recognize_res = iHCR_Recognize((iHCR_POINT_T*)trace, trace_len, 
            psave_result, max_cand_num);	  
        break;
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
    case GUIIM_LANG_CHT:
        iHCR_SetParam(iHCR_PARAM_LANGUAGE,  iHCR_LANGUAGE_Chinese_Big5 | iHCR_LANGUAGE_Numerals | iHCR_LANGUAGE_Symbols | iHCR_LANGUAGE_English);
	    recognize_res = iHCR_Recognize((iHCR_POINT_T*)trace, trace_len, 
            psave_result, max_cand_num);	  
        break;
#endif

#if defined (IM_ARABIC_SUPPORT)
    case GUIIM_LANG_ARABIC:
        {
            WD_HwSwitchIme(LANGUAGE_HW_ARABIC);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_BENGALI_SUPPORT
    case GUIIM_LANG_BENGALI:
        WD_HwSwitchIme(LANGUAGE_HW_BENGALI);
        recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
        recognize_res = max_cand_num;
        break;
#endif

#if defined (IM_VIETNAMESE_SUPPORT)
    case GUIIM_LANG_VIETNAMESE:
        {
            WD_HwSwitchIme(LANGUAGE_HW_VIETNAMESE);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#if defined (IM_THAI_SUPPORT)
    case GUIIM_LANG_THAI:
        {
            WD_HwSwitchIme(LANGUAGE_HW_THAI);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#if defined (IM_FRENCH_SUPPORT)
    case GUIIM_LANG_FRENCH:
        WD_HwSwitchIme(LANGUAGE_HW_FRENCH);
        recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
        recognize_res = max_cand_num;
        break;
#endif

#if defined (IM_ITALIAN_SUPPORT)
    case GUIIM_LANG_ITALIAN:
        WD_HwSwitchIme(LANGUAGE_HW_ITALIAN);
        recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
        recognize_res = max_cand_num;
        break;
#endif

#ifdef IM_PERSIAN_SUPPORT
    case GUIIM_LANG_PERSIAN:
        {
            WD_HwSwitchIme(LANGUAGE_HW_PERSIAN);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_URDU_SUPPORT
    case GUIIM_LANG_URDU:
        {
            WD_HwSwitchIme(LANGUAGE_HW_URDU);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_HEBREW_SUPPORT
    case GUIIM_LANG_HEBREW:
        {
            WD_HwSwitchIme(LANGUAGE_HW_HEBREW);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_RUSSIAN_SUPPORT
    case GUIIM_LANG_RUSSIAN:
        {
            WD_HwSwitchIme(LANGUAGE_HW_RUSSIAN);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_HINDI_SUPPORT
    case GUIIM_LANG_HINDI:
        {
            WD_HwSwitchIme(LANGUAGE_HW_HINDI);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_TAGALOG_SUPPORT
    case GUIIM_LANG_TAGALOG:
        {
            WD_HwSwitchIme(LANGUAGE_HW_TAGALOG);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_TAMIL_SUPPORT
    case GUIIM_LANG_TAMIL:
        {
            WD_HwSwitchIme(LANGUAGE_HW_TAMIL);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_TELUGU_SUPPORT
    case GUIIM_LANG_TELUGU:
        {
            WD_HwSwitchIme(LANGUAGE_HW_TELUGU);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_GERMAN_SUPPORT
    case GUIIM_LANG_GERMAN:
        {
            WD_HwSwitchIme(LANGUAGE_HW_GERMAN);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_GREEK_SUPPORT
    case GUIIM_LANG_GREEK:
        {
            WD_HwSwitchIme(LANGUAGE_HW_GREEK);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_INDONESIAN_SUPPORT
    case GUIIM_LANG_INDONESIAN:
        {
            WD_HwSwitchIme(LANGUAGE_HW_INDONESIAN);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_MALAY_SUPPORT
    case GUIIM_LANG_MALAY:
        {
            WD_HwSwitchIme(LANGUAGE_HW_MALAY);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_PORTUGUESE_SUPPORT
    case GUIIM_LANG_PORTUGUESE:
        {
            WD_HwSwitchIme(LANGUAGE_HW_PORTUGUESE);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_SPANISH_SUPPORT
    case GUIIM_LANG_SPANISH:
        {
            WD_HwSwitchIme(LANGUAGE_HW_SPANISH);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

#ifdef IM_MYANMAR_SUPPORT
    case GUIIM_LANG_MYANMAR:
        {
            WD_HwSwitchIme(LANGUAGE_HW_BURMESE);
            recognize_res = WD_HwOnIdentify_Ex((uint16*)trace,(uint16*)psave_result,0);
            recognize_res = max_cand_num;
        }
        break;
#endif

    case GUIIM_LANG_ENGLISH:        //可以复用中文识别库
    default:
#if defined (IM_SIMP_CHINESE_SUPPORT)
        iHCR_SetParam(iHCR_PARAM_LANGUAGE,   iHCR_LANGUAGE_English | iHCR_LANGUAGE_Numerals | iHCR_LANGUAGE_Symbols);
        recognize_res = iHCR_Recognize((iHCR_POINT_T*)trace, trace_len, psave_result, max_cand_num);
#elif defined (IM_TRAD_CHINESE_SUPPORT)
        iHCR_SetParam(iHCR_PARAM_LANGUAGE,   iHCR_LANGUAGE_English | iHCR_LANGUAGE_Numerals | iHCR_LANGUAGE_Symbols);
        recognize_res = iHCR_Recognize((iHCR_POINT_T*)trace, trace_len, psave_result, max_cand_num);
#else
        WD_HwSwitchIme(LANGUAGE_HW_ENGLISH);
        recognize_res = WD_HwOnIdentify_Ex((uint16*)trace, (uint16*)psave_result, 0);
        recognize_res = max_cand_num;
#endif
        break;
    }

    recognize_res = recognize_res < max_cand_num ? recognize_res : max_cand_num;
    
    return recognize_res;
}


/*==============================================================================
Description: HwEngineRecognize
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 HwEngineRecognize (IM_DATA_T *data_ptr, wchar *result_ptr)
{
    long recognize_res = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA (data_ptr);

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    recognize_res = Sogou_HwRecognize(
        MMIAPIIM_GetLanguage(),
        hw_data_p->trace, 
        hw_data_p->trace_len, 
        result_ptr, 
        SOGOU_HW_MAXCANDNUM);

    return recognize_res;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_CandToImeResult

Func:   将sogou手写识别出的数据保存到WD_imeResult数据结构中

Input:
            pime_result:  输入法输入输出数据
            pcand: 手写识别出的数据

Output:
            pime_result: 写掉该指针，将数据保存到该指针内

Return:
            err: < 0  succ: == 0

Thoughtway:
            1.  之所以添加该函数，是因为手写识别出的数据不是连续的字符，而是通过特殊分割
                字符0x002c来将识别出的字符分隔开的，该数据不能直接被我们的UI上层使用

Create: yaoguangchen@spread. 2011-01-18

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_CandToImeResult(WD_imeResult *pime_result, wchar *pcand)
{
    uint32 iloop = 0, cand_cur = 0;

    if (pime_result == PNULL || pcand == PNULL)
    {
        //SCI_TRACE_LOW:"Sogou_CandToImeResult"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_HW_4078_112_2_18_2_28_1_149,(uint8*)"");
        return -1;
    }

    pime_result->selecting[iloop][0] = pcand[0];
    cand_cur = 1;
    for (iloop = 2; iloop < SOGOU_HW_MAXCANDNUM << 2; iloop++)
    {
        if (pcand[iloop] == 0 || cand_cur >= MAX_ONEPAGE_RECORD)
        {
            break;
        }

        if (pcand[iloop-1] == 0x002c)     //分隔符
        {
            pime_result->selecting[cand_cur++][0] = pcand[iloop];
        }
    }

    return 0;
}


/*==============================================================================
Description: HwSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HwSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    KERNEL_DATA_HW_T *hw_data_p = PNULL;
    
    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}
	
    cand_key_idx = data_ptr->vkb.candidate_first;

    hw_data_p = (KERNEL_DATA_HW_T*)(data_ptr->data);
    if (0 == hw_data_p->cand_cnt)
    {
        goto HwSetCandEnd;
    }

    //set normal candidate
    while (cand_cnt < hw_data_p->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        MMIAPICOM_Wstrncpy (data_ptr->vkb.keys[cand_key_idx].text, hw_data_p->cand[cand_cnt], TEXT_LEN_LIMIT - 1);
        data_ptr->vkb.keys[cand_key_idx].text_len = MMIAPICOM_Wstrlen (data_ptr->vkb.keys[cand_key_idx].text);
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		//将text字段数据同步到display_text字段
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));

		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);	
		
        cand_key_idx++;
        cand_cnt++;
    }

    if (1 == data_ptr->vkb.is_expand)
    {
        hw_data_p->cand_cnt = cand_cnt;

        if (hw_data_p->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);

        if (1 == hw_data_p->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
    }

HwSetCandEnd:

    //隐藏剩下的候选按钮
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    return;
}
#endif



#endif  //((defined IM_HANDWRITING_HANWANG  && defined IM_ENGINE_CSTAR) || (defined IM_HANDWRITING_SOGOU && defined IM_ENGINE_SOGOU))

#endif  //defined (MMI_IM_PDA_SUPPORT) 



/*Edit by script, ignore 4 case. Thu Apr 26 19:00:56 2012*/ //IGNORE9527
