/*==============================================================================
File Name: mmiim_touch_ime_sogou.c
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

#include "mmi_app_im_trc.h"
#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif


#if defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_SOGOU)

/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "mmiim_touch_ime_sogou.h"
#include "mmiim_touch_ime_hw.h"

#if defined IM_BENGALI_SUPPORT
#define HASANATA (0x09CD)
#endif


extern WD_imeResult g_imeResult;
extern uint8 g_FlashData[FLASH_CAPACITY];
extern uint8 g_sg_save_position;//0:D��,1:E��,2:��D��E



/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatCandPage

Func:   ����sogou���뷨�Ƿ������һҳ��ѡ��

Input:
            data_ptr:  ��ǰ���뷨������

Output:

Return:

Thoughtway:
            1. sogou���뷨û����һ�θ����ж���ҳ��ѡ�֣���Ҫ�Լ�ģ������Ƿ��к�һҳ

Notes:

Create: yaoguangchen@spread. 2011-09-21

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN Sogou_TestCandNextPage(IM_DATA_T *data_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"Sogou_TestCandNextPage"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_61_112_2_18_2_28_2_150,(uint8*)"");
        return FALSE;
    }
    
    if (0 == (pime_result->selecting[((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->lastdisp][0])
            || (pime_result->imeType < WDI_FOREIGN_BEGIN && pime_result->curPage >= MAX_SOGOU_CANDIDATE_PAGE_CHINESE - 1)
            || (pime_result->imeType > WDI_FOREIGN_BEGIN && pime_result->curPage >= MAX_SOGOU_CANDIDATE_PAGE_ENGLISH - 1)
            || (pime_result->imeType < WDI_FOREIGN_BEGIN && pime_result->imeType > WDI_DIGIT
                && ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->lastdisp >= pime_result->selectingCount
                && pime_result->imeStatus != WD_STATUS_HOTKEY_SYMBOL && pime_result->imeStatus != WD_STATUS_DIGIT_INPUT)
       )
    {
        return FALSE;
    }

    return TRUE;
}




/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatCandPage

Func:   ģ��sogou���뷨��ѡ�ַ�ҳ����

Input:
            data_ptr:  ��ǰ���뷨������
            pageup: ���Ϸ�ҳor���·�ҳ

Output:

Return:
            err:  0
            > 0 ������̶�Ӧ������Ӧ�İ�����

Thoughtway:
            1.  ������ͨ������WD_SetVariableֱ�ӽ���ҳǰ��״̬ת��Ϊѡ��״̬�ģ����ƺ���Ч��
                ��ֻ��ͨ�������¼��ķ�ʽ��������״̬ת����ѡ��״̬

Notes:  �ú����и����������������⣬������sogou�⺯���У���sogou�Ǳߴ���

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SimulatCandPage(IM_DATA_T *data_ptr, BOOLEAN pageup)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_ENGINE_DATA(data_ptr);

    //��ǰΪѡ��״̬
    if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECT
            || pime_result->imeStatus == WD_STATUS_FOREIGN_SELECT)
    {
        if (pageup && pime_result->curPage > 0)
        {
            WD_OnKey(WDK_UP, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        }
        else if (pageup == FALSE)   //��һҳ
        {
            WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        }
    }
    else if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECTPINYIN)
    {
        if (pageup == FALSE)    //��һҳ
        {
            WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        }
    }
    //����״̬��ֻ��Ϊ��һҳ
    else if (WD_STATUS_CHINESE_INPUT == pime_result->imeStatus ||
             WD_STATUS_FOREIGN_INPUT == pime_result->imeStatus)
    {
        //if(pageup && pime_result->curPage == 0)   //����״̬��Ĭ��Ϊ��ҳ
        {
            //WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            //WD_OnKey(WDK_UP, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        }
        //else
        if (pageup == FALSE)   //��һҳ
        {
            WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            //WD_SetVariable(E_SELECT_CAND, 0, MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            //WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        }
    }
    //������չ��״̬
    else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND)
    {
        if(pageup)
        {
            WD_OnKey(WDK_UP, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            WD_OnKey(WDK_UP, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        }
        else
        {
            WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            WD_OnKey(WDK_DOWN, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        }
    }

    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatLetterPage

Func:   ģ��sogou���뷨ƴ����ҳ����

Input:
            data_ptr:  ��ǰ���뷨������
            pageup: ���Ϸ�ҳor���·�ҳ

Return:
            err: < 0
            succ:  0

Thoughtway:
            ��ѡƴ����ҳ

Notes:  �ú����и����������������⣬������sogou�⺯���У���sogou�Ǳߴ���

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SimulatLetterPage(IM_DATA_T *data_ptr, BOOLEAN pageup)
{
    uint8 letter_cnt = 0, page_cnt = 0;
    uint8 select_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_ENGINE_DATA(data_ptr);

    letter_cnt = Sogou_GetLetterTotalCnt(data_ptr);
    page_cnt = letter_cnt / data_ptr->vkb.py_candidate_cnt;
    if (letter_cnt % data_ptr->vkb.py_candidate_cnt)
        page_cnt ++;

    //��һҳ
    if (pageup && ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page > 0)
    {
        ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page --;
        select_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.py_candidate_cnt;
        WD_SetVariable(E_SELECT_LETTER, select_idx + 1, pime_result);
    }
    //��һҳ
    else if (!pageup && ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page < page_cnt)
    {
        ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page ++;
        select_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.py_candidate_cnt;
        WD_SetVariable(E_SELECT_LETTER, select_idx + 1, pime_result);
    }

    return 0;
}

/*----------------------------------------------------------------------------------------------------------------------
Name:   ConvetIndexToVKey

Func:   26��������̵İ���������ͨ������ֵ���������ص������ģ����ð���������
            ���뷨����֮��İ�������˳����Ȼ���ԣ���Ҫ��һ��ת��

Input:
            index:  26����������еİ�ť����ֵ

Output:

Return:
            err:  0
            > 0 ������̶�Ӧ������Ӧ�İ�����

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint16 ConvetIndexToVKey(uint16 index)
{
    if (index < g_kb_keymap_size)
    {
        return g_kb_keymap[index];
    }

    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ConvertSpCapToSogou

Func:   ������ƽ̨ʹ�õĴ�Сдת��Ϊsogou���뷨��Сд���ã���������м��ĸ��
            ���ں��ڿ���ά��������ƽ̨�����뷨����

Input:  CAPITTAL_MODE_T cap:ƽ̨��Сдֵ

Output:

Return:
            SOGOU_CAPLOCK_E:    sogou���뷨����Ĵ�Сдֵ

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC SOGOU_CAPLOCK_E ConvertSpCapToSogou(CAPITTAL_MODE_T cap)
{
    switch (cap)
    {
    case CAPITTAL_MODE_INITIAL:
        return E_CAPLOCK_LEAD_UPPER;

    case CAPITTAL_MODE_UPPER:
        return E_CAPLOCK_UPPER;

    case CAPITTAL_MODE_LOWER:
    default:
        return E_CAPLOCK_LOWER;
    }
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetCandDisplayCnt

Func:   ��ȡ���뷨��ǰ��ʾ�ĺ�ѡ�ֻ�������

Input:
            data_ptr:  ��ǰ���뷨������

Output:
            ���ݺ�ѡ�ָ����ͺ�ѡ��ť�������жϵ�ǰ��ʾ�ĺ�ѡ�ָ����������غ�ѡ�ָ���

Return:
            ���뷨��ǰ��ʾ�ĺ�ѡ�ֻ�������

Thoughtway:
            1.  �ú�����Ҫ����Ϊ���뷨������ÿ�ΰ���֮ǰ����Ҫ֪����ǰһ����ʾ�ĺ�ѡ��
                �����������ȡ�ĺ�ѡ�ֻ᲻׼ȷ�������������
            2. ������Sogou_GetLastDisp��ͬ�ĵط���Sogou_GetLastDisp����ȡ��ֵ����һ�����沢����
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetCandDisplayCnt(IM_DATA_T *data_ptr)    //��������ַ�����
{
    uint8 iloop = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_ENGINE_DATA(data_ptr);

    for (iloop = 0; iloop < MAX_ONEPAGE_RECORD && iloop < data_ptr->vkb.candidate_cnt; iloop ++)
    {
        if (MMIAPICOM_Wstrlen(pime_result->selecting[iloop]) == 0)
            break;
    }

    return iloop;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetLetterTotalCnt

Func:   ��ȡ���뷨��ǰ��ѡƴ�����ܸ���

Input:
            data_ptr:  ��ǰ���뷨������

Return:
            ���غ�ѡƴ���ܸ���

Thoughtway:
            1.��ѡƴ���ܸ�����������ʾ��ǰ��ѡƴ���ַ����Ƿ������һҳ
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetLetterTotalCnt(IM_DATA_T *data_ptr)
{
    uint8 iloop = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_ENGINE_DATA(data_ptr);
    
    for (iloop = 0; iloop < MAX_KEY_LETTERCOUNT; iloop ++)
    {
        if (MMIAPICOM_Wstrlen(pime_result->letterSelecting[iloop]) == 0)
            break;
    }

    return iloop;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_DrawVkbSkin

Func:   sogou��Ҫ������Ƥ��

Create: yaoguangchen@spread. 2011-02-02

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_DrawVkbSkin (IM_DATA_T *data_ptr, GUILCD_DRAW_DC_T *pdc_data)
{
    GUI_RECT_T cand_rect = { 0 };

    CHECK_DATA_EX (data_ptr);

    if(pdc_data == PNULL)
    {
        //SCI_TraceLow:"Sogou_DrawVkbSkin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_386_112_2_18_2_28_2_151,(uint8*)"");
        return -1;
    }

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    //ȫ����д���õ�Ϊȫ��������ǰ�������
    if (!data_ptr->is_symbol_panel
        && data_ptr->init_param.method_param_ptr
        && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING
        ) //����д�л���sym�����������뷨ֵ��ȻΪhw
    {
        if (data_ptr->vkb.is_expand)
        {
            HwFullScreenGetDisplayPanelRect (data_ptr, &cand_rect);
        }
        else if (data_ptr->vkb.vessel_key)  //��ֹ��ָ��
        {
            cand_rect = data_ptr->vkb.vessel_key->rect;
        }
        else
        {
            //SCI_TraceLow:"Sogou_DrawVkbSkin"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_407_112_2_18_2_28_2_152,(uint8*)"");
            return -1;
        }
    }
    else
#endif        
    {
        if (data_ptr->vkb.is_expand)
        {
            cand_rect = data_ptr->rect;     
            cand_rect.bottom -= cand_rect.top;      //ƫ����ȥ��
            cand_rect.top = 0;
        }
        else if (data_ptr->vkb.vessel_key)  //��ֹ��ָ��
        {
            cand_rect = data_ptr->vkb.vessel_key->rect;
        }
        else
        {
            //SCI_TraceLow:"Sogou_DrawVkbSkin"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_426_112_2_18_2_28_2_153,(uint8*)"");
            return -1;
        }
    }

    //ȥ�����¸�1�����أ���Ϊ�߿���
    cand_rect.top += 1;
    cand_rect.bottom -= 1;
#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_DrawSkinInRect(data_ptr->init_param.win_id, &cand_rect, pdc_data);
#endif
    return 0;
}


/*==============================================================================
Description: ThumbLatinSwitchExpandKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbLatinSwitchExpandKey(IM_DATA_T * data_ptr)
{
    SwitchExpandKey(data_ptr);
    SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
    SmartSetCand(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbLatinDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[MAX_WORD_LEN + 3] = {0};
    uint32 buffer_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/
    
    UILAYER_Clear(&data_ptr->edit_layer);

    //û���ַ����򷵻أ�������
    if (pime_result->inputKeys[0] == 0)
    {
        //SCI_TraceLow:"ThumbLatinDispEdit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_488_112_2_18_2_28_2_154,(uint8*)"");
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    while (buffer_len < MMIAPICOM_Wstrlen(pime_result->inputKeys))
    {
        buffer[buffer_len] = pime_result->inputKeys[buffer_len];
        buffer_len++;
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len >= edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}

/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIIM_TOUCH_Sogou_GetImeResult

Func:   ��ȡ���뷨����������ݽṹ��ָ��

Input:
            data_ptr:  ��ǰ���뷨������

Output:

Return:
            PNULL:������Ч������ָ��Ϊ��

Thoughtway:

Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:
        1.MMIIM_TOUCH_Sogou_GetImeResult ==>MMIIM_TOUCH_Sogou_GetImeResult;Դ�����Ѿ�����SOUGOU_GetImeResult
        
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC WD_imeResult* MMIIM_TOUCH_Sogou_GetImeResult(IM_DATA_T *data_ptr)
{
    WD_imeResult *presult = PNULL;
    
    CHECK_DATA (data_ptr);

    if (data_ptr->is_symbol_panel)
    {
        //SCI_TraceLow:"MMIIM_TOUCH_Sogou_GetImeResult"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_589_112_2_18_2_28_3_155,(uint8*)"");
        return PNULL;
    }

    switch (data_ptr->init_param.method_param_ptr->im_def_ptr->method)       
    {
#if defined IM_HANDWRITING_SOGOU        
        case GUIIM_M_HANDWRITING:
            presult = ((KERNEL_DATA_HW_T*)(data_ptr->data))->engine_data;
            break;
#endif

        //��������ֹpclint
        case GUIIM_M_TP_ENGLISH_ABC:
        case GUIIM_M_TPQ_ENGLISH_ABC:
            break;
            
        default:
        {
            //�漰�������������
            if (data_ptr->init_param.method_param_ptr->im_def_ptr->type == GUIIM_TYPE_SMART
                || data_ptr->init_param.method_param_ptr->im_def_ptr->type == GUIIM_TYPE_STROKE)
            {
                CHECK_ENGINE_DATA (data_ptr);
                presult = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->pime_result;
            }
            
            break;
        }
    }

    return presult;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SetLastDisp

Func:   �������뷨��һ����ʾ�ĺ�ѡ�ֻ�������

Input:
            data_ptr:  ��ǰ���뷨������
            lastdisp:   ��һ����ʾ�ĺ�ѡ�ֻ�������

Output:
            ����һ�εĺ�ѡ�ֻ����������浽���뷨��������

Return:
            PNULL:������Ч������ָ��Ϊ��

Thoughtway:
            1.  �ú�����Ҫ����Ϊ���뷨������ÿ�ΰ���֮ǰ����Ҫ֪����ǰһ����ʾ�ĺ�ѡ��
                �����������ȡ�ĺ�ѡ�ֻ᲻׼ȷ�������������
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void Sogou_SetLastDisp(IM_DATA_T *data_ptr, uint8 lastdisp)
{
    if (data_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"Sogou_SetLastDisp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_646_112_2_18_2_28_3_156,(uint8*)"");
        return;
    }

    ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->lastdisp = lastdisp;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetLastDisp

Func:   ��ȡ���뷨��һ����ʾ�ĺ�ѡ�ֻ�������

Input:
            data_ptr:  ��ǰ���뷨������

Output:
            ����һ�εĺ�ѡ�ֻ�����������

Return:
            0: ������Ч���״���ʾʱ�ĺ�ѡ�ָ���Ϊ0

Thoughtway:
            1.  �ú�����Ҫ����Ϊ���뷨������ÿ�ΰ���֮ǰ����Ҫ֪����ǰһ����ʾ�ĺ�ѡ��
                �����������ȡ�ĺ�ѡ�ֻ᲻׼ȷ�������������
            2.  �ú���ÿ�ε���ʱ�������½�lastdispֵд����ԭ���ǣ����ǵ�UI��ʱ��Ὣ��ѡ��
                չ�������𣬵�����ʾ�ĺ�ѡ�ָ�����ͬ
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetLastDisp(IM_DATA_T *data_ptr)
{
    if (data_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"Sogou_GetLastDisp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_684_112_2_18_2_28_3_157,(uint8*)"");
        return 0;
    }

    //((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->lastdisp = Sogou_GetCandDisplayCnt(data_ptr);     //�Ե�ǰ��ʾ���ַ�����Ϊ׼
    return ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->lastdisp;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SetCapsLock

Func:   ����sogou���뷨��Сд

Input:
            set_caps: sogou �����Ӧ�Ĵ�Сд

Output:

Return:
            < 0 err, else 0

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SetCapsLock(IM_DATA_T *data_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    int32 iloop = 0;
    SOGOU_CAPLOCK_E prev_cap = E_CAPLOCK_LEAD_UPPER;
    SOGOU_CAPLOCK_E set_caps = E_CAPLOCK_LEAD_UPPER;

    CHECK_DATA_EX (data_ptr);

    if (data_ptr->is_symbol_panel)
    {
        //SCI_TraceLow:"Sogou_SetCapsLock"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_724_112_2_18_2_28_3_158,(uint8*)"");
        return -1;
    }

    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"Sogou_SetCapsLock"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_730_112_2_18_2_28_3_159,(uint8*)"");
        return -1;
    }

    //������״̬�������ڴ�Сд
    if (!MMIAPIIM_CapsExist (data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
    {
        SCI_TraceLow("Sogou_SetCapsLock chinese caps");
        return -1;
    }
    
    set_caps = ConvertSpCapToSogou(data_ptr->vkb.cap);
    if (set_caps >= E_CAPLOCK_MAX) 
    {
        set_caps = E_CAPLOCK_LEAD_UPPER;
    }
    
    if (pime_result->inputKeys[0])  //�Ѿ��������ַ����л���ʽͨ�������ķ�ʽ�л�
    {
        prev_cap = pime_result->capLock % E_CAPLOCK_MAX;
        iloop = (set_caps - prev_cap + E_CAPLOCK_MAX) % E_CAPLOCK_MAX;  /*lint !e656 !e79*/

        for (; iloop > 0; iloop --)
        {
            WD_OnKey(WDK_SHARP, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        }
        
        return 0;
    }
    
    WD_SwitchForeignCapslock(set_caps);

    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_AssociateTo2DemensionArray

