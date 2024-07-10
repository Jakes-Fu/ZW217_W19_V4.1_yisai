/*==============================================================================
File Name: mmiim_touch_ime_common.c
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
#include "mmiim_touch_ime_common.h"


#if defined (MMI_IM_PDA_SUPPORT)

//虚拟键盘丝印部分需要
#include "mmiim_sp_ml26key_data.h"
#include "mmiim_sp_ml9key_data.h"


static wchar const s_symbol_0xa3[] = {0xa3, 0};
static wchar const s_symbol_0xa5[] = {0xa5, 0};
static wchar const s_symbol_0xbf[] = {0xbf, 0};
#ifdef IM_HINDI_SUPPORT		//the special symbol
static wchar const s_symbol_0x20a8[] = {0x20a8, 0};		//hindi symbol
static wchar const s_symbol_0x20b9[] = {0x20b9, 0};		//hindi symbol
#endif
//符号数据
static wchar const *s_sys_list_half_lower[] =
{
    //first page
    L"1", L"2", L"3",
    L"4", L"5", L"6",
    L"7", L"8", L"9",
    L"*", L"0", L"#",

    //second page
    L"a", L"b", L"c",
    L"d", L"e", L"f",
    L"g", L"h", L"i",
    L"j", L"k", L"l",

    //third page
    L"m", L"n", L"o",
    L"p", L"q", L"r",
    L"s", L"t", L"u",
    L"v", L"w", L"x",

    //four page
    L"y", L"z", L",",
    L".", L"!", L"?",
    L"~", L"@", L"%",
    L"/", L"(", L")",

    //five page
    L";", L"[", L"]",
    L"\'", L"+", L"\\",
    L"<", L">", L",",
    L"&", L":", L"|",

};

//符号数据
static wchar const *s_sys_list_half_upper[] =
{
    //first page
    L"1", L"2", L"3",
    L"4", L"5", L"6",
    L"7", L"8", L"9",
    L"*", L"0", L"#",

    //second page
    L"A", L"B", L"C",
    L"D", L"E", L"F",
    L"G", L"H", L"I",
    L"J", L"K", L"L",

    //third page
    L"M", L"N", L"O",
    L"P", L"Q", L"R",
    L"S", L"T", L"U",
    L"V", L"W", L"X",

    //four page
    L"Y", L"Z", L",",
    L".", L"!", L"?",
    L"~", L"@", L"%",
    L"/", L"(", L")",

    //five page
    L";", L"[", L"]",
    L"\'", L"+", L"\\",
    L"<", L">", L",",

    L"&", L":", L"|",

};

//符号数据
static wchar const *s_sym_list_half[] =
{
    L".", L",", L"1", L"2", L"3",
    L"?", L"!", L"4", L"5", L"6",
    L"%", L"'", L"7", L"8", L"9",
    L":", L"-", L"*", L"0", L"#",

	//second page
#ifdef IM_HINDI_SUPPORT		//the special symbol
    s_symbol_0x20a8, L",", L"?", L"!", L"\'",
    s_symbol_0x20b9, L";", L"*", L"#", L"\"",
#else
	L".", L",", L"?", L"!", L"\'",
	L":", L";", L"*", L"#", L"\"",
#endif
    L"%", L"&", L"@", L"+", L"-",
    L"=", L"_", L"$", L"/", L"\\",

    s_symbol_0xbf, L"^", L"(", L")", L"~",
    L"<", L">", L"[", L"]", L"|",
    L"{", L"}", s_symbol_0xa3, s_symbol_0xa5, L"\x20ac",
    L"http://", L"www.", L".com", L".cn", L".net",

    L":-)", L":-(", L";-)", L":-P", L":-Q",
    L":-|", L":-*", L":-X", L":-D", L":-O",
    L"-_-!", L"-_-#", L"*^_^*", L"@@", L"o_O",
    L":-\\", L"=_=", L"?_?", L"$_$", L"T^T",
};

static wchar const *s_sym_list_full[] =
{
    L"\x3002", L"\xff0c", L"1", L"2", L"3",
    L"\xff1f", L"\xff01", L"4", L"5", L"6",
    L"\x2026", L"\xff1a", L"7", L"8", L"9",
    L"\xffe5", L"\xff1b", L"*", L"0", L"#",

    L"\x3002", L"\xff0c", L"\x3001", L"\xff1f", L"\xff01",
    L"\xff1a", L"\xff1b", L"\x300a", L"\x300b", L"\xffe5",
    L"\x201c", L"\x201d", L"\x2018", L"\x2019", L"\x2026",
    L"\x3010", L"\x3011", L"\x300e", L"\x300f", L"\xff0a",

    s_symbol_0xbf, L"^", L"(", L")", L"~",
    L"<", L">", L"[", L"]", L"|",
    L"{", L"}", s_symbol_0xa3, s_symbol_0xa5, L"\x20ac",
    L"http://", L"www.", L".com", L".cn", L".net",

#if defined GBK_SUPPORT        //L"\x256f\x005f\x2570", L"\x2570\x005f\x256f",GBK字符集
    L":-)", L":-(", L";-)", L":-P", L"\x256f\x005f\x2570",
    L":-|", L":-*", L"\x2299\x003f\x2299", L":-D", L"\x2570\x005f\x256f",
    L"-_-!", L"-_-#", L"*^_^*", L"@@", L"o_O",
    L":-\\", L"=_=", L"?_?", L"$_$", L"T^T",
#else
    L":-)", L":-(", L";-)", L":-P", L":-Q",
    L":-|", L":-*", L"\x2299\x003f\x2299", L":-D", L":-O",
    L"-_-!", L"-_-#", L"*^_^*", L"@@", L"o_O",
    L":-\\", L"=_=", L"?_?", L"$_$", L"T^T",
#endif
};

#if defined IM_BENGALI_SUPPORT
static wchar const s_symbol_0x09CD[] = {0x09CD, 0};
static wchar const s_symbol_0x0964[] = {0x0964, 0};
static wchar const s_symbol_0x09BC[] = {0x09BC, 0};
static wchar const s_symbol_0x09F3[] = {0x09F3, 0};
//符号数据
static wchar const *s_sym_list_bengali[] =
{
    s_symbol_0x09CD, L",", L"1", L"2", L"3",
    s_symbol_0x0964, L"?", L"4", L"5", L"6",
    s_symbol_0x09BC, L"!", L"7", L"8", L"9",
    s_symbol_0x09F3, L"@", L"*", L"0", L"#",

    L".", L",", L"?", L"!", L"\'",
    L":", L";", L"*", L"#", L"\"",
    L"%", L"&", L"-", L"+", L"-",
    L"=", L"_", L"$", L"/", L"\\",

    s_symbol_0xbf, L"^", L"(", L")", L"~",
    L"<", L">", L"[", L"]", L"|",
    L"{", L"}", s_symbol_0xa3, s_symbol_0xa5, L"\x20ac",
    L"http://", L"www.", L".com", L".cn", L".net",

    L":-)", L":-(", L";-)", L":-P", L":-Q",
    L":-|", L":-*", L":-X", L":-D", L":-O",
    L"-_-!", L"-_-#", L"*^_^*", L"@@", L"o_O",
    L":-\\", L"=_=", L"?_?", L"$_$", L"T^T",
};
#endif


LOCAL int32 ForeignSymList (GUIIM_LANGUAGE_T guiim_lang, wchar const **symb_list, uint32 *sym_cnt);

/*==============================================================================
Description: SysDraw
Global resource dependence: NONE
Author: feiyue.ji
Note:
==============================================================================*/
PUBLIC void SysDraw(IM_DATA_T * data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}

/*==============================================================================
Description: 更新符号面板的显示
Global resource dependence: NONE
Author: feiyue.ji
Note:
==============================================================================*/
PUBLIC void SysUpdate(IM_DATA_T *data_ptr)
{
    wchar const **symb_list = PNULL;
    uint32 sym_cnt = 0;
    uint32 sym_offset = 0;
    uint32 key_idx = 0;

    CHECK_DATA_EX(data_ptr);

    if(CAPITTAL_MODE_UPPER == data_ptr->vkb.cap)
    {
        symb_list = s_sys_list_half_upper;
        sym_cnt = ARR_SIZE(s_sys_list_half_upper);
    }
    else
    {
        symb_list = s_sys_list_half_lower;
        sym_cnt = ARR_SIZE(s_sys_list_half_lower);
    }

    sym_offset = data_ptr->vkb.custom_cnt * ((SYS_DATA_T*)(data_ptr->data))->cur_page;

    //SCI_ASSERT(sym_offset + data_ptr->vkb.custom_cnt <= sym_cnt); /*assert verified*/
    if (sym_offset + data_ptr->vkb.custom_cnt > sym_cnt)
    {
        return;
    }

    for (key_idx = 0; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
    {
        uint8 abs_key_idx = data_ptr->vkb.custom_first + key_idx;
        wchar const *sym_ptr = PNULL;

        sym_ptr = symb_list[sym_offset + key_idx];
        data_ptr->vkb.keys[abs_key_idx].text_len = 0;

        while (*sym_ptr != 0)
        {
            data_ptr->vkb.keys[abs_key_idx].text[data_ptr->vkb.keys[abs_key_idx].text_len] = *sym_ptr;
            data_ptr->vkb.keys[abs_key_idx].text_len++;
            sym_ptr++;
        }
    }

    data_ptr->vkb.tip_key->text[0] = '1' + ((SYS_DATA_T*)(data_ptr->data))->cur_page;

    if (((SYS_DATA_T*)(data_ptr->data))->cur_page > 0)
    {
        data_ptr->vkb.pgup_key->is_inactive = 0;
    }
    else
    {
        data_ptr->vkb.pgup_key->is_inactive = 1;
    }

    if (((SYS_DATA_T*)(data_ptr->data))->cur_page + 1 < SYS_PAGES)
    {
        data_ptr->vkb.pgdn_key->is_inactive = 0;
    }
    else
    {
        data_ptr->vkb.pgdn_key->is_inactive = 1;
    }
    data_ptr->vkb.pgup_key->icon = IMAGE_IME_SYS_LEFT;
    data_ptr->vkb.pgdn_key->icon = IMAGE_IME_SYS_RIGHT;

    if((((SYS_DATA_T*)(data_ptr->data))->cur_page == SYS_PAGE_NUM)
        || ((((SYS_DATA_T*)(data_ptr->data))->cur_page == SYS_PAGE_SYM)))
    {
        data_ptr->vkb.caps_key->is_inactive = 1;
        data_ptr->vkb.caps_key->icon = IMAGE_IME_SYS_SHIFT_ON_DISABLE;
    }
    else
    {
        data_ptr->vkb.caps_key->is_inactive = 0;
        switch (data_ptr->vkb.cap)
        {
            case CAPITTAL_MODE_LOWER:
            data_ptr->vkb.caps_key->icon = IMAGE_IME_SYS_SHIFT;
            break;

            default:
            SCI_TraceLow("SysUpdate data_ptr->vkb.cap:%d", data_ptr->vkb.cap);
            data_ptr->vkb.caps_key->icon = IMAGE_IME_SYS_SHIFT_ON;
            break;
        }

    }
    return;
}
/*==============================================================================
Description: SysTerm
Global resource dependence: NONE
Author: feiyue.ji
Note:
==============================================================================*/
PUBLIC BOOLEAN SysTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}
/*==============================================================================
Description: 设置
Global resource dependence: NONE
Author: feiyue.ji
Note:
==============================================================================*/
LOCAL void SetSysVkbNextCaps(IM_DATA_T *data_ptr)
{
    CAPITTAL_MODE_T next_cap;
    GUIIM_CAPITAL_MODE_T im_cap = GUIIM_MODE_LEAD_UPPER;

    CHECK_DATA_EX(data_ptr);

    if(PNULL == data_ptr->vkb.caps_key)
    {
        return ;
    }

    switch (data_ptr->vkb.cap)
    {
    case CAPITTAL_MODE_LOWER:
        next_cap = CAPITTAL_MODE_UPPER;
        break;

    default:
        SCI_TraceLow("SetSysVkbNextCaps data_ptr->vkb.cap:%d", data_ptr->vkb.cap);
        next_cap = CAPITTAL_MODE_LOWER;
        break;
    }

   if (data_ptr->vkb.cap == next_cap)
    {
        return;
    }

    if (data_ptr->vkb.caps_key == PNULL)
    {
        return;
    }

    switch (next_cap)
    {
    case CAPITTAL_MODE_LOWER:
        im_cap = GUIIM_MODE_LOWER;
        data_ptr->vkb.caps_key->icon = IMG_IMT_CAP_L;

        break;

    case CAPITTAL_MODE_UPPER:
        im_cap = GUIIM_MODE_UPPER;
        data_ptr->vkb.caps_key->icon = IMG_IMT_CAP_U;

        break;

    default:
        break;
    }
    data_ptr->vkb.cap = next_cap;

    return;
}

