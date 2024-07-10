/*==============================================================================
File Name: spim_spells9k.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: ƴд������Ĺ�������
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/

#include "spim_spells9k.h"
#include "spim_spells9k_internal.h"




/*==============================================================================
Function Name: CheckIndex
Description: CheckIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 CheckIndex(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_INT8 const *index);

/*==============================================================================
Function Name: SearchWord
Description: SearchWord
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_DICT_INDEX_T SearchWord(
    SPIM_SPELLS9K_INFO_T *info_ptr,
    SPIM_DICT_INDEX_T begin_idx
);
/*==============================================================================
Function Name: TestInput
Description: TestInput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void TestInput(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_UINT8 page);

/*==============================================================================
Function Name: ReOutput
Description: ReOutput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void ReOutput(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_UINT8 page);

/*==============================================================================
Function Name: AppendChar
Description: AppendChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T AppendChar(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_INT8 c);

/*==============================================================================
Function Name: RemoveChar
Description: RemoveChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveChar(SPIM_SPELLS9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: RemoveAll
Description: RemoveAll
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_SPELLS9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: SelectIndex
Description: SelectIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T SelectIndex(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_UINT8 idx);

/*==============================================================================
Function Name: PreviousPage
Description: PreviousPage
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_SPELLS9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: NextPage
Description: NextPage
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_SPELLS9K_INFO_T *info_ptr);

/*==============================================================================
Function Name: GenContent
Description: GenContent
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void GenContent(
    SPIM_SPELLS9K_INFO_T *info_ptr,
    SPIM_CONTENT_T * content_ptr
);

/*==============================================================================
Function Name: CommitInput
Description: CommitInput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T CommitInput(SPIM_SPELLS9K_INFO_T *info_ptr);

/*==============================================================================
Description: SetCapital
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
static SPIM_RESULT_T SetCapital(
    SPIM_SPELLS9K_INFO_T *info_ptr,
    SPIM_CAPITAL_MODE_T cap
);

/*==============================================================================
Function Name: GetKeyChars
Description: GetKeyChars
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_UINT32 GetKeyChars(
    SPIM_SPELLS9K_INFO_T *info_ptr,
    SPIM_INT8 c_num,
    SPIM_UNICODE_T *chars
);

/*==============================================================================
Description: �����ʸ�ԭ��ԭʼ״̬
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
static SPIM_UINT32 RecoverWord(
    SPIM_SPELLS9K_INFO_T const *info_ptr,
    SPIM_DICT_INDEX_T word_idx,
    SPIM_UNICODE_T *word_uni_ptr
);



/*==============================================================================
Function Name: CheckIndex
Description: �Ƚ���������ִ�������������������Ƿ�������(����)ƥ��
    �ڱȽϵ�ʱ�򣬽�������ͨ��key mapת�������ֺ�Ƚ�
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 0 - ƥ��
==============================================================================*/
static SPIM_INT32 CheckIndex(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_INT8 const *index)
{
    SPIM_INT8 const *key = NULL;
    SPIM_INT8 const *idx = NULL;
    SPIM_UINT32 len = 0;

    SPIM_ASSERT(NULL != info_ptr);
    SPIM_ASSERT(NULL != index);

    idx = index;
    key = info_ptr->input.input;
    len = info_ptr->input.input_len;

    while (len--)
    {
        if (0 == *idx) return 1;

        if (*key != info_ptr->keymap_ptr[*idx]) return 1;

        key++;
        idx++;
    }

    return 0;
}




/*==============================================================================
Function Name: SearchWord
Description: ��ָ��������ʼ������һ�����������ִ�ƥ��ĵ���
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: δ�ҵ���¼���򷵻�-1�����򣬷��ص�������
==============================================================================*/
static SPIM_DICT_INDEX_T SearchWord(
    SPIM_SPELLS9K_INFO_T *info_ptr,
    SPIM_DICT_INDEX_T begin_idx
)
{
    SPIM_DICT_INDEX_T cur_idx = 0;
    SPIM_INT32 cmp_res = 0;

    SPIM_ASSERT(NULL != info_ptr);

    if (begin_idx >= info_ptr->vocabulary_size)
    {
        return -1;
    }

    for (cur_idx = begin_idx; cur_idx < info_ptr->vocabulary_size; cur_idx++)
    {
        cmp_res = CheckIndex(info_ptr, info_ptr->vocabulary_ptr[cur_idx]);

        if (0 == cmp_res) break;
    }


    return (cur_idx < info_ptr->vocabulary_size ? cur_idx : -1);
}



