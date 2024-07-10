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
�����ִʵĳ���ֿ�
����Ϊ���ֻ�򻯺ܶࡣ�����п��ܻ᲻֧�ִʵġ�

����ǰ��ҳ�����
һ��������ǰǰ���ҳ����������ǳ����֡��ң�����ĵ��ֲ����Ǻܿ��
�ʶ�����ȡֻ����ǰn��ҳ���ݵķ�ʽ�����ң�ÿҳ�����ݲ��ᳬ��9��

20100325 ����֧�ִ��������Ǹ����ƣ��Ժ���Կ���
*/

#include "spim_ch_stroke.h"
#include "spim_ch_stroke_internal.h"



/*lint -e64*/


//�ʻ���unicode
//������ʻ�û�г��õ����Σ���Ҫ�����ֿ����
// 1 ��(һ) 2 ��(ح) 3 Ʋ(د) 4 ��(ؼ) 5 ��(�^(0x4e5b) ��(0x30d5) ��(0x4e59)) 6 �ʺ�()
static SPIM_UNICODE_T const s_stroke_symbol[] =
{
    0x4e00, 0x4e28, 0x4e3f, 0x4e36, 0x30d5, 0xff1f,
};


//�������ıʻ������ֵ�
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
Description: ģ���Ƚ������ֱʻ������봮��ģ�����־����ʺſ�ƥ�����бʻ�
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �ʺž��� 6
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
Description: ��ָ����������ʼ������һ�����ϱʻ�����
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: δ���ҵ�ƥ����֣��򷵻�-1
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
Description: ������������һ���ʻ�����û���������ܾ�����
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ��������ݱ����ڲ��������������Ա����������
==============================================================================*/
static void TestInput(SPIM_CH_STROKE_INFO_T *info_ptr, SPIM_UINT8 page)
{
    SPIM_DICT_INDEX_T next_idx = 0;

    SPIM_ASSERT(NULL != info_ptr);

    //��ҳ��������������ҳ����
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
    //��Ҫ��������
    //
    //�״�
    if (SPIM_IDX_BUF_PAGE_CNT == page)
    {
        if (info_ptr->test.cand_cnt == info_ptr->cand_cnt_per_page)
        {
            next_idx = SearchIndex(info_ptr, next_idx);

            //������һҳ��Ϣ
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
            //����һҳ
            info_ptr->buffer.first_idx[0] = info_ptr->test.cand[0];
            info_ptr->buffer.page_cnt = 1;
        }
    }
    //���һҳ
    else if (info_ptr->buffer.page_cnt == page + 1)
    {
        if (info_ptr->test.cand_cnt == info_ptr->cand_cnt_per_page)
        {
            next_idx = SearchIndex(info_ptr, next_idx);

            //������һҳ��Ϣ
            if (-1 != next_idx)
            {
                info_ptr->buffer.first_idx[info_ptr->buffer.page_cnt] = next_idx;
                info_ptr->buffer.page_cnt++;

                info_ptr->test.is_searching = 1;
            }
        }
    }
    //�м�ҳ
    else
    {
        //�϶�����һҳ
        info_ptr->test.is_searching = 1;
    }


    return;
}



/*==============================================================================
Function Name: ReOutput
Description: �����������
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
Description: ����һ���ʻ�����������ӵıʻ�������Ӧ�ĺ���
    �������
    ����������
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

    //���Լ�������
    info_ptr->input.input[info_ptr->input.input_len] = c;
    info_ptr->input.input_len++;

    //�����Ƿ��������
    TestInput(info_ptr, SPIM_IDX_BUF_PAGE_CNT);

    //�����룬��ܾ�����
    if (0 == info_ptr->test.cand_cnt)
    {
        info_ptr->input.input_len--;
        res = SPIM_ERROR;
    }
    //�����룬���������
    else
    {
        info_ptr->output = info_ptr->test;
        info_ptr->buffer.cur_page = 0;
    }

    return res;
}


/*==============================================================================
Function Name: RemoveStroke
Description: ɾ�����һ���ʻ���ͬʱ�������
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveStroke(SPIM_CH_STROKE_INFO_T *info_ptr)
{
    //û������
    if (0 == info_ptr->input.input_len)
    {
        return SPIM_ERROR;
    }

    //ȥ�����һ������
    info_ptr->input.input_len--;

    //û��ɾ��
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
Description: ɾ�����еıʻ������ظ�����ʼ״̬
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_RESULT_T RemoveAll(SPIM_CH_STROKE_INFO_T *info_ptr)
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
Description: ����������ݵ�����ѡ���ѡ��ѡ��֮�󣬻ظ�������״̬
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �����ֲ��֣���Ӧ�����
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

    //������
    SPIM_MEMSET(&(info_ptr->output), 0, sizeof(info_ptr->output));
    SPIM_MEMSET(&(info_ptr->buffer), 0, sizeof(info_ptr->buffer));

    return SPIM_NO_ERROR;
}


/*==============================================================================
Function Name: PreviousPage
Description: ǰһҳ
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: ǰ��ҳ�����ڲ��������ʱ���Ѿ�����
==============================================================================*/
static SPIM_RESULT_T PreviousPage(SPIM_CH_STROKE_INFO_T *info_ptr)
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
Note: ǰ��ҳ�����ڲ��������ʱ���Ѿ�����
==============================================================================*/
static SPIM_RESULT_T NextPage(SPIM_CH_STROKE_INFO_T *info_ptr)
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
Function Name: GenContent
Description: ������Ӧ�ý������������
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
Description: ��ʼ��ʵ��
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

    //Ĭ�ϼ�������
    info_ptr->data_ptr = s_data_chs;
    info_ptr->data_size = SPIM_ARR_SIZE(s_data_chs);

    info_ptr->cand_cnt_per_page = para_ptr->width_win / (para_ptr->width_chr + para_ptr->width_spc);

    SPIM_ASSERT(info_ptr->cand_cnt_per_page > 0);


    return info_ptr;
}



/*==============================================================================
Function Name: SPIM_CH_STROKE_term
Description: �ͷ�ʵ��
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
Description: �¼�����
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









