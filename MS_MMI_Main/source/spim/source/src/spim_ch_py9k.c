/*==============================================================================
File Name: spim_ch_py9k.c
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 9��ƴ�������鷳����
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/18   Lianxiang.Zhou  Create.
==============================================================================*/
/*
�����ִʵĳ���ֿ�
����Ϊ���ֻ�򻯺ܶࡣ�����п��ܻ᲻֧�ִʵġ�

����ǰ��ҳ�����
һ��������ǰǰ���ҳ����������ǳ����֡��ң�����ĵ��ֲ����Ǻܿ��
�ʶ�����ȡֻ����ǰn��ҳ���ݵķ�ʽ�����ң�ÿҳ�����ݲ��ᳬ��9��

*/
#include "spim_general.h"

#include "spim_ch_py9k.h"

#include "spim_ch.h"
#include "spim_ch_py9k_internal.h"


#define SPIM_CH_PYW_PK_GET_IDX_LEN(flg) (flg & SPIM_CH_PYW_9K_IDX_LEN_MASK)
#define SPIM_CH_PYW_PK_GET_IDX_TYPE(flg) (flg & SPIM_CH_PYW_9K_IDX_NUM_FLAG_MASK)



/*==============================================================================
Function Name: Init
Description: Init
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_PY9K_INFO_T * Init(SPIM_TYPE_E spim_type);

/*==============================================================================
Function Name: Term
Description: Term
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void Term(SPIM_PY9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: InputChar
Description: InputChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T InputChar(SPIM_PY9K_INFO_T *info_ptr, SPIM_INT8 c);

/*==============================================================================
Function Name: SearchWord
Description: SearchWord
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 SearchWord(
    SPIM_PY9K_INFO_T *info_ptr,
    GUIME_CH_PYW_CAND_9K_T *begin_idx_ptr,
    SPIM_UINT16 cand_win_width
);

/*==============================================================================
Function Name: SearchChar
Description: SearchChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 SearchChar(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_PY_DICT_IDX_T chr_begin_idx,
    SPIM_UINT16 cand_win_width
);

/*==============================================================================
Function Name: IsNumFullMatchChar
Description: IsNumFullMatchChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 IsNumFullMatchChar(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_INT8 const * num_str,
    SPIM_UNICODE_T uni,
    SPIM_INT32 *match_len
);

/*==============================================================================
Function Name: IsNumPartMatchChar
Description: ����Ƿ�ǰ�����ִ����ֵ�ȫƴ����ǰ������ȫƥ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 IsNumPartMatchChar(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_INT8 const * num_str,
    SPIM_UNICODE_T uni,
    SPIM_INT32 *match_len_ptr
);

/*==============================================================================
Function Name: IsNumHeadMatchChar
Description: IsNumHeadMatchChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 IsNumHeadMatchChar(
    SPIM_PY9K_INFO_T *info_ptr, 
    SPIM_INT8 const * num_str, 
    SPIM_UNICODE_T uni);

/*==============================================================================
Function Name: ResetPyCand
Description: ResetPyCand
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T ResetPyCand(SPIM_PY9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: ReOutput
Description: ReOutput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T ReOutput(
    SPIM_PY9K_INFO_T *info_ptr,
    GUIME_CH_PYW_CAND_9K_T *first_idx_ptr
);

/*==============================================================================
Function Name: RefreshBuffer
Description: RefreshBuffer
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void RefreshBuffer(
    SPIM_PY9K_INFO_T *info_ptr,
    GUIME_CH_PYW_CAND_9K_T *buf_idx_ptr
);

/*==============================================================================
Function Name: AppendChar
Description: AppendChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T AppendChar(SPIM_PY9K_INFO_T *info_ptr, SPIM_INT8 c);

/*==============================================================================
Function Name: RemoveChar
Description: RemoveChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveChar(SPIM_PY9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: RemoveAll
Description: RemoveAll
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_PY9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: SelectIndex
Description: SelectIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T SelectIndex(SPIM_PY9K_INFO_T *info_ptr, SPIM_UINT8 idx);

/*==============================================================================
Function Name: SelectPinyin
Description: SelectPinyin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T SelectPinyin(SPIM_PY9K_INFO_T *info_ptr, SPIM_UINT8 idx);

/*==============================================================================
Function Name: PreviousPage
Description: PreviousPage
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_PY9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: NextPage
Description: NextPage
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_PY9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: GenContent
Description: GenContent
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void GenContent(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_CONTENT_T * content_ptr
);

/*==============================================================================
Function Name: GenDefaultPystr
Description: GenDefaultPystr
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void GenDefaultPystr(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_INT8 const *p_num,
    SPIM_UNICODE_T *py_str,
    SPIM_UINT32 *str_len
);

/*==============================================================================
Function Name: IsPyValid
Description: IsPyValid
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_BOOLEAN IsPyValid(SPIM_PY9K_INFO_T *info_ptr, SPIM_INT16 const *py_cnt, SPIM_UINT32 idx);

/*==============================================================================
Description: check whether we will receive this key
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_BOOLEAN IsKeyValid(
    SPIM_PY9K_INFO_T *info_ptr, // [in]
    SPIM_INT8 key               // [in]
);





/*==============================================================================
Function Name: Init
Description: ��ʼ��ʵ������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_PY9K_INFO_T * Init(SPIM_TYPE_E spim_type)
{
    SPIM_PY9K_INFO_T * info_ptr = NULL;

    info_ptr = malloc(sizeof(SPIM_PY9K_INFO_T));

    if (NULL == info_ptr)
    {
        return NULL;
    }

    SPIM_MEMSET(info_ptr, 0, sizeof(SPIM_PY9K_INFO_T));

    //Ĭ�ϼ�������
    switch (spim_type)
    {
#ifdef IM_TRAD_CHINESE_SUPPORT
    case SPIM_TYPE_ZHUYIN_9KEY:
        info_ptr->dict_cfg = spim_ch_zy_dict_cfg;
        info_ptr->dict_size = spim_ch_zy_dict_cfg_size;

        info_ptr->py_cnt = spim_ch_zy_cnt_chs;
        info_ptr->ch_pronc_pptr = spim_ch_zy_list;
        info_ptr->ch_key_list_pptr = spim_ch_zy_list_9k;
        info_ptr->ch_pronc_size = spim_ch_zy_list_9k_size;

        info_ptr->key_name_ptr = spim_ch_zy_9key_name;
        info_ptr->key_name_size = spim_ch_zy_9key_name_size;

        info_ptr->ch_ic_pptr = spim_ch_zy_ic;
        info_ptr->ch_ic_size = spim_ch_zy_ic_size;

        info_ptr->ch_key_dict_cfg_ptr = spim_ch_zy_key_dict_cfg;
        info_ptr->ch_dict_cfg_size = spim_ch_zy_key_dict_cfg_size;

        info_ptr->separator_char = 2;
        break;

    case SPIM_TYPE_ZHUYIN_26KEY:
        info_ptr->dict_cfg = spim_ch_zy_dict_cfg;
        info_ptr->dict_size = spim_ch_zy_dict_cfg_size;

        info_ptr->py_cnt = spim_ch_zy_cnt_chs;
        info_ptr->ch_pronc_pptr = spim_ch_zy_list;
        info_ptr->ch_key_list_pptr = spim_ch_zy_list_26k;
        info_ptr->ch_pronc_size = spim_ch_zy_list_26k_size;

        info_ptr->key_name_ptr = spim_ch_zy_26key_name;
        info_ptr->key_name_size = spim_ch_zy_26key_name_size;

        info_ptr->ch_ic_pptr = spim_ch_zy_ic;
        info_ptr->ch_ic_size = spim_ch_zy_ic_size;

        info_ptr->ch_key_dict_cfg_ptr = spim_ch_zy_key_dict_cfg;
        info_ptr->ch_dict_cfg_size = spim_ch_zy_key_dict_cfg_size;

        info_ptr->separator_char = 2;
        break;
#endif

    case SPIM_TYPE_PINYIN_9KEY:
    default:
        info_ptr->dict_cfg = spim_ch_dict_cfg;
        info_ptr->dict_size = spim_ch_dict_cfg_size;

        info_ptr->py_cnt = spim_ch_py_cnt_chs;
        info_ptr->ch_pronc_pptr = spim_ch_py_list;
        info_ptr->ch_key_list_pptr = spim_ch_py_list_9k;
        info_ptr->ch_pronc_size = spim_ch_py_list_9k_size;

        info_ptr->key_name_ptr = spim_ch_py_key_name;
        info_ptr->key_name_size = spim_ch_py_key_name_size;

        info_ptr->ch_ic_pptr = spim_ch_py_ic;
        info_ptr->ch_ic_size = spim_ch_py_ic_size;

        info_ptr->ch_key_dict_cfg_ptr = spim_ch_py_dict_cfg;
        info_ptr->ch_dict_cfg_size = spim_ch_py_dict_cfg_size;

        info_ptr->separator_char = '0';
        break;
    }

    return info_ptr;
}


/*==============================================================================
Function Name: Term
Description: �ͷ�ʵ������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void Term(SPIM_PY9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    free(info_ptr);

    return;
}


/*==============================================================================
Function Name: InputChar
Description: ��������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: input character one by one, it may be refused
==============================================================================*/
static SPIM_RESULT_T InputChar(SPIM_PY9K_INFO_T *info_ptr, SPIM_INT8 c)
{
    SPIM_ASSERT(NULL != info_ptr);

    //ֻ������������һ�볤����Ϊ����
    if (info_ptr->input.input_len >= SPIM_INPUT_LEN_MAX - 1)
    {
        return SPIM_ERROR;
    }

    if (!IsKeyValid(info_ptr, c))
    {
        return SPIM_ERROR;
    }

    if (info_ptr->separator_char == c)
    {
        //���������ǰһ���Ѿ��Ƿֶ�
        if (0 == info_ptr->input.input_len
            || info_ptr->separator_char == info_ptr->input.input[info_ptr->input.input_len - 1])
        {
            return SPIM_ERROR;
        }
    }

    //��������
    info_ptr->input.input[info_ptr->input.input_len++] = c;
    info_ptr->input.input[info_ptr->input.input_len] = '\0';

    return SPIM_NO_ERROR;
}