/*==============================================================================
Description: 输入按键提起触笔 - control键
Global resource dependence: NONE
Author: feiyue.ji
Note:
==============================================================================*/
PUBLIC BOOLEAN SysCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    switch (key_ptr->code)
    {

    case VKEY_PAGEUP:
        if (((SYS_DATA_T*)(data_ptr->data))->cur_page == 0)
        {
            break;
        }
        ((SYS_DATA_T*)(data_ptr->data))->cur_page--;
        SysUpdate(data_ptr);

        //将text字段数据同步到display_text字段，以备显示使用，免得再每个地方修改
        MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

        MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr);
        MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
        break;

    case VKEY_PAGEDOWN:
        if (((SYS_DATA_T*)(data_ptr->data))->cur_page + 1 >= SYS_PAGES)
        {
            break;
        }

        ((SYS_DATA_T*)(data_ptr->data))->cur_page++;
        SysUpdate(data_ptr);

        //将text字段数据同步到display_text字段，以备显示使用，免得再每个地方修改
        MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

        MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr);
        MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
        break;

    case VKEY_DEL:
        CommitChar(data_ptr, '\b');
        break;

    case VKEY_CAPS:
        SetSysVkbNextCaps(data_ptr);
        SysUpdate(data_ptr);
        //将text字段数据同步到display_text字段，以备显示使用，免得再每个地方修改
        MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

        MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr);
        MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 输入按键提起触笔 - character键
Global resource dependence: NONE
Author: feiyue.ji
Note:
==============================================================================*/
PUBLIC BOOLEAN SysCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CapsCommitStr (data_ptr, key_ptr->text, key_ptr->text_len);

    //将text字段数据同步到display_text字段，不需要在每个text调用地方修改
    MMIIM_TOUCH_SynchronizeAllDisplayText(data_ptr);

    key_ptr->is_press = 0;
    MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);//与其他键没有关联性，直接绘制，不要画整个键盘
    DrawVkb (data_ptr);

    return TRUE;
}
/*==============================================================================
Description: 初始化键盘为拇指英文字母输入
Global resource dependence: NONE
Author: feiyue.ji
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void SysInitKb(IM_DATA_T *data_ptr)
{
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width = 0;
    int16 panel_height = 0;
    uint32 keys_size = 0;
    uint32 key_idx = 0;
    uint32 const CHAR_CNT = SYS_COL*SYS_ROW;
    uint32 const CTRL_CNT = SYS_CTRL_CNT;
    uint32 ctrl_begin = 0;

    CHECK_DATA(data_ptr);
    data_ptr->theme = g_theme_sys_default;
    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    vkb_ptr = &data_ptr->vkb;
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_sys_panel_position_size;

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    key_idx = 0;

    for (; key_idx < CHAR_CNT; key_idx++)
    {
            ConvertRect(panel_width, panel_height,
                    &(g_sys_panel_position[key_idx]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        //vkb_ptr->keys[key_idx].rect = g_sys_panel_position[key_idx];
        vkb_ptr->keys[key_idx].code = VKEY_SYMB_STR + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_ALL_TEXT;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = SysCharUp;
    }
    vkb_ptr->custom_first = 0;
    vkb_ptr->custom_cnt = CHAR_CNT;

    ctrl_begin = key_idx;
    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &g_sys_panel_position[key_idx],
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = SysCtrlUp;
    }
    //--del
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMAGE_IME_SYS_DEL;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;

    //--Cap
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_CAPS;
    //特别的记住caps key
    vkb_ptr->caps_key = vkb_ptr->keys + key_idx;

    // back
    key_idx++;
    vkb_ptr->back_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMAGE_IME_SYS_HIDE;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;

    //--pgup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;

    //insert page tips
    //--page tips
    key_idx++;
    vkb_ptr->tip_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGE;
    vkb_ptr->keys[key_idx].text_len = 3;
    vkb_ptr->keys[key_idx].text[0] = '0';
    vkb_ptr->keys[key_idx].text[1] = '/';
    vkb_ptr->keys[key_idx].text[2] = '0' + SYS_PAGES;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;


    //--pgdn
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;

    //更新显示数据
    SysUpdate(data_ptr);

    //将text字段数据同步到display_text字段，以备显示使用，免得再每个地方修改
    MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

    MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr);

}
/*==============================================================================
Description: SymInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN SymInit(IM_DATA_T *data_ptr, SYMBOL_TYPE_T type, SYMBOL_INIT_T init)
{
    SYMBOL_DATA_T *sym_data = PNULL;

    CHECK_DATA(data_ptr);

    data_ptr->f_term_imm(data_ptr);

    data_ptr->is_symbol_panel = TRUE;

    sym_data = (SYMBOL_DATA_T*)SCI_ALLOC_APP(sizeof(SYMBOL_DATA_T));
    SCI_MEMSET(sym_data, 0, sizeof(SYMBOL_DATA_T));

    sym_data->type = type;

    switch (init)
    {
    case SYMBOL_INIT_NUM:
        sym_data->cur_page = SYMBOL_PAGE_NUM;
        sym_data->is_lock = 1;
        break;

    case SYMBOL_INIT_FACE:
        sym_data->cur_page = SYMBOL_PAGE_FACE;
        sym_data->is_lock = 0;
        break;

        //case SYMBOL_INIT_SYM:
    default:
        sym_data->cur_page = SYMBOL_PAGE_SYM;
        sym_data->is_lock = 0;
        break;

    }

    data_ptr->data = sym_data;

    SymInitKb(data_ptr);

	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return TRUE;
}



/*==============================================================================
Description: SysInit
Global resource dependence: NONE
Author: feiyue.ji
Note:
==============================================================================*/
PUBLIC BOOLEAN SysInit(IM_DATA_T *data_ptr, uint8 page_index)
{
    SYS_DATA_T *sym_data = PNULL;
    uint8 cur_page = 0;

    CHECK_DATA(data_ptr);
    SCI_ASSERT((PNULL == data_ptr->data));

    sym_data = (SYS_DATA_T*)SCI_ALLOC_APP(sizeof(SYS_DATA_T));
    if(PNULL == sym_data)
    {
        SCI_TraceLow("[input]sys data malloc fail!");
        return FALSE;
    }
    SCI_MEMSET(sym_data, 0, sizeof(SYS_DATA_T));

    if(page_index < SYS_PAGES)
    {
        cur_page = page_index;
    }
    sym_data->cur_page = cur_page;

    data_ptr->data = sym_data;

    data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
    SysInitKb(data_ptr);

    MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return TRUE;
}

