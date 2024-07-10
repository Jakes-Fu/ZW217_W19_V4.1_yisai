/*==============================================================================
File Name: spim_ch_py26k.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
/*
关于字词的程序分开
是因为纯字会简化很多。并且有可能会不支持词的。

关于前后翻页的设计
一般来讲，前前后后翻页的情况并不是常出现。且，后面的单字查找是很快的
故而，采取只缓冲前n个页数据的方式，并且，每页的数据不会超过9个
*/


#include "spim_ch.h"
#include "spim_ch_py26k.h"
#include "spim_ch_py26k_internal.h"


/*==============================================================================
Function Name: Init
Description: Init
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_PY26K_INFO_T * Init(void);

/*==============================================================================
Function Name: Term
Description: Term
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void Term(SPIM_PY26K_INFO_T *info_ptr);

/*==============================================================================
Function Name: CommitData
Description: CommitData
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void CommitData(SPIM_PY26K_INFO_T *info_ptr);

/*==============================================================================
Function Name: InputChar
Description: InputChar
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T InputChar(SPIM_PY26K_INFO_T *info_ptr, SPIM_UNICODE_T c);

/*==============================================================================
Function Name: SearchWord
Description: SearchWord
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 SearchWord(
    SPIM_PY26K_INFO_T *info_ptr,
    GUIME_CH_PYW_CAND_T *begin_idx_ptr,
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
    SPIM_PY26K_INFO_T *info_ptr,
    SPIM_PY_DICT_IDX_T chr_begin_idx,
    SPIM_UINT16 cand_win_width
);

/*==============================================================================
Function Name: ReOutput
Description: ReOutput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T ReOutput(
    SPIM_PY26K_INFO_T *info_ptr,
    GUIME_CH_PYW_CAND_T *begin_idx_ptr
);

/*==============================================================================
Function Name: RefreshBuffer
Description: RefreshBuffer
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void RefreshBuffer(
    SPIM_PY26K_INFO_T *info_ptr,
    GUIME_CH_PYW_CAND_T *buf_idx_ptr
);

/*==============================================================================
Function Name: AppendPy
Description: AppendPy
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T AppendPy(SPIM_PY26K_INFO_T *info_ptr, SPIM_UNICODE_T c);

/*==============================================================================
Function Name: RemovePy
Description: RemovePy
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemovePy(SPIM_PY26K_INFO_T *info_ptr);

/*==============================================================================
Function Name: RemoveAll
Description: RemoveAll
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_PY26K_INFO_T *info_ptr);

/*==============================================================================
Function Name: SelectIndex
Description: SelectIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T SelectIndex(SPIM_PY26K_INFO_T *info_ptr, SPIM_UINT8 idx);

/*==============================================================================
Function Name: PreviousPage
Description: PreviousPage
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_PY26K_INFO_T *info_ptr);

/*==============================================================================
Function Name: NextPage
Description: NextPage
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_PY26K_INFO_T *info_ptr);

/*==============================================================================
Function Name: GenContent
Description: GenContent
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void GenContent(
    SPIM_PY26K_INFO_T *info_ptr,
    SPIM_CONTENT_T * content_ptr
);




/*==============================================================================
Function Name: Init
Description: 初始化实例数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 仅仅先完成了简体中文
==============================================================================*/
static SPIM_PY26K_INFO_T * Init(void)
{
    SPIM_PY26K_INFO_T * info_ptr = NULL;

    info_ptr = SPIM_MALLOC(sizeof(SPIM_PY26K_INFO_T));

    if (NULL == info_ptr)
    {
        return NULL;
    }

    SPIM_MEMSET(info_ptr, 0, sizeof(SPIM_PY26K_INFO_T));

    //先默认简体中文
    info_ptr->dict_cfg = spim_ch_dict_cfg;
    info_ptr->dict_size = spim_ch_dict_cfg_size;
    info_ptr->py_cnt = spim_ch_py_cnt_chs;

    return info_ptr;
}