Func:   ��sogou����������ת����2ά�����б���

Return:
            < 0 err, else 0

Create: yaoguangchen@spread. 2012-02-01

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_AssociateTo2DemensionArray(wchar *array, wchar *associate, uint16 array_len)
{
    int32 cand_cur = 0, iloop = 0, char_index = 0;
    wchar *pword = PNULL;

    if (array == PNULL || associate == PNULL)
    {
        //SCI_TraceLow:"Sogou_AssociateTo2DemensionArray"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_778_112_2_18_2_28_3_160,(uint8*)"");
        return -1;
    }
    
    //����ʶ������ַ�����Ҫ��buffer��֯��ʽ��ͬ
    for (iloop = 0; iloop < SOGOU_HW_MAXCANDNUM << 2; iloop++)
    {
        if (associate[iloop] == 0 || cand_cur >= SOGOU_HW_MAXCANDNUM)
        {
            break;
        }

        pword = array + (array_len * cand_cur);
        if (associate[iloop] == 0x002c)     //�ָ���
        {
            char_index = iloop + 1;     //�ָ�������һ���ַ�
            cand_cur ++;
        }
        else
        {
            pword[iloop - char_index] = associate[iloop];
        }
    }

    return cand_cur;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_ImeInit

Func:   sogou���뷨�����ʼ��

Input:  g_imeResult: ��̬���������뷨���������������ݽṹ
           g_FlashData: ���뷨��Ҫ�����ݿ�
           MMIAPIIM_GetLanguage():  ��ǰ��������

Output:
            g_imeResult:��̬���������뷨���������������ݽṹ

Return:
            == 0�ɹ��� > 0ʧ��

Thoughtway:
            1. sogou���뷨�����ʼ������Ҫ�������Բ������ʶԵ�ǰ������һ��������ϵ

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_ImeInit(IM_DATA_T *data_ptr, WD_UINT8 method)
{
    uint8 default_ime = 0;
    WD_UINT8 ImeList[IME_LIST_MAX] = { 0 };
    GUIIM_LANGUAGE_T language_type = MMIAPIIM_GetLanguage();
    uint8 ret_value = 0;

    SCI_MEMSET(&g_imeResult, 0, sizeof(WD_imeResult));
    ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->pime_result = &g_imeResult;
    default_ime =	((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->pime_result->imeType;     //== 0

    ImeList[0] = WDI_ENGLISH;
    ImeList[1] = WDI_ENGLISH_ABC;
    ImeList[2] = WDI_DIGIT;

    //���»�ȡһ������
    if (data_ptr->init_param.method_param_ptr
            && data_ptr->init_param.method_param_ptr->im_def_ptr
            && data_ptr->init_param.method_param_ptr->im_def_ptr->lang != GUIIM_LANG_NONE)      //��дʱ��������ֵΪGUIIM_LANG_NONE
    {
        language_type = data_ptr->init_param.method_param_ptr->im_def_ptr->lang;
    }

    switch (language_type)
    {
        // �������뷨
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
        
        // �����������뷨
    case GUIIM_LANG_CHT:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        ImeList[3] = WDI_CN_FULLZHUYIN;
#else
        ImeList[3] = WDI_CN_ZHUYIN;
#endif
        ImeList[4] = WDI_CN_BIHUAF;
        ImeList[5] = 0;

        if (default_ime != WDI_ENGLISH && default_ime != WDI_ENGLISH_ABC &&
            default_ime != WDI_DIGIT   && default_ime != WDI_CN_BIHUAF && 
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

//�ż�����
#ifdef IM_GUJARATI_SUPPORT
    case GUIIM_LANG_GUJARATI:
        ImeList[3] = WDI_GUJARATI;
        ImeList[4] = WDI_GUJARATI_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_GUJARATI && default_ime != WDI_GUJARATI_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_GUJARATI;
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

//RUSSIAN ����
#if defined (IM_RUSSIAN_SUPPORT)
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

//ӡ��
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

//ӡ��
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

//��������
#ifdef IM_MALAY_SUPPORT
    case GUIIM_LANG_MALAY:
        ImeList[3] = WDI_MALAY;
        ImeList[4] = WDI_MALAY_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_MALAY && default_ime != WDI_MALAY_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_MALAY;
        }
        break;
#endif


//Խ��
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

//̩��
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

//��˹
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

//������
#ifdef IM_PUNJABI_SUPPORT
    case GUIIM_LANG_PUNJABI:
        ImeList[3] = WDI_PUNJABI;
        ImeList[4] = WDI_PUNJABI_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_PUNJABI && default_ime != WDI_PUNJABI_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_PUNJABI;
        }
        break;
#endif

//����
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

//����
#ifdef IM_HAUSA_SUPPORT
    case GUIIM_LANG_HAUSA:
        ImeList[3] = WDI_HAUSA;
        ImeList[4] = WDI_HAUSA_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_HAUSA && default_ime != WDI_HAUSA_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_HAUSA;
        }
        break;
#endif

//�ڶ���
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

//HUNGARIAN ������
#if defined (IM_HUNGARIAN_SUPPORT)
    case GUIIM_LANG_HUNGARIAN:
        ImeList[3] = WDI_HUNGARIAN;
        ImeList[4] = WDI_HUNGARIAN_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_HUNGARIAN && default_ime != WDI_HUNGARIAN_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_HUNGARIAN;
        }
        break;
#endif

//PORTUGUESE ������
#if defined (IM_PORTUGUESE_SUPPORT)
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

//SPANISH ������
#if defined (IM_SPANISH_SUPPORT)
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

//ITALIAN �����
#if defined (IM_ITALIAN_SUPPORT)
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

//˹������
#if defined (IM_SWAHILI_SUPPORT)
    case GUIIM_LANG_SWAHILI:
        ImeList[3] = WDI_SWAHILI;
        ImeList[4] = WDI_SWAHILI_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_SWAHILI && default_ime != WDI_SWAHILI_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_SWAHILI;
        }
        
        break;
#endif

//���
#if defined (IM_MYANMAR_SUPPORT)
    case GUIIM_LANG_MYANMAR:
        ImeList[3] = WDI_BURMESE;
        ImeList[4] = WDI_BURMESE_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_BURMESE && default_ime != WDI_BURMESE_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_BURMESE;
        }
        
        break;
#endif

//����կ ������
#if defined (IM_KHMER_SUPPORT)
    case GUIIM_LANG_KHMER:
        ImeList[3] = WDI_KHMER;
        ImeList[4] = WDI_KHMER_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_KHMER && default_ime != WDI_KHMER_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_KHMER;
        }
        
        break;
#endif

//����
#if defined (IM_LAO_SUPPORT)
    case GUIIM_LANG_LAO:
        ImeList[3] = WDI_LAO;
        ImeList[4] = WDI_LAO_ABC;
        ImeList[5] = 0;

        if (default_ime != WDI_LAO && default_ime != WDI_LAO_ABC && default_ime != WDI_DIGIT)
        {
            default_ime = WDI_LAO;
        }
        
        break;
#endif

//������
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

//˹�工��
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

//ά���
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

//ɮ٤��
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
    if(1==SG_Check_D_Space_Is_Enough())
    {
        SG_setBaseFolderPath(L"D:\\@sogou\\");
		g_sg_save_position=1;//Ƥ���ŵ�E��//2012_11_6
    }
    else if(1==SG_Check_E_Space_Is_Enough())
    {
        SG_setBaseFolderPath(L"E:\\@sogou\\");
		g_sg_save_position=1;
    }
	else
	{
		g_sg_save_position=2;
	}

	SCI_TraceLow("[MMIIM]Sogou_ImeInit 2 g_sg_save_position=%d", g_sg_save_position);//6_4
#endif
    {
        ret_value = IME_Initialize(ImeList, default_ime, g_FlashData, ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->pime_result);
        SG_Check_Build_Time();//6_19_1(2012_12_21)
        if (ret_value)
        {
            //SCI_TraceLow:"Sogou_ImeInit"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1233_112_2_18_2_28_4_161,(uint8*)"");
            return ret_value;
        }
    }
    
    ret_value = WD_SwitchIme(method);
    if (ret_value)
    {
        //SCI_TraceLow:"Sogou_ImeInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1240_112_2_18_2_28_4_162,(uint8*)"");
        return ret_value;
    }

    return ret_value;
}