/*==============================================================================
Description: SymTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN SymTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: SymDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void SymDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*==============================================================================
Description: 初始化键盘为拇指英文字母输入
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void SymInitKb(IM_DATA_T *data_ptr)
{
    uint32 const CHAR_CNT = SYMBOL_PAGE_SIZE;
    uint32 const CTRL_CNT = SYMBOL_COL;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    uint32 keys_size = 0;
    uint32 key_idx = 0;
    uint32 ctrl_begin = 0;
        
    CHECK_DATA(data_ptr);

    vkb_ptr = &(data_ptr->vkb);

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
   if (data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING)    
    {
        //恢复面板默认大小
        SetPanel (data_ptr);

        //Notes!! 切换到symbol时，需要通知IM控件，正常键盘区域大小，否则symbol没有初始化控件
        //而不知区域,导致某些控件被IM控件给挡住而无法处理消息
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};
            
            notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
            GUIIM_SetNotify (data_ptr->init_param.im_handle, &notify_pack);
        }
   }
#endif

    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = SYMBOL_PAGE_SIZE + SYMBOL_COL + 1; //+1是特别多出来的页提示

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    data_ptr->theme = g_theme_symbol_default;
    //5*4
    key_idx = 0;

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        int16 x = key_idx % SYMBOL_COL;
        int16 y = key_idx / SYMBOL_COL;
        int16 w = 1000;
        int16 h = 1000;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = y * h / (SYMBOL_ROW + 1);
        cand_rect.bottom = (y + 1) * h / (SYMBOL_ROW + 1) - 1;
        cand_rect.left = x * w / SYMBOL_COL;
        cand_rect.right = (x + 1) * w / SYMBOL_COL - 1;
            
        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_SYMB_STR + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_ALL_TEXT;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = SymCharUp;
    }

    vkb_ptr->custom_first = 0;
    vkb_ptr->custom_cnt = CHAR_CNT;

    //control * 6, back, pgup, (page), pgdn, del, lock
    //其中，page的位置由pgup和pgdn挤出来，三个分两份空间，各2/3
    //character key 的位置索引从开始到30
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        int16 x = key_idx - ctrl_begin;
        int16 y = SYMBOL_ROW;
        int16 w = 1000;
        int16 h = 1000;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = y * h / (SYMBOL_ROW + 1);
        cand_rect.bottom = (y + 1) * h / (SYMBOL_ROW + 1) - 1;
        cand_rect.left = x * w / SYMBOL_COL;
        cand_rect.right = (x + 1) * w / SYMBOL_COL - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = SymCtrlUp;
    }

    //--back
    key_idx = ctrl_begin;
    vkb_ptr->back_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_BACK;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_BACK;
    //--pgup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    //--pgdn
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    //--del
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;
    //--lock
    key_idx++;
    vkb_ptr->lock_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_LOCK;
    vkb_ptr->keys[key_idx].icon = ((SYMBOL_DATA_T*)(data_ptr->data))->is_lock ? IMG_IMT_LOCK : IMG_IMT_UNLOCK;

    //insert page tips
    //--page tips
    key_idx++;
    vkb_ptr->tip_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].rect.top = vkb_ptr->pgup_key->rect.top;
    vkb_ptr->keys[key_idx].rect.bottom = vkb_ptr->pgup_key->rect.bottom;
    vkb_ptr->keys[key_idx].rect.left = vkb_ptr->pgup_key->rect.left / 3 + vkb_ptr->pgup_key->rect.right * 2 / 3;
    vkb_ptr->keys[key_idx].rect.right = vkb_ptr->pgdn_key->rect.left * 2 / 3 + vkb_ptr->pgdn_key->rect.right / 3;
    vkb_ptr->keys[key_idx].code = VKEY_PAGE;
    vkb_ptr->keys[key_idx].text_len = 3;
    vkb_ptr->keys[key_idx].text[0] = '0';
    vkb_ptr->keys[key_idx].text[1] = '/';
    vkb_ptr->keys[key_idx].text[2] = '0' + SYMBOL_PAGES;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = SymCtrlUp;
    //modify pgup & pgdn 's rect
    vkb_ptr->pgup_key->rect.right = vkb_ptr->keys[key_idx].rect.left - 1;
    vkb_ptr->pgdn_key->rect.left = vkb_ptr->keys[key_idx].rect.right + 1;


    //更新显示数据
    SymUpdate(data_ptr);

	//将text字段数据同步到display_text字段，以备显示使用，免得再每个地方修改
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

    MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr);
    return;
}


/*==============================================================================
Description: Abc输入按键提起触笔 - character键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN SymCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    CapsCommitStr (data_ptr, key_ptr->text, key_ptr->text_len);

    if (((SYMBOL_DATA_T*)(data_ptr->data))->is_lock)
    {
        VkbUp(data_ptr, key_ptr);
    }
    else
    {
        SymTerm(data_ptr);
        ResetMethod(data_ptr);
    }

    return TRUE;
}


/*==============================================================================
Description: Abc输入按键提起触笔 - control键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN SymCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_BACK:
        SymTerm(data_ptr);
        ResetMethod(data_ptr);
        return TRUE;         //进入新的窗口，不需要继续处理up消息
        break;/*lint !e527*/

    case VKEY_PAGEUP:
        //SCI_ASSERT(((SYMBOL_DATA_T*)(data_ptr->data))->cur_page > 0); /*assert verified*/
        if (((SYMBOL_DATA_T*)(data_ptr->data))->cur_page == 0)
        {
            break;
        }
        ((SYMBOL_DATA_T*)(data_ptr->data))->cur_page--;
        SymUpdate(data_ptr);

		//将text字段数据同步到display_text字段，以备显示使用，免得再每个地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

        MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr);
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
		break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(((SYMBOL_DATA_T*)(data_ptr->data))->cur_page + 1 < SYMBOL_PAGES); /*assert verified*/
        if (((SYMBOL_DATA_T*)(data_ptr->data))->cur_page + 1 >= SYMBOL_PAGES)
        {
            break;
        }
        
        ((SYMBOL_DATA_T*)(data_ptr->data))->cur_page++;
        SymUpdate(data_ptr);

		//将text字段数据同步到display_text字段，以备显示使用，免得再每个地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
        
        MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr);  
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
        break;

    case VKEY_DEL:
        CommitChar(data_ptr, '\b');
        break;

    case VKEY_LOCK:

        if (0 == ((SYMBOL_DATA_T*)(data_ptr->data))->is_lock)
        {
            ((SYMBOL_DATA_T*)(data_ptr->data))->is_lock = 1;
            data_ptr->vkb.lock_key->icon = IMG_IMT_LOCK;
        }
        else
        {
            ((SYMBOL_DATA_T*)(data_ptr->data))->is_lock = 0;
            data_ptr->vkb.lock_key->icon = IMG_IMT_UNLOCK;
        }

        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 每个语言的符号面板需要定制
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL int32 ForeignSymList (GUIIM_LANGUAGE_T guiim_lang, wchar const **symb_list, uint32 *sym_cnt)
{
/*lint -save -e64*/
	if (symb_list == PNULL || sym_cnt == PNULL)
	{
		SCI_TRACE_LOW ("[MMIIM] ForeignSymList");
		return -1;
	}
	
	switch (guiim_lang)
	{
	//孟加拉
#if defined IM_BENGALI_SUPPORT
		case GUIIM_LANG_BENGALI:
		{
			*symb_list = s_sym_list_bengali;
			*sym_cnt = ARR_SIZE(s_sym_list_bengali);

			break;
		}
#endif	
		case GUIIM_LANG_ENGLISH:
		{
			*symb_list = s_sym_list_half;
			*sym_cnt = ARR_SIZE(s_sym_list_half);

			break;
		}
		
		default:
		{
			*symb_list = s_sym_list_half;
			*sym_cnt = ARR_SIZE(s_sym_list_half);

			break;
		}
	}

	return 0;
/*lint -restore*/	
}


/*==============================================================================
Description: 更新符号面板的显示
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void SymUpdate(IM_DATA_T *data_ptr)
{
    wchar const **symb_list = PNULL;
    uint32 sym_cnt = 0;
    uint32 sym_offset = 0;
    uint32 key_idx = 0;

    CHECK_DATA_EX(data_ptr);

    if (((SYMBOL_DATA_T*)(data_ptr->data))->type == SYMBOL_TYPE_FULL)
    {
        symb_list = s_sym_list_full;
        sym_cnt = ARR_SIZE(s_sym_list_full);
    }
    else
    {
    	int32 ret_value = 0;
		
        ret_value = ForeignSymList (MMIAPIIM_GetLanguage (), &symb_list, &sym_cnt);	/*lint !e64*/
		if (ret_value < 0)
		{
	        symb_list = s_sym_list_half;
	        sym_cnt = ARR_SIZE(s_sym_list_half);
		}
    }

    sym_offset = data_ptr->vkb.custom_cnt * ((SYMBOL_DATA_T*)(data_ptr->data))->cur_page;

    //SCI_ASSERT(sym_offset + data_ptr->vkb.custom_cnt <= sym_cnt); /*assert verified*/
    if (sym_offset + data_ptr->vkb.custom_cnt > sym_cnt)
    {
        return;
    }
        
    for (key_idx = 0; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
    {
        uint8 abs_key_idx = data_ptr->vkb.custom_first + key_idx;
        wchar const *sym_ptr = PNULL;

        sym_ptr = symb_list[sym_offset + key_idx];
        data_ptr->vkb.keys[abs_key_idx].text_len = 0;

        while (*sym_ptr != 0)
        {
            data_ptr->vkb.keys[abs_key_idx].text[data_ptr->vkb.keys[abs_key_idx].text_len] = *sym_ptr;
            data_ptr->vkb.keys[abs_key_idx].text_len++;
            sym_ptr++;
        }
    }

    data_ptr->vkb.tip_key->text[0] = '1' + ((SYMBOL_DATA_T*)(data_ptr->data))->cur_page;

    if (((SYMBOL_DATA_T*)(data_ptr->data))->cur_page > 0)
    {
        data_ptr->vkb.pgup_key->is_inactive = 0;
        data_ptr->vkb.pgup_key->icon = IMG_IMT_LEFT_TRIANGLE;
    }
    else
    {
        data_ptr->vkb.pgup_key->is_inactive = 1;
        data_ptr->vkb.pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
    }

    if (((SYMBOL_DATA_T*)(data_ptr->data))->cur_page + 1 < SYMBOL_PAGES)
    {
        data_ptr->vkb.pgdn_key->is_inactive = 0;
        data_ptr->vkb.pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE;
    }
    else
    {
        data_ptr->vkb.pgdn_key->is_inactive = 1;
        data_ptr->vkb.pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
    }


    return;
}