/*==============================================================================
Function Name: SearchWord
Description: ��ָ��������ʼ�����ʣ�ֱ�������������ѡ������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 SearchWord(
    SPIM_PY9K_INFO_T *info_ptr,
    GUIME_CH_PYW_CAND_9K_T *begin_idx_ptr,
    SPIM_UINT16 cand_win_width
)
{
    SPIM_PY_DICT_IDX_T index = 0;
    SPIM_INT16 word_len = 0;
    SPIM_UINT8 idx_from = 0; //ƴ���������
    SPIM_UINT8 idx_cnt = 0; //ƴ��������
    SPIM_PY_DICT_IDX_T word_size = 0;
    SPIM_UNICODE_T const *dict_data_ptr = NULL;
    SPIM_UNICODE_T const *word_ptr = NULL;
    SPIM_UINT8 char_i = 0;
    SPIM_INT32 is_idx_match = 0;
    SPIM_INT32 is_num_full_match = 0;
#if 0
    SPIM_INT32 is_num_head_match = 0;
#endif
    SPIM_UINT16 per_seg = 0;
    SPIM_UINT16 left_spc = 0;
    GUIME_CH_PYW_CAND_9K_T buf_idx = {0};
    SPIM_INT8 const * p_input = NULL;

    SPIM_ASSERT(NULL != info_ptr);
    SPIM_ASSERT(NULL != begin_idx_ptr);

    SPIM_MEMSET(&(info_ptr->test), 0, sizeof(SPIM_CH_PYW_OUTPUT_9K_T));

    word_len = begin_idx_ptr->page + 1;

    //������������
    if (word_len < 2)
    {
        return 0;
    }

    //�����ֵ���Ĵʵ���󳤶�
    if ((SPIM_UINT16)word_len > info_ptr->dict_size)
    {
        return 0;
    }

    word_size = (SPIM_PY_DICT_IDX_T)info_ptr->dict_cfg[word_len - 1].size;      // �ֵ��С
    dict_data_ptr = (SPIM_UNICODE_T *)info_ptr->dict_cfg[word_len - 1].data_ptr;// �ֵ�ָ��

    // ÿ���ʵĿ��
    per_seg = info_ptr->init_para.width_spc + (info_ptr->init_para.width_chr * word_len);

    //�����ʷֳ�������
    // 1 ȷ�������Ĳ��� - �ⲿ��ͬ26k
    // 2 ���ֲ��� - �ⲿ�ּ�������ĸ��ȫƴ���������

    //��ȷ��Ϊƴ������������ȷ��Ϊ���ֵ�����
    //��ȷ��Ϊ���ֵ���һ��������ʼ
    idx_from = info_ptr->input.confirm_idx_cnt;

    //���������ĸ���ΪΪȷ��Ϊ���ֵ�����ȷ��Ϊƴ��������
    idx_cnt = info_ptr->input.idx_cnt - info_ptr->input.confirm_idx_cnt;

    SPIM_ASSERT(idx_cnt >= 0);

    // �����ȷ��Ϊƴ���ĳ��ȴ��ڴʳ�����ֻҪ�Ҵʳ��Ϳ����ˣ�֮���û��Ҫ��
    if (idx_cnt > word_len)
    {
        idx_cnt = word_len;
    }

    index = begin_idx_ptr->idx;

    do
    {
        SPIM_INT32 all_match_len = 0;
        SPIM_INT32 match_len = 0;

        //��ʼ��
        while (index < word_size)
        {
            word_ptr = dict_data_ptr + index * word_len;

            //���ּ��

            //��ȷ��Ϊƴ���Ĳ���
            for (char_i = 0; char_i < idx_cnt; char_i++)
            {
                is_idx_match = IsPyIndexMatchChar(
                                   info_ptr->ch_pronc_pptr,
                                   info_ptr->input.idx[idx_from + char_i],
                                   info_ptr->ch_key_dict_cfg_ptr,
                                   *(word_ptr + char_i),
                                   (SPIM_UINT8)SPIM_GET_FLG_IC(info_ptr->input.idx_flag[idx_from + char_i])
                                   );

                if (!is_idx_match) break;
            }

            //��Ȼ�����ϣ����ٽ��бȽ�
            // �����ȷ��Ϊƴ�������ˣ������ֲ���������ʵ�ƴ�����Ǿ���ȫ�����ˣ�������һ��
            if (idx_cnt > 0 && !is_idx_match)
            {
                index++;
                continue;
            }

            //û�����ֲ��֣�ƥ��ɹ�
            // ���ȷ��Ϊƴ���ĳ�������鳤��һ�£����ߵ�����ֻ����ȫƥ��
            if (idx_cnt == word_len)
            {
                break;
            }

            //�������ֵĲ��ֵ�ƥ��
            //...

            //ȫƴ��
            is_num_full_match = 0;
            all_match_len = 0;

            // �Ѿ�ȷ��Ϊƴ�������Ƿ��ϴ�����֣����Դ���ʣ�������ֻҪ�Ӹ��ֺ��濪ʼ����
            for (char_i = idx_cnt; char_i < word_len; char_i++)
            {
                // ��λ����Ҫ��֤�İ���
                p_input  = info_ptr->input.input + info_ptr->input.idx_confirm_len + all_match_len;

                if (info_ptr->separator_char == *p_input)
                {
                    all_match_len += 1;
                    p_input++;
                }

                //���һ���ֿɲ���ƥ��
                if (char_i == word_len - 1)
                {
                    is_num_full_match = IsNumPartMatchChar(
                                info_ptr, p_input, *(word_ptr + char_i), &match_len);
                }
                else
                {
                    is_num_full_match = IsNumFullMatchChar(
                                info_ptr, p_input, *(word_ptr + char_i), &match_len);
                }


                if (!is_num_full_match) break;

                all_match_len += match_len;
            }

            //ȫƴ��ƥ�䣬�ɹ�
            if (is_num_full_match)
            {
                break;
            }
#if 0
            //��������ĸ��ƥ��ᵼ�³��ֹ�����֣��Ӷ�����ѡ��Ļ���
            //����ĸ
            is_num_head_match = 0;
            all_match_len = 0;

            for (char_i = idx_cnt; char_i < word_len; char_i++)
            {
                p_input = info_ptr->input.input + info_ptr->input.idx_confirm_len + all_match_len;

                if (info_ptr->separator_char == *p_input)
                {
                    all_match_len += 1;
                    p_input++;
                }

                is_num_head_match = IsNumHeadMatchChar(info_ptr, p_input,
                                                       *(word_ptr + char_i));

                if (!is_num_head_match) break;

                all_match_len++;
            }

            //����ĸƥ�䣬�ɹ�
            if (is_num_head_match)
            {
                break;
            }
#endif
            index++;
        }

        // �Ѿ��ұ��˸��ֵ�������д�
        if (index >= word_size)
        {
            info_ptr->test.is_searching = 0;
            break;
        }

        //������֮���ж����ȣ���Ϊ�ó����¿����Ѿ�û�з��ϵĴ���
        left_spc = cand_win_width - info_ptr->test.cand_win_width;

        if (per_seg > left_spc)
        {
            info_ptr->test.is_searching = 1;

            //����buffer��Ϣ
            buf_idx.dict = SPIM_DICT_IDX_SYS;
            buf_idx.page = word_len - 1;
            buf_idx.idx = index;
            RefreshBuffer(info_ptr, &buf_idx);

            break;
        }

        info_ptr->test.cand[info_ptr->test.cand_cnt].dict = SPIM_DICT_IDX_SYS;
        info_ptr->test.cand[info_ptr->test.cand_cnt].page = word_len - 1;
        info_ptr->test.cand[info_ptr->test.cand_cnt].idx = index;
        info_ptr->test.cand[info_ptr->test.cand_cnt].match_idx_count = idx_cnt;
        info_ptr->test.cand[info_ptr->test.cand_cnt].match_num_count = all_match_len;
        info_ptr->test.cand_cnt++;

        info_ptr->test.cand_win_width += per_seg;

        index++;
    }
    while (1 != info_ptr->test.is_searching);


    return info_ptr->test.cand_cnt;
}


/*==============================================================================
Function Name: SearchChar
Description: ��ָ��������ʼ�����֣�ֱ�������������ѡ������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 SearchChar(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_PY_DICT_IDX_T chr_begin_idx,
    SPIM_UINT16 cand_win_width
)
{
    SPIM_PY_DICT_IDX_T index = chr_begin_idx;
    SPIM_UNICODE_T const *char_set = (SPIM_UNICODE_T *)info_ptr->dict_cfg[0].data_ptr;  // �ֵ�
    SPIM_PY_DICT_IDX_T char_size = info_ptr->dict_cfg[0].size;                          // �ֵ����ֵĸ���
    SPIM_UINT16 per_seg = 0;
    SPIM_UINT16 left_spc = 0;
    GUIME_CH_PYW_CAND_9K_T buf_idx = {0};
    SPIM_INT32 is_match = 0;
    SPIM_INT32 is_idx = 0;

    SPIM_ASSERT(NULL != info_ptr);

    SPIM_MEMSET(&(info_ptr->test), 0, sizeof(SPIM_CH_PYW_OUTPUT_9K_T));

    per_seg = info_ptr->init_para.width_spc + info_ptr->init_para.width_chr;

    //�������δȷ��Ϊ���ֵĵ���ȷ�ϵ�ƴ��
    is_idx = (info_ptr->input.confirm_idx_cnt < info_ptr->input.idx_cnt);

    do
    {
        SPIM_INT32 match_len = 0;

        while (index < char_size)
        {
            if (is_idx)
            {
                // ���֮ǰ�Ѿ���ȷ��Ϊƴ���ģ��ǿ�������ǲ������ƴ�����
                is_match = IsPyIndexMatchChar(
                                info_ptr->ch_pronc_pptr,
                                info_ptr->input.idx[info_ptr->input.confirm_idx_cnt],
                                info_ptr->ch_key_dict_cfg_ptr,
                                char_set[index],
                                (SPIM_UINT8)SPIM_GET_FLG_IC(info_ptr->input.idx_flag[info_ptr->input.confirm_idx_cnt])
                                );
            }
            else
            {
                //����Ϊ1���ַ���������
                //���ȿ��Ƿ�ȫƥ��
                is_match = IsNumFullMatchChar(
                                info_ptr,
                                info_ptr->input.input + info_ptr->input.idx_confirm_len,
                                char_set[index], &match_len);

                //��һ�������Ƿ�����ĸƥ��
                if (!is_match)
                {
                    is_match = IsNumHeadMatchChar(
                                info_ptr,
                                info_ptr->input.input + info_ptr->input.idx_confirm_len, 
                                char_set[index]);

                    if (is_match) match_len = 1;
                }
            }

            if (is_match) break;

            index++;
        }

        if (index >= char_size)
        {
            info_ptr->test.is_searching = 0;
            break;
        }

        //������֮���ж�������ȷ����һҳ����Ϣ
        left_spc = cand_win_width - info_ptr->test.cand_win_width;

        if (per_seg > left_spc)
        {
            info_ptr->test.is_searching = 1;

            //����buffer��Ϣ
            buf_idx.dict = SPIM_DICT_IDX_SYS;
            buf_idx.page = 0;
            buf_idx.idx = index;
            RefreshBuffer(info_ptr, &buf_idx);

            break;
        }

        info_ptr->test.cand[info_ptr->test.cand_cnt].dict = SPIM_DICT_IDX_SYS;
        info_ptr->test.cand[info_ptr->test.cand_cnt].page = 0;
        info_ptr->test.cand[info_ptr->test.cand_cnt].idx = index;
        info_ptr->test.cand[info_ptr->test.cand_cnt].match_idx_count = (is_idx ? 1 : 0);
        info_ptr->test.cand[info_ptr->test.cand_cnt].match_num_count = (is_idx ? 0 : match_len);
        info_ptr->test.cand_cnt++;

        info_ptr->test.cand_win_width += per_seg;

        index += 1;
    }
    while (1 != info_ptr->test.is_searching);

    return info_ptr->test.cand_cnt;
}


/*==============================================================================
Function Name: IsNumFullMatchChar
Description: ����Ƿ�ǰ�����ִ����ֵ�ȫƴ��ƥ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 IsNumFullMatchChar(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_INT8 const * num_str,
    SPIM_UNICODE_T uni,
    SPIM_INT32 *match_len_ptr
)
{
    SPIM_PY_IDX_T chr_py = 0;
    SPIM_UINT16 mp_cnt = 0;
    SPIM_PY_IDX_T py_idx = 0;
    SPIM_PY_IDX_T const *mps_ptr = NULL;
    SPIM_UINT8 mp_i = 0;
    SPIM_INT32 is_match = 0;

    SPIM_ASSERT(NULL != num_str);
    SPIM_ASSERT(NULL != match_len_ptr);
    SPIM_ASSERT(NULL != info_ptr);

    *match_len_ptr = 0;

    chr_py = ((SPIM_PY_IDX_T*)(info_ptr->ch_key_dict_cfg_ptr[0].data_ptr))[uni - 0x4e00];
    mp_cnt = (SPIM_UINT16)SPIM_GET_MP_CNT(chr_py);
    py_idx = (SPIM_PY_IDX_T)SPIM_GET_PY_IDX(chr_py);

    if (1 == mp_cnt)
    {
        mps_ptr = &py_idx;
    }
    else
    {
        SPIM_ASSERT(py_idx < info_ptr->ch_key_dict_cfg_ptr[mp_cnt-1].size);

        mps_ptr = info_ptr->ch_key_dict_cfg_ptr[mp_cnt-1].data_ptr;
        mps_ptr += py_idx * mp_cnt;
    }

    //�Ƚ�ȫ����
    for (mp_i = 0; mp_i < mp_cnt; mp_i++)
    {
        SPIM_INT8 const *p_src = info_ptr->ch_key_list_pptr[mps_ptr[mp_i]];
        SPIM_INT8 const *p_dst = num_str;

        is_match = 1;

        while (*p_src)
        {
            if (*p_src != *p_dst)
            {
                is_match = 0;
                break;
            }

            p_src++;
            p_dst++;
        }

        if (is_match)
        {
            *match_len_ptr = p_dst - num_str;
            break;
        }
    }


    return (*match_len_ptr > 0);
}



/*==============================================================================
Function Name: IsNumPartMatchChar
Description: ����Ƿ�ǰ�����ִ����ֵ�ȫƴ����ǰ������ȫƥ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 IsNumPartMatchChar(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_INT8 const * num_str,
    SPIM_UNICODE_T uni,
    SPIM_INT32 *match_len_ptr
)
{
    SPIM_PY_IDX_T chr_py = 0;
    SPIM_UINT16 mp_cnt = 0;
    SPIM_PY_IDX_T py_idx = 0;
    SPIM_PY_IDX_T const *mps_ptr = NULL;
    SPIM_UINT8 mp_i = 0;
    SPIM_INT32 is_match = 0;

    SPIM_ASSERT(NULL != num_str);
    SPIM_ASSERT(NULL != match_len_ptr);
    SPIM_ASSERT(NULL != info_ptr);

    *match_len_ptr = 0;

    chr_py = ((SPIM_PY_IDX_T*)(info_ptr->ch_key_dict_cfg_ptr[0].data_ptr))[uni - 0x4e00];
    mp_cnt = (SPIM_UINT16)SPIM_GET_MP_CNT(chr_py);
    py_idx = (SPIM_PY_IDX_T)SPIM_GET_PY_IDX(chr_py);

    if (1 == mp_cnt)
    {
        mps_ptr = &py_idx;
    }
    else
    {
        SPIM_ASSERT(py_idx < info_ptr->ch_key_dict_cfg_ptr[mp_cnt-1].size);

        mps_ptr = info_ptr->ch_key_dict_cfg_ptr[mp_cnt-1].data_ptr;
        mps_ptr += py_idx * mp_cnt;
    }

    //�Ƚ�ȫ����
    for (mp_i = 0; mp_i < mp_cnt; mp_i++)
    {
        //�� full �ȣ�dst & src �Ĳ���
        SPIM_INT8 const *p_src = num_str;
        SPIM_INT8 const *p_dst = info_ptr->ch_key_list_pptr[mps_ptr[mp_i]];

        is_match = 1;

        while (*p_src)
        {
            if (*p_src != *p_dst)
            {
                is_match = 0;
                break;
            }

            p_src++;
            p_dst++;
        }

        if (is_match)
        {
            *match_len_ptr = p_src - num_str;
            break;
        }
    }


    return (*match_len_ptr > 0);
}

/*==============================================================================
Function Name: IsNumHeadMatchChar
Description: ��鵱ǰ�������Ƿ���ָ���ֵ�����ĸƥ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ƥ�䳤��һ����1
==============================================================================*/
static SPIM_INT32 IsNumHeadMatchChar(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_INT8 const * num_str, 
    SPIM_UNICODE_T uni)
{
    SPIM_PY_IDX_T chr_py = 0;
    SPIM_UINT16 mp_cnt = 0;
    SPIM_PY_IDX_T py_idx = 0;
    SPIM_PY_IDX_T const *mps_ptr = NULL;
    SPIM_UINT8 mp_i = 0;
    SPIM_INT32 is_match = 0;

    SPIM_ASSERT(NULL != num_str);
    SPIM_ASSERT(NULL != info_ptr);

    chr_py = ((SPIM_PY_IDX_T*)(info_ptr->ch_key_dict_cfg_ptr[0].data_ptr))[uni - 0x4e00];
    mp_cnt = (SPIM_UINT16)SPIM_GET_MP_CNT(chr_py);
    py_idx = (SPIM_PY_IDX_T)SPIM_GET_PY_IDX(chr_py);

    if (1 == mp_cnt)
    {
        mps_ptr = &py_idx;
    }
    else
    {
        SPIM_ASSERT(py_idx < info_ptr->ch_key_dict_cfg_ptr[mp_cnt-1].size);

        mps_ptr = info_ptr->ch_key_dict_cfg_ptr[mp_cnt-1].data_ptr;
        mps_ptr += py_idx * mp_cnt;
    }

    //�Ƚ�����ĸ
    is_match = 0;

    for (mp_i = 0; mp_i < mp_cnt; mp_i++)
    {
        SPIM_INT8 const *p_src = info_ptr->ch_key_list_pptr[mps_ptr[mp_i]];

        if (*p_src == *num_str)
        {
            is_match = 1;
            break;
        }
    }

    return is_match;
}