/*==============================================================================
Function Name: TestInput
Description: ���ݵ�ǰ�����룬�����д�����������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ����ݲ������ݵĽ��������������ݣ������������������Ϊ�˱����������
==============================================================================*/
static void TestInput(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_UINT8 page)
{
    SPIM_DICT_INDEX_T next_idx = 0;
    SPIM_INT32 word_width = 0;

    SPIM_ASSERT(NULL != info_ptr);

    //��ҳ��������������ҳ����
    if (SPIM_IDX_BUF_PAGE_CNT == page)
    {
        next_idx = 0;
        SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    }
    else if (page < info_ptr->buffer.page_cnt)
    {
        next_idx = info_ptr->buffer.first_idx[page];
    }
    else
    {
        SPIM_ASSERT(0);
    }

    SPIM_ASSERT(next_idx < info_ptr->vocabulary_size);

    SPIM_MEMSET(&(info_ptr->test), 0, sizeof(info_ptr->test));

    //fill test search output
    while (info_ptr->test.cand_win_width < info_ptr->init_para.width_win)
    {
        next_idx = SearchWord(info_ptr, next_idx);

        if (-1 == next_idx) break;

        if (PNULL != info_ptr->init_para.string_width_f)
        {
            SPIM_UNICODE_T word_uni[SPIM_INPUT_LEN_MAX] = {0};
            SPIM_UINT32 word_len = 0;

            word_len = RecoverWord(info_ptr, next_idx, word_uni);

            word_width = info_ptr->init_para.string_width_f(word_uni, word_len);
        }
        else
        {
            word_width = strlen(info_ptr->vocabulary_ptr[next_idx]) * info_ptr->init_para.width_chr;
        }

        word_width += info_ptr->init_para.width_spc;

        //�Ų��£���������һҳ��Ϣ������
        if (word_width + info_ptr->test.cand_win_width > info_ptr->init_para.width_win)
        {
            info_ptr->test.is_searching = 1;

            //����buffer��Ϣ
            //�״�
            if (SPIM_IDX_BUF_PAGE_CNT == page)
            {
                info_ptr->buffer.first_idx[0] = info_ptr->test.cand[0];
                info_ptr->buffer.first_idx[1] = next_idx;
                info_ptr->buffer.page_cnt = 2;
            }
            //���һҳ
            else if (info_ptr->buffer.page_cnt == page + 1)
            {
                info_ptr->buffer.first_idx[info_ptr->buffer.page_cnt] = next_idx;
                info_ptr->buffer.page_cnt++;
            }

            break;
        }

        info_ptr->test.cand[info_ptr->test.cand_cnt] = next_idx;
        info_ptr->test.cand_cnt++;
        info_ptr->test.cand_win_width += word_width;

        next_idx++;
    }


    return;
}