/*==============================================================================
Description: DigitInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN DigitInit(IM_DATA_T *data_ptr)
{ 
    data_ptr->data = (KERNEL_DATA_DIGIT_T*)SCI_ALLOC_APP(sizeof(KERNEL_DATA_DIGIT_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_DIGIT_T));

    DigitInitKb(data_ptr);

    return TRUE;
}


/*==============================================================================
Description: DigitTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN DigitTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: DigitDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void DigitDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*==============================================================================
Description: 初始化键盘为定制的数字键盘
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void DigitInitKb(IM_DATA_T *data_ptr)
{
    uint32 const CHAR_CNT = 12;
    uint32 const CTRL_CNT = 2;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    uint32 keys_size = 0;
    uint32 key_idx = 0;
    uint32 char_begin = 0;
    uint32 ctrl_begin = 0;
	MMI_STRING_T slide_cue = {PNULL, 0};
	
    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    vkb_ptr = &(data_ptr->vkb);

    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_digit_panel_position_size;

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //3*4
    key_idx = 0;
    data_ptr->vkb.charactor_first = char_begin = key_idx;
	
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_digit_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_ALL_TEXT;
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].text[0] = "123456789*0#"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].font_size = 8;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = DigitCharUp;
    }

    key_idx--;

    //key 10 0
    vkb_ptr->keys[10].code = VKEY_0;

    if (GUIIM_TAG_CLASSIC_NUM == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[9].code = VKEY_STAR;
        vkb_ptr->keys[11].code = VKEY_POUND;
    }
    else if (GUIIM_TAG_PURE_NUM == data_ptr->init_param.method_param_ptr->tag)
    {
        //key 9 none
        vkb_ptr->keys[9].code = VKEY_NULL;
        vkb_ptr->keys[9].is_inactive = 1;
        vkb_ptr->keys[9].text_len = 0;

        //key 11 none
        vkb_ptr->keys[11].code = VKEY_NULL;
        vkb_ptr->keys[11].is_inactive = 1;
        vkb_ptr->keys[11].text_len = 0;
    }
    else if (GUIIM_TAG_DOT_NUM == data_ptr->init_param.method_param_ptr->tag)
    {
        //key 9 none
        vkb_ptr->keys[9].code = VKEY_NULL;
        vkb_ptr->keys[9].is_inactive = 1;
        vkb_ptr->keys[9].text_len = 0;

        //key 11 dot
        vkb_ptr->keys[11].code = VKEY_DOT;
        vkb_ptr->keys[11].text_len = 1;
        vkb_ptr->keys[11].text[0] = '.';
    }
    else if (GUIIM_TAG_PHONE_NUM_MIN == data_ptr->init_param.method_param_ptr->tag)
    {
        //key 9 none
        vkb_ptr->keys[9].code = VKEY_NULL;
        vkb_ptr->keys[9].is_inactive = 1;
        vkb_ptr->keys[9].text_len = 0;

        //key 11 plus
        vkb_ptr->keys[11].code = VKEY_PLUS;
        vkb_ptr->keys[11].text_len = 1;
        vkb_ptr->keys[11].text[0] = '+';
    }
	else if (GUIIM_TAG_NUM_WITH_SWITCHING == data_ptr->init_param.method_param_ptr->tag)
    {
		//supporting slidepanel to switch vkb
		data_ptr->vkb.f_sliding = SlidingPanel;
		
		//now the key9 and key11 defines the same as GUIIM_TAG_CLASSIC_NUM
		vkb_ptr->keys[9].code = VKEY_STAR;
        vkb_ptr->keys[11].code = VKEY_POUND;
    }
    else //if (GUIIM_TAG_PHONE_NUM == data_ptr->init_param.method_param_ptr->tag)
    {
        //key 9 symbol
        vkb_ptr->keys[9].code = VKEY_SYMBOL;
        vkb_ptr->keys[9].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[9].text, L"*#PW", vkb_ptr->keys[9].text_len*sizeof(wchar));

        //key 11 plus
        vkb_ptr->keys[11].code = VKEY_PLUS;
        vkb_ptr->keys[11].text_len = 1;
        vkb_ptr->keys[11].text[0] = '+';
    }

    //control * 2
    //character key 的位置索引从开始到30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_digit_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_up = DigitCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;


    //sys key 1, system - hide 位置索引1
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    ConvertRect(panel_width, panel_height,
                &(g_digit_panel_position[1]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;

    //key art, 用于candidate的容器, 位置索引0
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_digit_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
	
	//to prompt user to switch vkb to input character and symbol if necessary
	if(GUIIM_TAG_NUM_WITH_SWITCHING == data_ptr->init_param.method_param_ptr->tag)
	{
		MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    	vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    	MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
	}

	//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: 拇指键盘Abc输入按键提起触笔 - character键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN DigitCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == data_ptr->vkb.keys || PNULL == key_ptr)
    {
        return  TRUE;
    }
    
    if (key_ptr->code == VKEY_SYMBOL)
    {
        if (((KERNEL_DATA_DIGIT_T*)(data_ptr->data))->is_expand)
        {
            ((KERNEL_DATA_DIGIT_T*)(data_ptr->data))->is_expand = 0;
            data_ptr->vkb.keys[0].is_inactive = 0;
            data_ptr->vkb.keys[1].is_inactive = 0;
            data_ptr->vkb.keys[2].is_inactive = 0;
            data_ptr->vkb.keys[3].is_inactive = 0;
            data_ptr->vkb.keys[4].is_inactive = 0;
            data_ptr->vkb.keys[5].is_inactive = 0;
            data_ptr->vkb.keys[6].is_inactive = 0;
            data_ptr->vkb.keys[7].text[0] = '8';
            data_ptr->vkb.keys[8].text[0] = '9';
            data_ptr->vkb.keys[9].text_len = 4;
            data_ptr->vkb.keys[9].icon = 0;
            data_ptr->vkb.keys[10].text[0] = '0';
            data_ptr->vkb.keys[11].text[0] = '+';
        }
        else
        {
            ((KERNEL_DATA_DIGIT_T*)(data_ptr->data))->is_expand = 1;
            data_ptr->vkb.keys[0].is_inactive = 1;
            data_ptr->vkb.keys[1].is_inactive = 1;
            data_ptr->vkb.keys[2].is_inactive = 1;
            data_ptr->vkb.keys[3].is_inactive = 1;
            data_ptr->vkb.keys[4].is_inactive = 1;
            data_ptr->vkb.keys[5].is_inactive = 1;
            data_ptr->vkb.keys[6].is_inactive = 1;
            data_ptr->vkb.keys[7].text[0] = '*';
            data_ptr->vkb.keys[8].text[0] = '#';
            data_ptr->vkb.keys[9].text_len = 0;
            data_ptr->vkb.keys[9].icon = IMG_IMT_BACK;
            data_ptr->vkb.keys[10].text[0] = 'P';
            data_ptr->vkb.keys[11].text[0] = 'W';
        }

		//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
        VkbUp(data_ptr, key_ptr);       ////与其他键有关联性
    }
    else
    {
        wchar commit_chr = 0;

        if (GUIIM_TAG_PB_HUOPIN == data_ptr->init_param.method_param_ptr->tag)
        {
            switch (key_ptr->code)
            {
            case VKEY_1:
            case VKEY_2:
            case VKEY_3:
            case VKEY_4:
            case VKEY_5:
            case VKEY_6:
            case VKEY_7:
            case VKEY_8:
            case VKEY_9:
                commit_chr = '1' + key_ptr->code - VKEY_1;
                break;

            case VKEY_0:
                commit_chr = '0';
                break;

            default:
                break;
            }
        }
        else
        {
            commit_chr = key_ptr->text[0];
        }

        CommitChar(data_ptr, commit_chr);

		//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText(data_ptr);

        key_ptr->is_press = 0;
	    MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);		//与其他键没有关联性，直接绘制，不要画整个键盘
		DrawVkb (data_ptr);
    }

    return TRUE;
}


/*==============================================================================
Description: 拇指键盘Abc输入按键提起触笔 - control键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN DigitCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return  TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:
        CommitChar(data_ptr, '\b');
        break;

    case VKEY_ENTER:
        CommitChar(data_ptr, '\n');
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcInit(IM_DATA_T *data_ptr)
{
    data_ptr->data = (KERNEL_DATA_ABC_T*)SCI_ALLOC_APP(sizeof(KERNEL_DATA_ABC_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_ABC_T));
#if 0   //火拼功能去掉
    if (GUIIM_TAG_PB_HUOPIN == data_ptr->init_param.method_param_ptr->tag)
    {
        ThumbAbcInitKbPbHuoPin(data_ptr);
    }
    else
#endif        
    {
        ThumbAbcInitKb(data_ptr);
    }

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer > 0)
    {
        MMK_StopTimer(((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer);
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer = 0;
    }

    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbAbcDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*==============================================================================
Description: 初始化键盘为拇指英文字母输入
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void ThumbAbcInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 6;
    uint8 const CAND_CNT = 5;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    
    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //增加candidate
    vkb_ptr->key_cnt -= g_thumb_panel_expand_page_cnt; //no expand page
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key 的位置索引从1开始的连续5个
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = ",.?!-"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbAbcCustUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //deal tag
    if (GUIIM_TAG_EMAIL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '@';
        vkb_ptr->keys[cust_begin+2].text[0] = '-';
        vkb_ptr->keys[cust_begin+3].text[0] = '_';
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }
    else if (GUIIM_TAG_URL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '/';
        vkb_ptr->keys[cust_begin+2].text_len = 7;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+2].text, L"http://", vkb_ptr->keys[cust_begin+2].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+3].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+3].text, L"www.", vkb_ptr->keys[cust_begin+3].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }

    //character * 10
    //character key 的位置索引从6开始到15
    key_idx++;
    char_begin = key_idx;

    key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    }
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_EXCLUDE_DIGIT; 
        //=========语言配置丝印部分
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //字符个数
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbAbcCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbAbcCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //--1 特别设定
    vkb_ptr->keys[char_begin].text_len = 0;
    //--0 最后一个char
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE;

    //control * 6, del, 123, caps, symbol, switch, enter
    //character key 的位置索引从16开始到21
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbAbcCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;
    //--Caps
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_CAPS;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
    //特别的记住caps key
    vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_ABC;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;    
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh拦截处理
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //被MMIIM_HandleSettingIMUp拦截处理

    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }

    //key 21 , system - hide 位置索引1
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[1]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;

    //key art, 用于candidate的容器, 位置索引0
    //art 要放在candidate前面，因为要先画
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;      //虽然不使用，但细节改进需要
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
    
    //candidate * 5
    //候选键的位置未定，内容未定
    //candidate key 总是放在最后，因为其可能会覆盖在其他的部分之上，比如art
    //Abc输入中，candidate的数目是固定的，位置也可以固定
    //且，cand的长度总是1
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        GUI_RECT_T cand_rect = {0};
        int16 cand_len = g_thumb_panel_position[0].right + 1;

        cand_rect.bottom = g_thumb_panel_position[0].bottom;
        cand_rect.left = (key_idx - cand_begin) * cand_len / CAND_CNT;
        cand_rect.right = (key_idx + 1 - cand_begin) * cand_len / CAND_CNT - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].text_len = 1;

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbAbcCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }

	{
		//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);		//上面的SetVkbCaps已经调用过该函数
	}

    return;
}


#if 0
/*==============================================================================
Description: 初始化键盘为拇指英文字母输入 - for pb huopin
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void ThumbAbcInitKbPbHuoPin(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 6;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;

    CHECK_DATA(data_ptr);
    //SCI_ASSERT(GUIIM_TAG_PB_HUOPIN == data_ptr->init_param.method_param_ptr->tag); /*assert verified*/
    if (GUIIM_TAG_PB_HUOPIN != data_ptr->init_param.method_param_ptr->tag)
    {
        return;
    }
    
    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt -= g_thumb_panel_expand_page_cnt; //no expand page
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key 的位置索引从1开始的连续5个
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].is_inactive = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 10
    //character key 的位置索引从6开始到15
    key_idx++;
    char_begin = key_idx;

    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_ALL_TEXT;
        vkb_ptr->keys[key_idx].text_len = g_thumb_key_disp_len[key_idx - char_begin];
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   g_thumb_key_disp[key_idx - char_begin],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbAbcCharUpPbHuoPin;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //--0 最后一个char
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE;

    //control * 6, del, 123, caps, symbol, switch, enter
    //character key 的位置索引从16开始到21
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbAbcCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;
    //--Caps
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].is_inactive = TRUE;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_ABC;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh拦截处理
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //被MMIIM_HandleSettingIMUp拦截处理
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;    
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //key 21 , system - hide 位置索引1
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[1]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;

    //key art, 用于candidate的容器, 位置索引0
    //art 要放在candidate前面，因为要先画
    key_idx++;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }

	{
		//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);		//上面的SetVkbCaps已经调用过该函数
	}

    return;
}
#endif

