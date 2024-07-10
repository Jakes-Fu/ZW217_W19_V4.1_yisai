
/*****************************************************************************
** File Name:      mmiim_sp_ml_data.c                                        *
** Author:         hua.fang                                                  *
** Date:           07/22/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2010        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmiim_sp_ml_data.h"


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*==============================================================================
Description: 输入法数据的初始化
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_Init(
    MMIIM_SP_ML_DATA_T      *ml_data_ptr       // [in]
)
{
    //SCI_ASSERT(PNULL != ml_data_ptr); /*assert verified*/
    if (PNULL == ml_data_ptr)
    {
        return FALSE;
    }
    
    SCI_MEMSET(ml_data_ptr, 0, sizeof(MMIIM_SP_ML_DATA_T));

    return TRUE;
}

/*==============================================================================
Description: 销毁输入法数据
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_Term(
    MMIIM_SP_ML_DATA_T      *ml_data_ptr    // [in]
)
{
    if (PNULL == ml_data_ptr)
    {
        return FALSE;
    }
    
    if (PNULL != ml_data_ptr->custom_kb_ptr)
    {
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
        if (ml_data_ptr->is_26key)
        {
            MMIIM_SP_ML26KEY_FreeCustomKb(ml_data_ptr->custom_kb_ptr);
        }
        else
        {
            MMIIM_SP_ML9KEY_FreeCustomKb(ml_data_ptr->custom_kb_ptr);
        }
#else
            MMIIM_SP_ML9KEY_FreeCustomKb(ml_data_ptr->custom_kb_ptr);
#endif

        ml_data_ptr->custom_kb_ptr = PNULL;
    }
    
    return TRUE;
}

/*==============================================================================
Description: Set Method
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_SetMethod(
    MMIIM_SP_ML_DATA_T      *ml_data_ptr,       // [in]
    GUIIM_METHOD_T           method
)
{
    BOOLEAN is_26key = FALSE;
    void const *key_table_ptr = PNULL;
    GUIIM_METHOD_CFG_T *method_cfg_ptr = PNULL;

    //SCI_ASSERT(PNULL != ml_data_ptr); /*assert verified*/
    if (PNULL == ml_data_ptr)
    {
        return FALSE;
    }
    
    // get method config
    method_cfg_ptr = MMITHEME_IMGetDefByMethod(method);
    if (PNULL == method_cfg_ptr)
    {
        return FALSE;
    }
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
    is_26key = (GUIIM_INPUT_TYPE_QWERT == method_cfg_ptr->input
             || GUIIM_INPUT_TYPE_TP_QWERT == method_cfg_ptr->input);

    if (is_26key)
    {
        key_table_ptr = MMIIM_SP_ML26KEY_GetKeyTable(method);
    }
    else
    {
        key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable(method);
    }
#else
        key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable(method);