/*==============================================================================
Function Name: ReOutput
Description: ���ݵ�ǰ��������������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void ReOutput(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_UINT8 page)
{
    SPIM_ASSERT(NULL != info_ptr);

    SPIM_ASSERT(0 == page || page < info_ptr->buffer.page_cnt);

    TestInput(info_ptr, page);

    SPIM_ASSERT(info_ptr->test.cand_cnt > 0);

    info_ptr->output = info_ptr->test;
    info_ptr->buffer.cur_page = page;

    return;
}





/*==============================================================================
Function Name: AppendChar
Description: ����һ���ַ���ͬʱ������������Ҹ��ݲ������ݽ�������������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �Ƿ����ݣ��򷵻�SPIM_ERROR
==============================================================================*/
static SPIM_RESULT_T AppendChar(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_INT8 c)
{
    SPIM_UNICODE_T keymap[SPIM_SPELLS9K_KEYMAP_CNT];
    SPIM_UINT32 keymap_cnt = 0;

    SPIM_ASSERT(NULL != info_ptr);

    if (info_ptr->input.input_len >= SPIM_INPUT_LEN_MAX - 1)
    {
        return SPIM_NO_ERROR;
    }

    keymap_cnt = GetKeyChars(info_ptr, c, keymap);

    if (0 == keymap_cnt)
    {
        return SPIM_ERROR;
    }

    //���Լ�������
    info_ptr->input.input[info_ptr->input.input_len] = c;
    info_ptr->input.input_len++;

    //��Ȼû�п�ѡ����
    if (info_ptr->input.no_cand > 0)
    {
        return SPIM_NO_ERROR;
    }

    //�����Ƿ��������
    TestInput(info_ptr, SPIM_IDX_BUF_PAGE_CNT);

    //�����룬�����ѡ������
    if (0 == info_ptr->test.cand_cnt)
    {
        info_ptr->input.no_cand = info_ptr->input.input_len;
        SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));
        SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));

        info_ptr->input.keymap_cnt = GetKeyChars(
                                         info_ptr,
                                         info_ptr->input.input[info_ptr->input.confirm_len],
                                         info_ptr->input.keymap);
    }
    //�����룬���������
    else
    {
        info_ptr->output = info_ptr->test;
        info_ptr->buffer.cur_page = 0;
    }

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: RemoveChar
Description: ɾ��һ���ַ����������ַ����������������������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveChar(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    //û������
    if (0 == info_ptr->input.input_len)
    {
        return SPIM_ERROR;
    }

    if (info_ptr->input.confirm_len > 0)
    {
        //ȥ�����һ��ȷ�ϵ���ĸ
        info_ptr->input.confirm_len--;
    }
    else
    {
        //ȥ�����һ������
        info_ptr->input.input_len--;
    }


    if (info_ptr->input.no_cand > info_ptr->input.input_len)
    {
        info_ptr->input.no_cand = 0;
    }

    //û��ɾ��
    if (info_ptr->input.input_len > 0)
    {
        //��ʱ�����no_cand�ĳ���Ϊ(0,input_len]������������
        if (0 == info_ptr->input.no_cand)
        {
            TestInput(info_ptr, SPIM_IDX_BUF_PAGE_CNT);
            SPIM_ASSERT(info_ptr->test.cand_cnt > 0);

            info_ptr->output = info_ptr->test;
            info_ptr->buffer.cur_page = 0;
        }
        else
        {
            info_ptr->input.keymap_cnt = GetKeyChars(
                                             info_ptr,
                                             info_ptr->input.input[info_ptr->input.confirm_len],
                                             info_ptr->input.keymap);
        }
    }
    else
    {
        SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));
        SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    }

    return SPIM_NO_ERROR;
}




