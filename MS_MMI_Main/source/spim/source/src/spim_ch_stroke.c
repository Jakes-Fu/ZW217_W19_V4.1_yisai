/*==============================================================================
File Name: spim_ch_stroke.c
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/18   Lianxiang.Zhou  Create.
==============================================================================*/
/*
关于字词的程序分开
是因为纯字会简化很多。并且有可能会不支持词的。

关于前后翻页的设计
一般来讲，前前后后翻页的情况并不是常出现。且，后面的单字查找是很快的
故而，采取只缓冲前n个页数据的方式，并且，每页的数据不会超过9个

20100325 看来支持词组联想是个趋势，以后可以考虑
*/

#include "spim_ch_stroke.h"
#include "spim_ch_stroke_internal.h"



/*lint -e64*/


//笔画的unicode
//折这个笔划没有常用的字形，需要根据字库调整
// 1 横(一) 2 竖(丨) 3 撇(丿) 4 点(丶) 5 折(乛(0x4e5b) フ(0x30d5) 乙(0x4e59)) 6 问号()
static SPIM_UNICODE_T const s_stroke_symbol[] =
{
    0x4e00, 0x4e28, 0x4e3f, 0x4e36, 0x30d5, 0xff1f,
};


//简体中文笔画数据字典
static SPIM_CH_STROKE_DATA_T const s_data_chs[] =
{
#include "..\\data\\chinese\\_stroke_chs.src"
};

#if 0
static  SPIM_CH_STROKE_DATA_T const s_data_cht[] =
{
#include "..\\data\\chinese\\_stroke_cht.src"
};
#endif

/*lint +e64*/





/*==============================================================================
Function Name: CompareFuzzy
Description: CompareFuzzy
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 CompareFuzzy(const SPIM_INT8 *data, const SPIM_INT8 *input, SPIM_UINT32 len);

/*==============================================================================
Function Name: SearchIndex
Description: SearchIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_DICT_INDEX_T SearchIndex(
    SPIM_CH_STROKE_INFO_T *info_ptr,
    SPIM_DICT_INDEX_T begin_idx
);

/*==============================================================================
Function Name: TestInput
Description: TestInput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void TestInput(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_UINT8 page);

/*==============================================================================
Function Name: ReOutput
Description: ReOutput
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void ReOutput(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_UINT8 page);

/*==============================================================================
Function Name: AppendStroke
Description: AppendStroke
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T AppendStroke(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_INT8 c);

/*==============================================================================
Function Name: RemoveStroke
Description: RemoveStroke
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveStroke(SPIM_CH_STROKE_INFO_T *info_ptr);

/*==============================================================================
Function Name: RemoveAll
Description: RemoveAll
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_CH_STROKE_INFO_T *info_ptr);

/*==============================================================================
Function Name: SelectIndex
Description: SelectIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T SelectIndex(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_UINT8 idx);

/*==============================================================================
Function Name: PreviousPage
Description: PreviousPage
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_CH_STROKE_INFO_T *info_ptr);

/*==============================================================================
Function Name: NextPage
Description: NextPage
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_CH_STROKE_INFO_T *info_ptr);

/*==============================================================================
Function Name: GenContent
Description: GenContent
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void GenContent(
    SPIM_CH_STROKE_INFO_T *info_ptr,
    SPIM_CONTENT_T * content_ptr
);







/*==============================================================================
Function Name: CompareFuzzy
Description: 模糊比较索引字笔划和输入串。模糊部分就是问号可匹配所有笔划
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 问号就是 6
==============================================================================*/
static SPIM_INT32 CompareFuzzy(const SPIM_INT8 *data, const SPIM_INT8 *input, SPIM_UINT32 len)
{
    SPIM_UINT8 c1 = 0, c2 = 0;

    while (len--)
    {
        c1 = *data;
        c2 = *input;
        data++;
        input++;

        if (!c1) break;

        if (!c2) break;

        if (c1 == c2) continue;

        //input is '?', always ok, 6 is '?'
        if ('6' == c2) continue;

        if (c1 != c2) break;
    }

    return ((('6' == c2) && c1) ? 0 : ((SPIM_INT32)c1 - (SPIM_INT32)c2));
}






/*==============================================================================
Function Name: SearchIndex
Description: 从指定的索引开始搜索下一个符合笔划的字
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 未查找到匹配的字，则返回-1
==============================================================================*/
static SPIM_DICT_INDEX_T SearchIndex(
    SPIM_CH_STROKE_INFO_T *info_ptr,
    SPIM_DICT_INDEX_T begin_idx
)
{
    SPIM_DICT_INDEX_T cur_idx = 0;
    SPIM_INT32 cmp_res = 0;

    SPIM_ASSERT(NULL != info_ptr);

    if (begin_idx >= info_ptr->data_size)
    {
        return -1;
    }

    for (cur_idx = begin_idx; cur_idx < info_ptr->data_size; cur_idx++)
    {
        cmp_res = CompareFuzzy(info_ptr->data_ptr[cur_idx].stroke,
                               info_ptr->input.input,
                               info_ptr->input.input_len);

        if (0 == cmp_res) break;
    }


    return (cur_idx < info_ptr->data_size ? cur_idx : -1);
}