/*==============================================================================
Function Name: ResetPyCand
Description: ��������ƴ����ѡ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �Ὣ�������ִ���ƴ�����г���
==============================================================================*/
static SPIM_RESULT_T ResetPyCand(SPIM_PY9K_INFO_T *info_ptr)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    SPIM_INT8 const *p_num = NULL;
    SPIM_UINT32 py_i = 0;
    SPIM_INT8 const *p_py = NULL;
    SPIM_INT32 char_i = 0;

    SPIM_ASSERT(NULL != info_ptr);

    SPIM_ASSERT(info_ptr->input.idx_confirm_len <= info_ptr->input.input_len);

    info_ptr->py_cand.cnt = 0;

    if (info_ptr->input.idx_confirm_len == info_ptr->input.input_len)
    {
        return res;
    }

    // ��δȷ�ϵİ���
    p_num = info_ptr->input.input + info_ptr->input.idx_confirm_len;

    // �ӻ�δȷ�ϵİ������п�ʼ���������п������ֵ�ƴ�����ŵ�cand������
    for (py_i = 0; py_i < info_ptr->ch_pronc_size; py_i++)
    {
        if (!IsPyValid(info_ptr, info_ptr->py_cnt, py_i)) continue;

        //spim_ch_py_list_9k[py_i]
        p_py = info_ptr->ch_key_list_pptr[py_i];

        char_i = 0;

        while (p_py[char_i] != 0)
        {
            if (p_py[char_i] != p_num[char_i])
            {
                break;
            }

            char_i++;
        }

        //ƥ���ƴ��
        if (0 == p_py[char_i])
        {
            SPIM_ASSERT(info_ptr->py_cand.cnt < SPIM_CH_PY9K_COMBINE_MAX);
            info_ptr->py_cand.idx[info_ptr->py_cand.cnt] = py_i;
            info_ptr->py_cand.cnt++;
        }
    }

    SPIM_ASSERT(info_ptr->py_cand.cnt > 0);
    //���ܵ�ƴ��������������? 74264 �� 19 �ֿ���, ����?
    SPIM_ASSERT(info_ptr->py_cand.cnt <= SPIM_CH_PY9K_COMBINE_MAX);


    return res;
}