/*==============================================================================
Function Name: RemoveAll
Description: ɾ�����е����룬�����ʵ���е��������ݣ��ظ���ʼ״̬
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    //û������
    if (0 == info_ptr->input.input_len)
    {
        return SPIM_ERROR;
    }

    SPIM_MEMSET(&(info_ptr->input), 0, sizeof(info_ptr->input));
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: SelectIndex
Description: ѡ������еĵ��ʣ�����Ϊ������ݵ��б�˳��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ѡ��֮�󣬴˴����������������һ�����׼��������ʼ���׶�
==============================================================================*/
static SPIM_RESULT_T SelectIndex(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_UINT8 idx)
{
    SPIM_ASSERT(NULL != info_ptr);

    if (info_ptr->input.no_cand > 0)
    {
        if (idx >= info_ptr->input.keymap_cnt)
        {
            return SPIM_PARA_ERROR;
        }

        if ((SPIM_CAPITAL_LEAD_UPPER == info_ptr->cap && 0 == info_ptr->input.confirm_len)
            || (SPIM_CAPITAL_UPPER == info_ptr->cap))
        {
            info_ptr->input.confirm[info_ptr->input.confirm_len++] = info_ptr->input.keymap[2*idx];
        }
        else
        {
            info_ptr->input.confirm[info_ptr->input.confirm_len++] = info_ptr->input.keymap[2*idx+1];
        }

        if (info_ptr->input.confirm_len == info_ptr->input.input_len)
        {
            //commit...
            info_ptr->input.has_commit = 1;
            info_ptr->input.commit = SPIM_COMMIT_INPUT_FLAG;
        }
        else
        {
            info_ptr->input.keymap_cnt = GetKeyChars(
                                             info_ptr,
                                             info_ptr->input.input[info_ptr->input.confirm_len],
                                             info_ptr->input.keymap);
        }
    }
    else
    {
        if (idx >= info_ptr->output.cand_cnt)
        {
            return SPIM_PARA_ERROR;
        }

        SPIM_ASSERT(info_ptr->output.cand[idx] < info_ptr->vocabulary_size);

        info_ptr->input.has_commit = 1;
        info_ptr->input.commit = info_ptr->output.cand[idx];

        //������
        SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));
        SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    }

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: PreviousPage
Description: ǰһҳ
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ǰ��ҳ�������ڲ��������ʱ���Ѿ�ȷ��
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    //�ǵ�һҳ
    SPIM_ASSERT(info_ptr->buffer.cur_page > 0);

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    info_ptr->buffer.cur_page -= 1;

    ReOutput(info_ptr, info_ptr->buffer.cur_page);

    return SPIM_NO_ERROR;
}



/*==============================================================================
Function Name: NextPage
Description: ��һҳ
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ǰ��ҳ�������ڲ��������ʱ���Ѿ�ȷ��
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    if (info_ptr->buffer.cur_page >= SPIM_IDX_BUF_PAGE_CNT - 1)
    {
        return SPIM_NO_ERROR;
    }

    //�����һҳ
    SPIM_ASSERT(info_ptr->buffer.cur_page < info_ptr->buffer.page_cnt);

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    info_ptr->buffer.cur_page += 1;

    ReOutput(info_ptr, info_ptr->buffer.cur_page);

    return SPIM_NO_ERROR;
}



/*==============================================================================
Function Name: CommitInput
Description: �ύ�������ĸ����
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �˹�����δ����
==============================================================================*/
static SPIM_RESULT_T CommitInput(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    SPIM_ASSERT(info_ptr->input.no_cand > 0);

    info_ptr->input.has_commit = 1;
    info_ptr->input.commit = SPIM_COMMIT_INPUT_FLAG;

    //�˴���Ҫ��������ʵ�����
    //...

    return SPIM_NO_ERROR;
}