/*==============================================================================
Description: 拇指键盘Abc输入长按 - character键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return  TRUE;
    }
    
    if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key != key_ptr)
    {
        ThumbAbcCommitLast(data_ptr);

        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key = key_ptr;

        //reset index
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index = 0;
    }

    if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer > 0)
    {
        MMK_StopTimer(((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer);
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer = 0;
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index = 0;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 拇指键盘Abc输入按键提起触笔 - character键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    uint16 key_chr_cnt = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    if (PNULL == data_ptr->vkb.last_key || PNULL == key_ptr) return TRUE;

    key_chr_cnt = ThumbAbcGetChar(data_ptr->vkb.cap, key_ptr->code, 0);

    if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key != key_ptr)
    {
#if 0
        int32 cand_idx = 0;

        for (cand_idx = 0; cand_idx < key_chr_cnt; cand_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.candidate_first + cand_idx].is_hide = 0;
            data_ptr->vkb.keys[data_ptr->vkb.candidate_first + cand_idx].text[0] = ThumbAbcGetChar(data_ptr->vkb.cap, key_ptr->code, cand_idx + 1);
        }

        for (; cand_idx < data_ptr->vkb.candidate_cnt; cand_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.candidate_first + cand_idx].is_hide = 1;
        }
#endif
        ThumbAbcCommitLast(data_ptr);

        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key = key_ptr;

        //reset index
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index = 0;
    }

    if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer > 0)
    {
        MMK_StopTimer(((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer);
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer = 0;
    }

    if (1 == key_chr_cnt)
    {
        CapsCommitChar(data_ptr, ThumbAbcGetChar(data_ptr->vkb.cap, key_ptr->code, 1));
    }
    else if (key_chr_cnt > 1)
    {
        //set underline character
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index += 1;

        if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index > key_chr_cnt)
        {
            ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index = 1;
        }

        notify_pack.data.commit_data.underline[0] = ThumbAbcGetChar(data_ptr->vkb.cap, key_ptr->code, ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index);
        notify_pack.data.commit_data.underline_len = 1;

        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer = MMK_CreateTimerCallback(
                    ABC_COMMIT_TICK,
                    ThumbAbcCommitCB,
                    (uint32)data_ptr,
                    FALSE);
    }


    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


#if 0
/*==============================================================================
Description: 拇指键盘Abc输入按键提起触笔 - character键 - for pb huopin
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcCharUpPbHuoPin(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    wchar commit_chr = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    //SCI_ASSERT(GUIIM_TAG_PB_HUOPIN == data_ptr->init_param.method_param_ptr->tag); /*assert verified*/
    if (PNULL == data_ptr->vkb.last_key || PNULL == key_ptr||GUIIM_TAG_PB_HUOPIN != data_ptr->init_param.method_param_ptr->tag) 
    {
        return TRUE;
    }

    switch (key_ptr->code)
    {
    case VKEY_1:
    case VKEY_2:
    case VKEY_3:
    case VKEY_4:
    case VKEY_5:
    case VKEY_6:
    case VKEY_7:
    case VKEY_8:
    case VKEY_9:
        commit_chr = '1' + key_ptr->code - VKEY_1;
        break;

    case VKEY_0:
        commit_chr = '0';
        break;

    default:
        break;
    }

    CommitChar(data_ptr, commit_chr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}
#endif

/*==============================================================================
Description: 拇指键盘Abc输入按键提起触笔 - control键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    ThumbAbcCommitLast(data_ptr);

    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        break;

    case VKEY_DEL:
        CommitChar(data_ptr, '\b');
        break;

    case VKEY_ENTER:
        CommitChar (data_ptr, '\n');
        break;

    case VKEY_123:
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;
        
    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 拇指键盘Abc输入按键提起触笔 - custom键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    ThumbAbcCommitLast(data_ptr);

    CapsCommitStr (data_ptr, key_ptr->text, key_ptr->text_len);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 拇指键盘Abc输入按键提起触笔 - control键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer > 0)
    {
        MMK_StopTimer(((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer);
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer = 0;
        ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index = 0;
    }

    CapsCommitChar(data_ptr, key_ptr->text[0]);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcGetChar
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC wchar ThumbAbcGetChar(CAPITTAL_MODE_T cap, uint16 key_code, uint8 key_idx)
{
    char const **data = PNULL;

    data = (CAPITTAL_MODE_LOWER == cap) ? g_thumb_data_lower : g_thumb_data_upper;

    return data[key_code - VKEY_1][key_idx];
}


/*==============================================================================
Description: ThumbAbcCommitLast
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbAbcCommitLast(IM_DATA_T *data_ptr)
{
    wchar last_char = 0;
    BOOLEAN is_caps_changed = FALSE;

    CHECK_DATA_EX(data_ptr);

    if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer == 0) return FALSE;

    MMK_StopTimer(((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer);
    ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer = 0;

    last_char = ThumbAbcGetChar(data_ptr->vkb.cap, ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key->code, ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index);

    is_caps_changed = CapsCommitChar(data_ptr, last_char);

    ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index = 0;

    return is_caps_changed;
}


/*==============================================================================
Description: ThumbAbcCommitCB
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbAbcCommitCB(uint8 timer_id, uint32 param)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)param;
    BOOLEAN is_caps_changed = FALSE;
    wchar last_char = 0;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    if (PNULL == data_ptr
            || (sizeof(IM_DATA_T) != data_ptr->check_info)
            || ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer != timer_id)
    {
        return;
    }

    CHECK_DATA_EX(data_ptr);

    if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer == 0) return;

    ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer = 0;

    last_char = ThumbAbcGetChar(data_ptr->vkb.cap, ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key->code, ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index);

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data.commit[0] = last_char;
    notify_pack.data.commit_data.commit_len = 1;

    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

    if (CAPITTAL_MODE_INITIAL == data_ptr->vkb.cap
            && ('A' <= last_char && last_char <= 'Z'))
    {
        SetVkbCaps(data_ptr, CAPITTAL_MODE_LOWER);
        is_caps_changed = TRUE;
    }

    ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index = 0;

    if (is_caps_changed)
    {
    	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
        DrawVkb(data_ptr);
    }

    return;
}


/*==============================================================================
Description: QwertAbcInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertAbcInit(IM_DATA_T *data_ptr)
{
    data_ptr->data = data_ptr;      /*该数据不会被使用，但不能为空*/
    
    QwertAbcInitKb(data_ptr);

    return TRUE;
}