#endif

    // check whether method is right. if no, do nothing, just return FALSE.
    if (PNULL != key_table_ptr)
    {
        ml_data_ptr->method_cfg_ptr = method_cfg_ptr;
        ml_data_ptr->is_26key = is_26key;
        ml_data_ptr->key_table_ptr = key_table_ptr;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*==============================================================================
Description: 获取对应按键下字母的个数
Author: hua.fang
Global resource dependence:
Note:
==============================================================================*/
PUBLIC uint16 MMIIM_SP_ML_GetCount(
    MMIIM_SP_ML_DATA_T       *ml_data_ptr,   // [in]
    MMIIM_SP_KEY_INDEX_T     key_index       // [in]
)
{
    uint16 char_count = 0;
    void const *key_table_ptr = PNULL;

    if (PNULL == ml_data_ptr)
    {
        return 0;
    }

    if (PNULL != ml_data_ptr->custom_kb_ptr)
    {
        key_table_ptr = ml_data_ptr->custom_kb_ptr;
    }
    else
    {
        key_table_ptr = ml_data_ptr->key_table_ptr;
    }

#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
    if (ml_data_ptr->is_26key)
    {
        char_count = MMIIM_SP_ML26KEY_GetChrCnt(key_table_ptr, key_index);
    }
    else
    {
        char_count = MMIIM_SP_ML9KEY_GetChrCnt(key_table_ptr, key_index);
    }
#else
        char_count = MMIIM_SP_ML9KEY_GetChrCnt(key_table_ptr, key_index);
#endif

    return char_count;
}

/*==============================================================================
Description: 根据字符得到对应的键值
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_GetKeyIndexByChar(
                                             MMIIM_SP_ML_DATA_T *ml_data_ptr,   // [in]
                                             wchar              key_char,       // [in]
                                             MMIIM_SP_KEY_INDEX_T*  key_index_ptr   // [out]
                                             )
{
    BOOLEAN result = TRUE;
    void const *key_table_ptr = PNULL;

    if (PNULL == ml_data_ptr)
    {
        return FALSE;
    }

    if (PNULL != ml_data_ptr->custom_kb_ptr)
    {
        key_table_ptr = ml_data_ptr->custom_kb_ptr;
    }
    else
    {
        key_table_ptr = ml_data_ptr->key_table_ptr;
    }
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
    if (ml_data_ptr->is_26key)
    {
        result = MMIIM_SP_ML26KEY_GetKeyIndexByChar(key_table_ptr, key_char, key_index_ptr);
    }
    else
    {
        result = MMIIM_SP_ML9KEY_GetKeyIndexByChar(key_table_ptr, key_char, key_index_ptr);
    }
#else
        result = MMIIM_SP_ML9KEY_GetKeyIndexByChar(key_table_ptr, key_char, key_index_ptr);
#endif

    return result;
}

/*==============================================================================
Description: 获取对应按键下指定索引的字母
Author: hua.fang
Global resource dependence: g_26key_table_common
Note: char_index从1开始计数
==============================================================================*/
PUBLIC wchar MMIIM_SP_ML_GetChar(
    MMIIM_SP_ML_DATA_T         *ml_data_ptr,   // [in]
    MMIIM_SP_KEY_INDEX_T       key_index,      // [in]
    uint16                     char_index      // [in]
)
{
    wchar char_data = 0;
    void const *key_table_ptr = PNULL;

    if (PNULL == ml_data_ptr)
    {
        return 0;
    }

    if (PNULL != ml_data_ptr->custom_kb_ptr)
    {
        key_table_ptr = ml_data_ptr->custom_kb_ptr;
    }
    else
    {
        key_table_ptr = ml_data_ptr->key_table_ptr;
    }
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
    if (ml_data_ptr->is_26key)
    {
        char_data = MMIIM_SP_ML26KEY_GetChr(key_table_ptr, key_index, char_index);
    }
    else
    {
        char_data = MMIIM_SP_ML9KEY_GetChr(key_table_ptr, key_index, char_index);
    }
#else
        char_data = MMIIM_SP_ML9KEY_GetChr(key_table_ptr, key_index, char_index);
#endif

    return char_data;
}

/*==============================================================================
Description: 检查字母表
Author: hua.fang
Global resource dependence:
Note:
  1 只针对9键的
  2 有些语言，某个字母后面只能跟特定的字母，这时就需要切换字母表。现通过这个
  方法检查字母表
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_CheckKeyTable(
    MMIIM_SP_ML_DATA_T     *ml_data_ptr,   // [in]
    CHECK_TABLE_FUNC_T     *checking_func_pptr
)
{
    if (ml_data_ptr->is_26key)
    {
        return FALSE;
    }

    *checking_func_pptr = MMIIM_SP_ML9KEY_GetCheckingFunc(ml_data_ptr->method_cfg_ptr->method);

    return (PNULL != *checking_func_pptr);
}


/*==============================================================================
Description: MMIIM_SP_ML_SetCustomKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_SetCustomKey(
    MMIIM_SP_ML_DATA_T       *ml_data_ptr,
    GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr
)
{
    BOOLEAN result = FALSE;
    // uint32 key_table_size = 0;
    GUIIM_TYPE_T type;


    if (PNULL == ml_data_ptr || PNULL == key_info_ptr)
    {
        return FALSE;
    }

    if (PNULL == ml_data_ptr->custom_kb_ptr)
    {
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
        if (ml_data_ptr->is_26key)
        {
            MMIIM_SP_ML26KEY_AllocCustomKb(ml_data_ptr->key_table_ptr, &ml_data_ptr->custom_kb_ptr);
        }
        else
        {
            MMIIM_SP_ML9KEY_AllocCustomKb(ml_data_ptr->key_table_ptr, &ml_data_ptr->custom_kb_ptr);
        }
#else
            MMIIM_SP_ML9KEY_AllocCustomKb(ml_data_ptr->key_table_ptr, &ml_data_ptr->custom_kb_ptr);
#endif
    }

    type = ml_data_ptr->method_cfg_ptr->type;

    //纠正abc，english等，此处只需要abc
    if (GUIIM_TYPE_ABC == type)
    {
        type = GUIIM_TYPE_MULTITAP;
    }
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
    
    if (ml_data_ptr->is_26key)
    {
        result = MMIIM_SP_ML26KEY_CustomKb(type, 
                                           key_info_ptr,
                                           ml_data_ptr->custom_kb_ptr);
    }
    else
    {
        result = MMIIM_SP_ML9KEY_CustomKb(type, 
                                          key_info_ptr,
                                          ml_data_ptr->custom_kb_ptr);
    }
#else
        result = MMIIM_SP_ML9KEY_CustomKb(type, 
                                          key_info_ptr,
                                          ml_data_ptr->custom_kb_ptr);
#endif
    return TRUE;
}