/*==============================================================================
Function Name: TestInput
Description: 测试输出，如果一个笔划输入没有输出，则拒绝输入
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 将输出数据保存在测试数据区，可以保护输出数据
==============================================================================*/
static void TestInput(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_UINT8 page)
{
    SPIM_DICT_INDEX_T next_idx = 0;

    SPIM_ASSERT(NULL != info_ptr);

    //首页搜索或者在已有页检索
    if (SPIM_IDX_BUF_PAGE_CNT == page)
    {
        next_idx = 0;
    }
    else if (page < info_ptr->buffer.page_cnt)
    {
        next_idx = info_ptr->buffer.first_idx[page];
    }
    else
    {
        SPIM_ASSERT(0);
    }

    SPIM_ASSERT(next_idx < info_ptr->data_size);

    SPIM_MEMSET(&(info_ptr->test), 0, sizeof(info_ptr->test));

    //fill test search output
    while (info_ptr->test.cand_cnt < info_ptr->cand_cnt_per_page)
    {
        next_idx = SearchIndex(info_ptr, next_idx);

        if (-1 == next_idx) break;

        info_ptr->test.cand[info_ptr->test.cand_cnt] = next_idx;
        info_ptr->test.cand_cnt++;

        next_idx++;
    }

    //
    //需要增加索引
    //
    //首次
    if (SPIM_IDX_BUF_PAGE_CNT == page)
    {
        if (info_ptr->test.cand_cnt == info_ptr->cand_cnt_per_page)
        {
            next_idx = SearchIndex(info_ptr, next_idx);

            //设置下一页信息
            if (-1 != next_idx)
            {
                info_ptr->buffer.first_idx[0] = info_ptr->test.cand[0];
                info_ptr->buffer.first_idx[1] = next_idx;
                info_ptr->buffer.page_cnt = 2;

                info_ptr->test.is_searching = 1;
            }
        }
        else
        {
            //不足一页
            info_ptr->buffer.first_idx[0] = info_ptr->test.cand[0];
            info_ptr->buffer.page_cnt = 1;
        }
    }
    //最后一页
    else if (info_ptr->buffer.page_cnt == page + 1)
    {
        if (info_ptr->test.cand_cnt == info_ptr->cand_cnt_per_page)
        {
            next_idx = SearchIndex(info_ptr, next_idx);

            //设置下一页信息
            if (-1 != next_idx)
            {
                info_ptr->buffer.first_idx[info_ptr->buffer.page_cnt] = next_idx;
                info_ptr->buffer.page_cnt++;

                info_ptr->test.is_searching = 1;
            }
        }
    }
    //中间页
    else
    {
        //肯定有下一页
        info_ptr->test.is_searching = 1;
    }


    return;
}