/*==============================================================================
Description: if a key in a keyboard bmp, it will need to paint the bg, so caused 
	need to repaint all other keys.
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_BmpBgKeyCausedRepaint (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
	KEY_INFO_T *key_cand_first_ptr = PNULL;

	if (data_ptr == PNULL || key_ptr == PNULL || data_ptr->vkb.keys == PNULL)
	{
		return FALSE;
	}

	key_cand_first_ptr = &data_ptr->vkb.keys[data_ptr->vkb.candidate_first];

	if (data_ptr->vkb.is_expand)	//expand status
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
	}
	else if (((key_ptr >= key_cand_first_ptr) && (key_ptr < key_cand_first_ptr + data_ptr->vkb.candidate_cnt))
		|| ((data_ptr->vkb.last_key >= key_cand_first_ptr) && (data_ptr->vkb.last_key < key_cand_first_ptr + data_ptr->vkb.candidate_cnt)))
	{
		uint16 iloop = 0;

		//vessel
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);
		
		//cand
		for (iloop = 0; iloop < data_ptr->vkb.candidate_cnt; iloop ++)
		{
			MMIIM_SetVKeyPaintStatus (key_cand_first_ptr + iloop, TRUE);
		}
	}

	return TRUE;
}

#if defined (IM_SIMP_CHINESE_SUPPORT)
/*==============================================================================
Description: ThumbPinyinInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinInit(IM_DATA_T *data_ptr, WD_UINT8 method)
{
    uint8 ret_value = 0;
    
    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_SOGOU_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_SOGOU_T));

    ThumbPinyinInitKb(data_ptr);

    //�����������
    ret_value = Sogou_ImeInit(data_ptr, method);
    if (ret_value != 0)
    {
        //SCI_TRACE_LOW:"ThumbPinyinInit Sogou_ImeInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1269_112_2_18_2_28_4_163,(uint8*)"");
        return FALSE;
    }

    return TRUE;
}




/*==============================================================================
Description: ThumbPinyinTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_SGFreeImeSkinBuffer();
#endif
    SG_Quit();
    
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
PUBLIC void ThumbPinyinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    ThumbPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbPinyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 8;
    uint8 const SYS_CNT = 4;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT;
    uint8 const PY_CAND_CNT = 4;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    int32 py_cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += PY_CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
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
        vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_cn_symb[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCustUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_EXCLUDE_DIGIT;
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbPinyinCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //--1 �ر��趨==>ϸ�ڸĽ� ƴ���ָ��
    vkb_ptr->keys[char_begin].code = VKEY_PY_SEPARATE;
    vkb_ptr->keys[char_begin].text_len = 1;
    SCI_MEMCPY(vkb_ptr->keys[char_begin].text, L"'", vkb_ptr->keys[char_begin].text_len*sizeof(wchar));
    vkb_ptr->keys[char_begin].f_long = ThumbPinyinCharLong;    //������Ϣû�д�����codeֵ�����ı�
    //vkb_ptr->keys[char_begin].font_size = 20;     //�Ŵ���ַ�1��ͬ���Ŵ󣬵������������ֲ�Э��

    //--0 ���һ��char
    key_idx--;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE;

    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
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
        vkb_ptr->keys[key_idx].f_long = ThumbPinyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //face
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_FACE;
    vkb_ptr->keys[key_idx].text_len = 2;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L":)", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));    
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
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;  
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    //vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
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
    //������������ͬһ��λ�ã��������, ��ʼ������expand
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
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinSysUp;
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

    //--expand py pgup - reset rect
    key_idx++;
    vkb_ptr->py_pgup_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[26]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PY_PAGEUP;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;

    //--expand py pgdn - reset rect
    key_idx++;
    vkb_ptr->py_pgdn_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[27]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PY_PAGEDOWN;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;


    //--expand py panel
    key_idx++;
    vkb_ptr->py_vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[25]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
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
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //������������ֹ��sogouƤ�������ǵ�
#endif
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
  
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_thumb_panel_position[0].right - g_thumb_panel_position[0].left - 5;         //�Ҳ���һ������
        int16 h = g_thumb_panel_position[0].bottom - g_thumb_panel_position[0].top - 2 * data_ptr->theme.key_ver_space - 5; //������Щ�հ�,������һ������
        GUI_RECT_T cand_rect = {0};

        //ƫ����ֱ�ӷ���ConvertRect������㣬�����ڶ�����ƫ�������Ǿ��ȵ�
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
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //pinyin candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    py_cand_begin = key_idx;

    for (; key_idx < py_cand_begin + PY_CAND_CNT; key_idx++)
    {
        int16 w = g_thumb_panel_position[25].right - g_thumb_panel_position[25].left + 1;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = g_thumb_panel_position[25].top;
        cand_rect.bottom = g_thumb_panel_position[25].bottom;
        cand_rect.left = g_thumb_panel_position[25].left + (key_idx - py_cand_begin) * w / PY_CAND_CNT;
        cand_rect.right = g_thumb_panel_position[25].left + (key_idx - py_cand_begin + 1) * w / PY_CAND_CNT - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_PY_CANDIDATE + key_idx - py_cand_begin;
        vkb_ptr->keys[key_idx].tag =  KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->py_candidate_first = py_cand_begin;
    vkb_ptr->py_candidate_cnt = PY_CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: Ĵָ����Pinyin���볤�� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result && pime_result->inputKeys[0] > 0 ||PNULL == key_ptr)
    {
        return TRUE;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }
    else if (VKEY_PY_SEPARATE == key_ptr->code)     // 1������Ϊ�ָ���
    {
        CommitChar(data_ptr, '1');
    }
    
    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    if (pime_result == PNULL ||PNULL == key_ptr )
    {
        //SCI_TraceLow:"ThumbPinyinCharUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1712_112_2_18_2_28_5_164,(uint8*)"");
        return FALSE;
    }

    if (VKEY_2 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECT/* && key_ptr->code >= VKEY_5*/)
        {
            // do nothing
            //ϸ�ڸĽ���ȥ��1~4ѡ���ַ�����
        }
        else
        {
            WD_OnKey(WDK_NORMAL_2 + key_ptr->code - VKEY_2, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            if (pime_result->output[0])
            {
                CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
            }
        }
        ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
        if (pime_result->imeStatus == WD_STATUS_CHINESE_LEGEND)
        {
            data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
        }
        else
        {
            data_ptr->vkb.state = KEYBOARD_CANDIDATE;
        }

        ThumbPinyinUpdate(data_ptr);
    }
    else if (VKEY_0 == key_ptr->code)
    {
		if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE || data_ptr->vkb.state == KEYBOARD_CANDIDATE)		//commit first candidates
        {
	        WD_SetVariable(E_SELECT_CAND, 1, pime_result);
	        WD_OnKey(WDK_OK, 0, Sogou_GetLastDisp(data_ptr), pime_result);
	        if (pime_result->output[0])
	        {
	            CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
	        }

	        data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
			((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���

            ThumbPinyinUpdate(data_ptr);
        }
		else if (pime_result->inputKeys[0] == 0)
        {
            //commit a space
            CommitChar(data_ptr, 0x3000);
        }
    }
    else if (VKEY_PY_SEPARATE == key_ptr->code) //ϸ�ڸĽ��� 1��������Ϊ�ָ�����
    {
        WD_OnKey(WDK_SEPERATER, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        ThumbPinyinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA (data_ptr);
    
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr )
    {
        //SCI_TraceLow:"ThumbPinyinCtrlLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1792_112_2_18_2_28_5_165,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (pime_result->inputKeys[0] > 0)
        {
            ThumbPinyinClear(data_ptr);
        }
        //������״̬��û�����밴��
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            ThumbPinyinClear(data_ptr);
        }
        else
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
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbPinyinCtrlUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1844_112_2_18_2_28_5_166,(uint8*)"");
        return FALSE;
    }
    

    switch (key_ptr->code)
    {
    case VKEY_FACE:
        if (pime_result->inputKeys[0] > 0)
            break;
        //open face symbol panel
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_FACE);
        key_ptr = PNULL;
        break;
        
    case VKEY_DEL:

        if (MMIAPICOM_Wstrlen(pime_result->outputKeys) > 1)
        {
            //ѡ��ƴ��״̬����ʹ��WDK_C���л�������״̬������û������һ��ɾ����
            if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECTPINYIN
                    || pime_result->imeStatus == WD_STATUS_CHINESE_SELECT)
            {
                WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            }

            WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            ThumbPinyinUpdate(data_ptr);
        }
        else if (MMIAPICOM_Wstrlen(pime_result->outputKeys) == 1)        //���뷵��ֵ
        {
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            ThumbPinyinClear(data_ptr);
        }
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            ThumbPinyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        if (MMIAPICOM_Wstrlen(pime_result->inputKeys))       //���ڼ��룬ֱ�ӽ�����������
        {
            wchar buffer[MAX_KEYS_LEN * MAX_CHINESE_WORD_LEN + 1] = { 0 };

            WD_OnKey(WDK_ENTER, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            MMIAPICOM_Wstrncpy(buffer, pime_result->output, MAX_KEYS_LEN * MAX_CHINESE_WORD_LEN);
            CommitStr(data_ptr, buffer, MMIAPICOM_Wstrlen(pime_result->output));
            ThumbPinyinClear(data_ptr);
        }
        else        //����
        {
            CommitChar(data_ptr, '\n');
        }
        break;

    case VKEY_PAGEUP:
        Sogou_SimulatCandPage(data_ptr, TRUE);
        ThumbPinyinUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        Sogou_SimulatCandPage(data_ptr, FALSE);
        ThumbPinyinUpdate(data_ptr);
        break;

    case VKEY_123:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - custom��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint8 const CUSTOM_PAGE_THRESHOLD = 2;
    BOOLEAN has_turn_page = FALSE;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        return FALSE;
    }
    
    if (KEY_STYLE_CUSTOM == data_ptr->vkb.first_key->style)
    {
        if (data_ptr->vkb.first_key->code - key_ptr->code <= -1*(CUSTOM_PAGE_THRESHOLD))
        {
            BOOLEAN has_next_page = (Sogou_GetLetterTotalCnt(data_ptr) > (((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page + 1) * data_ptr->vkb.custom_cnt);

            if (has_next_page)
            {
                ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page++;
                ThumbPinyinSetCust(data_ptr);
                has_turn_page = TRUE;
            }
        }
        else if (data_ptr->vkb.first_key->code - key_ptr->code >= CUSTOM_PAGE_THRESHOLD)
        {
            if (((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page > 0)
            {
                ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page--;
                ThumbPinyinSetCust(data_ptr);
                has_turn_page = TRUE;
            }
        }
        //�������һҳʱ���п���ֻ��һ����ť
        else if ((data_ptr->first_tp.y - data_ptr->last_tp.y > CUSTOM_PAGE_THRESHOLD *(key_ptr->rect.bottom - key_ptr->rect.top))       //�������볬��2������
                 && (data_ptr->first_tp.x < key_ptr->rect.right && data_ptr->first_tp.x >= key_ptr->rect.left))  //��cust�����ڻ���
        {
            if (((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page > 0)
            {
                ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page--;
                ThumbPinyinSetCust(data_ptr);
                has_turn_page = TRUE;
            }
        }

    }

    if (!has_turn_page)
    {
        uint16 select_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.custom_cnt;

        select_idx += key_ptr->code - VKEY_CUSTOM;

        if (select_idx < Sogou_GetLetterTotalCnt(data_ptr))       //ѡ���ѡƴ��
        {
            //�л���ѡ��ƴ��״̬
            WD_SetVariable(E_SELECT_LETTER, select_idx + 1, pime_result);
            ThumbPinyinUpdate(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, key_ptr->text[0]);
        }
    }
    else
    {
        //ѡ��ƴ������һҳ���׸�
        WD_SetVariable(E_SELECT_LETTER, ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.custom_cnt + 1, 
                pime_result);
        ThumbPinyinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return FALSE;
    }
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
        ThumbPinyinSetCand(data_ptr);
        break;

    case VKEY_PY_PAGEUP:

        Sogou_SimulatLetterPage(data_ptr, TRUE);
        ThumbPinyinUpdate(data_ptr);
        break;

    case VKEY_PY_PAGEDOWN:

        Sogou_SimulatLetterPage(data_ptr, FALSE);
        ThumbPinyinUpdate(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbPinyinSetLetterCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinSetLetterCand(IM_DATA_T *data_ptr)
{
    uint8 key_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.py_pgup_key); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->vkb.py_pgdn_key); /*assert verified*/
    if (pime_result == PNULL||PNULL == data_ptr->vkb.py_pgup_key||PNULL == data_ptr->vkb.py_pgdn_key)
    {
        //SCI_TraceLow:"ThumbPinyinSetLetterCand"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2093_112_2_18_2_28_6_167,(uint8*)"");
        return;
    }

    if (data_ptr->vkb.state == KEYBOARD_CANDIDATE
            || data_ptr->vkb.state == KEYBOARD_CANDIDATE_EXPAND
            || pime_result->inputKeys[0])
    {
        uint8 pinyin_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.py_candidate_cnt;
        uint8 pinyin_left = Sogou_GetLetterTotalCnt(data_ptr) - pinyin_idx;

        if (((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page > 0)
        {
            data_ptr->vkb.py_pgup_key->is_inactive = 0;
            data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE;
        }
        else
        {
            data_ptr->vkb.py_pgup_key->is_inactive = 1;
            data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgup_key, TRUE);

        if (pinyin_left > data_ptr->vkb.py_candidate_cnt)
        {
            pinyin_left = data_ptr->vkb.py_candidate_cnt;
            data_ptr->vkb.py_pgdn_key->is_inactive = 0;
            data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE;
        }
        else
        {
            data_ptr->vkb.py_pgdn_key->is_inactive = 1;
            data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgdn_key, TRUE);

        for (key_idx = 0; key_idx < pinyin_left; key_idx++)
        {
            //int16 char_idx = 0;

            //for (char_idx = 0; char_idx < pime_result->letterSelecting[pinyin_idx + key_idx].length; char_idx++)
            {
                MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text,
                                  pime_result->letterSelecting[pinyin_idx + key_idx], TEXT_LEN_LIMIT -1);
            }

            data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len =
                MMIAPICOM_Wstrlen(pime_result->letterSelecting[pinyin_idx + key_idx]);
            data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].is_inactive = 0;

			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx], TRUE);
		}
    }
    else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE
             || data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND)
    {
        data_ptr->vkb.py_pgup_key->is_inactive = 1;
        data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgup_key, TRUE);
		
        data_ptr->vkb.py_pgdn_key->is_inactive = 1;
        data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgdn_key, TRUE);
    }

    for (; key_idx < data_ptr->vkb.py_candidate_cnt; key_idx++)
    {
        data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len = 0;
        data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].is_inactive = 1;

		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT);
		SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text, 
			data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text, 
			data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len * sizeof (wchar));

		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx], TRUE);
    }

    return;
}
/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint16 select_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbPinyinCandUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2177_112_2_18_2_28_6_168,(uint8*)"");
        return FALSE;
    }

    select_idx = key_ptr->code - VKEY_CANDIDATE;
    //SCI_ASSERT(select_idx >= 0); /*assert verified*/
    
    //��ѡ��
    if (VKEY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_CANDIDATE + data_ptr->vkb.candidate_cnt)
    {
        WD_SetVariable(E_SELECT_CAND, select_idx + 1, pime_result);
        WD_OnKey(WDK_OK, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        if (pime_result->output[0])
        {
            CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
        }

        if (data_ptr->vkb.is_expand)
        {
            SwitchExpandKey(data_ptr);
        }

        data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
    }
    //��ѡƴ��
    else if (VKEY_PY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_PY_CANDIDATE + data_ptr->vkb.py_candidate_cnt)
    {
        select_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.py_candidate_cnt;
        select_idx += key_ptr->code - VKEY_PY_CANDIDATE;
        if (select_idx < Sogou_GetLetterTotalCnt(data_ptr))
        {
            WD_SetVariable(E_SELECT_LETTER, select_idx + 1, pime_result);
        }
    }

    ThumbPinyinUpdate(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbPinyinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    WD_OnKey(WDK_LONG_C, 0, Sogou_GetCandDisplayCnt(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
    data_ptr->vkb.state = KEYBOARD_INITIAL;
    SetHideKey(data_ptr, 0);
    ThumbPinyinSetCand(data_ptr);
    ThumbPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbPinyinUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinUpdate(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
    ThumbPinyinSetCand(data_ptr);
    ThumbPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbPinyinSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA (data_ptr);
	
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbPinyinSetCand"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2280_112_2_18_2_28_6_169,(uint8*)"");
        return;
    }

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}
	
    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == Sogou_GetCandDisplayCnt(data_ptr))
    {
        goto ThumbPinyinSetCandEnd;
    }
    
    //û��չ��״̬
    while (cand_cnt < Sogou_GetCandDisplayCnt(data_ptr))
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[cand_key_idx].text, pime_result->selecting[cand_cnt], TEXT_LEN_LIMIT - 1);
        data_ptr->vkb.keys[cand_key_idx].text_len = MMIAPICOM_Wstrlen(data_ptr->vkb.keys[cand_key_idx].text);
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
        
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
        cand_key_idx++;
        cand_cnt++;
    }

    //��Ȼ�޷�ȫ����ʾ������չ��ʱ�������õõ�
    Sogou_SetLastDisp(data_ptr, cand_cnt);
    if (1 == data_ptr->vkb.is_expand)
    {
        //��ǰһҳ
        if (pime_result->curPage > 0)
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
		
        //�к�һҳ
        //if (pime_result->curPage < MAX_SOGOU_CANDIDATE_PAGE - 1)
        if (Sogou_TestCandNextPage(data_ptr))
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

        ThumbPinyinSetLetterCand(data_ptr);
    }

ThumbPinyinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    if (0 == data_ptr->vkb.is_expand)
    {
        ThumbPinyinSetCust(data_ptr);
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[MAX_WORD_LEN + 3] = {0};
    uint32 buffer_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbPinyinDispEdit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2382_112_2_18_2_28_6_170,(uint8*)"");
        return;
    }
    
    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/

    UILAYER_Clear(&data_ptr->edit_layer);

    //û���ַ����򷵻أ�������
    if (pime_result->outputKeys[0] == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    while (buffer_len < MMIAPICOM_Wstrlen(pime_result->outputKeys))
    {
        buffer[buffer_len] = pime_result->outputKeys[buffer_len];
        buffer_len++;
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len >= edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*==============================================================================
Description: ThumbPinyinSetCust
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinSetCust(IM_DATA_T *data_ptr)
{
    uint8 key_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbPinyinSetCust"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2480_112_2_18_2_28_7_171,(uint8*)"");
        return;
    }

    //��ѡƴ��
    if (pime_result->letterSelecting[0][0] > 0)
    {
        uint8 pinyin_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.custom_cnt;
        uint8 pinyin_left = Sogou_GetLetterTotalCnt(data_ptr) - pinyin_idx;

        if (pinyin_left > data_ptr->vkb.custom_cnt)
        {
            pinyin_left = data_ptr->vkb.custom_cnt;
        }

        //��ѡƴ��
        for (key_idx = 0; key_idx < pinyin_left; key_idx++)
        {
            //int16 char_idx = 0;

            //for (char_idx = 0; char_idx < pime_result->letterSelecting[pinyin_idx + key_idx].length; char_idx++)
            {
                MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text,
                                  pime_result->letterSelecting[pinyin_idx + key_idx], TEXT_LEN_LIMIT - 1);
            }

            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = MMIAPICOM_Wstrlen(data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text);
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}

        //��ѡƴ������һҳʱ
        for (; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = 0;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 1;

			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len;
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
        }
    }
    //����
    else
    {
        for (key_idx = 0; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text[0] = g_thumb_cust_cn_symb[key_idx];
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = 1;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}
    }

    return;
}
#endif

