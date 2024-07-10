/*==============================================================================
File Name: spim_ch.h
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

#ifndef _SPIM_CH_PY_INTERNAL_H_
#define _SPIM_CH_PY_INTERNAL_H_

#include "spim_type.h"



//多音字最大读音数
#define SPIM_PY_MP_MAX (5)
//单音节字拼音最大字母数
#define SPIM_PY_SIMPLEX_MAX (6)
//索引的声母标识，0 - 非声母， 1 - 声母
#define SPIM_IDX_FLG_MASK_IC (0x80)
//索引的分段标识，0 - 非分段， 1 - 分段
#define SPIM_IDX_FLG_MASK_SEG (0x40)
//输出段的分隔符号
#define SPIM_COM_SEG_SYMBOL ('*')


#define SPIM_GET_MP_CNT(idx) (((0xe000 & idx) >> 13) + 1)
#define SPIM_GET_PY_IDX(idx) (0x1fff & idx)
#define SPIM_GET_FLG_IC(flg) (flg & SPIM_IDX_FLG_MASK_IC)
#define SPIM_GET_FLG_SEG(flg) (flg & SPIM_IDX_FLG_MASK_SEG)



typedef SPIM_UINT16 SPIM_PY_IDX_T;
typedef SPIM_INT32 SPIM_PY_DICT_IDX_T;


typedef struct
{
    void* data_ptr;
    SPIM_UINT32 size;
} SPIM_CH_DICT_CFG_T;

enum
{
    SPIM_DICT_IDX_NONE,
    SPIM_DICT_IDX_USER,
    SPIM_DICT_IDX_SYS
};
typedef SPIM_UINT8 SPIM_DICT_IDX_T;


//const
extern SPIM_INT16 const *spim_ch_py_list[];
extern SPIM_UINT32 const spim_ch_py_list_size;
extern SPIM_INT8 const *spim_ch_py_list_9k[];
extern SPIM_UINT32 const spim_ch_py_list_9k_size;
extern SPIM_INT16 const *spim_ch_py_ic[];
extern SPIM_UINT32 const spim_ch_py_ic_size;
extern SPIM_INT16 const spim_ch_py_cnt_chs[];
extern SPIM_UINT32 const spim_ch_py_cnt_chs_size;
extern SPIM_CH_DICT_CFG_T const spim_ch_py_dict_cfg[];
extern SPIM_UINT32 const spim_ch_py_dict_cfg_size;
extern SPIM_CH_DICT_CFG_T const spim_ch_dict_cfg[];
extern SPIM_UINT32 const spim_ch_dict_cfg_size;
extern SPIM_INT8 const spim_ch_py_key_name[];
extern SPIM_UINT32 const spim_ch_py_key_name_size;

#ifdef IM_TRAD_CHINESE_SUPPORT
//zhuyin
extern SPIM_INT8 const spim_ch_zy_26key_name[];
extern SPIM_INT8 const spim_ch_zy_9key_name[];
extern SPIM_INT8 const *spim_ch_zy_list_26k[];
extern SPIM_INT8 const *spim_ch_zy_list_9k[];
extern SPIM_INT16 const *spim_ch_zy_list[];
extern SPIM_INT16 const *spim_ch_zy_ic[];
extern SPIM_INT16 const spim_ch_zy_cnt_chs[];
extern SPIM_CH_DICT_CFG_T const spim_ch_zy_key_dict_cfg[];
extern SPIM_CH_DICT_CFG_T const spim_ch_zy_dict_cfg[];
extern SPIM_UINT32 const spim_ch_zy_list_26k_size;
extern SPIM_UINT32 const spim_ch_zy_list_9k_size;
extern SPIM_UINT32 const spim_ch_zy_26key_name_size;
extern SPIM_UINT32 const spim_ch_zy_9key_name_size;
extern SPIM_UINT32 const spim_ch_zy_list_size;
extern SPIM_UINT32 const spim_ch_zy_ic_size;
extern SPIM_UINT32 const spim_ch_zy_cnt_chs_size;
extern SPIM_UINT32 const spim_ch_zy_key_dict_cfg_size;
extern SPIM_UINT32 const spim_ch_zy_dict_cfg_size;
#endif

//function
/*==============================================================================
Function Name: FindStringIndex
Description: 查找字符串在字符串列表中的位置
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_INT32 FindStringIndex(
    SPIM_INT16 const *str_list[],
    SPIM_UINT32 list_size,
    SPIM_INT16 const * input
);


/*==============================================================================
Function Name: IsPyIndexMatchChar
Description: 检查拼音的索引是否与指定汉字匹配
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 多音字匹配
==============================================================================*/
SPIM_INT32 IsPyIndexMatchChar(
    SPIM_INT16 const *ch_pronc_ptr[],
    SPIM_PY_IDX_T py_idx,
    SPIM_CH_DICT_CFG_T const ch_key_dict_cfg[],
    SPIM_UNICODE_T chr_uni,
    SPIM_UINT8 ic_flag
);

/*==============================================================================
Description: 宽串比较
Author: hua.fang
Global resource dependence: NONE
Note: 
==============================================================================*/
SPIM_INT32 SPIM_WcsCmp(SPIM_UNICODE_T *str1, SPIM_UNICODE_T *str2);

/*==============================================================================
Description: 宽串的字符串连接
Author: hua.fang
Global resource dependence: NONE
Note: 
==============================================================================*/
SPIM_UNICODE_T *SPIM_WcsCat(SPIM_UNICODE_T * str1, SPIM_UNICODE_T * str2);

/*==============================================================================
Description: 求宽串长度
Author: hua.fang
Global resource dependence: NONE
Note: 
==============================================================================*/
SPIM_UINT32 SPIM_WcsLen(SPIM_UNICODE_T *str);



#endif //_SPIM_CH_PY_INTERNAL_H_



