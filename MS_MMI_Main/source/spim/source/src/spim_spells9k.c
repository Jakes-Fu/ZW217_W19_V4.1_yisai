/*==============================================================================
File Name: spim_spells9k.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 拼写类外语的公共函数
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
Description: 将单词复原成原始状态
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
Description: 比较输入的数字串和索引串，检查数字是否与索引(部分)匹配
    在比较的时候，将索引串通过key map转化成数字后比较
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 0 - 匹配
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
Description: 从指定索引开始查找下一个与输入数字串匹配的单词
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 未找到记录，则返回-1，否则，返回单词索引
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
Description: 根据当前的输入，将输出写入测试数据区
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 会根据测试数据的结果来决定输出数据，引入测试数据区，是为了保护输出数据
==============================================================================*/
static void TestInput(SPIM_SPELLS9K_INFO_T *info_ptr, SPIM_UINT8 page)
{
    SPIM_DICT_INDEX_T next_idx = 0;
    SPIM_INT32 word_width = 0;

    SPIM_ASSERT(NULL != info_ptr);

    //首页搜索或者在已有页检索
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

        //放不下，则设置下一页信息，跳出
        if (word_width + info_ptr->test.cand_win_width > info_ptr->init_para.width_win)
        {
            info_ptr->test.is_searching = 1;

            //更新buffer信息
            //首次
            if (SPIM_IDX_BUF_PAGE_CNT == page)
            {
                info_ptr->buffer.first_idx[0] = info_ptr->test.cand[0];
                info_ptr->buffer.first_idx[1] = next_idx;
                info_ptr->buffer.page_cnt = 2;
            }
            //最后一页
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
Description: 根据当前的输出，更新输出
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
Description: 增加一个字符，同时测试输出，并且根据测试数据结果更新输出数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 非法数据，则返回SPIM_ERROR
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

    //尝试加入输入
    info_ptr->input.input[info_ptr->input.input_len] = c;
    info_ptr->input.input_len++;

    //已然没有可选词了
    if (info_ptr->input.no_cand > 0)
    {
        return SPIM_NO_ERROR;
    }

    //测试是否有字输出
    TestInput(info_ptr, SPIM_IDX_BUF_PAGE_CNT);

    //无输入，则清空选择区域
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
    //有输入，则更新输入
    else
    {
        info_ptr->output = info_ptr->test;
        info_ptr->buffer.cur_page = 0;
    }

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: RemoveChar
Description: 删除一个字符，是增加字符的逆操作，会更新输出数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveChar(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    //没有输入
    if (0 == info_ptr->input.input_len)
    {
        return SPIM_ERROR;
    }

    if (info_ptr->input.confirm_len > 0)
    {
        //去掉最后一个确认的字母
        info_ptr->input.confirm_len--;
    }
    else
    {
        //去掉最后一个输入
        info_ptr->input.input_len--;
    }


    if (info_ptr->input.no_cand > info_ptr->input.input_len)
    {
        info_ptr->input.no_cand = 0;
    }

    //没有删光
    if (info_ptr->input.input_len > 0)
    {
        //此时入如果no_cand的长度为(0,input_len]，则无需搜索
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
Description: 删除所有的输入，即清空实例中的所有数据，回复初始状态
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    //没有输入
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
Description: 选择输出中的单词，索引为输出数据的列表顺序
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 选择之后，此处输入结束，进入下一个输出准备，即初始化阶段
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

        //清空输出
        SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));
        SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    }

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: PreviousPage
Description: 前一页
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 前后页的数据在测试输出的时候已经确定
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    //非第一页
    SPIM_ASSERT(info_ptr->buffer.cur_page > 0);

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    info_ptr->buffer.cur_page -= 1;

    ReOutput(info_ptr, info_ptr->buffer.cur_page);

    return SPIM_NO_ERROR;
}



/*==============================================================================
Function Name: NextPage
Description: 后一页
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 前后页的数据在测试输出的时候已经确定
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    if (info_ptr->buffer.cur_page >= SPIM_IDX_BUF_PAGE_CNT - 1)
    {
        return SPIM_NO_ERROR;
    }

    //非最后一页
    SPIM_ASSERT(info_ptr->buffer.cur_page < info_ptr->buffer.page_cnt);

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    info_ptr->buffer.cur_page += 1;

    ReOutput(info_ptr, info_ptr->buffer.cur_page);

    return SPIM_NO_ERROR;
}



/*==============================================================================
Function Name: CommitInput
Description: 提交输出的字母序列
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 此功能尚未完善
==============================================================================*/
static SPIM_RESULT_T CommitInput(SPIM_SPELLS9K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    SPIM_ASSERT(info_ptr->input.no_cand > 0);

    info_ptr->input.has_commit = 1;
    info_ptr->input.commit = SPIM_COMMIT_INPUT_FLAG;

    //此处需要处理自造词的问题
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
Description: 构造输出数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 输出数据是输入法内核与应用的交互部分，根据输出结构构造，是主要的返回数据
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
            //提交输入
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

            //恢复单词
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
            //如果有单词候选，则显示第一个候选字的前面【输入长度】的字母串
            RecoverWord(info_ptr, info_ptr->output.cand[0], content_ptr->pre_edit);
            //从输入长度截断
            content_ptr->pre_edit[info_ptr->input.input_len] = '\0';
            content_ptr->pre_edit_cnt = 1;

            //content_ptr->candidate;
            //content_ptr->cand_cnt;
            for (cand_idx = 0; cand_idx < info_ptr->output.cand_cnt; cand_idx++)
            {
                SPIM_ASSERT(info_ptr->output.cand[cand_idx] < info_ptr->vocabulary_size);

                //恢复单词
                cand_len += RecoverWord(info_ptr, info_ptr->output.cand[cand_idx], content_ptr->candidate + cand_len);

                //将最后的'\0'计入长度
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
            //没有相应的单词，则显示数字串
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

            //提供字母和数字候选
            //content_ptr->candidate; 字母
            //content_ptr->cand_cnt; 字母个数
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
Description: 将单词复原成原始状态
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
//不提供初始化程序，初始化程序由各语言提供

/*==============================================================================
Function Name: SPIM_SPELLS9K_Term
Description: 释放实例数据
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
Description: 处理事件
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 特定的事件有特定的处理
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











