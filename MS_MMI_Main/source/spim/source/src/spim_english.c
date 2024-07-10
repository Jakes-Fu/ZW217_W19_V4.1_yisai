/*==============================================================================
File Name: spim_english.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 英文的数据文件
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/

#include "spim_spells_internal.h"
#include "spim_spells9k_internal.h"

/*lint -e64*/

//英文的字母表
static SPIM_UNICODE_T const alphabet[] =
{
#include "..\\data\\spells\\_english_alphabet.src"
};

//英文的词汇表
static SPIM_INT8 const *vocabulary[] =
{
#include "..\\data\\spells\\_english_vocabulary.src"
};

//每个index上字母所对应的数字键
//这个是数据，要算好的
static SPIM_INT8 const *keymap =
#include "..\\data\\spells\\_english_key_map.src"
    ;

/*lint +e64*/

//init key
/*==============================================================================
Function Name: SPIM_SPELLS_InitEnglish
Description: 初始化实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 每种语言特别的初始化程序
==============================================================================*/
SPIM_HANDLE_T SPIM_SPELLS_InitEnglish(SPIM_INIT_PARAMETER_T const *para_ptr)
{
    SPIM_SPELLS_INFO_T *info_ptr = NULL;

    if (NULL == para_ptr)
    {
        return NULL;
    }

    info_ptr = SPIM_MALLOC(sizeof(SPIM_SPELLS_INFO_T));

    if (NULL == info_ptr)
    {
        return NULL;
    }

    SPIM_MEMSET(info_ptr, 0, sizeof(SPIM_SPELLS_INFO_T));

    info_ptr->init_para = *para_ptr;

    //字母表和词汇表设定
    //暂时只做英语，等需要的时候再扩展
    info_ptr->alphabet_ptr = alphabet;
    info_ptr->alphabet_size = SPIM_ARR_SIZE(alphabet);
    info_ptr->vocabulary_ptr = vocabulary;
    info_ptr->vocabulary_size = SPIM_ARR_SIZE(vocabulary);


    return info_ptr;
}


//init key
/*==============================================================================
Function Name: SPIM_SPELLS9K_InitEnglish
Description: 初始化实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 每种语言特别的初始化程序
==============================================================================*/
SPIM_HANDLE_T SPIM_SPELLS9K_InitEnglish(SPIM_INIT_PARAMETER_T const *para_ptr)
{
    SPIM_SPELLS9K_INFO_T *info_ptr = NULL;

    if (NULL == para_ptr)
    {
        return NULL;
    }

    info_ptr = SPIM_MALLOC(sizeof(SPIM_SPELLS9K_INFO_T));

    if (NULL == info_ptr)
    {
        return NULL;
    }

    SPIM_MEMSET(info_ptr, 0, sizeof(SPIM_SPELLS9K_INFO_T));

    info_ptr->init_para = *para_ptr;

    //字母表和词汇表设定
    //暂时只做英语，等需要的时候再扩展
    info_ptr->alphabet_ptr = alphabet;
    info_ptr->alphabet_size = SPIM_ARR_SIZE(alphabet);
    info_ptr->vocabulary_ptr = vocabulary;
    info_ptr->vocabulary_size = SPIM_ARR_SIZE(vocabulary);
    info_ptr->keymap_ptr = keymap;


    return info_ptr;
}