/*==============================================================================
Description: QwertAbcTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertAbcTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    data_ptr->data = PNULL;     //这个地方在初始化时，没有申请内存，不需要释放
    ReleaseVkb(&(data_ptr->vkb));
    
    return TRUE;
}


/*==============================================================================
Description: QwertAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertAbcDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*==============================================================================
Description: 初始化键盘为拇指英文字母输入
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void QwertAbcInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 const CHAR_CNT = 26;
    uint8 const CTRL_CNT = 7;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_qwert_default;

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_qwert_panel_position_size;
    vkb_ptr->key_cnt -= g_qwert_panel_expand_page_cnt; //no expand

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key 的位置索引从2开始到27
    data_ptr->vkb.charactor_first = key_idx = 0;

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].text[0] = "qwertyuiopasdfghjklzxcvbnm"[key_idx];
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertAbcCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key 的位置索引从28开始的连续2个
    key_idx++;
    cust_begin = key_idx;

    for (; key_idx < cust_begin + CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = ".,"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //deal tag
    if (GUIIM_TAG_EMAIL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '@';
    }
    else if (GUIIM_TAG_URL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '/';
    }

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key 的位置索引从开始到30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertAbcCtrlUp;
    }

    //--Caps
    key_idx = ctrl_begin;
    //特别的记住caps key
    vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_CAPS;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
    //--delete
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--space
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_Q;
    //--ime type set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_ABC;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh拦截处理
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //被MMIIM_HandleSettingIMUp拦截处理
    
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }

    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;

    //sys key 1, system - hide 位置索引1
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    ConvertRect(panel_width, panel_height,
                &(g_qwert_panel_position[1]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;

    //key art, 用于candidate的容器, 位置索引0
    //art 要放在candidate前面，因为要先画
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;      //虽然不使用，但细节改进需要
    ConvertRect(panel_width, panel_height,
                &(g_qwert_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect), 
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }

	{
		//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);		//上面的SetVkbCaps已经调用过该函数
	}
    
    return;
}


/*==============================================================================
Description: Abc输入按键提起触笔 - character键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertAbcCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    CapsCommitChar(data_ptr, key_ptr->text[0]);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Abc输入按键提起触笔 - control键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertAbcCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        break;

    case VKEY_DEL:
        CommitChar (data_ptr, '\b');
        break;

    case VKEY_SPACE:
        CommitChar (data_ptr, ' ');
        break;

    case VKEY_ENTER:
        CommitChar (data_ptr, '\n');
        break;

    case VKEY_123SYMBOL:
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}




/*==============================================================================
Description: ThumbAbcCommitLast
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN MultiTapCommitLast(IM_DATA_T *data_ptr)
{
    wchar last_char = 0;
    BOOLEAN is_caps_changed = FALSE;

    CHECK_DATA_EX(data_ptr);

//真是一个变态的做法
//其实，应该将data_ptr->multi_tap_data.last_key->text做一个修正，将真正有效的字符保存到multi-tap
//结构体中，然后通过multi-tap结构体来读取值，这样就不需要区分狗屁的孟加拉语了
//但是这样做，就需要修改前期的代码，还是少折腾吧		
#if (defined IM_BENGALI_SUPPORT && defined IM_ENGINE_CSTAR)		//sogou版本暂时没有添加该multi-tap输入法
	if (data_ptr
		&& data_ptr->init_param.method_param_ptr
		&& data_ptr->init_param.method_param_ptr->im_def_ptr
		&& data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_TP_BENGALI_ABC)	//全键盘没有规范，不需要判断
	{
		return ThumbBengaliMultiTapCommitLast (data_ptr);
	}
#endif

    if (data_ptr->multi_tap_data.commit_timer == 0) return FALSE;

    if (data_ptr->multi_tap_data.last_key_index == 0)
        return FALSE;

    MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
    data_ptr->multi_tap_data.commit_timer = 0;

    last_char = data_ptr->multi_tap_data.last_key->text[data_ptr->multi_tap_data.last_key_index - 1];

    is_caps_changed = CapsCommitChar(data_ptr, last_char);

    data_ptr->multi_tap_data.last_key_index = 0;

    return is_caps_changed;
}


/*==============================================================================
Description: ThumbAbcCommitCB
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void MultiTapCommitCB(uint8 timer_id, uint32 param)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)param;
    BOOLEAN is_caps_changed = FALSE;
    wchar last_char = 0;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    if (PNULL == data_ptr
            || (sizeof(IM_DATA_T) != data_ptr->check_info)
            || data_ptr->multi_tap_data.commit_timer != timer_id)
    {
        return;
    }

    CHECK_DATA_EX(data_ptr);

    if (data_ptr->multi_tap_data.commit_timer == 0)
    {
        //SCI_TraceLow:"MultiTapCommitCB"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_2108_112_2_18_2_27_25_98,(uint8*)"");
        return;
    }

    if (data_ptr->multi_tap_data.last_key == PNULL)
    {
        //SCI_TraceLow:"MultiTapCommitCB"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_2114_112_2_18_2_27_25_99,(uint8*)"");
        return;
    }

    if (data_ptr->multi_tap_data.last_key_index == 0)
    {
        //SCI_TraceLow:"MultiTapCommitCB"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_2120_112_2_18_2_27_25_100,(uint8*)"");
        return;
    }

    data_ptr->multi_tap_data.commit_timer = 0;

    last_char = data_ptr->multi_tap_data.last_key->text[(data_ptr->multi_tap_data.last_key_index - 1) % data_ptr->multi_tap_data.last_key->text_len];

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data.commit[0] = last_char;
    notify_pack.data.commit_data.commit_len = 1;

    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

    if (CAPITTAL_MODE_INITIAL == data_ptr->vkb.cap)
    {
        SetVkbCaps(data_ptr, CAPITTAL_MODE_LOWER);
        is_caps_changed = TRUE;
    }

    data_ptr->multi_tap_data.last_key_index = 0;

    if (is_caps_changed)
    {
    	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
        DrawVkb(data_ptr);
    }

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   MultiTapCharUp

Func:   多个字符选择按键消息处理

Input:
            data_ptr:  输入法句柄指针
            key_ptr:    操作的按键

Output:

Return:
            err: < 0  succ: == 0

Thoughtway:

Create: yaoguangchen@spread. 2011-10-11

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MultiTapCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    uint16 key_chr_cnt = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr || PNULL == data_ptr->vkb.keys) 
    {
        return TRUE;
    }
    
    if (PNULL == data_ptr->vkb.last_key)
    {
        return TRUE;
    }

    key_chr_cnt = key_ptr->text_len;

    if (data_ptr->multi_tap_data.last_key != key_ptr)
    {
        MultiTapCommitLast(data_ptr);

        data_ptr->multi_tap_data.last_key = key_ptr;

        //reset index
        data_ptr->multi_tap_data.last_key_index = 0;
    }

    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
    }

    if (1 == key_chr_cnt)
    {
        CapsCommitChar (data_ptr, key_ptr->text[0]);
    }
    else if (key_chr_cnt > 1)
    {
        //set underline character
        data_ptr->multi_tap_data.last_key_index += 1;
        if (data_ptr->multi_tap_data.last_key_index > key_chr_cnt)
        {
            data_ptr->multi_tap_data.last_key_index = 1;
        }

        notify_pack.data.commit_data.underline[0] = key_ptr->text[data_ptr->multi_tap_data.last_key_index - 1];
        notify_pack.data.commit_data.underline_len = 1;

        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

        data_ptr->multi_tap_data.commit_timer = MMK_CreateTimerCallback(
                    ABC_COMMIT_TICK,
                    MultiTapCommitCB,
                    (uint32)data_ptr,
                    FALSE);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 通常的提笔响应 - 提交按键显示字符串
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN MultitapVkeyUpDeal(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    MultiTapCommitLast (data_ptr);      //VkeyUpDeal缺少该处理，multitap输入字符时间片未到时，输入异常
    if (key_ptr->text_len > 0)
    {
        CapsCommitStr (data_ptr, key_ptr->text, key_ptr->text_len);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ClearMultitapTimer
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ClearMultitapTimer (IM_DATA_T *data_ptr)
{
    if (data_ptr == PNULL)
    {
        //SCI_TraceLow:"ClearMultitapTimer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_2274_112_2_18_2_27_25_101,(uint8*)"");
        return FALSE;
    }
    
    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
    }

    return TRUE;
}

/*==============================================================================
Description: ThumbMultiTapInit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbMultiTapInit(IM_DATA_T *data_ptr)
{
    data_ptr->data = (MULTI_TAP_DATA_T*)SCI_ALLOC_APP(sizeof(MULTI_TAP_DATA_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(MULTI_TAP_DATA_T));

    ThumbMultiTapInitKb(data_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbMultiTapTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbMultiTapTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
    }

    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: ThumbMultiTapDraw
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbMultiTapDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*==============================================================================
Description: 拇指键盘MultiTap输入长按 - character键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbMultiTapCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
        data_ptr->multi_tap_data.last_key_index = 0;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 拇指键盘MultiTap输入长按 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbMultiTapCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    MultiTapCommitLast (data_ptr);
    switch (key_ptr->code)
    {
    case VKEY_DEL:
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 拇指键盘MultiTap输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbMultiTapCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    MultiTapCommitLast(data_ptr);

    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        break;

    case VKEY_DEL:
        CommitChar (data_ptr, '\b');

        break;

    case VKEY_SPACE:
        CommitChar (data_ptr, ' ');
        break;

    case VKEY_ENTER:
        CommitChar (data_ptr, '\n');
        break;

    case VKEY_123:
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 拇指键盘MultiTap输入按键提起触笔 - custom键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbMultiTapCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    MultiTapCommitLast(data_ptr);

    CapsCommitStr (data_ptr, key_ptr->text, key_ptr->text_len);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 拇指键盘MultiTap输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbMultiTapCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
        data_ptr->multi_tap_data.last_key_index = 0;
    }

    CapsCommitChar(data_ptr, key_ptr->text[0]);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}

/*==============================================================================
Description: 初始化键盘为multitype 输入
Global resource dependence: NONE
Author: yaoguang.chen
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void ThumbMultiTapInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 8;
    uint8 const SYS_CNT = 2;
    uint8 const CAND_CNT = LATIN_CANDIDATE_COUNT;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};
    uint16 key_max_char_num = 0;

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;
    SetPanel(data_ptr);

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //增加candidate
    vkb_ptr->key_cnt += 1; //expand & hide 是重合的
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key 的位置索引从1开始的连续5个
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = ",.?!-"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = MultitapVkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //deal tag
    if (GUIIM_TAG_EMAIL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '@';
        vkb_ptr->keys[cust_begin+2].text[0] = '-';
        vkb_ptr->keys[cust_begin+3].text[0] = '_';
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }
    else if (GUIIM_TAG_URL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '/';
        vkb_ptr->keys[cust_begin+2].text_len = 7;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+2].text, L"http://", vkb_ptr->keys[cust_begin+2].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+3].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+3].text, L"www.", vkb_ptr->keys[cust_begin+3].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }

    //character * 10
    //character key 的位置索引从6开始到15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    }

    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_EXCLUDE_DIGIT;        //
        //=========语言配置丝印部分
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //字符个数
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        key_max_char_num = key_max_char_num > vkb_ptr->keys[key_idx].text_len ? key_max_char_num : vkb_ptr->keys[key_idx].text_len;
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbMultiTapCharLong;
        vkb_ptr->keys[key_idx].f_up = MultiTapCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //如果存在字符超过5个的按键，以下面的属性显示
    if (key_max_char_num > KEY_CHAR_LINE_MAX_CHARS_NUM + 1)       //数字键也在里面,所以为5而不是4
    {
        int32 iloop = 0;
        
        for (iloop = 0; iloop < CHAR_CNT; iloop++)
        {
            vkb_ptr->keys[key_idx - iloop - 1].add_style.text_disp = KASC_2X4_EXCLUDE_DIGIT;    //key_idx值在最后被加1，需要 -1
        }
    }
    
    key_idx--;

    //--0 最后一个char
    ThumbMultitapRedefineKey0 (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[key_idx]);
    
    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key 的位置索引从16开始到21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbMultiTapCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbMultiTapCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--Caps or Multi-char
    key_idx++;
    if (MMIAPIIM_CapsExist(data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
    {
        vkb_ptr->keys[key_idx].code = VKEY_CAPS;
        vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
        //特别的记住caps key
        vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
    }
    else
    {
        ThumbMultitapRedefineCaps (data_ptr->init_param.method_param_ptr->im_def_ptr->method, vkb_ptr->keys + key_idx);   
    }
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_EN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh拦截处理
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //被MMIIM_HandleSettingIMUp拦截处理

    //--ime lang set
    key_idx++;
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
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //这两个按键在同一个位置，交替出现, 初始化隐藏expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0,
                    0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbMultiTapCtrlUp;
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

    //key art, 用于candidate的容器, 位置索引0
    //art 要放在candidate前面，因为要先画
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
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
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
        
    //candidate
    //候选键的位置未定，内容未定
    //candidate key 总是放在最后，因为其可能会覆盖在其他的部分之上，比如art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbMultiTapCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }

	{
		//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);		

		MMIIM_TOUCH_ThumbKbRedefinePrintText (data_ptr);	//有些语言的丝印和按键码显示不一致
	}

    return;
}


//特殊按键配置
/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartRedefineKey0

Func:   默认的0键配置为0和空格键，但某些语言的0键需要特殊配置，如繁体注意为智能输入键，
            如果某个按键被配置为智能输入按键，则这个按键的text元素值不能配置为0或空格字符，
            否则此处配置将把智能输入按键给配置成了空格键
Input:  
            method:  输入法类型
            key_ptr:   0键对应的地址
            
Output: 

Return:
            err: < 0  succ: == 0
            
Thoughtway:
          
Create: yaoguangchen@spread. 2011-12-13
        
Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 ThumbSmartRedefineKey0(GUIIM_METHOD_T method, KEY_INFO_T *key_ptr)
{
    if(key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"ThumbSmartRedefineKey0 method == %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_2859_112_2_18_2_27_26_102,(uint8*)"d", method);
        return -1;
    }

    switch (method)
    {
        case GUIIM_M_TP_THAI:
        {
            key_ptr->f_up = MultiTapCharUp;  
            break;
        }
        
        default:
        {
            if(
                (key_ptr->text[0] == ' ' && key_ptr->text[1] == '0')    //如果配置为空格和数字 0
                )      
            {
                key_ptr->icon = IMG_IMT_SPACE;
                key_ptr->text_len = 2;
#if defined IM_ENGINE_SOGOU				
                //key_ptr->f_up = MultiTapCharUp;  
#elif defined IM_ENGINE_CSTAR
				 key_ptr->f_up = MultiTapCharUp;  
#endif
            }
            else if(
                        MMIAPICOM_Wstrlen(key_ptr->text) == 0     //没有配置字符
                     || (MMIAPICOM_Wstrlen(key_ptr->text) == 1 && key_ptr->text[0] == 0x0030)  //配一个0   
                )  
            {
                key_ptr->text[0] = ' ' ;
                key_ptr->text[1] = 0x0030;
                key_ptr->icon = IMG_IMT_SPACE;
                key_ptr->text_len = 2;
#if defined IM_ENGINE_SOGOU				
                //key_ptr->f_up = MultiTapCharUp;  
#elif defined IM_ENGINE_CSTAR
				 key_ptr->f_up = MultiTapCharUp;  
#endif
            }

			//将text字段数据同步到display_text字段
			key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));
			
            //其他情况不需要重新设置消息处理函数

            break;
        }
    }
    
    return 0;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartRedefineKey1

Func:   默认的1键配置为智能输入按键，但某些输入法的1键不需要，就被配置为符号键等
Input:  
            method:  输入法类型
            key_ptr:   1键对应的地址
            
Output: 

Return:
            err: < 0  succ: == 0
            
Thoughtway:
          
Create: yaoguangchen@spread. 2011-12-13
        
Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 ThumbSmartRedefineKey1(GUIIM_METHOD_T method, KEY_INFO_T *key_ptr)
{
    if(key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"ThumbSmartRedefineKey1 method == %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_2926_112_2_18_2_27_26_103,(uint8*)"d", method);
        return -1;
    }
    
    if (
        (key_ptr->text[0] == '1') && MMIAPICOM_Wstrlen(key_ptr->text) == 1   //如果配置为空格和数字 0
    )      
    {
        key_ptr->f_up = MultiTapCharUp;  
    }

    return 0;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartRedefineCaps

Func:   默认的caps键被配置为大小写切换键，但某些语言没有大小写，则该键需要被重定义为
            其他按键，此处默认配置为标点符号输入键
Input:  
            method:  输入法类型
            key_ptr:   caps 键对应的地址
            
Output: 

Return:
            err: < 0  succ: == 0
            
Thoughtway:
          
Create: yaoguangchen@spread. 2011-12-13
        
Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 ThumbSmartRedefineCaps (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr)
{
    if(key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"ThumbSmartRedefineCaps method == %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_2966_112_2_18_2_27_27_104,(uint8*)"d", method);
        return -1;
    }

    switch (method)
    {
        case GUIIM_M_TP_THAI:
        case GUIIM_M_TP_ZHUYIN:
        {
            key_ptr->text[0] = ' ';
            key_ptr->text_len = 1;
            key_ptr->icon = IMG_IMT_SPACE_CENTER;
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->f_up = MultiTapCharUp;  
            break;
        }
        
        default:
        {
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->tag = KEY_TAG_NORMAL;
            key_ptr->style = KEY_STYLE_CONTROL; //KEY_STYLE_CHARACTER;
            key_ptr->add_style.text_disp = KASC_ALL_TEXT;        //
            MMIAPICOM_Wstrncpy(key_ptr->text,
                       normal_9key_mul_sym,
                       TEXT_LEN_LIMIT - 1);
            key_ptr->text_len = MMIAPICOM_Wstrlen(key_ptr->text);
            key_ptr->f_down = VkbDown;
            key_ptr->f_move = VkbMove;
            key_ptr->f_long = PNULL;
            key_ptr->f_up = MultiTapCharUp;
            key_ptr->has_tip = TRUE;        
            
            break;
        }
    }

	//将text字段数据同步到display_text字段
	key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
	SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));

    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbMultitapRedefineKey0

Func:   默认的0键配置为0和空格键，但某些语言的0键需要特殊配置，如thai的0键就需要
            显示自己的字符
            
Input:  
            method:  输入法类型
            key_ptr:   0键对应的地址
            
Output: 

Return:
            err: < 0  succ: == 0
            
Thoughtway:
          
Create: yaoguangchen@spread. 2011-12-21
        
Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 ThumbMultitapRedefineKey0(GUIIM_METHOD_T method, KEY_INFO_T *key_ptr)
{
    if(key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"ThumbMultitapRedefineKey0 method == %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_3032_112_2_18_2_27_27_105,(uint8*)"d", method);
        return -1;
    }

    switch (method)
    {
        case GUIIM_M_TP_ENGLISH_ABC:
        {
            key_ptr->text[0] = ' ' ;
            key_ptr->text[1] = 0x0030;
            key_ptr->icon = IMG_IMT_SPACE;
            key_ptr->text_len = 2;
            key_ptr->f_up = MultiTapCharUp;  
            
            break;
        }
        
        default:
        {
            if(
                (key_ptr->text[0] == ' ' && key_ptr->text[1] == '0')    //如果配置为空格和数字 0
                )      
            {
                key_ptr->icon = IMG_IMT_SPACE;
                key_ptr->text_len = 2;
                key_ptr->f_up = MultiTapCharUp;  
            }
            else if(
                        MMIAPICOM_Wstrlen(key_ptr->text) == 0     //没有配置字符
                     || (MMIAPICOM_Wstrlen(key_ptr->text) == 1 && key_ptr->text[0] == 0x0030)  //配一个0   
                )  
            {
                key_ptr->text[0] = ' ' ;
                key_ptr->text[1] = 0x0030;
                key_ptr->icon = IMG_IMT_SPACE;
                key_ptr->text_len = 2;
                key_ptr->f_up = MultiTapCharUp;  
            }
            else
            {
                key_ptr->f_up = MultiTapCharUp; 
            }

            break;
        }
    }

	//将text字段数据同步到display_text字段
	key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
	SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));
			
    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbMultitapRedefineCaps

Func:   默认的caps键被配置为大小写切换键，但某些语言没有大小写，则该键需要被重定义为
            其他按键，此处默认配置为标点符号输入键
Input:  
            method:  输入法类型
            key_ptr:   caps 键对应的地址
            
Output: 

Return:
            err: < 0  succ: == 0
            
Thoughtway:
          
Create: yaoguangchen@spread. 2011-12-21
        
Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 ThumbMultitapRedefineCaps (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr)
{
    if(key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"ThumbMultitapRedefineCaps method == %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_3107_112_2_18_2_27_27_106,(uint8*)"d", method);
        return -1;
    }

    switch (method)
    {
        case GUIIM_M_TP_THAI_ABC:
        case GUIIM_M_TP_HINDI_ABC:    
        {
            key_ptr->text[0] = ' ';
            key_ptr->text_len = 1;
            key_ptr->icon = IMG_IMT_SPACE_CENTER;
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->f_up = MultiTapCharUp;  
            break;
        }
		
        case GUIIM_M_TP_TIBETAN_ABC:   // 2012 0529 tibetan 
        {
		    key_ptr->text[0] = ' ';
            key_ptr->text_len = 1;
            key_ptr->icon = IMG_IMT_SPACE_CENTER;
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->f_up = MultiTapCharUp;  
            break;
		}
		
        default:
        {
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->tag = KEY_TAG_NORMAL;
            key_ptr->style = KEY_STYLE_CONTROL; //KEY_STYLE_CHARACTER;
            key_ptr->add_style.text_disp = KASC_ALL_TEXT;        //
            MMIAPICOM_Wstrncpy(key_ptr->text,
                       normal_9key_mul_sym,
                       TEXT_LEN_LIMIT - 1);
            key_ptr->text_len = MMIAPICOM_Wstrlen(key_ptr->text);
            key_ptr->f_down = VkbDown;
            key_ptr->f_move = VkbMove;
            key_ptr->f_long = PNULL;
            key_ptr->f_up = MultiTapCharUp;
            key_ptr->has_tip = TRUE;        
            break;
        }
    }

	//将text字段数据同步到display_text字段
	key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
	SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));
	
    return 0;
}


/*==============================================================================
Description: QwertMultiTapInit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertMultiTapInit(IM_DATA_T *data_ptr)
{
    data_ptr->data = (MULTI_TAP_DATA_T*)SCI_ALLOC_APP(sizeof(MULTI_TAP_DATA_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(MULTI_TAP_DATA_T));
    
    QwertMultiTapInitKb(data_ptr);

    return TRUE;
}


/*==============================================================================
Description: QwertMultiTapTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertMultiTapTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
    }

    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));
    
    return TRUE;
}


/*==============================================================================
Description: QwertMultiTapDraw
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertMultiTapDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*==============================================================================
Description: 初始化键盘为拇指英文字母输入
Global resource dependence: NONE
Author: yaoguang.chen
Note: 此处完全采用数据写入，定制应用
==============================================================================*/
PUBLIC void QwertMultiTapInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 CHAR_CNT = 26;
    uint8 const CTRL_CNT = 7;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
    MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;
    QWERT_KEYBOARD_LAYOUT_TYPE_E layout = KEYBOARD_LAYOUT_26CHAR;
    uint16 const *keymap_ptr = g_kb_keymap;
    GUI_RECT_T const *kb_pos_ptr = g_qwert_panel_position;
    MMI_STRING_T slide_cue = {PNULL, 0};
    
    CHECK_DATA(data_ptr);
 
    layout = QwertKbLayoutType (data_ptr->init_param.method_param_ptr->im_def_ptr->method, 0);
    data_ptr->theme = g_theme_qwert_default;    //QwertKbGetTheme (layout);
	MMIIM_TOUCH_InitVkbMultiPageParam (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &data_ptr->vkb_multi_page);
	
    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = QwertKbGetKeyCnt (layout);
    vkb_ptr->key_cnt -= g_qwert_panel_expand_page_cnt; //no expand

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key 的位置索引从2开始到27
    data_ptr->vkb.charactor_first = key_idx = 0;
    CHAR_CNT = QwertKbGetCharKeyCnt (layout);
    keymap_ptr = QwertKbGetKeyMap_ptr (layout);
    kb_pos_ptr = QwertKbGetPosition_ptr (layout);
    key_table_ptr = MMIIM_SP_ML26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);    //此表的顺序是按a~z排列
    if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    }

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;    //仅仅显示首个字符

        //=========语言配置丝印部分
        cur_char_ptr = key_table_ptr + (keymap_ptr[key_idx] - VKEY_A);
        vkb_ptr->keys[key_idx].text_len = cur_char_ptr->key_chars[0];
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr->key_chars[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));

        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = MultiTapCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key 的位置索引从28开始的连续2个
    key_idx++;
    cust_begin = key_idx;

    //period按键
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[key_idx + 2]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].text[0] = '.';
    vkb_ptr->keys[key_idx].text_len = 1;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
    vkb_ptr->keys[key_idx].has_tip = TRUE;
    //有可能需要使用引擎配置的符号
    QwertMultitapConfigPeriod (&vkb_ptr->keys[key_idx], key_table_ptr);

    //,        
    key_idx ++;
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[key_idx + 2]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].text[0] = ',';
    vkb_ptr->keys[key_idx].text_len = 1;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
    vkb_ptr->keys[key_idx].has_tip = TRUE;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //deal tag
    if (GUIIM_TAG_EMAIL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '@';
    }
    else if (GUIIM_TAG_URL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '/';
    }

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key 的位置索引从开始到30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertMultiTapCtrlUp;
    }

    //--Caps
    key_idx = ctrl_begin;
    if (MMIAPIIM_CapsExist(data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
    {
        //特别的记住caps key
        vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
        vkb_ptr->keys[key_idx].code = VKEY_CAPS;
        vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
    }
    else
    {
        QwertMultitapRedefineCaps (data_ptr->init_param.method_param_ptr->im_def_ptr->method, vkb_ptr->keys + key_idx);
    }

    //--delete
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    vkb_ptr->keys[key_idx].f_long = VkeyLongDel;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--space
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_Q;
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_ABC;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh拦截处理
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //被MMIIM_HandleSettingIMUp拦截处理

    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;    
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;

    //sys key 1, system - hide 位置索引1
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[1]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;

    //key art, 用于candidate的容器, 位置索引0
    //art 要放在candidate前面，因为要先画
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
#if defined SOGOU_USE_PDA_SKIN
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //不画背景，防止将sogou皮肤给覆盖掉
#endif
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }

	{
		//将text字段数据同步到display_text字段，不需要在每个text调用地方修改
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);		//上面的SetVkbCaps已经调用过该函数
	}
	
    return;
}