/*==============================================================================
Function Name: Term
Description: 释放实例数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void Term(SPIM_PY26K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    SPIM_FREE(info_ptr);

    return;
}


/*
*/
/*==============================================================================
Function Name: CommitData
Description: 将输入数据转化成提交数据，提交规则
  1 先提交已经确定部分的汉字
  2 提交余下部分未确定的字母
  3 字母提交按照声母连续输出，忽略任何空格
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void CommitData(SPIM_PY26K_INFO_T *info_ptr)
{
    SPIM_UINT8 i = 0;
    SPIM_INT16 const *pc = NULL;

    SPIM_ASSERT(NULL != info_ptr);

    for (i = info_ptr->input.confirm_cnt; i < info_ptr->input.idx_cnt; i++)
    {
        pc = spim_ch_py_list[info_ptr->input.idx[i]];

        while (*pc)
        {
            info_ptr->input.confirm[info_ptr->input.confirm_cnt] = *pc;
            pc++;
            info_ptr->input.confirm_cnt++;
        }
    }

    info_ptr->input.confirm[info_ptr->input.confirm_cnt] = '\0';

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_T));
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(SPIM_CH_PYW_BUFFER_T));

    return;
}


/*==============================================================================
Function Name: InputChar
Description: 输入拼音字符，并且根据输入的拼音串重新设置拼音组合及相关属性标志
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: input character one by one, it may be refused
==============================================================================*/
static SPIM_RESULT_T InputChar(SPIM_PY26K_INFO_T *info_ptr, SPIM_UNICODE_T c)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    SPIM_INT32 input_len = 0;
    SPIM_INT16 py_str[SPIM_PY_SIMPLEX_MAX + 1] = {0};
    SPIM_INT32 back_idx = 0;
    SPIM_UINT32 idx_py_len = 0;
    SPIM_UINT32 back_py_len = 0;
    SPIM_INT32 new_py_idx = -1;
    SPIM_INT32 cat_idx = 0;
    SPIM_INT32 idx = 0;

    SPIM_ASSERT(NULL != info_ptr);

    //only can input [a-z] and space
    if (!SPIM_ISLOWER(c) && c != SPIM_PY_SEG_SYMBOL)
    {
        return SPIM_ERROR;
    }

    //refuse the first space
    if (SPIM_PY_SEG_SYMBOL == c && 0 == info_ptr->input.idx_cnt)
    {
        return SPIM_ERROR;
    }

    back_idx = info_ptr->input.idx_cnt - 1;

    //refuse the space after a space, maybe we would commit the pinyin as spell
    if (SPIM_PY_SEG_SYMBOL == c && SPIM_GET_FLG_SEG(info_ptr->input.idx_flag[back_idx]))
    {
        //CommitData(info_ptr);
        return SPIM_NO_ERROR;
    }

    //加入输入
    input_len = strlen(info_ptr->input.input);

    //只接受最大输入的一半长度作为输入
    if (input_len >= SPIM_INPUT_LEN_MAX - 1)
    {
        return SPIM_ERROR;
    }

    info_ptr->input.input[input_len] = c;
    info_ptr->input.input[input_len+1] = '\0';

    //SPIM_PY_SEG_SYMBOL用来【分段】pinyin
    if (SPIM_PY_SEG_SYMBOL == c)
    {
        info_ptr->input.idx_flag[back_idx] |= SPIM_IDX_FLG_MASK_SEG;
        return SPIM_NO_ERROR;
    }

    //当可能【向前合并】的索引存在
    while (back_idx >= 0)
    {
        //有【分段】标识，不参与合并
        if (SPIM_GET_FLG_SEG(info_ptr->input.idx_flag[back_idx])) break;

        idx_py_len = SPIM_WcsLen((SPIM_UNICODE_T *)spim_ch_py_list[info_ptr->input.idx[back_idx]]);
        back_py_len += idx_py_len;

        //简单判断【超长】，一个单独的拼音不会超过6个字符
        if (back_py_len >= SPIM_PY_SIMPLEX_MAX)
        {
            break;
        }

        back_idx--;
    }

    //回退到可组合的部分
    back_idx++;

    //从back_idx开始往后做合并判断
    while (back_idx < info_ptr->input.idx_cnt)
    {
        cat_idx = back_idx;
        py_str[0] = L'\0';

        while (cat_idx < info_ptr->input.idx_cnt)
        {
            SPIM_WcsCat(
                    (SPIM_UNICODE_T *)py_str, 
                    (SPIM_UNICODE_T *)spim_ch_py_list[info_ptr->input.idx[cat_idx]]);
            cat_idx++;
        }

        back_py_len = SPIM_WcsLen((SPIM_UNICODE_T *)py_str);
        py_str[back_py_len] = c;
        py_str[back_py_len + 1] = L'\0';

        new_py_idx = FindStringIndex(spim_ch_py_list, spim_ch_py_list_size, py_str);

        if (new_py_idx != -1)
        {
            //如果该index的pinyin下无字，则修生idx为-1
            if (info_ptr->py_cnt[new_py_idx] > 0)
            {
                break;
            }
            else
            {
                new_py_idx = -1;
            }
        }

        back_idx++;
    }


    if (new_py_idx != -1)
    {
        //【组合成功】
        info_ptr->input.idx[back_idx] = new_py_idx;
        info_ptr->input.idx_cnt = back_idx + 1;
        res = SPIM_NO_ERROR;
    }
    else
    {
        py_str[0] = c;
        py_str[1] = L'\0';
        new_py_idx = FindStringIndex(spim_ch_py_list, spim_ch_py_list_size, py_str);

        SPIM_ASSERT(-1 != new_py_idx);

        info_ptr->input.idx[info_ptr->input.idx_cnt] = new_py_idx;
        info_ptr->input.idx_cnt++;
        
        res = SPIM_NO_ERROR;
    }

    //设置声母标志
    for (idx = 0; idx < info_ptr->input.idx_cnt; idx++)
    {
        info_ptr->input.idx_flag[idx] &= ~SPIM_IDX_FLG_MASK_IC;

        if (SPIM_WcsLen((SPIM_UNICODE_T *)spim_ch_py_list[info_ptr->input.idx[idx]]) > 2) continue;

        if (-1 != FindStringIndex(spim_ch_py_ic, spim_ch_py_ic_size, spim_ch_py_list[info_ptr->input.idx[idx]]))
        {
            info_ptr->input.idx_flag[idx] |= SPIM_IDX_FLG_MASK_IC;
        }
    }

    //如果
    // 1 就一个字母
    // 2 且该字母所构成的拼音下无字
    // 3 非声母
    //则清空输入，拒绝
    if (0 == input_len
        && info_ptr->py_cnt[info_ptr->input.idx[0]] == 0
        && 0 == (SPIM_UINT8)SPIM_GET_FLG_IC(info_ptr->input.idx_flag[0]))
    {
        SPIM_MEMSET(&(info_ptr->input), 0, sizeof(SPIM_CH_PYW_INPUT_T));
        res = SPIM_ERROR;
    }

    return res;
}