/*==============================================================================
Function Name: ReOutput
Description: �����������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T ReOutput(
    SPIM_PY9K_INFO_T *info_ptr,
    GUIME_CH_PYW_CAND_9K_T *first_idx_ptr
)
{
    SPIM_UINT8 left_cnt = 0;
    GUIME_CH_PYW_CAND_9K_T first_idx = {0};
    GUIME_CH_PYW_CAND_9K_T begin_idx = {0};
    SPIM_PY_DICT_IDX_T chr_begin_idx = 0;
    SPIM_INT32 test_cnt = 0;
    SPIM_UINT8 test_page = 0;
    SPIM_INT32 word_i = 0;
    SPIM_INT32 char_i = 0;
    SPIM_UINT16 per_seg = 0; //ÿ���ʼ��ϼ���ĳ���
    SPIM_UINT16 left_spc = 0; //ʣ���ѡ����ռ�

    SPIM_ASSERT(NULL != info_ptr);

    left_cnt = info_ptr->input.idx_cnt - info_ptr->input.confirm_idx_cnt;       // ��δ��ȷ�ϵ�ƴ������
    left_cnt += info_ptr->input.input_len - info_ptr->input.idx_confirm_len;    // ��δ��ȷ�ϵİ������ȣ��������һ��ƴ��������һ������

    if (0 == left_cnt)
    {
        return SPIM_NO_ERROR;
    }

    if (NULL == first_idx_ptr)
    {
        if (left_cnt > info_ptr->dict_size)
        {
            left_cnt = info_ptr->dict_size;
        }

        first_idx.dict = SPIM_DICT_IDX_SYS;
        first_idx.page = left_cnt - 1;
        first_idx.idx = 0;
    }
    else
    {
        first_idx = *first_idx_ptr;
    }

    test_page = first_idx.page;

    SPIM_ASSERT(0 == info_ptr->output.cand_cnt);

    //���ô������
    SPIM_ASSERT(0 == info_ptr->output.is_searching);

    begin_idx.dict = SPIM_DICT_IDX_SYS;

    //�������������ֵ�
    while (test_page > 0)
    {
        left_spc = info_ptr->init_para.width_win - info_ptr->output.cand_win_width;

        //ȷ���������
        begin_idx.page = test_page;
        begin_idx.idx = (test_page == first_idx.page) ? first_idx.idx : 0;

        //����
        test_cnt = SearchWord(info_ptr, &begin_idx, left_spc);

        if (test_cnt > 0)
        {
            //����������ŵ�output����
            for (word_i = 0; word_i < test_cnt; word_i++)
            {
                info_ptr->output.cand[info_ptr->output.cand_cnt] = info_ptr->test.cand[word_i];
                info_ptr->output.cand_cnt++;
            }

            info_ptr->output.cand_win_width += info_ptr->test.cand_win_width;
        }

        // ���ʶ�����ʣ�࣬�Ͳ�����ѡ���ˣ����û�����һҳ�ļ�ͷ��
        if (1 == info_ptr->test.is_searching)
        {
            info_ptr->output.is_searching = 1;
            goto RE_OUTPUT_SEARCH_END;
        }

        //��ǰһҳ
        test_page--;
    }


    //�����������
    SPIM_ASSERT(0 == info_ptr->output.is_searching);
    //ֻʣ��һ��������û�м�����
    SPIM_ASSERT(0 == test_page);

    per_seg = info_ptr->init_para.width_spc + info_ptr->init_para.width_chr;    // �ֿ�
    left_spc = info_ptr->init_para.width_win - info_ptr->output.cand_win_width; // ʣ��ռ�

    //�Ѿ��Ų���һ����ѡ��
    if (per_seg > left_spc)
    {
        info_ptr->output.is_searching = 1;
        goto RE_OUTPUT_SEARCH_END;
    }

    //ȷ������������㣬����ǴӴʿ�ʼ�����ģ�����㿪ʼ
    chr_begin_idx = (0 == first_idx.page) ? first_idx.idx : 0;

    //������
    test_cnt = SearchChar(info_ptr, chr_begin_idx, left_spc);

    if (test_cnt > 0)
    {
        //����������ŵ�output����
        for (char_i = 0; char_i < test_cnt; char_i++)
        {
            info_ptr->output.cand[info_ptr->output.cand_cnt] = info_ptr->test.cand[char_i];
            info_ptr->output.cand_cnt++;
        }

        info_ptr->output.cand_win_width += info_ptr->test.cand_win_width;

        info_ptr->output.is_searching = info_ptr->test.is_searching;
    }
    else
    {
        info_ptr->output.is_searching = 0;
    }

RE_OUTPUT_SEARCH_END:

    //�����������棬��ҳ������
    //��ҳ��Ϣ�ᱻ��ҳ����ҳ��ʱ���ظ����棬�ⲿ�ֿ������Խ���
    if (0 == info_ptr->buffer.cur_page && info_ptr->output.cand_cnt > 0)
    {
        info_ptr->buffer.first_idx[0] = info_ptr->output.cand[0];
    }

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: RefreshBuffer
Description: ���»���ҳ��Ϣ
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void RefreshBuffer(SPIM_PY9K_INFO_T *info_ptr, GUIME_CH_PYW_CAND_9K_T *buf_idx_ptr)
{
    SPIM_INT32 buf_cmp_res = 0;

    SPIM_ASSERT((info_ptr->buffer.cur_page < info_ptr->buffer.page_cnt)
        || (0 == info_ptr->buffer.cur_page && 0 == info_ptr->buffer.page_cnt));

    if (info_ptr->buffer.cur_page + 1 < info_ptr->buffer.page_cnt)
    {
        //У���Ѹ���ͷ������һ����
        buf_cmp_res = memcmp(&(info_ptr->buffer.first_idx[info_ptr->buffer.cur_page + 1]),
                             buf_idx_ptr, sizeof(GUIME_CH_PYW_CAND_9K_T));
        SPIM_ASSERT(0 == buf_cmp_res);
    }
    else
    {
        //���ӻ���ҳ
        info_ptr->buffer.first_idx[info_ptr->buffer.cur_page + 1] = *buf_idx_ptr;
        info_ptr->buffer.page_cnt = info_ptr->buffer.cur_page + 2;
    }

    return;
}


/*==============================================================================
Function Name: AppendChar
Description: ����һ����������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T AppendChar(SPIM_PY9K_INFO_T *info_ptr, SPIM_INT8 c)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;

    SPIM_ASSERT(NULL != info_ptr);

    res = InputChar(info_ptr, c);

    //����δ�б��ʱ仯
    if (SPIM_NO_ERROR != res)
    {
        return res;
    }

    //
    //���º�ѡ��������
    //
    ResetPyCand(info_ptr);


    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_9K_T));
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(SPIM_CH_PYW_BUFFER_9K_T));

    (void)ReOutput(info_ptr, NULL);

    return SPIM_NO_ERROR;
}



/*==============================================================================
Function Name: RemoveChar
Description: ���յ�ɾ�����Ĵ���
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
1 �����ȷ�����֣�����ȥ���ֵ�ȷ��
2 �����ȷ����ƴ��������ȥ��ƴ����ȷ��
3 ��û�У�ɾ��һ����ĸ
4 ���¼�������
==============================================================================*/
static SPIM_RESULT_T RemoveChar(SPIM_PY9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    //û������
    if (0 == info_ptr->input.input_len)
    {
        return SPIM_ERROR;
    }

    //����confirm�������confirm����
    if (info_ptr->input.confirm_cnt > 0)    // ɾ��ȷ��Ϊ���ֵ�
    {
        info_ptr->input.confirm_cnt -= 1;
        info_ptr->input.confirm_idx_cnt -= info_ptr->input.confirm[info_ptr->input.confirm_cnt].match_idx_count;

        //������һ��idx��ģ��ƥ��ģ���ȥ�����һ��idx
        if (SPIM_CH_PYW_PK_GET_IDX_TYPE(info_ptr->input.idx_flag[info_ptr->input.idx_cnt - 1]) > 0)
        {
            info_ptr->input.confirm_idx_cnt -= 1;
            info_ptr->input.idx_cnt -= 1;
            info_ptr->input.idx_confirm_len -= SPIM_CH_PYW_PK_GET_IDX_LEN(info_ptr->input.idx_flag[info_ptr->input.idx_cnt]);
        }
    }
    else if (info_ptr->input.idx_cnt > 0)   // ɾ��ȷ��Ϊƴ����
    {
        info_ptr->input.idx_cnt -= 1;
        info_ptr->input.idx_confirm_len -= SPIM_CH_PYW_PK_GET_IDX_LEN(info_ptr->input.idx_flag[info_ptr->input.idx_cnt]);
    }
    else                                    // ɾ�����ఴ��
    {
        info_ptr->input.input_len -= 1;
        info_ptr->input.input[info_ptr->input.input_len] = '\0';
    }

    //
    //���¼�������
    //
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    if (info_ptr->input.input_len > 0)
    {
        //���º�ѡƴ������
        ResetPyCand(info_ptr);

        //���º�ѡ��������
        ReOutput(info_ptr, NULL);
    }


    return SPIM_NO_ERROR;
}