#if (defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT))
/*==============================================================================
Description: ThumbStrokeInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeInit(IM_DATA_T *data_ptr, WD_UINT8 method)
{
    uint8 ret_value = 0;
    
    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_SOGOU_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_SOGOU_T));

    ThumbStrokeInitKb(data_ptr);

    //������س�ʼ��
    ret_value = Sogou_ImeInit (data_ptr, method);
    if (ret_value != 0)
    {
        //SCI_TRACE_LOW:"QwertPinyinInit Sogou_ImeInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2553_112_2_18_2_28_7_172,(uint8*)"");
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: ThumbStrokeTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);
 
    //release engine
#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_SGFreeImeSkinBuffer();
#endif
    SG_Quit();
    
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
PUBLIC void ThumbStrokeDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    ThumbStrokeDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbStrokeInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 7; //����0�����ո�
    uint8 const CTRL_CNT = 7;
    uint8 const SYS_CNT = 4;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT_E;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_stroke_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_stroke_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_cn_symb[key_idx - cust_begin];
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

    //character * 6
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_stroke_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = g_stroke_symb[key_idx - char_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbStrokeCharUp;
    }

    key_idx--;

    //--0 ���һ��char
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].subtext[0] = 0x0027; // �ָ���
    vkb_ptr->keys[key_idx].subtext_len = 1;
    vkb_ptr->keys[key_idx].text_len = 0;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE;

    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_stroke_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbStrokeCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbStrokeCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
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
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
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
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_stroke_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0,
                    0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbStrokeSysUp;
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

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_stroke_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
#if defined SOGOU_USE_PDA_SKIN
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //������������ֹ��sogouƤ�������ǵ�
#endif
    vkb_ptr->keys[key_idx].is_inactive = 1;

    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_stroke_panel_position[0].right - g_stroke_panel_position[0].left - 5;         //�Ҳ���һ������
        int16 h = g_stroke_panel_position[0].bottom - g_stroke_panel_position[0].top - 2 * data_ptr->theme.key_ver_space - 5; //������Щ�հ�,������һ������
        GUI_RECT_T cand_rect = {0};

        //ƫ����ֱ�ӷ���ConvertRect������㣬�����ڶ�����ƫ�������Ǿ��ȵ�
        cand_rect.top = g_stroke_panel_position[0].top + y * (h + 2 * data_ptr->theme.key_ver_space) + data_ptr->theme.key_ver_space + 5;
        cand_rect.bottom = cand_rect.top + h;
        cand_rect.left = g_stroke_panel_position[0].left + x * w / CH_CANDIDATE_COL + 1;
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
        vkb_ptr->keys[key_idx].f_up = ThumbStrokeCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: Ĵָ����Stroke���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL || PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbStrokeCharUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2862_112_2_18_2_28_7_173,(uint8*)"");
        return FALSE;
    }
    
    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_6)
    {
        if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECT && key_ptr->code >= VKEY_5)
        {
            // do nothing
        }
        else
        {
            WD_OnKey(WDK_NORMAL_1 + key_ptr->code - VKEY_1, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            if (pime_result->output[0])
            {
                CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
            }
        }
        ThumbStrokeUpdate(data_ptr);
    }
    else if (VKEY_SPACE == key_ptr->code)
    {
        if (pime_result->inputKeys[0] == 0)
        {
            //commit a space
            CommitChar(data_ptr, 0x3000);
        }
        else
        {
            WD_OnKey(WDK_NORMAL_0, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            ThumbStrokeUpdate(data_ptr);
        }
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Stroke���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL || PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbStrokeCtrlLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2916_112_2_18_2_28_8_174,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (pime_result->inputKeys[0] > 0)
        {
            ThumbStrokeClear(data_ptr);
        }
        //������״̬��û�����밴��
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            ThumbStrokeClear(data_ptr);
        }
        else
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
Description: Ĵָ����Stroke���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL || PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbStrokeCtrlUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2968_112_2_18_2_28_8_175,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (MMIAPICOM_Wstrlen(pime_result->outputKeys) > 1)       //��������ֵ
        {
            //ѡ���ַ�״̬����ʹ��WDK_C���л�������״̬������û������һ��ɾ����
            if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECT)
            {
                WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            }

            WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            ThumbStrokeUpdate(data_ptr);
        }
        else if (MMIAPICOM_Wstrlen(pime_result->outputKeys) == 1)        //���뷵��ֵ
        {
            ThumbStrokeClear(data_ptr);
        }
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            ThumbStrokeClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        {
            if (pime_result->inputKeys[0])    //���ڼ�������
            {
                wchar buffer[MAX_KEYS_LEN + 3] = {0};
                uint32 buffer_len = 0;

                while (buffer_len < MMIAPICOM_Wstrlen(pime_result->inputKeys))
                {
                    int16 char_idx = pime_result->inputKeys[buffer_len] - '1';
                    if (pime_result->inputKeys[buffer_len] == 0x0027) // �ָ���
                    {
                        buffer[buffer_len++] = 0x0027;
                        continue;
                    }

                    //SCI_ASSERT(char_idx >= 0); /*assert verified*/
                    //SCI_ASSERT(char_idx < 6); /*assert verified*/

                    if(char_idx >= 0 && char_idx < 6)
                    {
                        buffer[buffer_len++] = g_stroke_symb[char_idx];
                    }
                }

                CommitStr(data_ptr, buffer, buffer_len);
                ThumbStrokeClear(data_ptr);
            }
            else
            {
                CommitChar(data_ptr, '\n');
            }

            break;
        }

    case VKEY_PAGEUP:
        Sogou_SimulatCandPage(data_ptr, TRUE);
        ThumbStrokeUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        Sogou_SimulatCandPage(data_ptr, FALSE);
        ThumbStrokeUpdate(data_ptr);
        break;

    case VKEY_123:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Stroke���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return FALSE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
        ThumbStrokeSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Stroke���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 select_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbStrokeCandUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3129_112_2_18_2_28_8_176,(uint8*)"");
        return FALSE;
    }

    select_idx = key_ptr->code - VKEY_CANDIDATE;
    //SCI_ASSERT(select_idx >= 0); /*assert verified*/

    //��ѡ��
    if (VKEY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_CANDIDATE + data_ptr->vkb.candidate_cnt)
    {
        WD_SetVariable(E_SELECT_CAND, select_idx + 1, pime_result);
        WD_OnKey(WDK_OK, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        if (pime_result->output[0])
        {
            CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
        }
        if (data_ptr->vkb.is_expand)
        {
            SwitchExpandKey(data_ptr);
        }

        data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
    }

    ThumbStrokeUpdate(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbStrokeClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    WD_OnKey(WDK_LONG_C, 0, Sogou_GetCandDisplayCnt(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));

    if (data_ptr->is_symbol_panel)
        return;

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    data_ptr->vkb.state = KEYBOARD_INITIAL;
    SetHideKey(data_ptr, 0);
    ThumbStrokeSetCand(data_ptr);
    ThumbStrokeDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbStrokeUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeUpdate(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
    ThumbStrokeSetCand(data_ptr);
    ThumbStrokeDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbStrokeSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbStrokeSetCand"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3221_112_2_18_2_28_8_177,(uint8*)"");
        return;
    }

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == Sogou_GetCandDisplayCnt(data_ptr))
    {
        goto ThumbStrokeSetCandEnd;
    }

    //û��չ��״̬
    while (cand_cnt < Sogou_GetCandDisplayCnt(data_ptr))
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[cand_key_idx].text, pime_result->selecting[cand_cnt], TEXT_LEN_LIMIT - 1);
        data_ptr->vkb.keys[cand_key_idx].text_len = MMIAPICOM_Wstrlen(data_ptr->vkb.keys[cand_key_idx].text);
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
		
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);	
		
        cand_key_idx++;
        cand_cnt++;
    }

    //��Ȼ�޷�ȫ����ʾ������չ��ʱ�������õõ�
    Sogou_SetLastDisp(data_ptr, cand_cnt);
    if (1 == data_ptr->vkb.is_expand)
    {
        //��ǰһҳ
        if (pime_result->curPage > 0)
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

        //�к�һҳ
        //if (pime_result->curPage < MAX_SOGOU_CANDIDATE_PAGE - 1 && data_ptr->vkb.state != KEYBOARD_ASSOCIATE_EXPAND)
        if (Sogou_TestCandNextPage(data_ptr) && data_ptr->vkb.state != KEYBOARD_ASSOCIATE_EXPAND)
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

ThumbStrokeSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[MAX_KEYS_LEN] = {0};
    uint32 buffer_len = 0;
    uint16 i = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbStrokeDispEdit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3317_112_2_18_2_28_8_178,(uint8*)"");
        return;
    }
    
    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/

    UILAYER_Clear(&data_ptr->edit_layer);

    if (pime_result->outputKeys[0] == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    //�ʻ�
    for (i = 0; i < MMIAPICOM_Wstrlen((const wchar*)pime_result->outputKeys); i++)
    {

        buffer[buffer_len++] = pime_result->outputKeys[i];

    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len >= edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}
#endif


/*==============================================================================
Description: QwertLatinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertLatinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel)
        return;

    WD_OnKey(WDK_LONG_C, 0, Sogou_GetCandDisplayCnt(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, 0);
    SmartSetCand (data_ptr);

    return;
}



/*==============================================================================
Description: QwertSmartSwitchExpandKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertSmartSwitchExpandKey(IM_DATA_T * data_ptr)
{
    SwitchExpandKey(data_ptr);
    SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
    SmartSetCand (data_ptr);

    return;
}


#if defined (IM_SIMP_CHINESE_SUPPORT)
/*==============================================================================
Description: QwertPinyinInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinInit(IM_DATA_T *data_ptr, WD_UINT8 method)
{
    uint8 ret_value = 0;
    
    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_SOGOU_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_SOGOU_T));

    QwertPinyinInitKb(data_ptr);

    ret_value = Sogou_ImeInit (data_ptr, method);
    if (ret_value != 0)
    {
        //SCI_TRACE_LOW:"QwertPinyinInit Sogou_ImeInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3465_112_2_18_2_28_9_179,(uint8*)"");
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: QwertPinyinTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_SGFreeImeSkinBuffer();
#endif
    SG_Quit();
    
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: QwertAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    QwertPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void QwertPinyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 const CHAR_CNT = 26;
    uint8 const CTRL_CNT = 9;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT_E;
    uint8 const SYS_CNT = 2;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
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
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key ��λ��������2��ʼ��27
    data_ptr->vkb.charactor_first = key_idx = 0;

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].text[0] = "qwertyuiopasdfghjklzxcvbnm"[key_idx];
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertPinyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key ��λ��������28��ʼ������2��
    key_idx++;
    cust_begin = key_idx;

    for (; key_idx < cust_begin + CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = L"\xff1f\xff0c"[key_idx - cust_begin];
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

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key ��λ�������ӿ�ʼ��30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertPinyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = QwertPinyinCtrlUp;
    }

    //--ƴ���ָ��
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_PY_SEPARATE;
    vkb_ptr->keys[key_idx].text_len = 1;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"'", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    vkb_ptr->keys[key_idx].font_size = 20;
    //--delete
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
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
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
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
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0,
                    0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertPinyinSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;


    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_qwert_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
#if defined SOGOU_USE_PDA_SKIN
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //������������ֹ��sogouƤ�������ǵ�
#endif
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
     
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_qwert_panel_position[0].right - g_qwert_panel_position[0].left - 5;         //�Ҳ���һ������
        int16 h = g_qwert_panel_position[0].bottom - g_qwert_panel_position[0].top - data_ptr->theme.key_ver_space - 5; //������Щ�հ�,������һ������
        GUI_RECT_T cand_rect = {0};

        //ƫ����ֱ�ӷ���ConvertRect������㣬�����ڶ�����ƫ�������Ǿ��ȵ�
        cand_rect.top = g_qwert_panel_position[0].top + y * (h + data_ptr->theme.key_ver_space) + data_ptr->theme.key_ver_space / 2 + 5;
        cand_rect.bottom = cand_rect.top + h;
        cand_rect.left = g_qwert_panel_position[0].left + x * w / CH_CANDIDATE_COL + 1;
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
        vkb_ptr->keys[key_idx].f_up = QwertPinyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint16 key_code = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult(data_ptr);

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    if(PNULL == key_ptr || pime_result == PNULL)
    {
        return FALSE;
    }   
    
    if (VKEY_A <= key_ptr->code && key_ptr->code <= VKEY_Z)
    {
        key_code = ConvetIndexToVKey(key_ptr->code - VKEY_A);

        WD_OnKey(WDK_NORMAL_A + key_code - VKEY_A, 0, Sogou_GetLastDisp(data_ptr), pime_result);

        if (pime_result->imeStatus == WD_STATUS_CHINESE_LEGEND)
        {
            data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
        }
        else
        {
            data_ptr->vkb.state = KEYBOARD_CANDIDATE;
        }
        
        QwertPinyinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL ||PNULL == key_ptr )
    {
        //SCI_TraceLow:"QwertPinyinCtrlLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3817_112_2_18_2_28_9_180,(uint8*)"");
        return FALSE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (pime_result->inputKeys[0] > 0)
        {
            QwertPinyinClear(data_ptr);
        }
        //������״̬��û�����밴��
        else if(data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            QwertPinyinClear(data_ptr);
        }
        else
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
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"QwertPinyinCtrlUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3869_112_2_18_2_28_9_181,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_PY_SEPARATE:
        WD_OnKey(WDK_SEPERATER, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        QwertPinyinUpdate(data_ptr);
        break;

    case VKEY_DEL:

        if (MMIAPICOM_Wstrlen(pime_result->outputKeys) > 1)       //��������ֵ
        {
            //ѡ��ƴ��״̬����ʹ��WDK_C���л�������״̬������û������һ��ɾ����
            if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECTPINYIN
                    || pime_result->imeStatus == WD_STATUS_CHINESE_SELECT)
            {
                WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            }

            WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            QwertPinyinUpdate(data_ptr);
        }
        else if (MMIAPICOM_Wstrlen(pime_result->outputKeys) == 1)        //���뷵��ֵ
        {
            QwertPinyinClear(data_ptr);
        }
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            QwertPinyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        {
            uint32 buffer_len = MMIAPICOM_Wstrlen(pime_result->inputKeys);

            if (buffer_len)       //���ڼ��룬ֱ�ӽ�����������
            {
                wchar buffer[MAX_KEYS_LEN * MAX_CHINESE_WORD_LEN + 1] = { 0 };

                WD_OnKey(WDK_ENTER, 0, Sogou_GetLastDisp(data_ptr), pime_result);
                MMIAPICOM_Wstrncpy(buffer, pime_result->output, MAX_KEYS_LEN * MAX_CHINESE_WORD_LEN);
                CommitStr(data_ptr, buffer, MMIAPICOM_Wstrlen(pime_result->output));
                QwertPinyinClear(data_ptr);
            }
            else        //����
            {
                CommitChar(data_ptr, '\n');
            }

            break;
        }

    case VKEY_SPACE:

        if (pime_result->inputKeys[0] == 0)
        {
            CommitChar(data_ptr, 0x3000);
        }

        break;

    case VKEY_PAGEUP:
        Sogou_SimulatCandPage(data_ptr, TRUE);
        QwertPinyinUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        Sogou_SimulatCandPage(data_ptr, FALSE);
        QwertPinyinUpdate(data_ptr);
        break;

    case VKEY_123SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return FALSE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
        QwertPinyinSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint16 select_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
   // SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL ||key_ptr == PNULL  )
    {
        //SCI_TraceLow:"QwertPinyinCandUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_4017_112_2_18_2_28_10_182,(uint8*)"");
        return FALSE;
    }

    select_idx = key_ptr->code - VKEY_CANDIDATE;
    //SCI_ASSERT(select_idx >= 0); /*assert verified*/

    //��ѡ��
    if (VKEY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_CANDIDATE + data_ptr->vkb.candidate_cnt)
    {
        WD_SetVariable(E_SELECT_CAND, select_idx + 1, pime_result);
        WD_OnKey(WDK_OK, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        if (pime_result->output[0])
        {
            CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
        }

        if (data_ptr->vkb.is_expand)
        {
            SwitchExpandKey(data_ptr);
        }

        data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
    }
    //��ѡƴ��
    else if (VKEY_PY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_PY_CANDIDATE + data_ptr->vkb.py_candidate_cnt)
    {
        select_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.py_candidate_cnt;

        select_idx += key_ptr->code - VKEY_PY_CANDIDATE;

        if (select_idx < Sogou_GetLetterTotalCnt(data_ptr))
        {
            WD_SetVariable(E_SELECT_LETTER, select_idx + 1, pime_result);
        }
    }

    QwertPinyinUpdate(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: QwertPinyinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinClear(IM_DATA_T *data_ptr)
{
    int32 iloop = Sogou_GetCandDisplayCnt(data_ptr);

    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    WD_OnKey(WDK_LONG_C, 0, iloop, MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }
    
    data_ptr->vkb.state = KEYBOARD_INITIAL;
    SetHideKey(data_ptr, 0);
    QwertPinyinSetCand(data_ptr);
    QwertPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: QwertPinyinUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinUpdate(IM_DATA_T *data_ptr)
{
    uint8 cand_cnt = 0;

    CHECK_DATA_EX(data_ptr);

    cand_cnt = Sogou_GetCandDisplayCnt(data_ptr);


    SetHideKey(data_ptr, cand_cnt);

    QwertPinyinSetCand(data_ptr);
    QwertPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: QwertPinyinSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"QwertPinyinSetCand"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_4129_112_2_18_2_28_10_183,(uint8*)"");
        return;
    }

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}
	
    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == MMIAPICOM_Wstrlen(pime_result->selecting[0]))
    {
        goto QwertPinyinSetCandEnd;
    }

    while (cand_cnt < Sogou_GetCandDisplayCnt(data_ptr) && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[cand_key_idx].text, pime_result->selecting[cand_cnt], TEXT_LEN_LIMIT - 1);
        data_ptr->vkb.keys[cand_key_idx].text_len = MMIAPICOM_Wstrlen(data_ptr->vkb.keys[cand_key_idx].text);
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));

        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);	
        cand_key_idx++;
        cand_cnt++;
    }

    Sogou_SetLastDisp(data_ptr, cand_cnt);
    if (1 == data_ptr->vkb.is_expand)       //չ��״̬
    {
        //��ǰһҳ
        if (pime_result->curPage > 0)
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

        //�к�һҳ
        //if (pime_result->curPage < MAX_SOGOU_CANDIDATE_PAGE - 1 && data_ptr->vkb.state != KEYBOARD_ASSOCIATE_EXPAND)
        if (Sogou_TestCandNextPage(data_ptr))       // && data_ptr->vkb.state != KEYBOARD_ASSOCIATE_EXPAND)  //����ط������󣬲���Ҫ�жϼ���״̬
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);
    }

QwertPinyinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinDispEdit(IM_DATA_T *data_ptr)
{
    ThumbPinyinDispEdit(data_ptr);
    return;
}
#endif

/* 1.sogou����������������Ļ�������һ��
// 2.��������û�д򿪣����ʹ��ͬһ�״��룬����Ҫ�ں�����
//    ��һЩ�޸����ּ��壬
// 3.Ϊ������cstar���뿴�����ͷ��һ�£����������׳����*/
#if defined (IM_TRAD_CHINESE_SUPPORT)
/*==============================================================================
Description: ThumbZhuyinInit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinInit(IM_DATA_T *data_ptr, WD_UINT8 method)
{
    uint8 ret_value = 0;
    
    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_SOGOU_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_SOGOU_T));

    ThumbZhuyinInitKb(data_ptr);

    //�����������
    ret_value = Sogou_ImeInit(data_ptr, method);
    if (ret_value != 0)
    {
        //SCI_TRACE_LOW:"ThumbZhuyinInit Sogou_ImeInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1269_112_2_18_2_28_4_163,(uint8*)"");
        return FALSE;
    }

    return TRUE;
}



/*==============================================================================
Description: ThumbZhuyinTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    MMIAPIIM_SGFreeImeSkinBuffer();
    SG_Quit();
    
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcDraw
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    ThumbZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: yaoguang.chen
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbZhuyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 8;
    uint8 const SYS_CNT = 4;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT;
    uint8 const PY_CAND_CNT = 4;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    int32 py_cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    
    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += PY_CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
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
        vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_cn_symb[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCustUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable(GUIIM_M_TP_ZHUYIN);
    if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(GUIIM_M_TP_ZHUYIN);
    }

    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_2X4_EXCLUDE_DIGIT;
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbZhuyinCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }
    
    //--1 �ر��趨==>ϸ�ڸĽ� ƴ���ָ��
    vkb_ptr->keys[char_begin].code = VKEY_PY_SEPARATE;
    vkb_ptr->keys[char_begin].text_len = 1;
    SCI_MEMCPY(vkb_ptr->keys[char_begin].text, L"'", vkb_ptr->keys[char_begin].text_len*sizeof(wchar));
    vkb_ptr->keys[char_begin].f_long = ThumbZhuyinCharLong;    //������Ϣû�д�����codeֵ�����ı�
    //vkb_ptr->keys[char_begin].font_size = 20;     //�Ŵ���ַ�1��ͬ���Ŵ󣬵������������ֲ�Э��

    //
    key_idx--;
    
    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
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
        vkb_ptr->keys[key_idx].f_long = ThumbZhuyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //face
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_FACE;
    vkb_ptr->keys[key_idx].text_len = 2;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L":)", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));    
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
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;  
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    //vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
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
    //������������ͬһ��λ�ã��������, ��ʼ������expand
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
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinSysUp;
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

    //--expand py pgup - reset rect
    key_idx++;
    vkb_ptr->py_pgup_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[26]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PY_PAGEUP;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;

    //--expand py pgdn - reset rect
    key_idx++;
    vkb_ptr->py_pgdn_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[27]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PY_PAGEDOWN;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;


    //--expand py panel
    key_idx++;
    vkb_ptr->py_vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[25]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
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
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //������������ֹ��sogouƤ�������ǵ�
#endif
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
  
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_thumb_panel_position[0].right - g_thumb_panel_position[0].left - 5;         //�Ҳ���һ������
        int16 h = g_thumb_panel_position[0].bottom - g_thumb_panel_position[0].top - 2 * data_ptr->theme.key_ver_space - 5; //������Щ�հ�,������һ������
        GUI_RECT_T cand_rect = {0};

        //ƫ����ֱ�ӷ���ConvertRect������㣬�����ڶ�����ƫ�������Ǿ��ȵ�
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
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //pinyin candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    py_cand_begin = key_idx;

    for (; key_idx < py_cand_begin + PY_CAND_CNT; key_idx++)
    {
        int16 w = g_thumb_panel_position[25].right - g_thumb_panel_position[25].left + 1;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = g_thumb_panel_position[25].top;
        cand_rect.bottom = g_thumb_panel_position[25].bottom;
        cand_rect.left = g_thumb_panel_position[25].left + (key_idx - py_cand_begin) * w / PY_CAND_CNT;
        cand_rect.right = g_thumb_panel_position[25].left + (key_idx - py_cand_begin + 1) * w / PY_CAND_CNT - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_PY_CANDIDATE + key_idx - py_cand_begin;
        vkb_ptr->keys[key_idx].tag =  KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->py_candidate_first = py_cand_begin;
    vkb_ptr->py_candidate_cnt = PY_CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

    return;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - character��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result && pime_result->inputKeys[0] > 0 ||PNULL == key_ptr)
    {
        return TRUE;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }
    else if (VKEY_PY_SEPARATE == key_ptr->code)     // 1������Ϊ�ָ���
    {
        CommitChar(data_ptr, '1');
    }
    
    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    if (pime_result == PNULL ||PNULL == key_ptr )
    {
        //SCI_TraceLow:"ThumbZhuyinCharUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1712_112_2_18_2_28_5_164,(uint8*)"");
        return FALSE;
    }

    if (VKEY_2 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECT/* && key_ptr->code >= VKEY_5*/)
        {
            // do nothing
            //ϸ�ڸĽ���ȥ��1~4ѡ���ַ�����
        }
        else
        {
            WD_OnKey(WDK_NORMAL_2 + key_ptr->code - VKEY_2, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
            if (pime_result->output[0])
            {
                CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
            }
        }
        ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
        if (pime_result->imeStatus == WD_STATUS_CHINESE_LEGEND)
        {
            data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
        }
        else
        {
            data_ptr->vkb.state = KEYBOARD_CANDIDATE;
        }

        ThumbZhuyinUpdate(data_ptr);
    }
    else if (VKEY_0 == key_ptr->code)
    {
        WD_OnKey(WDK_NORMAL_0, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
        if (pime_result->imeStatus == WD_STATUS_CHINESE_LEGEND)
        {
            data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
        }
        else
        {
            data_ptr->vkb.state = KEYBOARD_CANDIDATE;
        }
        
        ThumbZhuyinUpdate(data_ptr);
    }
    else if (VKEY_PY_SEPARATE == key_ptr->code) //ϸ�ڸĽ��� 1��������Ϊ�ָ�����
    {
        WD_OnKey(WDK_SEPERATER, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        ThumbZhuyinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA (data_ptr);
    
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr )
    {
        //SCI_TraceLow:"ThumbZhuyinCtrlLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1792_112_2_18_2_28_5_165,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (pime_result->inputKeys[0] > 0)
        {
            ThumbZhuyinClear(data_ptr);
        }
        //������״̬��û�����밴��
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            ThumbZhuyinClear(data_ptr);
        }
        else
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
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbZhuyinCtrlUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_1844_112_2_18_2_28_5_166,(uint8*)"");
        return FALSE;
    }
    

    switch (key_ptr->code)
    {
    case VKEY_FACE:
        if (pime_result->inputKeys[0] > 0)
            break;
        //open face symbol panel
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_FACE);
        key_ptr = PNULL;
        break;
        
    case VKEY_DEL:

        if (MMIAPICOM_Wstrlen(pime_result->outputKeys) > 1)
        {
            //ѡ��ƴ��״̬����ʹ��WDK_C���л�������״̬������û������һ��ɾ����
            if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECTPINYIN
                    || pime_result->imeStatus == WD_STATUS_CHINESE_SELECT)
            {
                WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            }

            WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            ThumbZhuyinUpdate(data_ptr);
        }
        else if (MMIAPICOM_Wstrlen(pime_result->outputKeys) == 1)        //���뷵��ֵ
        {
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            ThumbZhuyinClear(data_ptr);
        }
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            ThumbZhuyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        if (MMIAPICOM_Wstrlen(pime_result->inputKeys))       //���ڼ��룬ֱ�ӽ�����������
        {
            wchar buffer[MAX_KEYS_LEN * MAX_CHINESE_WORD_LEN + 1] = { 0 };

            WD_OnKey(WDK_ENTER, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            MMIAPICOM_Wstrncpy(buffer, pime_result->output, MAX_KEYS_LEN * MAX_CHINESE_WORD_LEN);
            CommitStr(data_ptr, buffer, MMIAPICOM_Wstrlen(pime_result->output));
            ThumbZhuyinClear(data_ptr);
        }
        else        //����
        {
            CommitChar(data_ptr, '\n');
        }
        break;

    case VKEY_PAGEUP:
        Sogou_SimulatCandPage(data_ptr, TRUE);
        ThumbZhuyinUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        Sogou_SimulatCandPage(data_ptr, FALSE);
        ThumbZhuyinUpdate(data_ptr);
        break;

    case VKEY_123:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - custom��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint8 const CUSTOM_PAGE_THRESHOLD = 2;
    BOOLEAN has_turn_page = FALSE;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        return FALSE;
    }
    
    if (KEY_STYLE_CUSTOM == data_ptr->vkb.first_key->style)
    {
        if (data_ptr->vkb.first_key->code - key_ptr->code <= -1*(CUSTOM_PAGE_THRESHOLD))
        {
            BOOLEAN has_next_page = (Sogou_GetLetterTotalCnt(data_ptr) > ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.custom_cnt);

            if (has_next_page)
            {
                ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page++;
                ThumbZhuyinSetCust(data_ptr);
                has_turn_page = TRUE;
            }
        }
        else if (data_ptr->vkb.first_key->code - key_ptr->code >= CUSTOM_PAGE_THRESHOLD)
        {
            if (((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page > 0)
            {
                ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page--;
                ThumbZhuyinSetCust(data_ptr);
                has_turn_page = TRUE;
            }
        }
        //�������һҳʱ���п���ֻ��һ����ť
        else if ((data_ptr->first_tp.y - data_ptr->last_tp.y > CUSTOM_PAGE_THRESHOLD *(key_ptr->rect.bottom - key_ptr->rect.top))       //�������볬��2������
                 && (data_ptr->first_tp.x < key_ptr->rect.right && data_ptr->first_tp.x >= key_ptr->rect.left))  //��cust�����ڻ���
        {
            if (((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page > 0)
            {
                ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page--;
                ThumbZhuyinSetCust(data_ptr);
                has_turn_page = TRUE;
            }
        }
    }

    if (!has_turn_page)
    {
        uint16 select_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.custom_cnt;

        select_idx += key_ptr->code - VKEY_CUSTOM;

        if (select_idx < Sogou_GetLetterTotalCnt(data_ptr))       //ѡ���ѡƴ��
        {
            //�л���ѡ��ƴ��״̬
            WD_SetVariable(E_SELECT_LETTER, select_idx + 1, pime_result);
            ThumbZhuyinUpdate(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, key_ptr->text[0]);
        }
    }
    else
    {
        //ѡ��ƴ������һҳ���׸�
        WD_SetVariable(E_SELECT_LETTER, ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.custom_cnt + 1, 
                pime_result);
        ThumbZhuyinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return FALSE;
    }
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
        ThumbZhuyinSetCand(data_ptr);
        break;

    case VKEY_PY_PAGEUP:

        Sogou_SimulatLetterPage(data_ptr, TRUE);
        ThumbZhuyinUpdate(data_ptr);
        break;

    case VKEY_PY_PAGEDOWN:

        Sogou_SimulatLetterPage(data_ptr, FALSE);
        ThumbZhuyinUpdate(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbZhuyinSetLetterCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetLetterCand(IM_DATA_T *data_ptr)
{
    uint8 key_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.py_pgup_key); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->vkb.py_pgdn_key); /*assert verified*/
    if (pime_result == PNULL||PNULL == data_ptr->vkb.py_pgup_key||PNULL == data_ptr->vkb.py_pgdn_key)
    {
        //SCI_TraceLow:"ThumbZhuyinSetLetterCand"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2093_112_2_18_2_28_6_167,(uint8*)"");
        return;
    }

    if (data_ptr->vkb.state == KEYBOARD_CANDIDATE
            || data_ptr->vkb.state == KEYBOARD_CANDIDATE_EXPAND
            || pime_result->inputKeys[0])
    {
        uint8 pinyin_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.py_candidate_cnt;
        uint8 pinyin_left = Sogou_GetLetterTotalCnt(data_ptr) - pinyin_idx;

        if (((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page > 0)
        {
            data_ptr->vkb.py_pgup_key->is_inactive = 0;
            data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE;
        }
        else
        {
            data_ptr->vkb.py_pgup_key->is_inactive = 1;
            data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgup_key, TRUE);	

        if (pinyin_left > data_ptr->vkb.py_candidate_cnt)
        {
            pinyin_left = data_ptr->vkb.py_candidate_cnt;
            data_ptr->vkb.py_pgdn_key->is_inactive = 0;
            data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE;
        }
        else
        {
            data_ptr->vkb.py_pgdn_key->is_inactive = 1;
            data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgdn_key, TRUE);
		
        for (key_idx = 0; key_idx < pinyin_left; key_idx++)
        {
            //int16 char_idx = 0;

            //for (char_idx = 0; char_idx < pime_result->letterSelecting[pinyin_idx + key_idx].length; char_idx++)
            {
                MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text,
                                  pime_result->letterSelecting[pinyin_idx + key_idx], TEXT_LEN_LIMIT -1);
            }

            data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len =
                MMIAPICOM_Wstrlen(pime_result->letterSelecting[pinyin_idx + key_idx]);
            data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len * sizeof (wchar));
			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx], TRUE);
		}
    }
    else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE
             || data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND)
    {
        data_ptr->vkb.py_pgup_key->is_inactive = 1;
        data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgup_key, TRUE);
		
        data_ptr->vkb.py_pgdn_key->is_inactive = 1;
        data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgdn_key, TRUE);
    }

    for (; key_idx < data_ptr->vkb.py_candidate_cnt; key_idx++)
    {
        data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len = 0;
        data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].is_inactive = 1;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx], TRUE);
    }

    return;
}
/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint16 select_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbZhuyinCandUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2177_112_2_18_2_28_6_168,(uint8*)"");
        return FALSE;
    }

    select_idx = key_ptr->code - VKEY_CANDIDATE;
    //SCI_ASSERT(select_idx >= 0); /*assert verified*/
    
    //��ѡ��
    if (VKEY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_CANDIDATE + data_ptr->vkb.candidate_cnt)
    {
        WD_SetVariable(E_SELECT_CAND, select_idx + 1, pime_result);
        WD_OnKey(WDK_OK, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        if (pime_result->output[0])
        {
            CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
        }

        if (data_ptr->vkb.is_expand)
        {
            SwitchExpandKey(data_ptr);
        }

        data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
    }
    //��ѡƴ��
    else if (VKEY_PY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_PY_CANDIDATE + data_ptr->vkb.py_candidate_cnt)
    {
        select_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.py_candidate_cnt;
        select_idx += key_ptr->code - VKEY_PY_CANDIDATE;
        if (select_idx < Sogou_GetLetterTotalCnt(data_ptr))
        {
            WD_SetVariable(E_SELECT_LETTER, select_idx + 1, pime_result);
        }
    }

    ThumbZhuyinUpdate(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbZhuyinClear
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    WD_OnKey(WDK_LONG_C, 0, Sogou_GetCandDisplayCnt(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
    data_ptr->vkb.state = KEYBOARD_INITIAL;
    SetHideKey(data_ptr, 0);
    ThumbZhuyinSetCand(data_ptr);
    ThumbZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbZhuyinUpdate
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinUpdate(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
    ThumbZhuyinSetCand(data_ptr);
    ThumbZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbZhuyinSetCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA (data_ptr);
	
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbZhuyinSetCand"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2280_112_2_18_2_28_6_169,(uint8*)"");
        return;
    }

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}
	
    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == Sogou_GetCandDisplayCnt(data_ptr))
    {
        goto ThumbZhuyinSetCandEnd;
    }
    
    //û��չ��״̬
    while (cand_cnt < Sogou_GetCandDisplayCnt(data_ptr))
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[cand_key_idx].text, pime_result->selecting[cand_cnt], TEXT_LEN_LIMIT - 1);
        data_ptr->vkb.keys[cand_key_idx].text_len = MMIAPICOM_Wstrlen(data_ptr->vkb.keys[cand_key_idx].text);
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
        
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);	
        cand_key_idx++;
        cand_cnt++;
    }

    //��Ȼ�޷�ȫ����ʾ������չ��ʱ�������õõ�
    Sogou_SetLastDisp(data_ptr, cand_cnt);
    if (1 == data_ptr->vkb.is_expand)
    {
        //��ǰһҳ
        if (pime_result->curPage > 0)
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

        //�к�һҳ
        //if (pime_result->curPage < MAX_SOGOU_CANDIDATE_PAGE - 1)
        if (Sogou_TestCandNextPage(data_ptr))
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

        ThumbZhuyinSetLetterCand(data_ptr);
    }

ThumbZhuyinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    if (0 == data_ptr->vkb.is_expand)
    {
        ThumbZhuyinSetCust(data_ptr);
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[MAX_WORD_LEN + 3] = {0};
    uint32 buffer_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbZhuyinDispEdit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2382_112_2_18_2_28_6_170,(uint8*)"");
        return;
    }
    
    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/

    UILAYER_Clear(&data_ptr->edit_layer);

    //û���ַ����򷵻أ�������
    if (pime_result->outputKeys[0] == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    while (buffer_len < MMIAPICOM_Wstrlen(pime_result->outputKeys))
    {
        buffer[buffer_len] = pime_result->outputKeys[buffer_len];
        buffer_len++;
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len >= edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*==============================================================================
Description: ThumbZhuyinSetCust
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetCust(IM_DATA_T *data_ptr)
{
    uint8 key_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
	
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbZhuyinSetCust"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_2480_112_2_18_2_28_7_171,(uint8*)"");
        return;
    }

    //��ѡƴ��
    if (pime_result->letterSelecting[0][0] > 0)
    {
        uint8 pinyin_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.custom_cnt;
        uint8 pinyin_left = Sogou_GetLetterTotalCnt(data_ptr) - pinyin_idx;

        if (pinyin_left > data_ptr->vkb.custom_cnt)
        {
            pinyin_left = data_ptr->vkb.custom_cnt;
        }

        //��ѡƴ��
        for (key_idx = 0; key_idx < pinyin_left; key_idx++)
        {
            //int16 char_idx = 0;

            //for (char_idx = 0; char_idx < pime_result->letterSelecting[pinyin_idx + key_idx].length; char_idx++)
            {
                MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text,
                                  pime_result->letterSelecting[pinyin_idx + key_idx], TEXT_LEN_LIMIT - 1);
            }

            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = MMIAPICOM_Wstrlen(data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text);
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}

        //��ѡƴ������һҳʱ
        for (; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = 0;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 1;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}
    }
    //����
    else
    {
        for (key_idx = 0; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text[0] = g_thumb_cust_cn_symb[key_idx];
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = 1;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}
    }

    return;
}