/*==============================================================================
Function Name: SearchWord
Description: 从指定的索引开始向后查找匹配的单词，直到放满整个候选区
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 SearchWord(SPIM_PY26K_INFO_T *info_ptr,
                      GUIME_CH_PYW_CAND_T *begin_idx_ptr,
                      SPIM_UINT16 cand_win_width
                     )
{
    SPIM_PY_DICT_IDX_T index = 0;
    SPIM_INT16 word_len = 0;
    SPIM_PY_DICT_IDX_T word_size = 0;
    SPIM_UNICODE_T const *dict_data_ptr = NULL;
    SPIM_UNICODE_T const *word_ptr = NULL;
    SPIM_UINT8 idx_from = 0;
    SPIM_UINT8 char_i = 0;
    SPIM_INT32 is_match = 0;
    SPIM_UINT16 per_seg = 0;
    SPIM_UINT16 left_spc = 0;
    GUIME_CH_PYW_CAND_T buf_idx = {0};

    SPIM_ASSERT(NULL != info_ptr);
    SPIM_ASSERT(NULL != begin_idx_ptr);

    SPIM_MEMSET(&(info_ptr->test), 0, sizeof(SPIM_CH_PYW_OUTPUT_T));

    word_len = begin_idx_ptr->page + 1;

    //词至少两个字
    if (word_len < 2)
    {
        return 0;
    }

    //超过字典里的词的最大长度
    if ((SPIM_UINT16)word_len > info_ptr->dict_size)
    {
        return 0;
    }

    idx_from = info_ptr->input.confirm_cnt;

    word_size = (SPIM_PY_DICT_IDX_T)info_ptr->dict_cfg[word_len - 1].size;
    dict_data_ptr = (SPIM_UNICODE_T *)info_ptr->dict_cfg[word_len - 1].data_ptr;

    per_seg = info_ptr->init_para.width_spc + (info_ptr->init_para.width_chr * word_len);

    //1 输入索引被改变
    //2 输入索引被增加
    index = begin_idx_ptr->idx;

    do
    {
        //逐词检测
        while (index < word_size)
        {
            is_match = 0;

            word_ptr = dict_data_ptr + index * word_len;


            //逐字检查
            for (char_i = 0; char_i < word_len; char_i++)
            {
                is_match = IsPyIndexMatchChar(
                                    spim_ch_py_list,
                                    info_ptr->input.idx[idx_from + char_i],
                                    spim_ch_py_dict_cfg,
                                    *(word_ptr + char_i),
                                    (SPIM_UINT8)SPIM_GET_FLG_IC(info_ptr->input.idx_flag[idx_from + char_i])
                                    );

                if (!is_match) break;
            }

            if (is_match) break;

            index++;
        }

        if (index >= word_size)
        {
            info_ptr->test.is_searching = 0;
            break;
        }

        //在搜索之后判定长度，因为该长度下可能已经没有符合的词了
        left_spc = cand_win_width - info_ptr->test.cand_win_width;

        if (per_seg > left_spc)
        {
            info_ptr->test.is_searching = 1;

            //更新buffer信息
            buf_idx.dict = SPIM_DICT_IDX_SYS;
            buf_idx.page = word_len - 1;
            buf_idx.idx = index;
            RefreshBuffer(info_ptr, &buf_idx);

            break;
        }

        info_ptr->test.cand[info_ptr->test.cand_cnt].dict = SPIM_DICT_IDX_SYS;
        info_ptr->test.cand[info_ptr->test.cand_cnt].page = word_len - 1;
        info_ptr->test.cand[info_ptr->test.cand_cnt].idx = index;
        info_ptr->test.cand_cnt++;

        info_ptr->test.cand_win_width += per_seg;

        index += 1;
    }
    while (1 != info_ptr->test.is_searching);


    return info_ptr->test.cand_cnt;
}


/*==============================================================================
Function Name: SearchChar
Description: 查找字，直到放满整个候选区
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 SearchChar(SPIM_PY26K_INFO_T *info_ptr, SPIM_PY_DICT_IDX_T chr_begin_idx, SPIM_UINT16 cand_win_width)
{
    SPIM_PY_DICT_IDX_T index = chr_begin_idx;
    SPIM_UNICODE_T const *char_set = (SPIM_UNICODE_T *)info_ptr->dict_cfg[0].data_ptr;
    SPIM_PY_DICT_IDX_T char_size = info_ptr->dict_cfg[0].size;
    SPIM_UINT16 per_seg = 0;
    SPIM_UINT16 left_spc = 0;
    GUIME_CH_PYW_CAND_T buf_idx = {0};

    SPIM_ASSERT(NULL != info_ptr);

    SPIM_MEMSET(&(info_ptr->test), 0, sizeof(SPIM_CH_PYW_OUTPUT_T));

    per_seg = info_ptr->init_para.width_spc + info_ptr->init_para.width_chr;

    do
    {
        while (index < char_size
                && !IsPyIndexMatchChar(
                            spim_ch_py_list,
                            info_ptr->input.idx[info_ptr->input.confirm_cnt],
                            spim_ch_py_dict_cfg,
                            char_set[index],
                            (SPIM_UINT8)SPIM_GET_FLG_IC(info_ptr->input.idx_flag[info_ptr->input.confirm_cnt])
                            ))
        {
            index++;
        }

        if (index >= char_size)
        {
            info_ptr->test.is_searching = 0;
            break;
        }

        //在搜索之后判定，可以确定下一页的信息
        left_spc = cand_win_width - info_ptr->test.cand_win_width;

        if (per_seg > left_spc)
        {
            info_ptr->test.is_searching = 1;

            //更新buffer信息
            buf_idx.dict = SPIM_DICT_IDX_SYS;
            buf_idx.page = 0;
            buf_idx.idx = index;
            RefreshBuffer(info_ptr, &buf_idx);

            break;
        }

        info_ptr->test.cand[info_ptr->test.cand_cnt].dict = SPIM_DICT_IDX_SYS;
        info_ptr->test.cand[info_ptr->test.cand_cnt].page = 0;
        info_ptr->test.cand[info_ptr->test.cand_cnt].idx = index;
        info_ptr->test.cand_cnt++;

        info_ptr->test.cand_win_width += per_seg;

        index += 1;
    }
    while (1 != info_ptr->test.is_searching);

    return info_ptr->test.cand_cnt;
}


/*==============================================================================
Function Name: ReOutput
Description: 更训输出数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T ReOutput(SPIM_PY26K_INFO_T *info_ptr, GUIME_CH_PYW_CAND_T *begin_idx_ptr)
{
    GUIME_CH_PYW_CAND_T begin_idx = {0};
    SPIM_PY_DICT_IDX_T chr_begin_idx = 0;
    SPIM_INT32 test_cnt = 0;
    SPIM_UINT8 test_page = 0;
    SPIM_INT32 word_i = 0;
    SPIM_INT32 char_i = 0;
    SPIM_UINT16 per_seg = 0; //每个词加上间隔的长度
    SPIM_UINT16 left_spc = 0; //剩余的选择项空间
    SPIM_INT32 left_idx = 0;

    SPIM_ASSERT(NULL != info_ptr);

    left_idx = info_ptr->input.idx_cnt - info_ptr->input.confirm_cnt;

    //未确定的索引数需【有】
    SPIM_ASSERT(left_idx > 0);

    //未确定的索引必须大于等于查找长度
    SPIM_ASSERT(left_idx >= begin_idx_ptr->page + 1);

    test_page = begin_idx_ptr->page;

    SPIM_ASSERT(0 == info_ptr->output.cand_cnt);

    //先用词来填充
    SPIM_ASSERT(0 == info_ptr->output.is_searching);

    begin_idx.dict = SPIM_DICT_IDX_SYS;

    //词至少是两个字的
    while (test_page > 0)
    {
        left_spc = info_ptr->init_para.width_win - info_ptr->output.cand_win_width;

        //确定搜索起点
        begin_idx.page = test_page;
        begin_idx.idx = (test_page == begin_idx_ptr->page) ? begin_idx_ptr->idx : 0;

        //搜索
        test_cnt = SearchWord(info_ptr, &begin_idx, left_spc);

        if (test_cnt > 0)
        {
            //将搜索结果放到output后面
            for (word_i = 0; word_i < test_cnt; word_i++)
            {
                info_ptr->output.cand[info_ptr->output.cand_cnt] = info_ptr->test.cand[word_i];
                info_ptr->output.cand_cnt++;
            }

            info_ptr->output.cand_win_width += info_ptr->test.cand_win_width;
        }

        if (1 == info_ptr->test.is_searching)
        {
            info_ptr->output.is_searching = 1;
            goto RE_OUTPUT_SEARCH_END;
        }

        //前一页
        test_page--;
    }


    //再用字来填充
    SPIM_ASSERT(0 == info_ptr->output.is_searching);
    //只剩下一个索引的没有检索了
    SPIM_ASSERT(0 == test_page);

    per_seg = info_ptr->init_para.width_spc + info_ptr->init_para.width_chr;
    left_spc = info_ptr->init_para.width_win - info_ptr->output.cand_win_width;

    //已经放不下一个候选字
    if (per_seg > left_spc)
    {
        info_ptr->output.is_searching = 1;
        goto RE_OUTPUT_SEARCH_END;
    }

    //确定字搜索的起点，如果是从词开始检索的，则从零开始
    chr_begin_idx = (0 == begin_idx_ptr->page) ? begin_idx_ptr->idx : 0;

    //检索字
    test_cnt = SearchChar(info_ptr, chr_begin_idx, left_spc);

    if (test_cnt > 0)
    {
        //将搜索结果放到output后面
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

    //未搜索到，则将剩余的拼音直接提交
    if (0 == info_ptr->output.cand_cnt)
    {
        CommitData(info_ptr);
    }


RE_OUTPUT_SEARCH_END:

    //更新搜索缓存，即页首索引
    //首页信息会被翻页到该页的时候重复缓存，这部分开销可以接受
    if (0 == info_ptr->buffer.cur_page && info_ptr->output.cand_cnt > 0)
    {
        info_ptr->buffer.first_idx[0] = info_ptr->output.cand[0];
    }

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: RefreshBuffer
Description: 更新缓存页信息
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void RefreshBuffer(SPIM_PY26K_INFO_T *info_ptr, GUIME_CH_PYW_CAND_T *buf_idx_ptr)
{
    SPIM_INT32 buf_cmp_res = 0;

    SPIM_ASSERT((info_ptr->buffer.cur_page < info_ptr->buffer.page_cnt)
        || (0 == info_ptr->buffer.cur_page && 0 == info_ptr->buffer.page_cnt));


    if (info_ptr->buffer.cur_page + 1 < info_ptr->buffer.page_cnt)
    {
        //校验已复制头索引的一致性
        buf_cmp_res = memcmp(&(info_ptr->buffer.first_idx[info_ptr->buffer.cur_page + 1]),
                             buf_idx_ptr, sizeof(GUIME_CH_PYW_CAND_T));
        SPIM_ASSERT(0 == buf_cmp_res);
    }
    else
    {
        //增加缓存页
        info_ptr->buffer.first_idx[info_ptr->buffer.cur_page + 1] = *buf_idx_ptr;
        info_ptr->buffer.page_cnt = info_ptr->buffer.cur_page + 2;
    }

    return;
}


/*==============================================================================
Function Name: AppendPy
Description: 增加一个输入的拼音，根据输入情况更新输出
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T AppendPy(SPIM_PY26K_INFO_T *info_ptr, SPIM_UNICODE_T c)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    GUIME_CH_PYW_CAND_T begin_idx = {0};

    SPIM_ASSERT(NULL != info_ptr);

    res = InputChar(info_ptr, c);

    //输入未有本质变化
    if (SPIM_NO_ERROR != res)
    {
        return res;
    }

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_T));
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(SPIM_CH_PYW_BUFFER_T));

    begin_idx.dict = SPIM_DICT_IDX_SYS;
    begin_idx.page = info_ptr->input.idx_cnt - info_ptr->input.confirm_cnt - 1;
    begin_idx.idx = 0;

    return ReOutput(info_ptr, &begin_idx);
}



/*==============================================================================
Function Name: RemovePy
Description: 删除一个拼音，删除会导致拼音组合的变化
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
    算法:
      1 取最后一个拼音索引
      2 得到拼音字串
      3 去掉输入的最后一个索引及相关数据
      4 将输入拼音串逐个重新输入，除了最后一个
      5 重新搜索
==============================================================================*/
static SPIM_RESULT_T RemovePy(SPIM_PY26K_INFO_T *info_ptr)
{
    SPIM_INT16 const *p_py = NULL;
    SPIM_UINT32 py_len = 0;
    SPIM_UINT32 input_len = 0;
    SPIM_UINT32 char_i = 0;
    GUIME_CH_PYW_CAND_T begin_idx = {0};


    SPIM_ASSERT(NULL != info_ptr);

    //没有输入
    if (0 == info_ptr->input.idx_cnt)
    {
        return SPIM_ERROR;
    }

    //最后一个idx的分段标志
    if (SPIM_GET_FLG_SEG(info_ptr->input.idx_flag[info_ptr->input.idx_cnt - 1]))
    {
        info_ptr->input.idx_flag[info_ptr->input.idx_cnt - 1] &= ~SPIM_IDX_FLG_MASK_SEG;
        return SPIM_NO_ERROR;
    }

    //已有confirm，则减少confirm数据
    if (info_ptr->input.confirm_cnt > 0)
    {
        info_ptr->input.confirm_cnt--;
    }
    else
    {
        //修改最后一个索引
        // 1 全部删除
        // 2 部分删除
        // 3 删除后分拆，比如 long 删除 g 之后，变成 l o n 这三个索引了
        // 4 至此，可以保证是连续的字母输入
        p_py = spim_ch_py_list[info_ptr->input.idx[info_ptr->input.idx_cnt - 1]];
        info_ptr->input.idx_cnt -= 1;
        py_len = SPIM_WcsLen((SPIM_UNICODE_T *)p_py);
        input_len = strlen(info_ptr->input.input);
        info_ptr->input.input[input_len - py_len] = '\0';

        for (char_i = 0; char_i < py_len - 1; char_i++)
        {
            InputChar(info_ptr, *(p_py + char_i));
        }
    }

    //更新输出
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    //还有未确定的，则刷新输入
    if (info_ptr->input.confirm_cnt < info_ptr->input.idx_cnt)
    {
        begin_idx.dict = SPIM_DICT_IDX_SYS;
        begin_idx.page = info_ptr->input.idx_cnt - info_ptr->input.confirm_cnt - 1;
        begin_idx.idx = 0;

        ReOutput(info_ptr, &begin_idx);
    }


    return SPIM_NO_ERROR;
}

