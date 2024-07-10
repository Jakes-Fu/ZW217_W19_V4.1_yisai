/*==============================================================================
File Name: mmiim_touch_app_sprd.c
Author: yaoguang.chen
Date: 2012/04/20
Copyright: 2012 Spreadtrum, Incorporated. All Rights Reserved.
Description:    因某些flash空间不足，需要将输入法引擎关闭，而当前的输入法均没有此种设计，
                    需要单独拉出一个文件来处理这种tp输入
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2012/04/23   yaoguang.chen  Create.
==============================================================================*/

/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/

#include "mmi_app_im_trc.h"
#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif

#include "mmiim_touch_ime_common.h"
#include "mmiim_touch_set.h"

#if (defined MMI_IM_PDA_SUPPORT && defined IM_ENGINE_NONE)


/* 系统 */
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr);
LOCAL BOOLEAN Terminate(void *handle);
LOCAL BOOLEAN HandleMsg(
    void *handle,
    GUIIM_EVENT_T event,
    GUIIM_EVENT_DATA_U *event_data_ptr
);

/* 系统辅助 */
LOCAL BOOLEAN HandleSysMsg(
    IM_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);
LOCAL void ClearState(IM_DATA_T *data_ptr);
LOCAL BOOLEAN ClearTimer(IM_DATA_T *data_ptr);
LOCAL void TermImm(IM_DATA_T *data_ptr);
LOCAL BOOLEAN SetMethod(
    IM_DATA_T *data_ptr,
    GUIIM_METHOD_T method
);


GUIIM_KERNEL_APIS_T const g_guiim_touch_sprd_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};

static CAF_HANDLE_T s_last_ctrl_handle = 0;





static SETTING_MENU_DATA_T const s_setting_menu_data[] =
{
#if defined (IM_RUSSIAN_SUPPORT)
    {
        GUIIM_LANG_RUSSIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_RUSSIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_ARABIC_SUPPORT)
    {
        GUIIM_LANG_ARABIC, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ARABIC_ABC
    },
#endif

#if defined (IM_ASSAMESE_SUPPORT)
    {
        GUIIM_LANG_ASSAMESE, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ASSAMESE_ABC
    },
#endif

#if defined (IM_BENGALI_SUPPORT)
    {
        GUIIM_LANG_BENGALI, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_BENGALI_ABC
    },
#endif

#if defined (IM_GEORGIAN_SUPPORT)
    {
        GUIIM_LANG_GEORGIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_GEORGIAN_ABC
    },
#endif

#if defined (IM_GUJARATI_SUPPORT)
    {
        GUIIM_LANG_GUJARATI, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_GUJARATI_ABC
    },
#endif

#if defined (IM_HEBREW_SUPPORT)
    {
        GUIIM_LANG_HEBREW, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_HEBREW_ABC
    },
#endif

#if defined (IM_HINDI_SUPPORT)
    {
        GUIIM_LANG_HINDI, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_HINDI_ABC
    },
#endif

#if defined (IM_KANNADA_SUPPORT)
    {
        GUIIM_LANG_KANNADA, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_KANNADA_ABC
    },
#endif

#if defined (IM_MALAYALAM_SUPPORT)
    {
        GUIIM_LANG_MALAYALAM, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_MALAYALAM_ABC
    },
#endif

//MYANMAR 缅甸
#if defined (IM_MYANMAR_SUPPORT)
    {
        GUIIM_LANG_MYANMAR, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_INPUT_LANGUAGE_MYANMAR
    },
#endif

#if defined (IM_MARATHI_SUPPORT)
    {
        GUIIM_LANG_MARATHI, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_MARATHI_ABC
    },
#endif

#if defined (IM_ORIYA_SUPPORT)
    {
        GUIIM_LANG_ORIYA, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ORIYA_ABC
    },
#endif

#if defined (IM_PERSIAN_SUPPORT)
    {
        GUIIM_LANG_PERSIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_PERSIAN_ABC
    },
#endif

#if defined (IM_PUNJABI_SUPPORT)
    {
        GUIIM_LANG_PUNJABI, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_PUNJABI_ABC
    },
#endif

#if defined (IM_TAGALOG_SUPPORT)
    {
        GUIIM_LANG_TAGALOG, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_TAGALOG_ABC
    },
#endif

#if defined (IM_TAMIL_SUPPORT)
    {
        GUIIM_LANG_TAMIL, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_TAMIL_ABC
    },
#endif

#if defined (IM_TELUGU_SUPPORT)
    {
        GUIIM_LANG_TELUGU, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_TELUGU_ABC
    },
#endif

#if defined (IM_THAI_SUPPORT)
    {
        GUIIM_LANG_THAI, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_THAI_ABC
    },
#endif

#if defined (IM_URDU_SUPPORT)
    {
        GUIIM_LANG_URDU, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_URDU_ABC
    },
#endif

#if defined (IM_AFRIKAANS_SUPPORT)
    {
        GUIIM_LANG_AFRIKAANS, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_AFRIKAANS_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_ALBANIAN_SUPPORT)
    {
        GUIIM_LANG_ALBANIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ALBANIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_ARMENIAN_SUPPORT)
    {
         GUIIM_LANG_ARMENIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ARMENIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_AZERBAIJANI_SUPPORT)
    {
        GUIIM_LANG_AZERBAIJANI, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_AZERBAIJANI_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_BASQUE_SUPPORT)
    {
        GUIIM_LANG_BASQUE, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_BASQUE_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_BULGARIAN_SUPPORT)
    {
        GUIIM_LANG_BULGARIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_BULGARIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_CATALAN_SUPPORT)
    {
        GUIIM_LANG_CATALAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_CATALAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_CROATIAN_SUPPORT)
    {
        GUIIM_LANG_CROATIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_CROATIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_CZECH_SUPPORT)
    {
        GUIIM_LANG_CZECH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_CZECH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_DANISH_SUPPORT)
    {
        GUIIM_LANG_DANISH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_DANISH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_DUTCH_SUPPORT)
    {
        GUIIM_LANG_DUTCH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_DUTCH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_ESTONIAN_SUPPORT)
    {
        GUIIM_LANG_ESTONIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ESTONIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_FILIPINO_SUPPORT)
    {
        GUIIM_LANG_FILIPINO, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_FILIPINO_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_FINNISH_SUPPORT)
    {
        GUIIM_LANG_FINNISH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_FINNISH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_FRENCH_SUPPORT)
    {
        GUIIM_LANG_FRENCH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_FRENCH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_GALICIAN_SUPPORT)
    {
        GUIIM_LANG_GALICIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_GALICIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_GERMAN_SUPPORT)
    {
        GUIIM_LANG_GERMAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_GERMAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_GREEK_SUPPORT)
    {
        GUIIM_LANG_GREEK, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_GREEK_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_HAUSA_SUPPORT)
    {
        GUIIM_LANG_HAUSA, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_HAUSA_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_HUNGARIAN_SUPPORT)
    {
        GUIIM_LANG_HUNGARIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_HUNGARIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_ICELANDIC_SUPPORT)
    {
        GUIIM_LANG_ICELANDIC, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ICELANDIC_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_IGBO_SUPPORT)
    {
        GUIIM_LANG_IGBO, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_IGBO_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_INDONESIAN_SUPPORT)
    {
        GUIIM_LANG_INDONESIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_INDONESIAN_ABC_UPPER
    },
#endif

#if defined (IM_IRISH_SUPPORT)
    {
        GUIIM_LANG_IRISH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_IRISH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_ITALIAN_SUPPORT)
    {
        GUIIM_LANG_ITALIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ITALIAN_ABC_UPPER
    },
#endif

#if defined (IM_KAZAKH_SUPPORT)
    {
        GUIIM_LANG_KAZAKH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_KAZAKH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_LATVIAN_SUPPORT)
    {
        GUIIM_LANG_LATVIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_LATVIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_LITHUANIAN_SUPPORT)
    {
        GUIIM_LANG_LITHUANIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_LITHUANIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_MACEDONIAN_SUPPORT)
    {
        GUIIM_LANG_MACEDONIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_MACEDONIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_MALAY_SUPPORT)
    {
        GUIIM_LANG_MALAY, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_MALAY_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_MOLDOVAN_SUPPORT)
    {
        GUIIM_LANG_MOLDOVAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_MOLDOVAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_NORWEGIAN_SUPPORT)
    {
        GUIIM_LANG_NORWEGIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_NORWEGIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_POLISH_SUPPORT)
    {
        GUIIM_LANG_POLISH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_POLISH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_PORTUGUESE_SUPPORT)
    {
        GUIIM_LANG_PORTUGUESE, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_PORTUGUESE_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_ROMANIAN_SUPPORT)
    {
        GUIIM_LANG_ROMANIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ROMANIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_SERBIAN_SUPPORT)
    {
        GUIIM_LANG_SERBIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_SERBIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_SESOTHO_SUPPORT)
    {
        GUIIM_LANG_SESOTHO, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_SESOTHO_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_SLOVAK_SUPPORT)
    {
        GUIIM_LANG_SLOVAK, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_SLOVAK_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_SLOVENIAN_SUPPORT)
    {
        GUIIM_LANG_SLOVENIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_SLOVENIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_SPANISH_SUPPORT)
    {
        GUIIM_LANG_SPANISH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_SPANISH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_SWAHILI_SUPPORT)
    {
        GUIIM_LANG_SWAHILI, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_SWAHILI_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_SWEDISH_SUPPORT)
    {
        GUIIM_LANG_SWEDISH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_SWEDISH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_TURKISH_SUPPORT)
    {
        GUIIM_LANG_TURKISH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_TURKISH_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_UKRAINIAN_SUPPORT)
    {
        GUIIM_LANG_UKRAINIAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_UKRAINIAN_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_VIETNAMESE_SUPPORT)
    {
        GUIIM_LANG_VIETNAMESE, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_VIETNAMESE_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_XHOSA_SUPPORT)
    {
        GUIIM_LANG_XHOSA, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_XHOSA_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_YORUBA_SUPPORT)
    {
        GUIIM_LANG_YORUBA, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_YORUBA_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_ZULU_SUPPORT)
    {
        GUIIM_LANG_ZULU, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ZULU_ABC_LEAD_UPPER
    },
#endif

#if defined (IM_AMHARIC_SUPPORT)
    {
        GUIIM_LANG_AMHARIC, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_AMHARIC_ABC
    },
#endif

#if defined (IM_KHMER_SUPPORT)
    {
        GUIIM_LANG_KHMER, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_KHMER_ABC
    },
#endif

#if defined (IM_LAO_SUPPORT)
    {
        GUIIM_LANG_LAO, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_LAO_ABC
    },
#endif

#if defined (IM_UYGHUR_SUPPORT)
    {
        GUIIM_LANG_UYGHUR, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_UYGHUR_ABC
    },
#endif

#if defined (IM_TIBETAN_SUPPORT)
    {
        GUIIM_LANG_TIBETAN, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_TIBETAN_ABC
    },
#endif

#if defined (IM_SINHALESE_SUPPORT)
    {
        GUIIM_LANG_SINHALESE, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_SINHALESE_ABC
    },
#endif

    {GUIIM_LANG_ENGLISH, GUIIM_TYPE_MULTITAP, GUIIM_INPUT_TYPE_NONE, IM_TXT_IM_SHORT_ABC_LEAD_UPPER},

    {0, 0, 0, TXT_COMN_MORE},
};