/*==============================================================================
Description: QwertZhuyinInit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinInit(IM_DATA_T *data_ptr, WD_UINT8 method)
{
    uint8 ret_value = 0;
    
    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_SOGOU_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_SOGOU_T));

    QwertZhuyinInitKb(data_ptr);

    ret_value = Sogou_ImeInit (data_ptr, method);
    if (ret_value != 0)
    {
        //SCI_TRACE_LOW:"QwertZhuyinInit Sogou_ImeInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3465_112_2_18_2_28_9_179,(uint8*)"");
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: QwertZhuyinTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    MMIAPIIM_SGFreeImeSkinBuffer();
    SG_Quit();
    
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: QwertAbcDraw
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    QwertZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: yaoguang.chen
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void QwertZhuyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 const CHAR_CNT = 26;
    uint8 const CTRL_CNT = 9;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT_E;
    uint8 const SYS_CNT = 2;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
    MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_qwert_default;

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_qwert_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key ��λ��������2��ʼ��27
    data_ptr->vkb.charactor_first = key_idx = 0;
    key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);    //�˱��˳���ǰ�a~z����

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;
        //=========��������˿ӡ����
        cur_char_ptr = key_table_ptr + (g_kb_keymap[key_idx] - VKEY_A);
        vkb_ptr->keys[key_idx].text_len = cur_char_ptr->key_chars[0];
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr->key_chars[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key ��λ��������28��ʼ������2��
    key_idx++;
    cust_begin = key_idx;

    for (; key_idx < cust_begin + CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = L"\xff1f\xff0c"[key_idx - cust_begin];
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

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key ��λ�������ӿ�ʼ��30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertZhuyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCtrlUp;
    }

    //--ƴ���ָ��
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_PY_SEPARATE;
    vkb_ptr->keys[key_idx].text_len = 1;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"'", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    vkb_ptr->keys[key_idx].font_size = 20;
    //--delete
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
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
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
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
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0,
                    0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;


    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_qwert_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
#if defined SOGOU_USE_PDA_SKIN
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //������������ֹ��sogouƤ�������ǵ�
#endif
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
     
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_qwert_panel_position[0].right - g_qwert_panel_position[0].left - 5;         //�Ҳ���һ������
        int16 h = g_qwert_panel_position[0].bottom - g_qwert_panel_position[0].top - data_ptr->theme.key_ver_space - 5; //������Щ�հ�,������һ������
        GUI_RECT_T cand_rect = {0};

        //ƫ����ֱ�ӷ���ConvertRect������㣬�����ڶ�����ƫ�������Ǿ��ȵ�
        cand_rect.top = g_qwert_panel_position[0].top + y * (h + data_ptr->theme.key_ver_space) + data_ptr->theme.key_ver_space / 2 + 5;
        cand_rect.bottom = cand_rect.top + h;
        cand_rect.left = g_qwert_panel_position[0].left + x * w / CH_CANDIDATE_COL + 1;
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
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

	return;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint16 key_code = 0;
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    if(PNULL == key_ptr)
    {
        return FALSE;
    }   
    
    if (VKEY_A <= key_ptr->code && key_ptr->code <= VKEY_Z)
    {
        key_code = ConvetIndexToVKey(key_ptr->code - VKEY_A);

        WD_OnKey(WDK_NORMAL_A + key_code - VKEY_A, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        QwertZhuyinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL ||PNULL == key_ptr )
    {
        //SCI_TraceLow:"QwertZhuyinCtrlLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3817_112_2_18_2_28_9_180,(uint8*)"");
        return FALSE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (pime_result->inputKeys[0] > 0)
        {
            QwertZhuyinClear(data_ptr);
        }
        //������״̬��û�����밴��
        else if(data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            QwertZhuyinClear(data_ptr);
        }
        else
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
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"QwertZhuyinCtrlUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_3869_112_2_18_2_28_9_181,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_PY_SEPARATE:
        WD_OnKey(WDK_SEPERATER, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        QwertZhuyinUpdate(data_ptr);
        break;

    case VKEY_DEL:

        if (MMIAPICOM_Wstrlen(pime_result->outputKeys) > 1)       //��������ֵ
        {
            //ѡ��ƴ��״̬����ʹ��WDK_C���л�������״̬������û������һ��ɾ����
            if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECTPINYIN
                    || pime_result->imeStatus == WD_STATUS_CHINESE_SELECT)
            {
                WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            }

            WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            QwertZhuyinUpdate(data_ptr);
        }
        else if (MMIAPICOM_Wstrlen(pime_result->outputKeys) == 1)        //���뷵��ֵ
        {
            QwertZhuyinClear(data_ptr);
        }
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            QwertZhuyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        {
            uint32 buffer_len = MMIAPICOM_Wstrlen(pime_result->inputKeys);

            if (buffer_len)       //���ڼ��룬ֱ�ӽ�����������
            {
                wchar buffer[MAX_KEYS_LEN * MAX_CHINESE_WORD_LEN + 1] = { 0 };

                WD_OnKey(WDK_ENTER, 0, Sogou_GetLastDisp(data_ptr), pime_result);
                MMIAPICOM_Wstrncpy(buffer, pime_result->output, MAX_KEYS_LEN * MAX_CHINESE_WORD_LEN);
                CommitStr(data_ptr, buffer, MMIAPICOM_Wstrlen(pime_result->output));
                QwertZhuyinClear(data_ptr);
            }
            else        //����
            {
                CommitChar(data_ptr, '\n');
            }

            break;
        }

    case VKEY_SPACE:

        if (pime_result->inputKeys[0] == 0)
        {
            CommitChar(data_ptr, 0x3000);
        }

        break;

    case VKEY_PAGEUP:
        Sogou_SimulatCandPage(data_ptr, TRUE);
        QwertZhuyinUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        Sogou_SimulatCandPage(data_ptr, FALSE);
        QwertZhuyinUpdate(data_ptr);
        break;

    case VKEY_123SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return FALSE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
        QwertZhuyinSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint16 select_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
   // SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL ||key_ptr == PNULL  )
    {
        //SCI_TraceLow:"QwertZhuyinCandUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_4017_112_2_18_2_28_10_182,(uint8*)"");
        return FALSE;
    }

    select_idx = key_ptr->code - VKEY_CANDIDATE;
    //SCI_ASSERT(select_idx >= 0); /*assert verified*/

    //��ѡ��
    if (VKEY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_CANDIDATE + data_ptr->vkb.candidate_cnt)
    {
        WD_SetVariable(E_SELECT_CAND, select_idx + 1, pime_result);
        WD_OnKey(WDK_OK, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        if (pime_result->output[0])
        {
            CommitStr(data_ptr, pime_result->output, MMIAPICOM_Wstrlen(pime_result->output));
        }

        if (data_ptr->vkb.is_expand)
        {
            SwitchExpandKey(data_ptr);
        }

        data_ptr->vkb.state = KEYBOARD_ASSOCIATE;
    }
    //��ѡƴ��
    else if (VKEY_PY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_PY_CANDIDATE + data_ptr->vkb.py_candidate_cnt)
    {
        select_idx = ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page * data_ptr->vkb.py_candidate_cnt;

        select_idx += key_ptr->code - VKEY_PY_CANDIDATE;

        if (select_idx < Sogou_GetLetterTotalCnt(data_ptr))
        {
            WD_SetVariable(E_SELECT_LETTER, select_idx + 1, pime_result);
        }
    }

    QwertZhuyinUpdate(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: QwertZhuyinClear
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinClear(IM_DATA_T *data_ptr)
{
    int32 iloop = Sogou_GetCandDisplayCnt(data_ptr);

    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    WD_OnKey(WDK_LONG_C, 0, iloop, MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }
    
    data_ptr->vkb.state = KEYBOARD_INITIAL;
    SetHideKey(data_ptr, 0);
    QwertZhuyinSetCand(data_ptr);
    QwertZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: QwertZhuyinUpdate
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinUpdate(IM_DATA_T *data_ptr)
{
    uint8 cand_cnt = 0;

    CHECK_DATA_EX(data_ptr);

    cand_cnt = Sogou_GetCandDisplayCnt(data_ptr);


    SetHideKey(data_ptr, cand_cnt);

    QwertZhuyinSetCand(data_ptr);
    QwertZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: QwertZhuyinSetCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);

	if (pime_result == PNULL)
    {
        //SCI_TraceLow:"QwertZhuyinSetCand"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_4129_112_2_18_2_28_10_183,(uint8*)"");
        return;
    }

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}
    
    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == MMIAPICOM_Wstrlen(pime_result->selecting[0]))
    {
        goto QwertZhuyinSetCandEnd;
    }

    while (cand_cnt < Sogou_GetCandDisplayCnt(data_ptr) && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[cand_key_idx].text, pime_result->selecting[cand_cnt], TEXT_LEN_LIMIT - 1);
        data_ptr->vkb.keys[cand_key_idx].text_len = MMIAPICOM_Wstrlen(data_ptr->vkb.keys[cand_key_idx].text);
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
		
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
		
        cand_key_idx++;
        cand_cnt++;
    }

    Sogou_SetLastDisp(data_ptr, cand_cnt);
    if (1 == data_ptr->vkb.is_expand)       //չ��״̬
    {
        //��ǰһҳ
        if (pime_result->curPage > 0)
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

        //�к�һҳ
        //if (pime_result->curPage < MAX_SOGOU_CANDIDATE_PAGE - 1 && data_ptr->vkb.state != KEYBOARD_ASSOCIATE_EXPAND)
        if (Sogou_TestCandNextPage(data_ptr) && data_ptr->vkb.state != KEYBOARD_ASSOCIATE_EXPAND)
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

QwertZhuyinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinDispEdit(IM_DATA_T *data_ptr)
{
    ThumbZhuyinDispEdit(data_ptr);
    return;
}
#endif


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartInitKb

Func:   9���������뷨���̳�ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartInitKb(IM_DATA_T *data_ptr)
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
    //wchar const * const *cust_symb = PNULL;   //���ư���˿ӡ
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
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
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
    //character key ��λ��������6��ʼ��15
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
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        key_max_char_num = key_max_char_num > vkb_ptr->keys[key_idx].text_len ? key_max_char_num : vkb_ptr->keys[key_idx].text_len;
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbSmartCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //��������ַ�����5���İ������������������ʾ
    if (key_max_char_num > KEY_CHAR_LINE_MAX_CHARS_NUM + 1)       //���ּ�Ҳ������,����Ϊ5������4
    {
        int32 iloop = 0;
        
        for (iloop = 0; iloop < CHAR_CNT; iloop++)
        {
            vkb_ptr->keys[key_idx - iloop - 1].add_style.text_disp = KASC_2X4_EXCLUDE_DIGIT;         //key_idxֵ����󱻼�1����Ҫ -1
        }
    }

    //--1�������ã��������Ϊһ���ַ�'1'����ֱ������
    ThumbSmartRedefineKey1 (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[char_begin]);
    
    //--0 ���һ��char
    key_idx--;
    ThumbSmartRedefineKey0 (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[key_idx]);

    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
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
        vkb_ptr->keys[key_idx].f_long = ThumbSmartCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCtrlUp;
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
        //�ر�ļ�סcaps key
        vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
    }
    else
    {
        ThumbSmartRedefineCaps (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[key_idx]);
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
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
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
    //������������ͬһ��λ�ã��������, ��ʼ������expand
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
        vkb_ptr->keys[key_idx].f_up = ThumbSmartSysUp;
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

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
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
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //������������ֹ��sogouƤ�������ǵ�
#endif
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
     
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
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
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCandUp;
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
	
	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	MMIIM_TOUCH_ThumbKbRedefinePrintText (data_ptr);	//��Щ���Ե�˿ӡ�Ͱ�������ʾ��һ��
	
    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartInit

Func:   9���������뷨��ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return:
            TRUE:�ɹ���FALSE:�������Ϸ���ʧ��

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartInit(IM_DATA_T *data_ptr, WD_UINT8 sogou_method)
{
    WD_UINT8 rev_value = 0;

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_SOGOU_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_SOGOU_T));

    //���̳�ʼ��
    ThumbSmartInitKb(data_ptr);
    
    //�����ʼ��
    rev_value = Sogou_ImeInit(data_ptr, sogou_method);
    if (rev_value != 0)
    {
        //SCI_TRACE_LOW:"ThumbSmartInit Sogou_ImeInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_4566_112_2_18_2_28_11_184,(uint8*)"");
        return FALSE;
    }

    Sogou_SetCapsLock (data_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartTerm

Func:   9���������뷨�ر�

Input:
            data_ptr:    ���뷨ָ��

Output:

Return:
            TRUE:�ɹ���FALSE:�������Ϸ���ʧ��

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
#ifdef SOGOU_USE_PDA_SKIN    
    MMIAPIIM_SGFreeImeSkinBuffer();
#endif
    SG_Quit();
    
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartDraw

Func:   9���������뷨����

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);
    ThumbSmartDispEdit(data_ptr);
    return;
}




/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCharLong

Func:   9���������뷨�������ַ���0 ~ 9��Ϣ�������Ϊ�����׸��ַ�ʱ��������ֱ������

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�İ��������ȼ��ð����Ƿ�Ϊ�ַ�����������������

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL|| PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbSmartCharLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_4666_112_2_18_2_28_11_185,(uint8*)"");
        return FALSE;
    }

    if (pime_result->inputKeys[0] > 0)
    {
        return TRUE;
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


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCharUp

Func:   9���������뷨���ַ���0 ~ 9��Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�İ��������ȼ��ð����Ƿ�Ϊ�ַ�����������������

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
	WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
	
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr || pime_result == PNULL)
    {
        return FALSE;
    }
    
    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        WD_OnKey(WDK_NORMAL_1 + key_ptr->code - VKEY_1, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        //data_ptr->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
        data_ptr->vkb.state = KEYBOARD_CANDIDATE;
        ThumbSmartUpdate(data_ptr);
    }
    else if (VKEY_0 == key_ptr->code)
    {
		if (data_ptr->vkb.candidate_cnt > 0)		//commit first candidates
        {
        	int16 out_str_len = 0;
			
		    WD_SetVariable(E_SELECT_CAND, 1, pime_result);
		    WD_OnKey(WDK_OK, 0, Sogou_GetCandDisplayCnt(data_ptr), pime_result);
		    out_str_len = MMIAPICOM_Wstrlen(pime_result->output);
		    if (out_str_len > 1 && pime_result->output[out_str_len - 1] == ' ')
		    {
		        CapsCommitStr(data_ptr, pime_result->output, out_str_len - 1);   
		    }
		    else
		    {
		        CapsCommitStr(data_ptr, pime_result->output, out_str_len);
		    }
		    
		    Sogou_SetCapsLock(data_ptr);

		    //clear input
		    ThumbSmartClear(data_ptr);
		}
		else if (pime_result->inputKeys[0] == 0)
        {
            //commit a space
            CommitChar(data_ptr, ' ');
        }
    }
	else if (VKEY_STAR == key_ptr->code)	//�������Կ�����Ҫʹ��*�ż�
	{
        WD_OnKey(WDK_STAR, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        data_ptr->vkb.state = KEYBOARD_CANDIDATE;
        ThumbSmartUpdate(data_ptr);
    }
	else if (VKEY_HASH == key_ptr->code)	//�������Կ�����Ҫʹ��*�ż�
	{
        WD_OnKey(WDK_SHARP, 0, Sogou_GetLastDisp(data_ptr), pime_result);
        data_ptr->vkb.state = KEYBOARD_CANDIDATE;
        ThumbSmartUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCtrlLong

Func:   9���������뷨�����Ƽ�������Ϣ����,�˴���ʱֱ�ӵ���

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbSmartCtrlLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_4768_112_2_18_2_28_11_186,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (pime_result->inputKeys[0] > 0)
        {
            ThumbSmartClear(data_ptr);
        }
        //������״̬��û�����밴��
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            ThumbSmartClear(data_ptr);
        }
        else
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



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCtrlUp

Func:   9���������뷨�����Ƽ���Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL || PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbSmartCtrlUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_4835_112_2_18_2_28_11_187,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        Sogou_SetCapsLock(data_ptr);

        if (pime_result->inputKeys[0] > 0)
        {
            ThumbSmartUpdate(data_ptr);
        }

        break;

    case VKEY_DEL:

        if (MMIAPICOM_Wstrlen(pime_result->outputKeys) > 1)       //��������ֵ
        {
            //ѡ��ƴ��״̬����ʹ��WDK_C���л�������״̬������û������һ��ɾ����
            if (pime_result->imeStatus == WD_STATUS_CHINESE_SELECTPINYIN
                    || pime_result->imeStatus == WD_STATUS_CHINESE_SELECT)
            {
                WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            }

            WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            ThumbSmartUpdate(data_ptr);
        }
        else if (MMIAPICOM_Wstrlen(pime_result->outputKeys) == 1)        //���뷵��ֵ
        {
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            ThumbSmartClear(data_ptr);
        }
        else if (data_ptr->vkb.state == KEYBOARD_ASSOCIATE_EXPAND || data_ptr->vkb.state == KEYBOARD_ASSOCIATE)
        {
            ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->choice_pinyin_page = 0;       //��0�󣬷�ҳ��ѡ��ʱ���Ƿ��ܵ��øú���
            ThumbSmartClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        {
            uint32 buffer_len = MMIAPICOM_Wstrlen(pime_result->inputKeys);

            if (buffer_len)       //���ڼ��룬ֱ�ӽ�����������
            {
                wchar buffer[MAX_WORD_LEN] = {0};

                MMIAPICOM_Wstrncpy(buffer, pime_result->inputKeys, buffer_len);
                CapsCommitStr (data_ptr, buffer, buffer_len);
                ThumbSmartClear(data_ptr);
            }
            else        //����
            {
                CommitChar (data_ptr, '\n');
            }

            break;
        }

    case VKEY_PAGEUP:
        Sogou_SimulatCandPage(data_ptr, TRUE);
        ThumbSmartUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        Sogou_SimulatCandPage(data_ptr, FALSE);
        ThumbSmartUpdate(data_ptr);
        break;

    case VKEY_123:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (pime_result->inputKeys[0] > 0)
            break;
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
Name:   ThumbSmartCustUp

Func:   9���������뷨���û��Զ��尴����Ϣ�����˴�Ĭ�϶���Ϊ�������밴��

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ���û��Զ��尴�����������ļ������ż�

Return: TRUE:success;  else fail;

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return FALSE;
    }
    ThumbAbcCommitLast(data_ptr);

    CapsCommitStr (data_ptr, key_ptr->text, key_ptr->text_len);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartSysUp

Func:   9���������뷨��ϵͳ������Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ��ϵͳ����

Return: TRUE:success;  else fail;

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return FALSE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, Sogou_GetCandDisplayCnt(data_ptr));
        SmartSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCandUp

Func:   9���������뷨����ѡ�ֵ����Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�ĺ�ѡ�ְ���

Return: TRUE:success;  else fail;

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 select_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    uint16 out_str_len = 0;
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"ThumbSmartCandUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_5036_112_2_18_2_28_12_188,(uint8*)"");
        return FALSE;
    }

    select_idx = key_ptr->code - VKEY_CANDIDATE;
    //SCI_ASSERT(select_idx >= 0); /*assert verified*/

    WD_SetVariable(E_SELECT_CAND, select_idx + 1, pime_result);
    WD_OnKey(WDK_OK, 0, Sogou_GetCandDisplayCnt(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
    out_str_len = MMIAPICOM_Wstrlen(pime_result->output);
    if (out_str_len > 1 && pime_result->output[out_str_len - 1] == ' ')
    {
        CapsCommitStr(data_ptr, pime_result->output, out_str_len - 1);   //���һ���ո�����
    }
    else
    {
        CapsCommitStr(data_ptr, pime_result->output, out_str_len);
    }
    
    Sogou_SetCapsLock(data_ptr);

    //clear input
    ThumbSmartClear(data_ptr);
    VkbUp(data_ptr, key_ptr);

    return TRUE;
}




/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartClear

Func:   9���������뷨�������������

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel)
    {
        return;
    }

    WD_OnKey(WDK_LONG_C, 0, Sogou_GetCandDisplayCnt(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));

    if (data_ptr->vkb.is_expand)
    {
        ThumbLatinSwitchExpandKey(data_ptr);
    }

	data_ptr->vkb.state = KEYBOARD_INITIAL;  
    SetHideKey(data_ptr, 0);
    SmartSetCand(data_ptr);
    ThumbSmartDispEdit(data_ptr);

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartUpdate

Func:   9���������뷨������ˢ��

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartUpdate(IM_DATA_T *data_ptr)
{
    uint8 cand_cnt = Sogou_GetCandDisplayCnt(data_ptr);

    CHECK_DATA_EX(data_ptr);

    SetHideKey(data_ptr, cand_cnt);
    SmartSetCand(data_ptr);
    ThumbSmartDispEdit(data_ptr);

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   SmartSetCand

Func:   9���������뷨�����ú�ѡ��

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void SmartSetCand (IM_DATA_T *data_ptr)
{
    int8 cand_layer = 0;
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    GUI_RECT_T cand_rect = {0};
    int16 cand_height = 0;
    uint8 line_idx = 0;
    int16 panel_width, panel_height, dw, dh;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"SmartSetCand"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_5149_112_2_18_2_28_12_189,(uint8*)"");
        return;
    }

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;

    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;
    dw = 6; //data_ptr->theme.key_hor_space * panel_width / 1000;
    dh = 6; //data_ptr->theme.key_ver_space * panel_height / 1000;

    //set normal candidate
    cand_rect = data_ptr->vkb.vessel_key->rect;
    cand_height = cand_rect.bottom - cand_rect.top - dh - 1;    //cand ���� ����û��ȥ��space���
    cand_rect.left += dw / 2;
    cand_rect.top += 1 + dh / 2;    //������һ������
    cand_rect.bottom -= dh / 2;
    
    while (cand_cnt < Sogou_GetCandDisplayCnt(data_ptr) && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        int16 word_width = 0;
        int16 disp_right = 0;

        //������ʾ����
        word_width = GUI_GetStringWidth(data_ptr->theme.key_cand[0].text_font,
                                        pime_result->selecting[cand_cnt], MMIAPICOM_Wstrlen(pime_result->selecting[cand_cnt]));
        word_width += 2 * data_ptr->theme.key_padding;

        if (word_width < CAND_WIDTH_MIN)
        {
            word_width = CAND_WIDTH_MIN;
        }

        disp_right = cand_rect.left + word_width - 1;

        if (disp_right > cand_rect.right && line_idx > 0)
        {
            if (0 == data_ptr->vkb.is_expand)
                break;

            if (++cand_layer >= LATIN_CANDIDATE_ROW)
                break;

            cand_rect.left = data_ptr->vkb.vessel_key->rect.left + dw / 2;
            cand_rect.right = data_ptr->vkb.vessel_key->rect.right - dw / 2;
            cand_rect.top += cand_height + dh;
            cand_rect.bottom = cand_rect.top + cand_height;

            line_idx = 0;

            disp_right = cand_rect.left + word_width - 1;
        }

        line_idx++;

        data_ptr->vkb.keys[cand_key_idx].rect = cand_rect;
        data_ptr->vkb.keys[cand_key_idx].rect.right = disp_right;

        if (data_ptr->vkb.keys[cand_key_idx].rect.right > cand_rect.right)
        {
            data_ptr->vkb.keys[cand_key_idx].rect.right = cand_rect.right;
        }

        //�ַ�����
        MMIAPICOM_Wstrncpy(data_ptr->vkb.keys[cand_key_idx].text, pime_result->selecting[cand_cnt], TEXT_LEN_LIMIT - 1);
        data_ptr->vkb.keys[cand_key_idx].text_len = MMIAPICOM_Wstrlen(data_ptr->vkb.keys[cand_key_idx].text);
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));

        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
		
        cand_rect.left = disp_right + 1 + dw / 2;
        cand_key_idx++;
        cand_cnt++;
    }

    Sogou_SetLastDisp(data_ptr, cand_key_idx - data_ptr->vkb.candidate_first);

    if (1 == data_ptr->vkb.is_expand)
    {
        //��ǰһҳ
        if (pime_result->curPage > 0)
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
		
        //�к�һҳ
        //if (pime_result->curPage < MAX_SOGOU_CANDIDATE_PAGE - 1)
        if (Sogou_TestCandNextPage(data_ptr))
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

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    return;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartDispEdit

Func:   9���������뷨����ǰ��������ַ�����ʾ

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[MAX_WORD_LEN + 3] = {0};
    uint32 buffer_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);

    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL)
    {
        //SCI_TraceLow:"ThumbSmartDispEdit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_5294_112_2_18_2_28_12_190,(uint8*)"");
        return;
    }

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/

    UILAYER_Clear(&data_ptr->edit_layer);

    //û���ַ����򷵻أ�������
    if (pime_result->inputKeys[0] == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    while (buffer_len < MMIAPICOM_Wstrlen(pime_result->outputKeys))
    {
        buffer[buffer_len] = pime_result->outputKeys[buffer_len];
        buffer_len++;
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len > edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartInit

Func:   26���������뷨��ʼ��

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartInit(IM_DATA_T *data_ptr, WD_UINT8 sogou_method)
{
    uint8 ret_value = 0;
    
    CHECK_DATA(data_ptr);
    
    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_SOGOU_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_SOGOU_T));
    
    QwertSmartInitKb(data_ptr);

    //������س�ʼ��
    ret_value = Sogou_ImeInit (data_ptr, sogou_method);
    if (ret_value != 0)
    {
        //SCI_TRACE_LOW:"QwertPinyinInit Sogou_ImeInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_5403_112_2_18_2_28_12_191,(uint8*)"");
        return FALSE;
    }

    Sogou_SetCapsLock(data_ptr);
    
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartTerm

Func:   26���������뷨�ر�

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
#ifdef SOGOU_USE_PDA_SKIN    
    MMIAPIIM_SGFreeImeSkinBuffer();
#endif
    SG_Quit();
    
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartDraw

Func:   26���������뷨����

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartInitKb

Func:   26���������뷨��ʼ���������

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 const CHAR_CNT = 26;
    uint8 const CTRL_CNT = 9;
    uint8 const CAND_CNT = LATIN_CANDIDATE_COUNT;
    uint8 const SYS_CNT = 2;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
    MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;
	GUI_RECT_T const *kb_pos_ptr = g_qwert_panel_position;		//current sogou use 26keys layout,needn't use QwertKbLayoutType () for layout
    MMI_STRING_T slide_cue = {PNULL, 0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_qwert_default;
    SetPanel(data_ptr);

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_qwert_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key ��λ��������2��ʼ��27
    data_ptr->vkb.charactor_first = key_idx = 0;
    key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);    //�˱��˳���ǰ�a~z����
    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;    //������ʾ�׸��ַ�

        //=========��������˿ӡ����
        cur_char_ptr = key_table_ptr + (g_kb_keymap[key_idx] - VKEY_A);
        vkb_ptr->keys[key_idx].text_len = cur_char_ptr->key_chars[0];
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr->key_chars[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));

        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertSmartCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key ��λ��������28��ʼ������2��
    key_idx++;
    cust_begin = key_idx;

    for (; key_idx < cust_begin + CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
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
    //character key ��λ�������ӿ�ʼ��30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertSmartCtrlLong;
        vkb_ptr->keys[key_idx].f_up = QwertSmartCtrlUp;
    }

    //--Caps
    key_idx = ctrl_begin;
    //�ر�ļ�סcaps key
    if (MMIAPIIM_CapsExist(data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
    {
        vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
        vkb_ptr->keys[key_idx].code = VKEY_CAPS;
        vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
    }
    else
    {
        const wchar *text_ptr = GetMultiSymCharPtr(data_ptr->init_param.method_param_ptr->im_def_ptr->lang);
        MultiTapCharKeyInit(&vkb_ptr->keys[key_idx], text_ptr);
    }

    //--delete
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
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
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_EN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
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
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0,
                    0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertSmartSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;


    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0,
                0);     //data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
#if defined SOGOU_USE_PDA_SKIN
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //������������ֹ��sogouƤ�������ǵ�
#endif    
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
 
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
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
        vkb_ptr->keys[key_idx].f_up = QwertSmartCandUp;
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
	
	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartUpdate

Func:   26���������뷨ˢ��

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartUpdate(IM_DATA_T *data_ptr)
{
    uint8 cand_cnt = Sogou_GetCandDisplayCnt(data_ptr);

    CHECK_DATA_EX(data_ptr);
    SetHideKey(data_ptr, cand_cnt);

    SmartSetCand(data_ptr);

    return;
}

/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCharUp

Func:   26���������뷨�ַ�������Ϣ�����������潻���İ�����Ϣ
           key_ptr:    ��Ӧ���ַ�����

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint16 key_code = 0;
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return FALSE;
    }
    
    if (VKEY_A <= key_ptr->code && key_ptr->code <= VKEY_Z)
    {
        key_code = ConvetIndexToVKey(key_ptr->code - VKEY_A);

        WD_OnKey(WDK_NORMAL_A + key_code - VKEY_A, 0, Sogou_GetLastDisp(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
        QwertSmartUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCtrlLong

Func:   26���������뷨���ư���������Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    CHECK_ENGINE_DATA(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"QwertSmartCtrlLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_5830_112_2_18_2_28_13_192,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (pime_result->inputKeys[0] > 0)
        {
            QwertLatinClear(data_ptr);
        }
        else
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


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCtrlUp

Func:   26���������뷨���ư�����Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL||PNULL == key_ptr)
    {
        //SCI_TraceLow:"QwertSmartCtrlUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_5883_112_2_18_2_28_13_193,(uint8*)"");
        return FALSE;
    }

    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        Sogou_SetCapsLock(data_ptr);
        
        if (pime_result->inputKeys[0])
        {
            QwertSmartUpdate(data_ptr);
        }

        break;

    case VKEY_DEL:

        if (MMIAPICOM_Wstrlen(pime_result->inputKeys) > 1)
        {
            WD_OnKey(WDK_C, 0, Sogou_GetLastDisp(data_ptr), pime_result);
            QwertSmartUpdate(data_ptr);
        }
        else if (MMIAPICOM_Wstrlen(pime_result->inputKeys) == 1)
        {
            QwertSmartClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (0 != pime_result->selecting[0][0])
        {
            CapsCommitStr (data_ptr, pime_result->selecting[0], MMIAPICOM_Wstrlen(pime_result->selecting[0]));
            QwertSmartClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\n');
        }

        break;

    case VKEY_SPACE:

        //commit candidate first
        if (0 != pime_result->selecting[0][0])
        {
            CapsCommitStr (data_ptr, pime_result->selecting[0], MMIAPICOM_Wstrlen(pime_result->selecting[0]));      //��������ܰ�������
        }

        //clear input
        QwertSmartClear(data_ptr);

        //commit a space
        CommitChar (data_ptr, ' ');

        break;

    case VKEY_PAGEUP:
        Sogou_SimulatCandPage(data_ptr, TRUE);
        QwertSmartUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        Sogou_SimulatCandPage(data_ptr, FALSE);
        QwertSmartUpdate(data_ptr);
        break;

    case VKEY_123SYMBOL:
        // if (data_ptr->data.q_latin.input_string_len > 0) break;
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
Name:   QwertSmartSysUp

Func:   26���������뷨ϵͳ������Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ��ϵͳ����

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return FALSE;
    }
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        QwertSmartSwitchExpandKey(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCandUp

Func:   26���������뷨��ѡ����Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ��ϵͳ����

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 select_idx = 0;
    WD_imeResult *pime_result = MMIIM_TOUCH_Sogou_GetImeResult (data_ptr);
    uint16 out_str_len = 0;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    CHECK_ENGINE_DATA(data_ptr);
    if (pime_result == PNULL || PNULL == key_ptr)
    {
        //SCI_TraceLow:"QwertSmartCandUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_SOGOU_6031_112_2_18_2_28_14_194,(uint8*)"");
        return FALSE;
    }

    select_idx = key_ptr->code - VKEY_CANDIDATE;

    //SCI_ASSERT(select_idx >= 0); /*assert verified*/

    WD_SetVariable(E_SELECT_CAND, select_idx + 1, pime_result);
    WD_OnKey(WDK_OK, 0, Sogou_GetCandDisplayCnt(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));
    out_str_len = MMIAPICOM_Wstrlen(pime_result->output);
    if (out_str_len > 1 && pime_result->output[out_str_len - 1] == ' ')
    {
        CapsCommitStr(data_ptr, pime_result->output, out_str_len - 1);   //���һ���ո�����
    }
    else
    {
        CapsCommitStr(data_ptr, pime_result->output, out_str_len);
    }

    Sogou_SetCapsLock(data_ptr);
    
    //clear input
    QwertSmartClear(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartClear

Func:   26���������뷨�����������

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel)
        return;

    WD_OnKey(WDK_LONG_C, 0, Sogou_GetCandDisplayCnt(data_ptr), MMIIM_TOUCH_Sogou_GetImeResult(data_ptr));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

	data_ptr->vkb.state = KEYBOARD_INITIAL;
    SetHideKey(data_ptr, 0);
    SmartSetCand(data_ptr);

    return;
}


//�ϼ��������⴦��
#if defined IM_BENGALI_SUPPORT
PUBLIC BOOLEAN ThumbBengaliInit (IM_DATA_T *data_ptr)
{
    WD_UINT8 rev_value = 0;

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_SOGOU_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_SOGOU_T));

    //���̳�ʼ��
    ThumbBengaliInitKb (data_ptr);
    
    //�����ʼ��
    rev_value = Sogou_ImeInit(data_ptr, WDI_BENGALI);
    if (rev_value != 0)
    {
        SCI_TRACE_LOW("[MMIIM]ThumbBengaliInit Sogou_ImeInit");
        return FALSE;
    }

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbBengaliInitKb

Func:   9���������뷨���̳�ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-07-12

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbBengaliInitKb(IM_DATA_T *data_ptr)
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
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL, *print_key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};
    
    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;
    SetPanel(data_ptr);

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
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
        vkb_ptr->keys[key_idx].print_text[0] = vkb_ptr->keys[key_idx].text[0] = ",.?!-"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].print_text_len = vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
	print_key_table_ptr = MMIIM_SP_Smart9KEY_GetPrintKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        cur_char_ptr = (wchar *)print_key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].print_text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].print_text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].print_text_len*sizeof(wchar));
		vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;

        vkb_ptr->keys[key_idx].f_long = ThumbSmartCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //--0 ���һ��char
    key_idx--;
    
    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
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
        vkb_ptr->keys[key_idx].f_long = ThumbSmartCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--VKEY_SET
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--star *
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_STAR;
	vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
    vkb_ptr->keys[key_idx].subtext_len = 1;
    vkb_ptr->keys[key_idx].subtext[0] = '*';
    vkb_ptr->keys[key_idx].text_len = 1;
	vkb_ptr->keys[key_idx].text[0] = HASANATA;
    vkb_ptr->keys[key_idx].print_text_len = 1;
	vkb_ptr->keys[key_idx].print_text[0] = HASANATA;
	vkb_ptr->keys[key_idx].f_up = ThumbSmartCharUp;
    //-- #
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_HASH;
	vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
    cur_char_ptr = (wchar *)key_table_ptr + ((VKEY_0 - VKEY_1 + 2) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
    vkb_ptr->keys[key_idx].print_text_len = vkb_ptr->keys[key_idx].text_len = 1;      //�ַ�����
    vkb_ptr->keys[key_idx].print_text[0] = vkb_ptr->keys[key_idx].text[0] = '#';
    vkb_ptr->keys[key_idx].f_up = MultiTapCharUp;    
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
    //������������ͬһ��λ�ã��������, ��ʼ������expand
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
        vkb_ptr->keys[key_idx].f_up = ThumbSmartSysUp;
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

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
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
    vkb_ptr->keys[key_idx].add_style.art_disp = KASA_NOT_BG_E;    //������������ֹ��sogouƤ�������ǵ�
#endif    
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].print_text_len = vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
	MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].print_text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].print_text_len);
        
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
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
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    return;
}



PUBLIC BOOLEAN ThumbBengaliTerm (IM_DATA_T *data_ptr)
{
	return ThumbSmartTerm (data_ptr);
}

#endif		//bengali end

#endif //defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_SOGOU)