/*==============================================================================
Function Name: RemoveAll
Description: 清空所有的输入
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 即回复到初始状态
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_PY26K_INFO_T *info_ptr)
{
    SPIM_ASSERT(NULL != info_ptr);

    SPIM_MEMSET(&(info_ptr->input), 0, sizeof(SPIM_CH_PYW_INPUT_T));
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_T));
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(SPIM_CH_PYW_BUFFER_T));

    return SPIM_NO_ERROR;
}

/*==============================================================================
Function Name: SelectIndex
Description: 根据输出数据的索引选择候选字词
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 选定之后进入初始状态，候选部分由应用完成
==============================================================================*/
static SPIM_RESULT_T SelectIndex(SPIM_PY26K_INFO_T *info_ptr, SPIM_UINT8 idx)
{
    SPIM_UINT8 i = 0;
    SPIM_UNICODE_T const *pwc = NULL;
    GUIME_CH_PYW_CAND_T begin_idx = {0};


    SPIM_ASSERT(NULL != info_ptr);

    if (idx >= info_ptr->output.cand_cnt)
    {
        return SPIM_PARA_ERROR;
    }

    pwc = (SPIM_UNICODE_T *)info_ptr->dict_cfg[info_ptr->output.cand[idx].page].data_ptr;
    pwc += (info_ptr->output.cand[idx].idx * (info_ptr->output.cand[idx].page + 1));

    for (i = 0; i <= info_ptr->output.cand[idx].page; i++)
    {
        info_ptr->input.confirm[info_ptr->input.confirm_cnt + i] = pwc[i];
    }

    info_ptr->input.confirm_cnt += info_ptr->output.cand[idx].page + 1;

    //更新输出
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));

    //还有未确定的，则刷新输入
    if (info_ptr->input.confirm_cnt < info_ptr->input.idx_cnt)
    {
        begin_idx.dict = SPIM_DICT_IDX_SYS;
        begin_idx.page = info_ptr->input.idx_cnt - info_ptr->input.confirm_cnt - 1;
        begin_idx.idx = 0;

        ReOutput(info_ptr, &begin_idx);
    }

    return SPIM_NO_ERROR;
}