/*==============================================================================
Function Name: RemoveAll
Description: ɾ�����е�����
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �ظ�����ʼ״̬
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_PY9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    //û������
    if (0 == info_ptr->input.input_len)
    {
        return SPIM_ERROR;
    }

    SPIM_MEMSET(&(info_ptr->input), 0, sizeof(SPIM_CH_PYW_INPUT_9K_T));
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_9K_T));
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(SPIM_CH_PYW_BUFFER_9K_T));

    return SPIM_NO_ERROR;
}




/*==============================================================================
Function Name: SelectIndex
Description: ����������ݵ�����ѡ����ѡ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �����ֲ���Ӧ�����
1 ����ȷ�ϵ��ֻ��ߴ�
2 ���ñ�ȷ�ϵ������ĸ���
3 ��������ƥ�䲿�֣�������ƥ�䲿�ֵ���һ�����������
4 ��������ʣ�µĲ���
==============================================================================*/
static SPIM_RESULT_T SelectIndex(SPIM_PY9K_INFO_T *info_ptr, SPIM_UINT8 idx)
{
    SPIM_ASSERT(NULL != info_ptr);

    if (idx >= info_ptr->output.cand_cnt)
    {
        return SPIM_PARA_ERROR;
    }

    // ���õ�ȷ��������
    info_ptr->input.confirm[info_ptr->input.confirm_cnt] = info_ptr->output.cand[idx];
    info_ptr->input.confirm_cnt += 1;

    //����index��Ϣ
    info_ptr->input.confirm_idx_cnt += info_ptr->output.cand[idx].match_idx_count;

    //����δȷ�ϵ���������
    // 1 ȷ�ϵ�������+1
    // 2 ��������������Ϊһ���������������У��������������
    if (info_ptr->output.cand[idx].match_num_count > 0)
    {
        info_ptr->input.confirm_idx_cnt += 1;
        info_ptr->input.idx[info_ptr->input.idx_cnt] = info_ptr->input.idx_confirm_len;
        info_ptr->input.idx_flag[info_ptr->input.idx_cnt] =
            (info_ptr->output.cand[idx].match_num_count & SPIM_CH_PYW_9K_IDX_LEN_MASK);
        info_ptr->input.idx_flag[info_ptr->input.idx_cnt] |= SPIM_CH_PYW_9K_IDX_NUM_FLAG_MASK;
        info_ptr->input.idx_cnt++;

        info_ptr->input.idx_confirm_len += info_ptr->output.cand[idx].match_num_count;
    }

    //
    //���º�ѡ��������
    //
    ResetPyCand(info_ptr);

    //�������
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    //����δȷ���ģ���ˢ������
    ReOutput(info_ptr, NULL);

    return SPIM_NO_ERROR;
}