/*==============================================================================
Description: MultiTap输入长按 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertMultiTapCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    MultiTapCommitLast (data_ptr);
    switch (key_ptr->code)
    {
    case VKEY_DEL:
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: MultiTap输入按键提起触笔 - control键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertMultiTapCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }
    
    MultiTapCommitLast(data_ptr);

    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        break;

    case VKEY_DEL:
        CommitChar (data_ptr, '\b');
        break;

    case VKEY_SPACE:
        CommitChar (data_ptr, ' ');
        break;

    case VKEY_ENTER:
        CommitChar (data_ptr, '\n');
        break;

    case VKEY_123SYMBOL:
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertMultiTapSwitchVkb

Func:   

Input:
        data_ptr:  输入法句柄指针
        key_ptr:    操作的按键

Output:

Return:
            err: < 0  succ: == 0

Thoughtway:

Create: yaoguangchen@spread. 2012-06-19

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertMultiTapSwitchVkb (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
	uint8 set_page = 0;
	
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr) 
    {
        return TRUE;
    }

	set_page = (data_ptr->vkb_multi_page.cur_page + 1) % data_ptr->vkb_multi_page.total_page;
	MMIIM_TOUCH_ResetQwertVkbChars (data_ptr, set_page);

	switch (set_page)
	{
		case 1:
		{
			key_ptr->icon = IMG_IMT_CAP_U;
			break;
		}

		default:
		{
			key_ptr->icon = IMG_IMT_CAP_L;
			break;
		}
	}
	
	VkbUp (data_ptr, key_ptr);
	
	return TRUE;
}




PUBLIC int32 QwertSmartRedefineCaps (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr)
{
    if(key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"QwertSmartRedefineCaps method == %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_3529_112_2_18_2_27_28_107,(uint8*)"d", method);
        return -1;
    }

    switch (method)
    {
        case GUIIM_M_TPQ_URDU:
        {
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->tag = KEY_TAG_NORMAL;
            key_ptr->style = KEY_STYLE_CONTROL; //KEY_STYLE_CHARACTER;
            key_ptr->add_style.text_disp = KASC_ALL_TEXT;        //
            SCI_MEMCPY(key_ptr->text,
                       normal_mul_sym,
                       TEXT_LEN_LIMIT - 1);
            key_ptr->text_len = MMIAPICOM_Wstrlen(key_ptr->text);
            key_ptr->f_down = VkbDown;
            key_ptr->f_move = VkbMove;
            key_ptr->f_long = PNULL;
            key_ptr->f_up = MultiTapCharUp;
            key_ptr->has_tip = TRUE;        
            break;
        }
        
        default:
        {
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->tag = KEY_TAG_NORMAL;
            key_ptr->style = KEY_STYLE_CONTROL; //KEY_STYLE_CHARACTER;
            key_ptr->add_style.text_disp = KASC_ALL_TEXT;        //
            SCI_MEMCPY(key_ptr->text,
                       normal_mul_sym,
                       TEXT_LEN_LIMIT - 1);
            key_ptr->text_len = MMIAPICOM_Wstrlen(key_ptr->text);
            key_ptr->f_down = VkbDown;
            key_ptr->f_move = VkbMove;
            key_ptr->f_long = PNULL;
            key_ptr->f_up = MultiTapCharUp;
            key_ptr->has_tip = TRUE;        
            break;
        }
    }

	//将text字段数据同步到display_text字段
	key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
	SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));
	
    return 0;
}


PUBLIC int32 QwertMultitapRedefineCaps (GUIIM_METHOD_T method, KEY_INFO_T *key_ptr)
{
    if(key_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"QwertMultitapRedefineCaps method == %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_3573_112_2_18_2_27_28_108,(uint8*)"d", method);
        return -1;
    }

    switch (method)
    {
        case GUIIM_M_TPQ_THAI_ABC:
        case GUIIM_M_TPQ_HINDI_ABC:    
        {
            key_ptr->text[0] = ' ';
            key_ptr->text_len = 1;
            key_ptr->icon = IMG_IMT_SPACE_CENTER;
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->f_up = MultiTapCharUp;  
            break;
        }

		case GUIIM_M_TPQ_TIBETAN:
		case GUIIM_M_TPQ_TIBETAN_ABC:
        {
            key_ptr->code = VKEY_SWITCH_VKB;
			key_ptr->icon = IMG_IMT_CAP_L;
            key_ptr->f_up = QwertMultiTapSwitchVkb;  
            break;
        }
			
        default:
        {
            key_ptr->code = VKEY_MULTI_CHAR;
            key_ptr->tag = KEY_TAG_NORMAL;
            key_ptr->style = KEY_STYLE_CONTROL; //KEY_STYLE_CHARACTER;
            key_ptr->add_style.text_disp = KASC_ALL_TEXT;        //
            SCI_MEMCPY(key_ptr->text,
                       normal_mul_sym,
                       TEXT_LEN_LIMIT - 1);
            key_ptr->text_len = MMIAPICOM_Wstrlen(key_ptr->text);
            key_ptr->f_down = VkbDown;
            key_ptr->f_move = VkbMove;
            key_ptr->f_long = PNULL;
            key_ptr->f_up = MultiTapCharUp;
            key_ptr->has_tip = TRUE;        
            break;
        }
    }

	//将text字段数据同步到display_text字段
	key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
	SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));
			
    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   MultiTapCharKeyInit

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
PUBLIC int32 MultiTapCharKeyInit(KEY_INFO_T *key_ptr, const wchar *const text_ptr)
{
    uint16 src_str_len = 0;        
    if (key_ptr == PNULL || text_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"MultiTapCharKeyInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_COMMON_3637_112_2_18_2_27_28_109,(uint8*)"");
        return -1;
    }

    src_str_len = MMIAPICOM_Wstrlen (text_ptr);
    key_ptr->code = VKEY_MULTI_CHAR;
    key_ptr->tag = KEY_TAG_NORMAL;
    key_ptr->style = KEY_STYLE_CHARACTER;
    key_ptr->add_style.text_disp = KASC_ALL_TEXT;        //
    key_ptr->text_len = MIN (src_str_len, TEXT_LEN_LIMIT - 1);
    SCI_MEMCPY(key_ptr->text,
               text_ptr,
               key_ptr->text_len * sizeof (wchar));
    key_ptr->f_down = VkbDown;
    key_ptr->f_move = VkbMove;
    key_ptr->f_long = PNULL;
    key_ptr->f_up = MultiTapCharUp;
    key_ptr->has_tip = TRUE;

	//将text字段数据同步到display_text字段
	key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
	SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));
			
    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertMultitapConfigPeriod

Func:   设置点号按键字符

Input:
            key_ptr:    操作的按键
            key_table_ptr:   使用哪个按键数据表中的period键数据

Output:

Return:
            err: < 0  succ: == 0

Thoughtway:

Create: yaoguangchen@spread. 2012-05-25

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 QwertMultitapConfigPeriod (KEY_INFO_T *key_ptr, MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr)
{
    int32 iloop = 0;

    if(key_ptr == PNULL || key_table_ptr == PNULL)
    {
        SCI_TRACE_LOW("[MMIIM] QwertMultitapConfigPeriodKeyText");
        return -1;
    }

    for (iloop = 0; iloop < MMIIM_SP_26KEY_INDEX_MAX; iloop ++)
    {
        if (key_table_ptr[iloop].key_index == MMIIM_SP_26KEY_INDEX_PERIOD)      //存在period键
        {
            key_ptr->tag = KEY_TAG_NORMAL;
            key_ptr->style = KEY_STYLE_CHARACTER;
            key_ptr->add_style.text_disp = KASC_FIRST;        //
            key_ptr->text_len = MIN (key_table_ptr[iloop].key_chars[0], MMIIM_SP_26KEY_CHAR_MAX);
            SCI_MEMCPY(key_ptr->text,
                   &key_table_ptr[iloop].key_chars[1],
                   key_ptr->text_len * sizeof (wchar));
            key_ptr->f_down = VkbDown;
            key_ptr->f_move = VkbMove;
            key_ptr->f_long = PNULL;
            key_ptr->f_up = MultiTapCharUp;
            key_ptr->has_tip = FALSE;

            return 0;
        }
        else if (key_table_ptr[iloop].key_index == MMIIM_SP_26KEY_INDEX_MAX)
        {
            break;
        }
    }

	//将text字段数据同步到display_text字段
	key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
	SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));
			
    return -1;
}



#endif //defined (MMI_IM_PDA_SUPPORT) 