/*==============================================================================
Function Name: ReOutput
Description: 更新输出数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void ReOutput(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_UINT8 page)
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
Function Name: AppendStroke
Description: 增加一个笔划，会测试增加的笔划有无相应的汉字
    无则拒收
    有则更新输出
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T AppendStroke(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_INT8 c)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;

    SPIM_ASSERT(NULL != info_ptr);

    if (info_ptr->input.input_len >= SPIM_INPUT_LEN_MAX - 1)
    {
        return SPIM_NO_ERROR;
    }

    if (c < '1' || '6' < c)
    {
        return SPIM_ERROR;
    }

    //尝试加入输入
    info_ptr->input.input[info_ptr->input.input_len] = c;
    info_ptr->input.input_len++;

    //测试是否有字输出
    TestInput(info_ptr, SPIM_IDX_BUF_PAGE_CNT);

    //无输入，则拒绝接受
    if (0 == info_ptr->test.cand_cnt)
    {
        info_ptr->input.input_len--;
        res = SPIM_ERROR;
    }
    //有输入，则更新输入
    else
    {
        info_ptr->output = info_ptr->test;
        info_ptr->buffer.cur_page = 0;
    }

    return res;
}


/*==============================================================================
Function Name: RemoveStroke
Description: 删除最后一个笔划，同时更新输出
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveStroke(SPIM_CH_STROKE_INFO_T *info_ptr)
{
    //没有输入
    if (0 == info_ptr->input.input_len)
    {
        return SPIM_ERROR;
    }

    //去掉最后一个输入
    info_ptr->input.input_len--;

    //没有删光
    if (info_ptr->input.input_len > 0)
    {
        TestInput(info_ptr, SPIM_IDX_BUF_PAGE_CNT);
        SPIM_ASSERT(info_ptr->test.cand_cnt > 0);

        info_ptr->output = info_ptr->test;
        info_ptr->buffer.cur_page = 0;
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
Description: 删除所有的笔划，即回复到初始状态
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_CH_STROKE_INFO_T *info_ptr)
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
Description: 根据输出数据的索引选择后选择，选择之后，回复到输入状态
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 联想字部分，由应用完成
==============================================================================*/
static SPIM_RESULT_T SelectIndex(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_UINT8 idx)
{
    SPIM_ASSERT(NULL != info_ptr);

    if (idx >= info_ptr->output.cand_cnt)
    {
        return SPIM_PARA_ERROR;
    }

    SPIM_ASSERT(info_ptr->output.cand[idx] < info_ptr->data_size);

    info_ptr->input.commit = info_ptr->data_ptr[info_ptr->output.cand[idx]].ch;

    //清空输出
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: PreviousPage
Description: 前一页
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 前后页数据在测试输出的时候已经保存
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_CH_STROKE_INFO_T *info_ptr)
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
Note: 前后页数据在测试输出的时候已经保存
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_CH_STROKE_INFO_T *info_ptr)
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
Function Name: GenContent
Description: 构造与应用交互的输出数据
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static void GenContent(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_CONTENT_T * content_ptr)
{
    SPIM_UINT32 input_idx = 0;
    SPIM_INT32 input_number = 0;
    SPIM_UINT32 cand_idx = 0;
    SPIM_UNICODE_T cand_ch = 0;

    SPIM_ASSERT(NULL != info_ptr);
    SPIM_ASSERT(NULL != content_ptr);

    SPIM_MEMSET(content_ptr, 0, sizeof(SPIM_CONTENT_T));

    if (0 == info_ptr->input.input_len)
    {
        return;
    }

    content_ptr->has_data = 1;

    // cand
    if (info_ptr->output.cand_cnt > 0)
    {
        //content_ptr->pre_edit;
        for (input_idx = 0; input_idx < info_ptr->input.input_len; input_idx++)
        {
            input_number = info_ptr->input.input[input_idx] - '1';
            content_ptr->pre_edit[input_idx] = s_stroke_symbol[input_number];
        }

        content_ptr->pre_edit[input_idx] = '\0';

        //content_ptr->candidate;
        //content_ptr->cand_cnt;
        for (cand_idx = 0; cand_idx < info_ptr->output.cand_cnt; cand_idx++)
        {
            SPIM_ASSERT(info_ptr->output.cand[cand_idx] < info_ptr->data_size);
            cand_ch = info_ptr->data_ptr[info_ptr->output.cand[cand_idx]].ch;
            content_ptr->candidate[cand_idx] = cand_ch;
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
    // commit
    else
    {
        SPIM_ASSERT(info_ptr->input.commit > 0);
        content_ptr->commit[0] = info_ptr->input.commit;
        content_ptr->commit_len = 1;

        SPIM_MEMSET(&(info_ptr->input), 0, sizeof(info_ptr->input));
    }


    return;
}








/*==============================================================================
Function Name: SPIM_CH_STROKE_Init
Description: 初始化实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_HANDLE_T SPIM_CH_STROKE_Init(SPIM_INIT_PARAMETER_T const *para_ptr)
{
    SPIM_CH_STROKE_INFO_T *info_ptr = NULL;

    if (NULL == para_ptr)
    {
        return NULL;
    }

    info_ptr = SPIM_MALLOC(sizeof(SPIM_CH_STROKE_INFO_T));

    if (NULL == info_ptr)
    {
        return NULL;
    }

    SPIM_MEMSET(info_ptr, 0, sizeof(SPIM_CH_STROKE_INFO_T));

    //默认简体中文
    info_ptr->data_ptr = s_data_chs;
    info_ptr->data_size = SPIM_ARR_SIZE(s_data_chs);

    info_ptr->cand_cnt_per_page = para_ptr->width_win / (para_ptr->width_chr + para_ptr->width_spc);

    SPIM_ASSERT(info_ptr->cand_cnt_per_page > 0);


    return info_ptr;
}



/*==============================================================================
Function Name: SPIM_CH_STROKE_term
Description: 释放实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void SPIM_CH_STROKE_term(SPIM_HANDLE_T handle)
{
    SPIM_CH_STROKE_INFO_T *info_ptr = (SPIM_CH_STROKE_INFO_T *)handle;

    if (NULL != info_ptr)
    {
        SPIM_FREE(info_ptr);
    }

    return;
}



/*==============================================================================
Function Name: SPIM_CH_STROKE_Handle
Description: 事件处理
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_RESULT_T SPIM_CH_STROKE_Handle(
    SPIM_HANDLE_T * handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    SPIM_CONTENT_T * content_ptr
)
{
    SPIM_RESULT_T res = SPIM_NO_ERROR;
    SPIM_CH_STROKE_INFO_T * info_ptr = (SPIM_CH_STROKE_INFO_T *)handle;

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

        res = AppendStroke(info_ptr, para_ptr->append);

        break;

    case SPIM_EVENT_CHAR_REMOVE:
        res = RemoveStroke(info_ptr);
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

    if (SPIM_NO_ERROR == res && NULL != content_ptr)
    {
        GenContent(info_ptr, content_ptr);
    }


    return res;
}