/*==============================================================================
Function Name: SelectPinyin
Description: ����ƴ����ѡ������ݵ���������������ƴ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
1 ���ƴ���ĳ���
2 ����Ǵ��и������ţ���0�ģ���0��Ϊƴ���Ľ�β����ƴ������
3 ���õ�ǰƴ����һЩ���ԣ����糤�ȡ��Ƿ���ĸ��
4 ���¼���
==============================================================================*/
static SPIM_RESULT_T SelectPinyin(SPIM_PY9K_INFO_T *info_ptr, SPIM_UINT8 idx)
{
    SPIM_UINT32 py_len = 0;

    SPIM_ASSERT(NULL != info_ptr);

    if (idx >= info_ptr->py_cand.cnt)
    {
        return SPIM_PARA_ERROR;
    }

    //
    //ƴ����������
    //
    py_len = SPIM_WcsLen((SPIM_UNICODE_T *)info_ptr->ch_pronc_pptr[info_ptr->py_cand.idx[idx]]);

    if (info_ptr->separator_char == info_ptr->input.input[info_ptr->input.idx_confirm_len + py_len])
    {
        py_len += 1;
    }

    info_ptr->input.idx[info_ptr->input.idx_cnt] = info_ptr->py_cand.idx[idx];
    info_ptr->input.idx_flag[info_ptr->input.idx_cnt] = (py_len & SPIM_CH_PYW_9K_IDX_LEN_MASK);

    //��鵱ǰƴ���Ƿ���ĸ
    if (-1 != FindStringIndex(
                    info_ptr->ch_ic_pptr, 
                    info_ptr->ch_ic_size, 
                    info_ptr->ch_pronc_pptr[info_ptr->py_cand.idx[idx]]))
    {
        info_ptr->input.idx_flag[info_ptr->input.idx_cnt] |= SPIM_IDX_FLG_MASK_IC;
    }

    info_ptr->input.idx_confirm_len += py_len;
    info_ptr->input.idx_cnt += 1;

    //
    //���º�ѡ��������
    //
    ResetPyCand(info_ptr);

    //
    //���¼�������
    //
    //�������
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    //����δȷ���ģ���ˢ������
    ReOutput(info_ptr, NULL);

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: PreviousPage
Description: ǰһҳ
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ǰ��ҳ�����ݣ��������ʱ���Ѿ���������ʱ������
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_PY9K_INFO_T *info_ptr)
{
    GUIME_CH_PYW_CAND_9K_T *begin_idx_ptr = NULL;

    SPIM_ASSERT(NULL != info_ptr);

    //�ǵ�һҳ
    SPIM_ASSERT(info_ptr->buffer.cur_page > 0);

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_9K_T));

    info_ptr->buffer.cur_page -= 1;

    begin_idx_ptr = &(info_ptr->buffer.first_idx[info_ptr->buffer.cur_page]);

    (void)ReOutput(info_ptr, begin_idx_ptr);

    return SPIM_NO_ERROR;
}




