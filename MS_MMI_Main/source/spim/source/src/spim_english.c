/*==============================================================================
File Name: spim_english.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: Ӣ�ĵ������ļ�
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/

#include "spim_spells_internal.h"
#include "spim_spells9k_internal.h"

/*lint -e64*/

//Ӣ�ĵ���ĸ��
static SPIM_UNICODE_T const alphabet[] =
{
#include "..\\data\\spells\\_english_alphabet.src"
};

//Ӣ�ĵĴʻ��
static SPIM_INT8 const *vocabulary[] =
{
#include "..\\data\\spells\\_english_vocabulary.src"
};

//ÿ��index����ĸ����Ӧ�����ּ�
//��������ݣ�Ҫ��õ�
static SPIM_INT8 const *keymap =
#include "..\\data\\spells\\_english_key_map.src"
    ;

/*lint +e64*/

//init key
/*==============================================================================
Function Name: SPIM_SPELLS_InitEnglish
Description: ��ʼ��ʵ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ÿ�������ر�ĳ�ʼ������
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

    //��ĸ��ʹʻ���趨
    //��ʱֻ��Ӣ�����Ҫ��ʱ������չ
    info_ptr->alphabet_ptr = alphabet;
    info_ptr->alphabet_size = SPIM_ARR_SIZE(alphabet);
    info_ptr->vocabulary_ptr = vocabulary;
    info_ptr->vocabulary_size = SPIM_ARR_SIZE(vocabulary);


    return info_ptr;
}


//init key
/*==============================================================================
Function Name: SPIM_SPELLS9K_InitEnglish
Description: ��ʼ��ʵ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ÿ�������ر�ĳ�ʼ������
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

    //��ĸ��ʹʻ���趨
    //��ʱֻ��Ӣ�����Ҫ��ʱ������չ
    info_ptr->alphabet_ptr = alphabet;
    info_ptr->alphabet_size = SPIM_ARR_SIZE(alphabet);
    info_ptr->vocabulary_ptr = vocabulary;
    info_ptr->vocabulary_size = SPIM_ARR_SIZE(vocabulary);
    info_ptr->keymap_ptr = keymap;


    return info_ptr;
}