/*==============================================================================
Description: Initialize
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr)
{
    IM_DATA_T *data_ptr = PNULL;
    BOOLEAN is_method_set = FALSE;
    
    SCI_TRACE_LOW("[MMIIM] TP Initialize: param_ptr=%08X", param_ptr);
    
    //<-- 系统信息检查
    //1 必须支持多层
    if (!UILAYER_IsMultiLayerEnable())
    {
        SCI_TRACE_LOW("[MMIIM] TP Initialize: enable multilayer.");
        return PNULL;
    }

    //-->

    if (PNULL == param_ptr)
    {
        return PNULL;
    }

    data_ptr = (IM_DATA_T*)SCI_ALLOC_APP(sizeof(IM_DATA_T));
    SCI_MEMSET(data_ptr, 0, sizeof(IM_DATA_T));

    //data
    data_ptr->check_info = sizeof(IM_DATA_T);
    data_ptr->ctrl_handle = param_ptr->im_handle;
    data_ptr->init_param = *param_ptr;

    data_ptr->ui_layer.block_id = UILAYER_NULL_HANDLE;
    data_ptr->tip_layer.block_id = UILAYER_NULL_HANDLE;
    data_ptr->edit_layer.block_id = UILAYER_NULL_HANDLE;

    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr->im_def_ptr); /*assert verified*/
    if(PNULL == data_ptr->init_param.method_param_ptr || PNULL == data_ptr->init_param.method_param_ptr->im_def_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] TP Initialize: method_param_ptr is null.");
        return  PNULL;
    }
    
    data_ptr->f_term_imm = TermImm;
    data_ptr->f_set_method = SetMethod;
    data_ptr->f_draw_all = MMIIM_DrawAll;

    data_ptr->setting_menu_data = s_setting_menu_data;
    data_ptr->setting_menu_size = ARR_SIZE(s_setting_menu_data);

    data_ptr->kb_size = MMIIM_GetKeyboardSize();
    SetPanel(data_ptr);

    is_method_set = SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_def_ptr->method);

    //SCI_ASSERT(is_method_set); /*assert verified*/
    if(!is_method_set)
    {
        SCI_TRACE_LOW("[MMIIM] TP Initialize: is_method_set false.");
        return  PNULL;
    }
    //out effect 总是激活的，中途可关闭
    data_ptr->is_out_effect_enabled = FALSE;

    //如果启动输入法的控件handle还是上一个，则关闭in effect
    if (param_ptr->im_handle != s_last_ctrl_handle)
    {
        s_last_ctrl_handle = param_ptr->im_handle;
        //data_ptr->is_in_effect_enabled = TRUE;    //NEWMS00201394 【6530  量产客户】新建分组编辑分组名称时编辑名称框反应有迟钝
    }


    return data_ptr;
}