/*==============================================================================
Function Name: NextPage
Description: ��һҳ
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ǰ��ҳ�����ݣ��������ʱ���Ѿ���������ʱ������
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_PY9K_INFO_T *info_ptr)
{
    GUIME_CH_PYW_CAND_9K_T *begin_idx_ptr = NULL;

    SPIM_ASSERT(NULL != info_ptr);

    if (info_ptr->buffer.cur_page == SPIM_IDX_BUF_PAGE_CNT - 1)
    {
        SPIM_ASSERT(SPIM_IDX_BUF_PAGE_CNT == info_ptr->buffer.page_cnt);

        //�ܷ�����������ҳ�Ķ���ǿ��
        return SPIM_ERROR;
    }

    //�����һҳ
    SPIM_ASSERT(info_ptr->buffer.cur_page + 1 < info_ptr->buffer.page_cnt);

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_9K_T));

    info_ptr->buffer.cur_page += 1;

    begin_idx_ptr = &(info_ptr->buffer.first_idx[info_ptr->buffer.cur_page]);

    (void)ReOutput(info_ptr, begin_idx_ptr);

    return SPIM_NO_ERROR;
}






/*==============================================================================
Function Name: GenContent
Description: ���������Ӧ�õ�����
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void GenContent(SPIM_PY9K_INFO_T *info_ptr,
                       SPIM_CONTENT_T * content_ptr
                      )
{
    SPIM_INT32 i = 0;
    SPIM_INT16 const *ch_pronc_ptr = NULL;
    SPIM_INT8 const *input_ptr = NULL;
    SPIM_UINT32 com_len = 0;
    SPIM_UNICODE_T const *pwc = NULL;
    SPIM_INT32 cand_len = 0;
    SPIM_UINT8 dict_page = 0;
    SPIM_UINT8 char_i = 0;
    SPIM_INT32 py_cand_len = 0;

    SPIM_ASSERT(NULL != info_ptr);
    SPIM_ASSERT(NULL != content_ptr);

    SPIM_MEMSET(content_ptr, 0, sizeof(SPIM_CONTENT_T));

    if (0 == info_ptr->input.input_len)
    {
        return;
    }

    content_ptr->has_data = 1;

    //û�м��������ȫ������������ϣ����ύ
    if (0 == info_ptr->output.cand_cnt)
    {
        //content_ptr->commit
        //content_ptr->commit_len

        content_ptr->commit_len = 0;

        //��ȷ�ϵĴ�
        for (i = 0; i < info_ptr->input.confirm_cnt; i++)
        {
            if (SPIM_DICT_IDX_SYS == info_ptr->input.confirm[i].dict)
            {
                dict_page = info_ptr->input.confirm[i].page;

                SPIM_ASSERT(dict_page < info_ptr->dict_size);

                pwc = (SPIM_UNICODE_T *)info_ptr->dict_cfg[dict_page].data_ptr;

                pwc += (info_ptr->input.confirm[i].idx * (dict_page + 1));

                for (char_i = 0; char_i < dict_page + 1; char_i++)
                {
                    content_ptr->commit[content_ptr->commit_len] = *pwc;
                    pwc++;
                    content_ptr->commit_len++;
                }
            }
        }

        //��ȷ�ϵ�����
        for (i = info_ptr->input.confirm_idx_cnt; i < info_ptr->input.idx_cnt; i++)
        {
            ch_pronc_ptr = info_ptr->ch_pronc_pptr[info_ptr->input.idx[i]];

            while (*ch_pronc_ptr)
            {
                content_ptr->commit[content_ptr->commit_len] = *ch_pronc_ptr;
                ch_pronc_ptr++;
                content_ptr->commit_len++;
            }
        }

        //ʣ������ֲ���
        //�ⲿ�ֿ���ѡ���趨Ϊ"����ܵ�ƴ����"���ɲ��ÿ�ѡ���ƴ����һ������
        input_ptr = info_ptr->input.input + info_ptr->input.idx_confirm_len;

        while (*input_ptr)
        {
            content_ptr->commit[content_ptr->commit_len] = *input_ptr;
            input_ptr++;
            content_ptr->commit_len++;
        }

        content_ptr->commit[content_ptr->commit_len] = '\0';

        SPIM_ASSERT(content_ptr->commit_len < SPIM_INPUT_LEN_MAX);
        
        SPIM_MEMSET(&(info_ptr->input), 0, sizeof(SPIM_CH_PYW_INPUT_9K_T));
    }
    else
    {
        //
        //content_ptr->pre_edit
        //
        for (i = 0; i < info_ptr->input.confirm_cnt; i++)
        {
            if (SPIM_DICT_IDX_SYS == info_ptr->input.confirm[i].dict)
            {
                dict_page = info_ptr->input.confirm[i].page;

                SPIM_ASSERT(dict_page < info_ptr->dict_size);

                pwc = (SPIM_UNICODE_T *)info_ptr->dict_cfg[dict_page].data_ptr;

                pwc += (info_ptr->input.confirm[i].idx * (dict_page + 1));

                for (char_i = 0; char_i < dict_page + 1; char_i++)
                {
                    content_ptr->pre_edit[com_len] = pwc[char_i];
                    com_len++;
                }
            }
        }

        content_ptr->pre_edit[com_len] = '\0';
        com_len++;

        for (i = info_ptr->input.confirm_idx_cnt; i < info_ptr->input.idx_cnt; i++)
        {
            ch_pronc_ptr = info_ptr->ch_pronc_pptr[info_ptr->input.idx[i]];

            while (*ch_pronc_ptr)
            {
                content_ptr->pre_edit[com_len] = *ch_pronc_ptr;
                ch_pronc_ptr++;
                com_len++;
            }

            content_ptr->pre_edit[com_len] = SPIM_PY_SEG_SYMBOL;
            com_len++;
        }

        //�Ե����һ���ָ��� '
        content_ptr->pre_edit[com_len - 1] = '\0';

        //��ǲ�ͬ����
        content_ptr->pre_edit[com_len++] = SPIM_COM_SEG_SYMBOL;

        //��input��δȷ�ϵ����ֲ��ֽ���
        //�ⲿ�ֿ���ѡ���趨Ϊ"����ܵ�ƴ����"���ɲ��ÿ�ѡ���ƴ����һ������
        input_ptr = info_ptr->input.input + info_ptr->input.idx_confirm_len;

        if (*input_ptr)
        {
            SPIM_UINT32 len = 0;
            GenDefaultPystr(info_ptr, input_ptr, content_ptr->pre_edit + com_len, &len);
            com_len += len;
        }

        SPIM_ASSERT(com_len <= SPIM_INPUT_LEN_MAX);

        //
        //content_ptr->candidate
        //
        for (i = 0; i < info_ptr->output.cand_cnt; i++)
        {
            if (SPIM_DICT_IDX_SYS == info_ptr->output.cand[i].dict)
            {
                dict_page = info_ptr->output.cand[i].page;

                SPIM_ASSERT(dict_page < info_ptr->dict_size);

                pwc = (SPIM_UNICODE_T *)info_ptr->dict_cfg[dict_page].data_ptr;

                pwc += (info_ptr->output.cand[i].idx * (dict_page + 1));

                for (char_i = 0; char_i < dict_page + 1; char_i++)
                {
                    content_ptr->candidate[cand_len] = pwc[char_i];
                    cand_len++;
                }

                content_ptr->candidate[cand_len] = '\0';
                cand_len++;
            }
        }

        content_ptr->cand_cnt = info_ptr->output.cand_cnt;

        SPIM_ASSERT(cand_len <= SPIM_INPUT_LEN_MAX);

        //
        //pinyin candidate
        //
        py_cand_len = 0;

        for (i = 0; i < info_ptr->py_cand.cnt; i++)
        {
            ch_pronc_ptr = info_ptr->ch_pronc_pptr[info_ptr->py_cand.idx[i]];

            while (*ch_pronc_ptr)
            {
                content_ptr->py_choice[py_cand_len] = *ch_pronc_ptr;
                ch_pronc_ptr++;
                py_cand_len++;
            }

            content_ptr->py_choice[py_cand_len] = '\0';
            py_cand_len++;
        }

        content_ptr->py_cnt = info_ptr->py_cand.cnt;

        SPIM_ASSERT(py_cand_len <= SPIM_PY_CHOICE_LEN_MAX);

        //
        //content_ptr->page_flag
        //test...
        if (info_ptr->buffer.cur_page > 0)
        {
            content_ptr->page_flag |= SPIM_PAGE_FLAG_PREVIOUS;
        }

        if (info_ptr->buffer.cur_page + 1 < info_ptr->buffer.page_cnt)
        {
            content_ptr->page_flag |= SPIM_PAGE_FLAG_NEXT;
        }
    }

    return;
}



/*==============================================================================
Function Name: GenDefaultPystr
Description: ����Ĭ�ϵ�ƴ��������ÿ��ƴ����ѡ�ֵĵ�һ�����
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
    1 ��ȡ��ǰ���ִ����׸���ѡ��
    2 �������֣��ٴλ�ȡ�׸���ѡ��
    3 �������ֶ���ƥ����
==============================================================================*/
static void GenDefaultPystr(
    SPIM_PY9K_INFO_T *info_ptr,
    SPIM_INT8 const *p_num,
    SPIM_UNICODE_T *py_str,
    SPIM_UINT32 *str_len
)
{
    SPIM_INT8 const *p = p_num;
    SPIM_INT8 py_len = 0;

    if (NULL == p_num || NULL == py_str || NULL == str_len)
    {
        return;
    }

    while (*p)
    {
        SPIM_UINT32 py_i = 0;
        SPIM_INT8 const *p_py = NULL;
        SPIM_INT16 const *pronc_ptr = NULL;
        SPIM_UINT32 ch_i = 0;
        SPIM_UINT32 str_i = 0;

        for (py_i = 0; py_i < info_ptr->ch_pronc_size; py_i++)
        {
            if (!IsPyValid(info_ptr, info_ptr->py_cnt, py_i)) continue;

            p_py = info_ptr->ch_key_list_pptr[py_i];
            ch_i = 0;

            while (p_py[ch_i] != 0)
            {
                if (p_py[ch_i] != p[ch_i]) break;

                ch_i++;
            }

            //ƥ���ƴ��
            if (0 == p_py[ch_i]) break;
        }

        //û��ƥ������ֻ�����Ƿָ���0
        if (py_i == info_ptr->ch_pronc_size)
        {
            SPIM_ASSERT(info_ptr->separator_char == *p);
            p += 1;
            continue;
        }

        pronc_ptr = info_ptr->ch_pronc_pptr[py_i];

        //��ƴ��д��ƴ����
        for (str_i = 0; str_i < ch_i; str_i++)
        {
            py_str[py_len++] = pronc_ptr[str_i];
        }

        //�������ϸ�������
        py_str[py_len++] = SPIM_PY_SEG_SYMBOL;

        //���ִ�����ƴ�����ĳ���
        p += ch_i;
    }

    //�����һ�����������޸ĳ�0
    if (py_len > 0)
    {
        py_len--;
        py_str[py_len] = '\0';
    }

    *str_len = py_len;

    return;
}