/*==============================================================================
Description: SetCapital
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
static SPIM_RESULT_T SetCapital(
    SPIM_SPELLS9K_INFO_T *info_ptr,
    SPIM_CAPITAL_MODE_T cap
)
{
    SPIM_ASSERT(NULL != info_ptr);

    switch (cap)
    {
    case SPIM_CAPITAL_LEAD_UPPER:
    case SPIM_CAPITAL_UPPER:
        info_ptr->cap = cap;
        break;
        
    default:
        info_ptr->cap = SPIM_CAPITAL_LOWER;
        break;
    }

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: GenContent
Description: �����������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ������������뷨�ں���Ӧ�õĽ������֣���������ṹ���죬����Ҫ�ķ�������
==============================================================================*/
static void GenContent(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_CONTENT_T * content_ptr)
{
    SPIM_UINT32 cmt_idx = 0;
    SPIM_UINT32 comp_idx = 0;
    SPIM_UINT32 cand_idx = 0;
    SPIM_UINT32 cand_len = 0;

    SPIM_ASSERT(NULL != info_ptr);
    SPIM_ASSERT(NULL != content_ptr);

    SPIM_MEMSET(content_ptr, 0, sizeof(SPIM_CONTENT_T));

    if (0 == info_ptr->input.input_len)
    {
        return;
    }

    content_ptr->has_data = 1;

    // commit
    if (1 == info_ptr->input.has_commit)
    {
        if (SPIM_COMMIT_INPUT_FLAG == info_ptr->input.commit)
        {
            //�ύ����
            for (cmt_idx = 0; cmt_idx < info_ptr->input.confirm_len; cmt_idx++)
            {
                content_ptr->commit[cmt_idx] = info_ptr->input.confirm[cmt_idx];
            }

            for (cmt_idx = info_ptr->input.confirm_len; cmt_idx < info_ptr->input.input_len; cmt_idx++)
            {
                content_ptr->commit[cmt_idx] = info_ptr->input.input[cmt_idx];
            }

            content_ptr->commit_len = info_ptr->input.input_len;
            content_ptr->commit[content_ptr->commit_len] = '\0';
        }
        else
        {
            SPIM_ASSERT(info_ptr->input.commit < info_ptr->vocabulary_size);

            //�ָ�����
            content_ptr->commit_len = RecoverWord(info_ptr, info_ptr->input.commit, content_ptr->commit);
        }

        SPIM_MEMSET(&(info_ptr->input), 0, sizeof(info_ptr->input));
    }
    // cand
    else
    {
        //content_ptr->pre_edit;
        if (0 == info_ptr->input.no_cand)
        {
            //����е��ʺ�ѡ������ʾ��һ����ѡ�ֵ�ǰ�桾���볤�ȡ�����ĸ��
            RecoverWord(info_ptr, info_ptr->output.cand[0], content_ptr->pre_edit);
            //�����볤�Ƚض�
            content_ptr->pre_edit[info_ptr->input.input_len] = '\0';
            content_ptr->pre_edit_cnt = 1;

            //content_ptr->candidate;
            //content_ptr->cand_cnt;
            for (cand_idx = 0; cand_idx < info_ptr->output.cand_cnt; cand_idx++)
            {
                SPIM_ASSERT(info_ptr->output.cand[cand_idx] < info_ptr->vocabulary_size);

                //�ָ�����
                cand_len += RecoverWord(info_ptr, info_ptr->output.cand[cand_idx], content_ptr->candidate + cand_len);

                //������'\0'���볤��
                cand_len++;
            }

            content_ptr->cand_cnt = info_ptr->output.cand_cnt;

            //content_ptr->page_flag;
            if (info_ptr->buffer.cur_page > 0)
            {
                content_ptr->page_flag |= SPIM_PAGE_FLAG_PREVIOUS;
            }

            if (info_ptr->buffer.cur_page + 1 < info_ptr->buffer.page_cnt)
            {
                content_ptr->page_flag |= SPIM_PAGE_FLAG_NEXT;
            }
        }
        else
        {
            //û����Ӧ�ĵ��ʣ�����ʾ���ִ�
            for (comp_idx = 0; comp_idx < info_ptr->input.confirm_len; comp_idx++)
            {
                content_ptr->pre_edit[comp_idx] = info_ptr->input.confirm[comp_idx];
            }

            for (comp_idx = info_ptr->input.confirm_len; comp_idx < info_ptr->input.input_len; comp_idx++)
            {
                content_ptr->pre_edit[comp_idx] = info_ptr->input.input[comp_idx];
            }

            content_ptr->pre_edit[info_ptr->input.input_len] = '\0';
            content_ptr->pre_edit_cnt = 1;

            //�ṩ��ĸ�����ֺ�ѡ
            //content_ptr->candidate; ��ĸ
            //content_ptr->cand_cnt; ��ĸ����
            if ((SPIM_CAPITAL_LEAD_UPPER == info_ptr->cap && 0 == info_ptr->input.confirm_len)
                || (SPIM_CAPITAL_UPPER == info_ptr->cap))
            {
                cand_idx = 0;
            }
            else
            {
                cand_idx = 1;
            }
 
            for (; cand_idx < info_ptr->input.keymap_cnt; cand_idx += 2)
            {
                content_ptr->candidate[cand_len++] = info_ptr->input.keymap[cand_idx];
                content_ptr->candidate[cand_len++] = '\0';
            }

            content_ptr->cand_cnt = info_ptr->input.keymap_cnt/2;
        }


    }


    return;
}