/*==============================================================================
Description: Terminate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(void *handle)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)handle;

    CHECK_DATA_EX(data_ptr);
    
    SCI_TRACE_LOW("[MMIIM] TP Terminate: handle=%08X, method=%04X, lang=%d, \
        data=%08X, vkb=%08X", handle, 
        data_ptr->init_param.method_param_ptr->im_def_ptr->method,
        data_ptr->init_param.method_param_ptr->im_def_ptr->lang,
        data_ptr->data, &(data_ptr->vkb));

    //check input lang and reset it if need...
    if (MMIAPIIM_GetLanguage() != data_ptr->init_param.method_param_ptr->im_def_ptr->lang
            && GUIIM_LANG_NONE != data_ptr->init_param.method_param_ptr->im_def_ptr->lang)
    {
        MMIAPIIM_SetLanguage(data_ptr->init_param.method_param_ptr->im_def_ptr->lang);
    }

    TermImm(data_ptr);

    ReleaseUILayer(data_ptr);
    ReleaseTipLayer(data_ptr);
    ReleaseEditLayer(data_ptr);

    SCI_FREEIF(data_ptr);

    return TRUE;
}


/*==============================================================================
Description: HandleMsg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMsg(
    void *handle,
    GUIIM_EVENT_T event,
    GUIIM_EVENT_DATA_U *event_data_ptr
)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)handle;
    BOOLEAN result = FALSE;
	uint32 time_begin = SCI_GetTickCount();
	
    CHECK_DATA(data_ptr);

    switch (event)
    {
    case GUIIM_EVENT_SYS_MSG:
        result = HandleSysMsg(data_ptr, event_data_ptr);
        break;

    case GUIIM_EVENT_PAINT:
        //针对部分模块，比如pubwin的蒙板问题，总是在full paint的时候将层移出来
        UILAYER_RemoveBltLayer(&data_ptr->ui_layer);
        result = MMIIM_DrawAll(data_ptr);
        break;

    case GUIIM_EVENT_SET_METHOD:
        TermImm(data_ptr);
        result = SetMethod(data_ptr, event_data_ptr->set_method);
        break;

    case GUIIM_EVENT_GET_METHOD:
        break;

    case GUIIM_EVENT_SLEEP:
    case GUIIM_EVENT_FORCE_COMMIT:
        //如果是强制提交的话，下面会把首字母大写的定时器停掉，导致无法自动
        //从首字大写切换到小写的功能
        if (CAPITTAL_MODE_INITIAL == data_ptr->vkb.cap
            && (data_ptr->multi_tap_data.commit_timer > 0))
        {
            SetVkbCaps(data_ptr, CAPITTAL_MODE_LOWER);
        }
        ClearState(data_ptr);
        MMIIM_DrawAll(data_ptr);
        break;

    case GUIIM_EVENT_GET_CTRL_RECT:
        event_data_ptr->get_ctrl_rect = data_ptr->rect;
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_RECT:
        event_data_ptr->get_rect = data_ptr->rect;
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_EDIT_STATE:
        break;

    case GUIIM_EVENT_CUSTOM_KEY:
        break;

    default:
        break;
    }

	SCI_TRACE_LOW ("[MMIIM] time_begin == %d, time_interval == %d", time_begin, SCI_GetTickCount() - time_begin);
	
    return result;
}



/*==============================================================================
Description: HandleSysMsg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsg(
    IM_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_omit_msg = FALSE;
    KEY_INFO_T *key_ptr = PNULL;
    GUI_POINT_T point = {0};

    CHECK_DATA(data_ptr);
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if(PNULL == event_data_ptr)
    {
        return  FALSE;
    }
    
    switch (event_data_ptr->sys_msg.msg_id)
    {
    case MSG_CTL_GET_FOCUS:

        if (data_ptr->more_setting_win_handle != 0)
        {
            MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_GetKeyboardSize();
            GUIIM_NOTIFY_PACK_T notify_pack = {0};
            BOOLEAN need_reset = FALSE;
            GUIIM_METHOD_T new_method = GUIIM_M_NONE;
            
            GUIIM_RemoveWin(data_ptr->init_param.im_handle,  data_ptr->more_setting_win_handle);
            data_ptr->more_setting_win_handle = 0;

            if (data_ptr->kb_size != kb_size)
            {
                data_ptr->kb_size = kb_size;
                SetPanel(data_ptr);
                need_reset = TRUE;
            }

            //有可能从更多窗口设置了新的输入语言，导致输入法发生改变
            if (MMIAPIIM_GetLanguage() != data_ptr->init_param.method_param_ptr->im_def_ptr->lang)  //输入语言发生改变
            {
                //get new method
                new_method = GetMethodBySwitchLang(data_ptr, MMIAPIIM_GetLanguage());
            }

            //输入法改变引起的重设
            if (new_method != GUIIM_M_NONE)
            {
                notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
                notify_pack.data.set_method = new_method;
                GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            }
            //面板重设
            else if (need_reset)
            {
                TermImm(data_ptr);
                SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_def_ptr->method);

                notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
                GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            }
        }

        break;

    case MSG_CTL_LOSE_FOCUS:

        if (PNULL != data_ptr->vkb.last_key)
        {
            if (data_ptr->vkb.last_key->is_press) data_ptr->vkb.last_key->is_press = 0;

            data_ptr->vkb.last_key = PNULL;
        }

        ClearTimer(data_ptr);
        ReleaseUILayer(data_ptr);
        ReleaseTipLayer(data_ptr);
        ReleaseEditLayer(data_ptr);
        MMIIM_CloseMethodSettingMenu (data_ptr);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        if (data_ptr->vkb.state & KEYBOARD_CHOOSE_MENU)
        {
            MMIIM_MethodSettingMenuDown (data_ptr, point, TRUE);
            break;
        }        
        
        data_ptr->first_tp = point;
        data_ptr->last_tp = point;
        data_ptr->vkb.down_point = point;

        key_ptr = FindKey(data_ptr, &point);

        if (PNULL != key_ptr && PNULL != key_ptr->f_down
                && (0 == data_ptr->vkb.is_key_block || key_ptr == data_ptr->vkb.last_key))
        {
            PanelSlideCueClose (data_ptr, FALSE);  //有按键操作，关闭

            data_ptr->vkb.first_key = key_ptr;
            key_ptr->f_down(data_ptr, key_ptr);
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        //在失去焦点并且重新获得之后，可能会丢失last key信息
        if (PNULL == data_ptr->vkb.last_key)
            break;

        if (data_ptr->vkb.state & KEYBOARD_CHOOSE_LETTER)
        {
            RefreshTipLetterSelectIndex(data_ptr, &point, TRUE);
            break;
        }
        else if (data_ptr->vkb.state & KEYBOARD_CHOOSE_MENU)
        {
            MMIIM_MethodSettingMenuRefresh (data_ptr, point, TRUE);
            break;
        }        

        data_ptr->last_tp = point;

        key_ptr = FindKey(data_ptr, &point);

        if (PNULL != key_ptr && PNULL != key_ptr->f_move
                && PNULL != data_ptr->vkb.last_key
                && (0 == data_ptr->vkb.is_key_block || key_ptr == data_ptr->vkb.last_key))
        {
            key_ptr->f_move(data_ptr, key_ptr);
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        key_ptr = FindKey(data_ptr, &point);

        if (PNULL != key_ptr && PNULL != key_ptr->f_long
                && (0 == data_ptr->vkb.is_key_block || key_ptr == data_ptr->vkb.last_key))
        {
            key_ptr->f_long(data_ptr, key_ptr);
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            int16 delta_x = 0;
            int16 test_len = 0;
            BOOLEAN is_msg_deal = FALSE;

            point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
            point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

            if (data_ptr->vkb.state & KEYBOARD_CHOOSE_MENU)
            {
                MMIIM_MethodSettingMenuUp (data_ptr);
                break;
            }      
            
            //在失去焦点并且重新获得之后，可能会丢失last key信息
            if (PNULL == data_ptr->vkb.last_key)
                break;

            test_len = (data_ptr->rect.right - data_ptr->rect.left) / 10;
            test_len *= VKB_SLIDING_THRESHOLD;

            data_ptr->last_tp = point;

            delta_x = data_ptr->vkb.down_point.x - point.x;

            if (1 == data_ptr->vkb.is_key_block)
            {
                data_ptr->vkb.last_key->f_up(data_ptr, data_ptr->vkb.last_key);
                is_msg_deal = TRUE;
            }
            else if ((delta_x < (-1)*test_len || test_len < delta_x)
                     && PNULL != data_ptr->vkb.f_sliding)
            {
                int32 dir = (delta_x < (-1) * test_len) ? -1 : 1;

                is_msg_deal = data_ptr->vkb.f_sliding(data_ptr, dir);
            }

            if (!is_msg_deal)
            {
                key_ptr = FindKey(data_ptr, &point);

                if (PNULL != key_ptr && PNULL != key_ptr->f_up
                        && data_ptr->vkb.last_key == key_ptr)
                {
                    key_ptr->f_up(data_ptr, key_ptr);
                }
                else if (data_ptr->vkb.last_key->is_press)
                {
                    data_ptr->vkb.last_key->is_press = 0;
                    HideTip(data_ptr);
                    MMIIM_DrawAll(data_ptr);
                }
            }
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
//NEWMS00181788 [6530][64X64_320X240TK_QW]日历日程主题调出虚拟键盘后，右下角应该显示返回        
//PDA版本没有softkey，直接隐藏，但是非PDA版本有softkey，应该与softkey字符信息一致
#if defined MMI_PDA_SUPPORT         
        // 如果是PUBWIN窗口，不处理cancel消息，由PUBWIN直接退出窗口
        // 用param来判断是触屏还是按键来的CANCEL消息
        if (MMK_GetWinDisplayStyleState(data_ptr->init_param.win_handle, WS_HAS_PROMPT_WIN)
            && 0 == event_data_ptr->sys_msg.param)
        {
            is_omit_msg = TRUE;
        }
        else
        {
            HidePanel(data_ptr);
        }
#else
        is_omit_msg = TRUE;    //让softkey自己处理
#endif
        break;

    default:
        is_omit_msg = TRUE;
        break;
    }

    return (!is_omit_msg);
}




/*==============================================================================
Description: 强制提交未提交的字符后的处理
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ClearState(IM_DATA_T *data_ptr)
{
    BOOLEAN is_all_clear = FALSE;

    CHECK_DATA(data_ptr);

    is_all_clear = ClearTimer(data_ptr);

    UILAYER_RemoveBltLayer(&data_ptr->edit_layer);

    if (PNULL == data_ptr->vkb.keys)
    {
        return;
    }

    if (data_ptr->is_symbol_panel)
    {
        return;
    }

    switch (data_ptr->init_param.method_param_ptr->im_def_ptr->method)
    {
    case GUIIM_M_TP_ENGLISH_ABC:
    case GUIIM_M_TP_ENGLISH_ABC_LOWER:
    case GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ENGLISH_ABC_UPPER:
        break;

    case GUIIM_M_TP_123:
    case GUIIM_M_TPQ_123:
        break;

    case GUIIM_M_TPQ_ENGLISH_ABC:
    case GUIIM_M_TPQ_ENGLISH_ABC_LOWER:
    case GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ENGLISH_ABC_UPPER:
        break;

    default:
        break;
    }

    return;
}


/*==============================================================================
Description: 清楚定时处理信息
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN ClearTimer(IM_DATA_T *data_ptr)
{
    BOOLEAN is_all_clear = FALSE;

    CHECK_DATA(data_ptr);

    if (0 != data_ptr->tip_hide_timer)
    {
        MMK_StopTimer(data_ptr->tip_hide_timer);
        data_ptr->tip_hide_timer = 0;
        UILAYER_RemoveBltLayer(&data_ptr->tip_layer);
        data_ptr->is_tip_display = FALSE;
    }

    if (0 != data_ptr->cue_hide_timer_id)
    {
        MMK_StopTimer(data_ptr->cue_hide_timer_id);
        data_ptr->cue_hide_timer_id = 0;

        if (data_ptr->vkb.vessel_key)
        {
            SCI_MEMSET (data_ptr->vkb.vessel_key->print_text, 0, sizeof(data_ptr->vkb.vessel_key->print_text)); 
            data_ptr->vkb.vessel_key->print_text_len = 0;
        }        
    }

    //force commit时，需要清空该状态，与输入法本身无关，某些职能输入法也许存在multi-tap字符
    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
        data_ptr->multi_tap_data.last_key_index = 0;
    }
    
    if (PNULL == data_ptr->vkb.keys)
    {
        return FALSE;
    }

    if (data_ptr->is_symbol_panel)
    {
        return FALSE;
    }

    switch (data_ptr->init_param.method_param_ptr->im_def_ptr->method)
    {
    case GUIIM_M_TP_ENGLISH_ABC:
    case GUIIM_M_TP_ENGLISH_ABC_LOWER:
    case GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ENGLISH_ABC_UPPER:

        if (((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer > 0)
        {
            MMK_StopTimer(((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer);
            ((KERNEL_DATA_ABC_T*)(data_ptr->data))->commit_timer = 0;
            ((KERNEL_DATA_ABC_T*)(data_ptr->data))->last_key_index = 0;
        }

        break;

#if defined  IM_TRAD_CHINESE_SUPPORT
        case GUIIM_M_TP_ZHUYIN:
        //case GUIIM_M_TPQ_ZHUYIN:  //没有起timer
        if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer > 0)
        {
            MMK_StopTimer(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer);
            ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer = 0;
            ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->last_key_index = 0;
        }

        break;
#endif

    default:
        break;
    }

    return is_all_clear;
}


/*==============================================================================
Description: TermImm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void TermImm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    if (0 != data_ptr->tip_hide_timer)
    {
        MMK_StopTimer(data_ptr->tip_hide_timer);
        data_ptr->tip_hide_timer = 0;
    }

    if (0 != data_ptr->cue_hide_timer_id)
    {
        MMK_StopTimer(data_ptr->cue_hide_timer_id);
        data_ptr->cue_hide_timer_id = 0;
    }

    ClearMultitapTimer (data_ptr);
    
    UILAYER_RemoveBltLayer(&data_ptr->tip_layer);
    data_ptr->is_tip_display = FALSE;
    UILAYER_RemoveBltLayer(&data_ptr->edit_layer);
    MMIIM_CloseMethodSettingMenu (data_ptr);
    
    if (PNULL == data_ptr->vkb.keys)
    {
        SCI_TRACE_LOW("[MMIIM] TP TermImm: vkb.keys is null.");
        
        return;
    }

    if (data_ptr->is_symbol_panel)
    {
        SCI_TRACE_LOW("[MMIIM] TP TermImm: SymTerm.");
        SymTerm(data_ptr);
        return;
    }

    switch (data_ptr->init_param.method_param_ptr->im_def_ptr->method)
    {
    case GUIIM_M_TP_123:
    case GUIIM_M_TPQ_123:
        DigitTerm(data_ptr);
        break;
    case GUIIM_M_TP_SYS:
        SysTerm(data_ptr);
        break;

    case GUIIM_M_TP_ENGLISH_ABC:
    case GUIIM_M_TP_ENGLISH_ABC_LOWER:
    case GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ENGLISH_ABC_UPPER:
        ThumbAbcTerm(data_ptr);
        g_default_en_type = GUIIM_TYPE_MULTITAP;
        g_default_input_type = GUIIM_INPUT_TYPE_TOUCHPEN;
        break;

    case GUIIM_M_TPQ_ENGLISH_ABC:
    case GUIIM_M_TPQ_ENGLISH_ABC_LOWER:
    case GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ENGLISH_ABC_UPPER:
        QwertAbcTerm(data_ptr);
        g_default_en_type = GUIIM_TYPE_MULTITAP;
        g_default_input_type = GUIIM_INPUT_TYPE_TP_QWERT;
        break;

//阿姆哈拉9键没有走multi-tap流程
#ifdef IM_AMHARIC_SUPPORT
    case GUIIM_M_TP_AMHARIC_ABC:
        {
            ThumbAmharicTerm (data_ptr);
            g_default_ch_type = GUIIM_TYPE_MULTITAP;
            g_default_input_type = GUIIM_INPUT_TYPE_TOUCHPEN;
            break;
        }
#endif

    default:
        {
            GUIIM_METHOD_CFG_T *method_cfg_ptr = PNULL;
            
            method_cfg_ptr = MMITHEME_IMGetDefByMethod(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
            if(method_cfg_ptr)
            {
                //9键
                if (method_cfg_ptr->input == GUIIM_INPUT_TYPE_TOUCHPEN)
                {
                    if (method_cfg_ptr->type == GUIIM_TYPE_MULTITAP)
                    {
                        ThumbMultiTapTerm (data_ptr);
                        g_default_ch_type = GUIIM_TYPE_MULTITAP;
                        g_default_input_type = GUIIM_INPUT_TYPE_TOUCHPEN;
                    }
                }
                //26键
                else if (method_cfg_ptr->input == GUIIM_INPUT_TYPE_TP_QWERT)
                {
                    if (method_cfg_ptr->type == GUIIM_TYPE_MULTITAP)
                    {
                        QwertMultiTapTerm(data_ptr);
                        g_default_ch_type = GUIIM_TYPE_MULTITAP;
                        g_default_input_type = GUIIM_INPUT_TYPE_TP_QWERT;
                    }
                }
            }
            else
            {
                SCI_TRACE_LOW("[MMIIM] TP TermImm: default unreason switch.");
            }
        }    
        break;
    }

    return;
}


/*==============================================================================
Description: SetMethod
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetMethod(
    IM_DATA_T *data_ptr,
    GUIIM_METHOD_T method
)
{
    BOOLEAN is_init = FALSE;

    CHECK_DATA(data_ptr);
    
    SCI_TRACE_LOW("[MMIIM] TP SetMethod: data_ptr=%08X, method=%04X", data_ptr, method);

    switch (method)
    {
    case GUIIM_M_TP_123:
    case GUIIM_M_TPQ_123:
        is_init = DigitInit(data_ptr);
        break;
    case GUIIM_M_TP_SYS:
        is_init = SysInit(data_ptr, 0);
        break;
        //ARABIC 阿拉伯
#if defined (IM_ARABIC_SUPPORT)
    case GUIIM_M_TP_ARABIC_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ARABIC_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

//ASSAMESE
#if defined (IM_ASSAMESE_SUPPORT)
    case GUIIM_M_TP_ASSAMESE_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ASSAMESE_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

//BENGALI
#if defined (IM_BENGALI_SUPPORT)
    case GUIIM_M_TP_BENGALI_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_BENGALI_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

//GEORGIAN
#if defined (IM_GEORGIAN_SUPPORT)
    case GUIIM_M_TP_GEORGIAN_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_GEORGIAN_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

//GUJARATI
#if defined (IM_GUJARATI_SUPPORT)
    case GUIIM_M_TP_GUJARATI_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_GUJARATI_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

//HEBREW
#if defined (IM_HEBREW_SUPPORT)
    case GUIIM_M_TP_HEBREW_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_HEBREW_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //印地
#ifdef IM_HINDI_SUPPORT
    case GUIIM_M_TP_HINDI_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_HINDI_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //卡纳达
#ifdef IM_KANNADA_SUPPORT
    case GUIIM_M_TP_KANNADA_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_KANNADA_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //MALAYALAM
#ifdef IM_MALAYALAM_SUPPORT
    case GUIIM_M_TP_MALAYALAM_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_MALAYALAM_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //MARATHI
#ifdef IM_MARATHI_SUPPORT
    case GUIIM_M_TP_MARATHI_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_MARATHI_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //ORIYA
#ifdef IM_ORIYA_SUPPORT
    case GUIIM_M_TP_ORIYA_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ORIYA_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //波斯
#ifdef IM_PERSIAN_SUPPORT
    case GUIIM_M_TP_PERSIAN_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_PERSIAN_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //PUNJABI
#ifdef IM_PUNJABI_SUPPORT
    case GUIIM_M_TP_PUNJABI_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_PUNJABI_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //TAGALOG
#ifdef IM_TAGALOG_SUPPORT
    case GUIIM_M_TP_TAGALOG_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_TAGALOG_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //TAMIL
#ifdef IM_TAMIL_SUPPORT
    case GUIIM_M_TP_TAMIL_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_TAMIL_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //TELUGU
#ifdef IM_TELUGU_SUPPORT
    case GUIIM_M_TP_TELUGU_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_TELUGU_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //THAI
#ifdef IM_THAI_SUPPORT
    case GUIIM_M_TP_THAI_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_THAI_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //乌尔都
#ifdef IM_URDU_SUPPORT
    case GUIIM_M_TP_URDU_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_URDU_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //AFRIKAANS
#ifdef IM_AFRIKAANS_SUPPORT
    case GUIIM_M_TP_AFRIKAANS_ABC:
    case GUIIM_M_TP_AFRIKAANS_ABC_LEAD_UPPER:
    case GUIIM_M_TP_AFRIKAANS_ABC_LOWER:
    case GUIIM_M_TP_AFRIKAANS_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_AFRIKAANS_ABC:
    case GUIIM_M_TPQ_AFRIKAANS_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_AFRIKAANS_ABC_LOWER:
    case GUIIM_M_TPQ_AFRIKAANS_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //ALBANIAN
#ifdef IM_ALBANIAN_SUPPORT
    case GUIIM_M_TP_ALBANIAN_ABC:
    case GUIIM_M_TP_ALBANIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ALBANIAN_ABC_LOWER:
    case GUIIM_M_TP_ALBANIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ALBANIAN_ABC:
    case GUIIM_M_TPQ_ALBANIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ALBANIAN_ABC_LOWER:
    case GUIIM_M_TPQ_ALBANIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //ARMENIAN
#ifdef IM_ARMENIAN_SUPPORT
    case GUIIM_M_TP_ARMENIAN_ABC:
    case GUIIM_M_TP_ARMENIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ARMENIAN_ABC_LOWER:
    case GUIIM_M_TP_ARMENIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ARMENIAN_ABC:
    case GUIIM_M_TPQ_ARMENIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ARMENIAN_ABC_LOWER:
    case GUIIM_M_TPQ_ARMENIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //AZERBAIJANI
#ifdef IM_AZERBAIJANI_SUPPORT
    case GUIIM_M_TP_AZERBAIJANI_ABC:
    case GUIIM_M_TP_AZERBAIJANI_ABC_LEAD_UPPER:
    case GUIIM_M_TP_AZERBAIJANI_ABC_LOWER:
    case GUIIM_M_TP_AZERBAIJANI_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_AZERBAIJANI_ABC:
    case GUIIM_M_TPQ_AZERBAIJANI_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_AZERBAIJANI_ABC_LOWER:
    case GUIIM_M_TPQ_AZERBAIJANI_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //BASQUE
#ifdef IM_BASQUE_SUPPORT
    case GUIIM_M_TP_BASQUE_ABC:
    case GUIIM_M_TP_BASQUE_ABC_LEAD_UPPER:
    case GUIIM_M_TP_BASQUE_ABC_LOWER:
    case GUIIM_M_TP_BASQUE_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_BASQUE_ABC:
    case GUIIM_M_TPQ_BASQUE_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_BASQUE_ABC_LOWER:
    case GUIIM_M_TPQ_BASQUE_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //BULGARIAN
#ifdef IM_BULGARIAN_SUPPORT
    case GUIIM_M_TP_BULGARIAN_ABC:
    case GUIIM_M_TP_BULGARIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_BULGARIAN_ABC_LOWER:
    case GUIIM_M_TP_BULGARIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_BULGARIAN_ABC:
    case GUIIM_M_TPQ_BULGARIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_BULGARIAN_ABC_LOWER:
    case GUIIM_M_TPQ_BULGARIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //CATALAN
#ifdef IM_CATALAN_SUPPORT
    case GUIIM_M_TP_CATALAN_ABC:
    case GUIIM_M_TP_CATALAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_CATALAN_ABC_LOWER:
    case GUIIM_M_TP_CATALAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_CATALAN_ABC:
    case GUIIM_M_TPQ_CATALAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_CATALAN_ABC_LOWER:
    case GUIIM_M_TPQ_CATALAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //CROATIAN
#ifdef IM_CROATIAN_SUPPORT
    case GUIIM_M_TP_CROATIAN_ABC:
    case GUIIM_M_TP_CROATIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_CROATIAN_ABC_LOWER:
    case GUIIM_M_TP_CROATIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_CROATIAN_ABC:
    case GUIIM_M_TPQ_CROATIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_CROATIAN_ABC_LOWER:
    case GUIIM_M_TPQ_CROATIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //CZECH
#ifdef IM_CZECH_SUPPORT
    case GUIIM_M_TP_CZECH_ABC:
    case GUIIM_M_TP_CZECH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_CZECH_ABC_LOWER:
    case GUIIM_M_TP_CZECH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_CZECH_ABC:
    case GUIIM_M_TPQ_CZECH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_CZECH_ABC_LOWER:
    case GUIIM_M_TPQ_CZECH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //DANISH
#ifdef IM_DANISH_SUPPORT
    case GUIIM_M_TP_DANISH_ABC:
    case GUIIM_M_TP_DANISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_DANISH_ABC_LOWER:
    case GUIIM_M_TP_DANISH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_DANISH_ABC:
    case GUIIM_M_TPQ_DANISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_DANISH_ABC_LOWER:
    case GUIIM_M_TPQ_DANISH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //DUTCH
#ifdef IM_DUTCH_SUPPORT
    case GUIIM_M_TP_DUTCH_ABC:
    case GUIIM_M_TP_DUTCH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_DUTCH_ABC_LOWER:
    case GUIIM_M_TP_DUTCH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_DUTCH_ABC:
    case GUIIM_M_TPQ_DUTCH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_DUTCH_ABC_LOWER:
    case GUIIM_M_TPQ_DUTCH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

    case GUIIM_M_TP_ENGLISH_ABC:
    case GUIIM_M_TP_ENGLISH_ABC_LOWER:
    case GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ENGLISH_ABC_UPPER:
        is_init = ThumbAbcInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ENGLISH_ABC:
    case GUIIM_M_TPQ_ENGLISH_ABC_LOWER:
    case GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ENGLISH_ABC_UPPER:
        is_init = QwertAbcInit(data_ptr);
        break;

        //ESTONIAN
#ifdef IM_ESTONIAN_SUPPORT
    case GUIIM_M_TP_ESTONIAN_ABC:
    case GUIIM_M_TP_ESTONIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ESTONIAN_ABC_LOWER:
    case GUIIM_M_TP_ESTONIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ESTONIAN_ABC:
    case GUIIM_M_TPQ_ESTONIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ESTONIAN_ABC_LOWER:
    case GUIIM_M_TPQ_ESTONIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //FILIPINO
#ifdef IM_FILIPINO_SUPPORT
    case GUIIM_M_TP_FILIPINO_ABC:
    case GUIIM_M_TP_FILIPINO_ABC_LEAD_UPPER:
    case GUIIM_M_TP_FILIPINO_ABC_LOWER:
    case GUIIM_M_TP_FILIPINO_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_FILIPINO_ABC:
    case GUIIM_M_TPQ_FILIPINO_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_FILIPINO_ABC_LOWER:
    case GUIIM_M_TPQ_FILIPINO_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //FINNISH
#ifdef IM_FINNISH_SUPPORT
    case GUIIM_M_TP_FINNISH_ABC:
    case GUIIM_M_TP_FINNISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_FINNISH_ABC_LOWER:
    case GUIIM_M_TP_FINNISH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_FINNISH_ABC:
    case GUIIM_M_TPQ_FINNISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_FINNISH_ABC_LOWER:
    case GUIIM_M_TPQ_FINNISH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //french 法语
#if defined (IM_FRENCH_SUPPORT)
    case GUIIM_M_TP_FRENCH_ABC:
    case GUIIM_M_TP_FRENCH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_FRENCH_ABC_LOWER:
    case GUIIM_M_TP_FRENCH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_FRENCH_ABC:
    case GUIIM_M_TPQ_FRENCH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_FRENCH_ABC_LOWER:
    case GUIIM_M_TPQ_FRENCH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //GALICIAN
#if defined (IM_GALICIAN_SUPPORT)
    case GUIIM_M_TP_GALICIAN_ABC:
    case GUIIM_M_TP_GALICIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_GALICIAN_ABC_LOWER:
    case GUIIM_M_TP_GALICIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_GALICIAN_ABC:
    case GUIIM_M_TPQ_GALICIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_GALICIAN_ABC_LOWER:
    case GUIIM_M_TPQ_GALICIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //德语
#ifdef IM_GERMAN_SUPPORT
    case GUIIM_M_TP_GERMAN_ABC:
    case GUIIM_M_TP_GERMAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_GERMAN_ABC_LOWER:
    case GUIIM_M_TP_GERMAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_GERMAN_ABC:
    case GUIIM_M_TPQ_GERMAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_GERMAN_ABC_LOWER:
    case GUIIM_M_TPQ_GERMAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //GREEK
#ifdef IM_GREEK_SUPPORT
    case GUIIM_M_TP_GREEK_ABC:
    case GUIIM_M_TP_GREEK_ABC_LEAD_UPPER:
    case GUIIM_M_TP_GREEK_ABC_LOWER:
    case GUIIM_M_TP_GREEK_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_GREEK_ABC:
    case GUIIM_M_TPQ_GREEK_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_GREEK_ABC_LOWER:
    case GUIIM_M_TPQ_GREEK_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //HAUSA
#ifdef IM_HAUSA_SUPPORT
    case GUIIM_M_TP_HAUSA_ABC:
    case GUIIM_M_TP_HAUSA_ABC_LEAD_UPPER:
    case GUIIM_M_TP_HAUSA_ABC_LOWER:
    case GUIIM_M_TP_HAUSA_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_HAUSA_ABC:
    case GUIIM_M_TPQ_HAUSA_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_HAUSA_ABC_LOWER:
    case GUIIM_M_TPQ_HAUSA_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //HUNGARIAN 匈牙利
#if defined (IM_HUNGARIAN_SUPPORT)
    case GUIIM_M_TP_HUNGARIAN_ABC:
    case GUIIM_M_TP_HUNGARIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_HUNGARIAN_ABC_LOWER:
    case GUIIM_M_TP_HUNGARIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_HUNGARIAN_ABC:
    case GUIIM_M_TPQ_HUNGARIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_HUNGARIAN_ABC_LOWER:
    case GUIIM_M_TPQ_HUNGARIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //ICELANDIC
#ifdef IM_ICELANDIC_SUPPORT
    case GUIIM_M_TP_ICELANDIC_ABC:
    case GUIIM_M_TP_ICELANDIC_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ICELANDIC_ABC_LOWER:
    case GUIIM_M_TP_ICELANDIC_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ICELANDIC_ABC:
    case GUIIM_M_TPQ_ICELANDIC_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ICELANDIC_ABC_LOWER:
    case GUIIM_M_TPQ_ICELANDIC_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //IGBO
#ifdef IM_IGBO_SUPPORT
    case GUIIM_M_TP_IGBO_ABC:
    case GUIIM_M_TP_IGBO_ABC_LEAD_UPPER:
    case GUIIM_M_TP_IGBO_ABC_LOWER:
    case GUIIM_M_TP_IGBO_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_IGBO_ABC:
    case GUIIM_M_TPQ_IGBO_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_IGBO_ABC_LOWER:
    case GUIIM_M_TPQ_IGBO_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //印尼
#ifdef IM_INDONESIAN_SUPPORT
    case GUIIM_M_TP_INDONESIAN_ABC:
    case GUIIM_M_TP_INDONESIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_INDONESIAN_ABC_LOWER:
    case GUIIM_M_TP_INDONESIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_INDONESIAN_ABC:
    case GUIIM_M_TPQ_INDONESIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_INDONESIAN_ABC_LOWER:
    case GUIIM_M_TPQ_INDONESIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //IRISH
#ifdef IM_IRISH_SUPPORT
    case GUIIM_M_TP_IRISH_ABC:
    case GUIIM_M_TP_IRISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_IRISH_ABC_LOWER:
    case GUIIM_M_TP_IRISH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_IRISH_ABC:
    case GUIIM_M_TPQ_IRISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_IRISH_ABC_LOWER:
    case GUIIM_M_TPQ_IRISH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //ITALIAN 意大利
#if defined (IM_ITALIAN_SUPPORT)
    case GUIIM_M_TP_ITALIAN_ABC:
    case GUIIM_M_TP_ITALIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ITALIAN_ABC_LOWER:
    case GUIIM_M_TP_ITALIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ITALIAN_ABC:
    case GUIIM_M_TPQ_ITALIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ITALIAN_ABC_LOWER:
    case GUIIM_M_TPQ_ITALIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //KAZAKH
#ifdef IM_KAZAKH_SUPPORT
    case GUIIM_M_TP_KAZAKH_ABC:
    case GUIIM_M_TP_KAZAKH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_KAZAKH_ABC_LOWER:
    case GUIIM_M_TP_KAZAKH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_KAZAKH_ABC:
    case GUIIM_M_TPQ_KAZAKH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_KAZAKH_ABC_LOWER:
    case GUIIM_M_TPQ_KAZAKH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //LATVIAN
#ifdef IM_LATVIAN_SUPPORT
    case GUIIM_M_TP_LATVIAN_ABC:
    case GUIIM_M_TP_LATVIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_LATVIAN_ABC_LOWER:
    case GUIIM_M_TP_LATVIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_LATVIAN_ABC:
    case GUIIM_M_TPQ_LATVIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_LATVIAN_ABC_LOWER:
    case GUIIM_M_TPQ_LATVIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //LITHUANIAN
#ifdef IM_LITHUANIAN_SUPPORT
    case GUIIM_M_TP_LITHUANIAN_ABC:
    case GUIIM_M_TP_LITHUANIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_LITHUANIAN_ABC_LOWER:
    case GUIIM_M_TP_LITHUANIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_LITHUANIAN_ABC:
    case GUIIM_M_TPQ_LITHUANIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_LITHUANIAN_ABC_LOWER:
    case GUIIM_M_TPQ_LITHUANIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //MACEDONIAN
#ifdef IM_MACEDONIAN_SUPPORT
    case GUIIM_M_TP_MACEDONIAN_ABC:
    case GUIIM_M_TP_MACEDONIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_MACEDONIAN_ABC_LOWER:
    case GUIIM_M_TP_MACEDONIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_MACEDONIAN_ABC:
    case GUIIM_M_TPQ_MACEDONIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_MACEDONIAN_ABC_LOWER:
    case GUIIM_M_TPQ_MACEDONIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //MALAY //马来西亚
#ifdef IM_MALAY_SUPPORT
    case GUIIM_M_TP_MALAY_ABC:
    case GUIIM_M_TP_MALAY_ABC_LEAD_UPPER:
    case GUIIM_M_TP_MALAY_ABC_LOWER:
    case GUIIM_M_TP_MALAY_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_MALAY_ABC:
    case GUIIM_M_TPQ_MALAY_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_MALAY_ABC_LOWER:
    case GUIIM_M_TPQ_MALAY_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //MOLDOVAN
#ifdef IM_MOLDOVAN_SUPPORT
    case GUIIM_M_TP_MOLDOVAN_ABC:
    case GUIIM_M_TP_MOLDOVAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_MOLDOVAN_ABC_LOWER:
    case GUIIM_M_TP_MOLDOVAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_MOLDOVAN_ABC:
    case GUIIM_M_TPQ_MOLDOVAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_MOLDOVAN_ABC_LOWER:
    case GUIIM_M_TPQ_MOLDOVAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //NORWEGIAN
#ifdef IM_NORWEGIAN_SUPPORT
    case GUIIM_M_TP_NORWEGIAN_ABC:
    case GUIIM_M_TP_NORWEGIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_NORWEGIAN_ABC_LOWER:
    case GUIIM_M_TP_NORWEGIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_NORWEGIAN_ABC:
    case GUIIM_M_TPQ_NORWEGIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_NORWEGIAN_ABC_LOWER:
    case GUIIM_M_TPQ_NORWEGIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //POLISH
#ifdef IM_POLISH_SUPPORT
    case GUIIM_M_TP_POLISH_ABC:
    case GUIIM_M_TP_POLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_POLISH_ABC_LOWER:
    case GUIIM_M_TP_POLISH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_POLISH_ABC:
    case GUIIM_M_TPQ_POLISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_POLISH_ABC_LOWER:
    case GUIIM_M_TPQ_POLISH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //PORTUGUESE 葡萄牙
#if defined (IM_PORTUGUESE_SUPPORT)
    case GUIIM_M_TP_PORTUGUESE_ABC:
    case GUIIM_M_TP_PORTUGUESE_ABC_LEAD_UPPER:
    case GUIIM_M_TP_PORTUGUESE_ABC_LOWER:
    case GUIIM_M_TP_PORTUGUESE_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_PORTUGUESE_ABC:
    case GUIIM_M_TPQ_PORTUGUESE_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_PORTUGUESE_ABC_LOWER:
    case GUIIM_M_TPQ_PORTUGUESE_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //ROMANIAN
#ifdef IM_ROMANIAN_SUPPORT
    case GUIIM_M_TP_ROMANIAN_ABC:
    case GUIIM_M_TP_ROMANIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ROMANIAN_ABC_LOWER:
    case GUIIM_M_TP_ROMANIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ROMANIAN_ABC:
    case GUIIM_M_TPQ_ROMANIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ROMANIAN_ABC_LOWER:
    case GUIIM_M_TPQ_ROMANIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //RUSSIAN 俄语
#if defined (IM_RUSSIAN_SUPPORT)
    case GUIIM_M_TP_RUSSIAN_ABC:
    case GUIIM_M_TP_RUSSIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_RUSSIAN_ABC_LOWER:
    case GUIIM_M_TP_RUSSIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_RUSSIAN_ABC:
    case GUIIM_M_TPQ_RUSSIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_RUSSIAN_ABC_LOWER:
    case GUIIM_M_TPQ_RUSSIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //SERBIAN
#if defined (IM_SERBIAN_SUPPORT)
    case GUIIM_M_TP_SERBIAN_ABC:
    case GUIIM_M_TP_SERBIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_SERBIAN_ABC_LOWER:
    case GUIIM_M_TP_SERBIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_SERBIAN_ABC:
    case GUIIM_M_TPQ_SERBIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_SERBIAN_ABC_LOWER:
    case GUIIM_M_TPQ_SERBIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //SESOTHO
#if defined (IM_SESOTHO_SUPPORT)
    case GUIIM_M_TP_SESOTHO_ABC:
    case GUIIM_M_TP_SESOTHO_ABC_LEAD_UPPER:
    case GUIIM_M_TP_SESOTHO_ABC_LOWER:
    case GUIIM_M_TP_SESOTHO_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_SESOTHO_ABC:
    case GUIIM_M_TPQ_SESOTHO_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_SESOTHO_ABC_LOWER:
    case GUIIM_M_TPQ_SESOTHO_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //SLOVAK
#if defined (IM_SLOVAK_SUPPORT)
    case GUIIM_M_TP_SLOVAK_ABC:
    case GUIIM_M_TP_SLOVAK_ABC_LEAD_UPPER:
    case GUIIM_M_TP_SLOVAK_ABC_LOWER:
    case GUIIM_M_TP_SLOVAK_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_SLOVAK_ABC:
    case GUIIM_M_TPQ_SLOVAK_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_SLOVAK_ABC_LOWER:
    case GUIIM_M_TPQ_SLOVAK_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //SLOVENIAN
#if defined (IM_SLOVENIAN_SUPPORT)
    case GUIIM_M_TP_SLOVENIAN_ABC:
    case GUIIM_M_TP_SLOVENIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_SLOVENIAN_ABC_LOWER:
    case GUIIM_M_TP_SLOVENIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_SLOVENIAN_ABC:
    case GUIIM_M_TPQ_SLOVENIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_SLOVENIAN_ABC_LOWER:
    case GUIIM_M_TPQ_SLOVENIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //SPANISH 西班牙
#if defined (IM_SPANISH_SUPPORT)
    case GUIIM_M_TP_SPANISH_ABC:
    case GUIIM_M_TP_SPANISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_SPANISH_ABC_LOWER:
    case GUIIM_M_TP_SPANISH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_SPANISH_ABC:
    case GUIIM_M_TPQ_SPANISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_SPANISH_ABC_LOWER:
    case GUIIM_M_TPQ_SPANISH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //SWAHILI
#if defined (IM_SWAHILI_SUPPORT)
    case GUIIM_M_TP_SWAHILI_ABC:
    case GUIIM_M_TP_SWAHILI_ABC_LEAD_UPPER:
    case GUIIM_M_TP_SWAHILI_ABC_LOWER:
    case GUIIM_M_TP_SWAHILI_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_SWAHILI_ABC:
    case GUIIM_M_TPQ_SWAHILI_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_SWAHILI_ABC_LOWER:
    case GUIIM_M_TPQ_SWAHILI_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //SWEDISH
#if defined (IM_SWEDISH_SUPPORT)
    case GUIIM_M_TP_SWEDISH_ABC:
    case GUIIM_M_TP_SWEDISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_SWEDISH_ABC_LOWER:
    case GUIIM_M_TP_SWEDISH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_SWEDISH_ABC:
    case GUIIM_M_TPQ_SWEDISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_SWEDISH_ABC_LOWER:
    case GUIIM_M_TPQ_SWEDISH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //TURKISH
#if defined (IM_TURKISH_SUPPORT)
    case GUIIM_M_TP_TURKISH_ABC:
    case GUIIM_M_TP_TURKISH_ABC_LEAD_UPPER:
    case GUIIM_M_TP_TURKISH_ABC_LOWER:
    case GUIIM_M_TP_TURKISH_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_TURKISH_ABC:
    case GUIIM_M_TPQ_TURKISH_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_TURKISH_ABC_LOWER:
    case GUIIM_M_TPQ_TURKISH_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //UKRAINIAN
#if defined (IM_UKRAINIAN_SUPPORT)
    case GUIIM_M_TP_UKRAINIAN_ABC:
    case GUIIM_M_TP_UKRAINIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TP_UKRAINIAN_ABC_LOWER:
    case GUIIM_M_TP_UKRAINIAN_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_UKRAINIAN_ABC:
    case GUIIM_M_TPQ_UKRAINIAN_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_UKRAINIAN_ABC_LOWER:
    case GUIIM_M_TPQ_UKRAINIAN_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //越南
#ifdef IM_VIETNAMESE_SUPPORT
    case GUIIM_M_TP_VIETNAMESE_ABC:
    case GUIIM_M_TP_VIETNAMESE_ABC_LEAD_UPPER:
    case GUIIM_M_TP_VIETNAMESE_ABC_LOWER:
    case GUIIM_M_TP_VIETNAMESE_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_VIETNAMESE_ABC:
    case GUIIM_M_TPQ_VIETNAMESE_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_VIETNAMESE_ABC_LOWER:
    case GUIIM_M_TPQ_VIETNAMESE_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //XHOSA
#if defined (IM_XHOSA_SUPPORT)
    case GUIIM_M_TP_XHOSA_ABC:
    case GUIIM_M_TP_XHOSA_ABC_LEAD_UPPER:
    case GUIIM_M_TP_XHOSA_ABC_LOWER:
    case GUIIM_M_TP_XHOSA_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_XHOSA_ABC:
    case GUIIM_M_TPQ_XHOSA_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_XHOSA_ABC_LOWER:
    case GUIIM_M_TPQ_XHOSA_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //YORUBA
#if defined (IM_YORUBA_SUPPORT)
    case GUIIM_M_TP_YORUBA_ABC:
    case GUIIM_M_TP_YORUBA_ABC_LEAD_UPPER:
    case GUIIM_M_TP_YORUBA_ABC_LOWER:
    case GUIIM_M_TP_YORUBA_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_YORUBA_ABC:
    case GUIIM_M_TPQ_YORUBA_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_YORUBA_ABC_LOWER:
    case GUIIM_M_TPQ_YORUBA_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //ZULU
#if defined (IM_ZULU_SUPPORT)
    case GUIIM_M_TP_ZULU_ABC:
    case GUIIM_M_TP_ZULU_ABC_LEAD_UPPER:
    case GUIIM_M_TP_ZULU_ABC_LOWER:
    case GUIIM_M_TP_ZULU_ABC_UPPER:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_ZULU_ABC:
    case GUIIM_M_TPQ_ZULU_ABC_LEAD_UPPER:
    case GUIIM_M_TPQ_ZULU_ABC_LOWER:
    case GUIIM_M_TPQ_ZULU_ABC_UPPER:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //MYANMAR
#if defined (IM_MYANMAR_SUPPORT)
    case GUIIM_M_TP_MYANMAR_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_MYANMAR_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //AMHARIC
#if defined (IM_AMHARIC_SUPPORT) 
    case GUIIM_M_TP_AMHARIC_ABC:
        is_init = ThumbAmharicInit (data_ptr);
        break;

    case GUIIM_M_TPQ_AMHARIC_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //KHMER
#if defined (IM_KHMER_SUPPORT)
    case GUIIM_M_TP_KHMER_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_KHMER_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //LAO
#if defined (IM_LAO_SUPPORT)
    case GUIIM_M_TP_LAO_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_LAO_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

#if defined (IM_UYGHUR_SUPPORT)
    case GUIIM_M_TP_UYGHUR_ABC:
        is_init = ThumbMultiTapInit(data_ptr);
        break;

    case GUIIM_M_TPQ_UYGHUR_ABC:
        is_init = QwertMultiTapInit(data_ptr);
        break;
#endif

        //防止有些语言忘记配置导致死机问题，如果某些语言有自己的特殊需求，请在上面配置
    default:
        {
            GUIIM_METHOD_CFG_T *method_cfg_ptr = PNULL;

            method_cfg_ptr = MMITHEME_IMGetDefByMethod(method);
            if (method_cfg_ptr)
            {
                //9键
                if (method_cfg_ptr->input == GUIIM_INPUT_TYPE_TOUCHPEN)
                {
                    if (method_cfg_ptr->type == GUIIM_TYPE_MULTITAP)
                    {
                        is_init = ThumbMultiTapInit(data_ptr);
                    }
                }
                //26键
                else if (method_cfg_ptr->input == GUIIM_INPUT_TYPE_TP_QWERT)
                {
                    if (method_cfg_ptr->type == GUIIM_TYPE_MULTITAP)
                    {
                        is_init = QwertMultiTapInit(data_ptr);
                    }
                }
            }

            break;
        }
    }

    data_ptr->is_symbol_panel = FALSE;

    //输入法切换后，应该保存当前输入法语言，原因是，从输入法选择菜单中，涉及到
    //当前输入语言，需要及时更新语言
    if (MMIAPIIM_GetLanguage() != data_ptr->init_param.method_param_ptr->im_def_ptr->lang
            && GUIIM_LANG_NONE != data_ptr->init_param.method_param_ptr->im_def_ptr->lang)
    {
        MMIAPIIM_SetLanguage(data_ptr->init_param.method_param_ptr->im_def_ptr->lang);
    }

    //输入方式需要记录，防止直接切换语言，初始化的输入方式和前一次记录的输入方式不同
    //记录前一次使用的输入方式
    if(MMIAPIIM_GetLanguage() == GUIIM_LANG_ENGLISH 
        && (method != GUIIM_M_TP_123 && method != GUIIM_M_TPQ_123))
    {
        g_default_en_type = data_ptr->init_param.method_param_ptr->im_def_ptr->type;
    }
    else if(MMIAPIIM_GetLanguage() != GUIIM_LANG_NONE
        && (method != GUIIM_M_TP_123 && method != GUIIM_M_TPQ_123))
    {
        g_default_ch_type = data_ptr->init_param.method_param_ptr->im_def_ptr->type;
    }

    if (data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TOUCHPEN)
    {
        g_default_input_type = data_ptr->init_param.method_param_ptr->im_def_ptr->input;
    }

    //细节改进
    if (
		(data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TP_QWERT
            || data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TOUCHPEN)
		&& (data_ptr->init_param.method_param_ptr->im_def_ptr->type == GUIIM_TYPE_MULTITAP)
		)
    {
        PanelSlideCueCloseByTimer (data_ptr);
    }

    //显示问题修改，在这里修改，尽量少修改代码
    MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr); 
    
    SCI_TRACE_LOW("[MMIIM] TP SetMethod: is_init=%d", is_init);    

    return is_init;
}


#else
//define a fake api list...
GUIIM_KERNEL_APIS_T const g_guiim_touch_sprd_apis = {0};
#endif




/*Edit by script, ignore 4 case. Thu Apr 26 19:00:56 2012*/ //IGNORE9527