/*==============================================================================
Function Name: IsPyValid
Description: IsPyValid
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �����ƴ���¸��ַ�����û�к���, �Ҹ�ƴ������ĸ
==============================================================================*/
static SPIM_BOOLEAN IsPyValid(SPIM_PY9K_INFO_T *info_ptr, SPIM_INT16 const *py_cnt, SPIM_UINT32 idx)
{
    SPIM_INT32 ic_idx = 0;

    SPIM_ASSERT(NULL != info_ptr);

    if (idx >= info_ptr->ch_pronc_size || NULL == py_cnt)
    {
        return FALSE;
    }

    //�����ƴ���¸��ַ�����û�к���
    if (0 == py_cnt[idx])
    {
        ic_idx = FindStringIndex(
                        info_ptr->ch_ic_pptr,
                        info_ptr->ch_ic_size,
                        info_ptr->ch_pronc_pptr[idx]);

        //�ҷ���ĸ
        if (-1 == ic_idx)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*==============================================================================
Description: check whether we will receive this key
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_BOOLEAN IsKeyValid(
    SPIM_PY9K_INFO_T *info_ptr, // [in]
    SPIM_INT8 key               // [in]
)
{
    SPIM_UINT32 i = 0;
    SPIM_BOOLEAN is_key_valid = FALSE;

    SPIM_ASSERT(NULL != info_ptr);
    SPIM_ASSERT(PNULL != info_ptr->key_name_ptr);

    for (i = 0; i < info_ptr->key_name_size; i++)
    {
        if (key == info_ptr->key_name_ptr[i])
        {
            is_key_valid = TRUE;
            break;
        }
    }

    return is_key_valid;
}

/*==============================================================================
Function Name: SPIM_CH_PY9K_Init
Description: ��ʼ��ʵ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_HANDLE_T SPIM_CH_PY9K_Init(SPIM_INIT_PARAMETER_T const *para_ptr)
{
    SPIM_PY9K_INFO_T * info_ptr = NULL;

    if (NULL == para_ptr) return NULL;

    info_ptr = Init(para_ptr->type);

    if (NULL == info_ptr)
    {
        return NULL;
    }

    info_ptr->init_para = *para_ptr;

    //�ṩ�ĳ�������Ҫ�ܷ���һ����Ĵ�
    SPIM_ASSERT(para_ptr->width_win > para_ptr->width_chr * info_ptr->dict_size + para_ptr->width_spc);

    return (SPIM_HANDLE_T)info_ptr;
}

/*==============================================================================
Function Name: SPIM_CH_PY9K_term
Description: �ͷ�ʵ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void SPIM_CH_PY9K_term(SPIM_HANDLE_T * handle)
{
    SPIM_PY9K_INFO_T * info_ptr = (SPIM_PY9K_INFO_T *)handle;

    if (NULL != info_ptr)
    {
        Term(info_ptr);
    }

    return;
}

/*==============================================================================
Function Name: SPIM_CH_PY9K_Handle
Description: �¼�����
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_RESULT_T SPIM_CH_PY9K_Handle(
    SPIM_HANDLE_T * handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    SPIM_CONTENT_T * content_ptr
)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    SPIM_PY9K_INFO_T * info_ptr = (SPIM_PY9K_INFO_T *)handle;

    if (NULL == handle)
    {
        return SPIM_PARA_ERROR;
    }

    if (SPIM_EVENT_GET_CONTENT == event && NULL == content_ptr)
    {
        return SPIM_PARA_ERROR;
    }

    switch (event)
    {
    case SPIM_EVENT_CHAR_APPEND:

        if (NULL == para_ptr)
        {
            return SPIM_PARA_ERROR;
        }

        res = AppendChar(info_ptr, para_ptr->append);

        break;

    case SPIM_EVENT_CHAR_REMOVE:
        res = RemoveChar(info_ptr);
        break;

    case SPIM_EVENT_CHAR_REMOVE_ALL:
        res = RemoveAll(info_ptr);
        break;

    case SPIM_EVENT_CAND_SELECT:

        if (NULL == para_ptr)
        {
            return SPIM_PARA_ERROR;
        }

        res = SelectIndex(info_ptr, para_ptr->select);

        break;

    case SPIM_EVENT_PINYIN_SELECT:

        if (NULL == para_ptr)
        {
            return SPIM_PARA_ERROR;
        }

        res = SelectPinyin(info_ptr, para_ptr->select);

        break;

    case SPIM_EVENT_PAGE_PREVIOUS:
        res = PreviousPage(info_ptr);
        break;

    case SPIM_EVENT_PAGE_NEXT:
        res = NextPage(info_ptr);
        break;

    case SPIM_EVENT_GET_CONTENT:
        //do nothing
        break;

    default:
        res = SPIM_PARA_ERROR;
        break;
    }

    //����content
    if (SPIM_NO_ERROR == res && NULL != content_ptr)
    {
        GenContent(info_ptr, content_ptr);
    }

    return res;
}