/*==============================================================================
Function Name: PreviousPage
Description: 前一页
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 前后页的数据在检索的时候已经保存
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_PY26K_INFO_T *info_ptr)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    GUIME_CH_PYW_CAND_T *begin_idx_ptr = NULL;

    SPIM_ASSERT(NULL != info_ptr);

    //非第一页
    SPIM_ASSERT(info_ptr->buffer.cur_page > 0);

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_T));

    info_ptr->buffer.cur_page -= 1;

    begin_idx_ptr = &(info_ptr->buffer.first_idx[info_ptr->buffer.cur_page]);

    res = ReOutput(info_ptr, begin_idx_ptr);

    return res;
}




/*==============================================================================
Function Name: NextPage
Description: 后一页
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 前后页的数据在检索的时候已经保存
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_PY26K_INFO_T *info_ptr)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    GUIME_CH_PYW_CAND_T *begin_idx_ptr = NULL;

    SPIM_ASSERT(NULL != info_ptr);

    if (info_ptr->buffer.cur_page == SPIM_IDX_BUF_PAGE_CNT - 1)
    {
        SPIM_ASSERT(SPIM_IDX_BUF_PAGE_CNT == info_ptr->buffer.page_cnt);

        //能翻到定义的最大页的都是强人
        return SPIM_ERROR;
    }

    //非最后一页
    SPIM_ASSERT(info_ptr->buffer.cur_page + 1 < info_ptr->buffer.page_cnt);

    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(SPIM_CH_PYW_OUTPUT_T));

    info_ptr->buffer.cur_page += 1;

    begin_idx_ptr = &(info_ptr->buffer.first_idx[info_ptr->buffer.cur_page]);

    res = ReOutput(info_ptr, begin_idx_ptr);

    return res;
}






/*==============================================================================
Function Name: GenContent
Description: 构造输出数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void GenContent(SPIM_PY26K_INFO_T *info_ptr,
                       SPIM_CONTENT_T * content_ptr
                      )
{
    SPIM_INT32 i = 0;
    SPIM_INT16 const *pc = NULL;
    SPIM_INT32 com_len = 0;
    SPIM_UNICODE_T const *pwc = NULL;
    SPIM_INT32 cand_len = 0;
    SPIM_UINT8 dict_page = 0;
    SPIM_UINT8 char_i = 0;

    SPIM_ASSERT(NULL != info_ptr);
    SPIM_ASSERT(NULL != content_ptr);

    SPIM_MEMSET(content_ptr, 0, sizeof(SPIM_CONTENT_T));

    if (0 == info_ptr->input.idx_cnt)
    {
        return;
    }

    content_ptr->has_data = 1;

    //没有检索结果，全不当作检索完毕，则提交
    if (0 == info_ptr->output.cand_cnt)
    {
        //content_ptr->commit
        //content_ptr->commit_len
        content_ptr->commit_len = info_ptr->input.confirm_cnt;

        if (content_ptr->commit_len > SPIM_ARR_SIZE(content_ptr->commit) - 1)
        {
            content_ptr->commit_len = SPIM_ARR_SIZE(content_ptr->commit) - 1;
        }

        SPIM_MEMCPY(content_ptr->commit, info_ptr->input.confirm,
               content_ptr->commit_len*sizeof(SPIM_UNICODE_T));

        content_ptr->commit[content_ptr->commit_len] = '\0';

        SPIM_MEMSET(&(info_ptr->input), 0, sizeof(SPIM_CH_PYW_INPUT_T));
    }
    else
    {
        //content_ptr->pre_edit
        for (i = 0; i < info_ptr->input.confirm_cnt; i++)
        {
            content_ptr->pre_edit[com_len] = info_ptr->input.confirm[i];
            com_len++;
        }

        content_ptr->pre_edit[com_len] = '\0';
        com_len++;

        for (i = info_ptr->input.confirm_cnt; i < info_ptr->input.idx_cnt; i++)
        {
            pc = spim_ch_py_list[info_ptr->input.idx[i]];

            while (*pc)
            {
                content_ptr->pre_edit[com_len] = *pc;
                pc++;
                com_len++;
            }

            content_ptr->pre_edit[com_len] = SPIM_PY_SEG_SYMBOL;
            com_len++;
        }

        //没有主动分段标识，则去掉分段标识
        if (!SPIM_GET_FLG_SEG(info_ptr->input.idx_flag[info_ptr->input.idx_cnt - 1]))
        {
            content_ptr->pre_edit[com_len - 1] = '\0';
        }

        //content_ptr->candidate
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



//init
/*==============================================================================
Function Name: SPIM_CH_PY26K_Init
Description: 初始化实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_HANDLE_T SPIM_CH_PY26K_Init(SPIM_INIT_PARAMETER_T const *para_ptr)
{
    SPIM_PY26K_INFO_T * info_ptr = NULL;

    if (NULL == para_ptr) return NULL;

    info_ptr = Init();

    if (NULL == info_ptr)
    {
        return NULL;
    }

    info_ptr->init_para = *para_ptr;

    //提供的长度至少要能放下一个最长的词
    SPIM_ASSERT(para_ptr->width_win > para_ptr->width_chr * info_ptr->dict_size + para_ptr->width_spc);


    return (SPIM_HANDLE_T *)info_ptr;
}

//term
/*==============================================================================
Function Name: SPIM_CH_PY26K_term
Description: 释放实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void SPIM_CH_PY26K_term(SPIM_HANDLE_T handle)
{
    SPIM_PY26K_INFO_T * info_ptr = (SPIM_PY26K_INFO_T *)handle;

    if (NULL != info_ptr)
    {
        Term(info_ptr);
    }

    return;
}

//handle
/*==============================================================================
Function Name: SPIM_CH_PY26K_Handle
Description: 事件处理
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_RESULT_T SPIM_CH_PY26K_Handle(
    SPIM_HANDLE_T * handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    SPIM_CONTENT_T * content_ptr
)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    SPIM_PY26K_INFO_T * info_ptr = (SPIM_PY26K_INFO_T *)handle;

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
        
        res = AppendPy(info_ptr, para_ptr->append);

        break;

    case SPIM_EVENT_CHAR_REMOVE:
        res = RemovePy(info_ptr);
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

    case SPIM_EVENT_GET_CONTENT:
        //do nothing
        break;

    default:
        res = SPIM_PARA_ERROR;
        break;
    }

    //生成content
    if (SPIM_NO_ERROR == res && NULL != content_ptr)
    {
        GenContent(info_ptr, content_ptr);
    }


    return res;
}



