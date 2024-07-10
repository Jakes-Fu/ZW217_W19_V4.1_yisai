/*==============================================================================
File Name: spim_ch_asso.c
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
#include "spim_ch_asso.h"
#include "spim_ch_asso_internal.h"


//默认简体中文
SPIM_CH_ASSO_DICT_T const asso_dict[] =
{
#include "..\\data\\chinese\\_asso.src"
};


/*==============================================================================
Function Name: FindIndex
Description: FindIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 FindIndex(SPIM_UNICODE_T ch);



/*==============================================================================
Function Name: SPIM_CH_QueryAsso
Description: 查找联想字
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 在 asso_ptr - [out] 中填充 *asso_cnt_ptr - [in/out] 个联想字
==============================================================================*/
SPIM_INT32 SPIM_CH_QueryAsso(
    SPIM_UNICODE_T last_ch,
    SPIM_UNICODE_T * asso_ptr,
    SPIM_INT32 * asso_cnt_ptr
)
{
    SPIM_INT32 index = 0;
    SPIM_UNICODE_T const *pwc = NULL;
    SPIM_INT32 get_cnt = 0;
    SPIM_INT32 cnt = 0;
    SPIM_INT32 asso_i = 0;

    if (NULL == asso_ptr || NULL == asso_cnt_ptr)
    {
        return 0;
    }

    if (*asso_cnt_ptr  <= 0)
    {
        return 0;
    }

    if (*asso_cnt_ptr > SPIM_CH_ASSO_CNT_MAX)
    {
        *asso_cnt_ptr = SPIM_CH_ASSO_CNT_MAX;
    }

    index = FindIndex(last_ch);

    if (-1 != index)
    {
        pwc = asso_dict[index].asso;

        while (*pwc)
        {
            asso_ptr[cnt] = *pwc;
            pwc++;
            cnt++;

            if (cnt == *asso_cnt_ptr) break;
        }
    }

    get_cnt = cnt;

    //用默认联想字填充
    if (get_cnt < *asso_cnt_ptr)
    {
        pwc = asso_dict[0].asso;

        while (*pwc)
        {
            for (asso_i = 0; asso_i < get_cnt; asso_i++)
            {
                if (asso_ptr[asso_i] == *pwc) break;
            }

            if (asso_i == get_cnt)
            {
                asso_ptr[cnt] = *pwc;
                cnt++;

                if (cnt == *asso_cnt_ptr) break;
            }

            pwc++;
        }
    }

    SPIM_ASSERT(cnt == *asso_cnt_ptr);


    return (*asso_cnt_ptr);
}



/*==============================================================================
Function Name: FindIndex
Description: 根据当前的汉字，确定汉字的索引
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
static SPIM_INT32 FindIndex(SPIM_UNICODE_T ch)
{
    SPIM_INT32 index = -1;
    SPIM_INT32 low = 0, mid = 0, high = 0;

    low = 0;
    high = SPIM_ARR_SIZE(asso_dict) - 1;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (ch < asso_dict[mid].ch)
        {
            high = mid - 1;
        }
        else if (ch > asso_dict[mid].ch)
        {
            low = mid + 1;
        }
        else
        {
            index = mid;
            break;
        }
    }

    return index;
}