/*==============================================================================
Function Name: GetKeyChars
Description: GetKeyChars
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_UINT32 GetKeyChars(
    SPIM_SPELLS9K_INFO_T *info_ptr,
    SPIM_INT8 c_num,
    SPIM_UNICODE_T *chars
)
{
    SPIM_UINT32 ch_i = 0;
    SPIM_UINT32 keymap_size = 0;
    SPIM_UINT32 chr_cnt = 0;

    if (NULL == info_ptr || NULL == chars)
    {
        return 0;
    }

    keymap_size = info_ptr->alphabet_size;

    for (ch_i = 0; ch_i < keymap_size; ch_i++)
    {
        if (info_ptr->keymap_ptr[ch_i] == c_num)
        {
            chars[chr_cnt++] = info_ptr->alphabet_ptr[ch_i];
        }
    }

    SPIM_ASSERT(chr_cnt <= SPIM_SPELLS9K_KEYMAP_CNT);

    return chr_cnt;
}



/*==============================================================================
Description: �����ʸ�ԭ��ԭʼ״̬
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
static SPIM_UINT32 RecoverWord(
    SPIM_SPELLS9K_INFO_T const *info_ptr,
    SPIM_DICT_INDEX_T word_idx,
    SPIM_UNICODE_T *word_uni_ptr
)
{
    SPIM_INT8 const *word_ptr = PNULL;
    SPIM_UINT32 word_len = 0;
    SPIM_UINT8 alphabet_idx = 0;

    SPIM_ASSERT(PNULL != info_ptr);

    word_ptr = info_ptr->vocabulary_ptr[word_idx];

    while (*(word_ptr + word_len))
    {
        alphabet_idx = *(word_ptr + word_len);

        SPIM_ASSERT(alphabet_idx < info_ptr->alphabet_size);


        if ((SPIM_CAPITAL_LEAD_UPPER == info_ptr->cap && 0 == word_len)
            || (SPIM_CAPITAL_UPPER == info_ptr->cap))
        {
            if (1 == alphabet_idx % 2)
            {
                alphabet_idx -= 1;
            }
        }

        word_uni_ptr[word_len] = info_ptr->alphabet_ptr[alphabet_idx];
        word_len++;
    }

    word_uni_ptr[word_len] = '\0';

    return word_len;
}


//init
//���ṩ��ʼ�����򣬳�ʼ�������ɸ������ṩ

/*==============================================================================
Function Name: SPIM_SPELLS9K_Term
Description: �ͷ�ʵ������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void SPIM_SPELLS9K_Term(SPIM_HANDLE_T handle)
{
    SPIM_SPELLS9K_INFO_T *info_ptr = (SPIM_SPELLS9K_INFO_T *)handle;

    if (NULL != info_ptr)
    {
        SPIM_FREE(info_ptr);
    }

    return;
}



/*==============================================================================
Function Name: SPIM_SPELLS9K_Handle
Description: �����¼�
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �ض����¼����ض��Ĵ���
==============================================================================*/
SPIM_RESULT_T SPIM_SPELLS9K_Handle(
    SPIM_HANDLE_T * handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    SPIM_CONTENT_T * content_ptr
)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    SPIM_SPELLS9K_INFO_T * info_ptr = (SPIM_SPELLS9K_INFO_T *)handle;

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

    case SPIM_EVENT_PAGE_PREVIOUS:
        res = PreviousPage(info_ptr);
        break;

    case SPIM_EVENT_PAGE_NEXT:
        res = NextPage(info_ptr);
        break;

    case SPIM_EVENT_COMMIT_INPUT:
        res = CommitInput(info_ptr);
        break;

    case SPIM_EVENT_SET_CAPITAL:
        
        if (NULL == para_ptr)
        {
            return SPIM_PARA_ERROR;
        }

        res = SetCapital(info_ptr, para_ptr->capital);
        break;

    case SPIM_EVENT_GET_CONTENT:
        //do nothing
        break;

    default:
        res = SPIM_PARA_ERROR;
        break;
    }

    if (SPIM_NO_ERROR == res && NULL != content_ptr)
    {
        GenContent(info_ptr, content_ptr);
    }


    return res;
}











