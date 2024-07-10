/*==============================================================================
File Name: spim_ch_asso.h
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 联想字的对外函数
    联想字的多少，可以裁切数据库，大致，32个就足够了
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/18   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_CH_ASSO_H_
#define _SPIM_CH_ASSO_H_

#include "spim_type.h"

//最多返回 32 个联想字
#define SPIM_CH_ASSO_CNT_MAX (32)


/*==============================================================================
Function Name: SPIM_CH_QueryAsso
Description: SPIM_CH_QueryAsso
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_INT32 SPIM_CH_QueryAsso(
    SPIM_UNICODE_T last_ch,
    SPIM_UNICODE_T * asso_ptr, //[out]
    SPIM_INT32 * asso_cnt_ptr        //[in/out]
);



#endif //_SPIM_CH_ASSO_H_



