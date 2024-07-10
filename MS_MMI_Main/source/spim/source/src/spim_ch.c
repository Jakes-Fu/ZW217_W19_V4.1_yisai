/*==============================================================================
File Name: spim_ch.c
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 汉字的通用函数和数据
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

*/

#include "spim_ch.h"

/*lint -e64*/

// 可以接收的键值
SPIM_INT8 const spim_ch_py_key_name[] =
{
#include "..\\data\\chinese\\_py_key_name.src"
};

// valid pinyin
SPIM_INT16 const *spim_ch_py_list[] =
{
#include "..\\data\\chinese\\_pinyin.src"
};

// 与拼音一一对应的9key字符串
// 此处建立映射表，直接获取转化后的数字，减少检索中的操作，尝试增加速度
SPIM_INT8 const *spim_ch_py_list_9k[] =
{
#include "..\\data\\chinese\\_pinyin_9k.src"
};

// initial consonant
SPIM_INT16 const *spim_ch_py_ic[] =
{
    L"b", L"c", L"ch", L"d", L"f", L"g", L"h", L"j", L"k", L"l", L"m", L"n",
    L"p", L"q", L"r", L"s", L"sh", L"t", L"w", L"x", L"y", L"z", L"zh"
};

// count of the charaters in the same pinyin
SPIM_INT16 const spim_ch_py_cnt_chs[] =
{
#include "..\\data\\chinese\\_py_cnt_gb2312.src"
};

// 0x4e00..0x9fa5, pinyin
static SPIM_PY_IDX_T const py_all[] =
{
#include "..\\data\\chinese\\_chr_py_idx.src"
};

// multi-pronouce, 2
static SPIM_PY_IDX_T const py_mp2[][2] =
{
#include "..\\data\\chinese\\_chr_py_mp2.src"
};

// multi-pronouce, 3
static SPIM_PY_IDX_T const py_mp3[][3] =
{
#include "..\\data\\chinese\\_chr_py_mp3.src"
};

// multi-pronouce, 4
static SPIM_PY_IDX_T const py_mp4[][4] =
{
#include "..\\data\\chinese\\_chr_py_mp4.src"
};

// multi-pronouce, 5
static SPIM_PY_IDX_T const py_mp5[][5] =
{
#include "..\\data\\chinese\\_chr_py_mp5.src"
};

// character list, chs, order by use frequence
static SPIM_UNICODE_T const ch_chs_f[] =
{
#include "..\\data\\chinese\\_chrset_gb2312.src"
};

//字数在八个以上的直接无视
static SPIM_UNICODE_T const word_chs_f2[][2] =
{
#include "..\\data\\chinese\\_word2.src"
};

static SPIM_UNICODE_T const word_chs_f3[][3] =
{
#include "..\\data\\chinese\\_word3.src"
};

static SPIM_UNICODE_T const word_chs_f4[][4] =
{
#include "..\\data\\chinese\\_word4.src"
};

static SPIM_UNICODE_T const word_chs_f5[][5] =
{
#include "..\\data\\chinese\\_word5.src"
};

static SPIM_UNICODE_T const word_chs_f6[][6] =
{
#include "..\\data\\chinese\\_word6.src"
};

static SPIM_UNICODE_T const word_chs_f7[][7] =
{
#include "..\\data\\chinese\\_word7.src"
};

static SPIM_UNICODE_T const word_chs_f8[][8] =
{
#include "..\\data\\chinese\\_word8.src"
};


SPIM_CH_DICT_CFG_T const spim_ch_py_dict_cfg[] =
{
    {(void*)py_all, SPIM_ARR_SIZE(py_all)},
    {(void*)py_mp2, SPIM_ARR_SIZE(py_mp2)},
    {(void*)py_mp3, SPIM_ARR_SIZE(py_mp3)},
    {(void*)py_mp4, SPIM_ARR_SIZE(py_mp4)},
    {(void*)py_mp5, SPIM_ARR_SIZE(py_mp5)},
};


SPIM_CH_DICT_CFG_T const spim_ch_dict_cfg[] =
{
    {(void*)ch_chs_f, SPIM_ARR_SIZE(ch_chs_f)},
    {(void*)word_chs_f2, SPIM_ARR_SIZE(word_chs_f2)},
    {(void*)word_chs_f3, SPIM_ARR_SIZE(word_chs_f3)},
    {(void*)word_chs_f4, SPIM_ARR_SIZE(word_chs_f4)},
    {(void*)word_chs_f5, SPIM_ARR_SIZE(word_chs_f5)},
    {(void*)word_chs_f6, SPIM_ARR_SIZE(word_chs_f6)},
    {(void*)word_chs_f7, SPIM_ARR_SIZE(word_chs_f7)},
    {(void*)word_chs_f8, SPIM_ARR_SIZE(word_chs_f8)},
};


SPIM_UINT32 const spim_ch_py_list_size = SPIM_ARR_SIZE(spim_ch_py_list);
SPIM_UINT32 const spim_ch_py_list_9k_size = SPIM_ARR_SIZE(spim_ch_py_list_9k);
SPIM_UINT32 const spim_ch_py_ic_size = SPIM_ARR_SIZE(spim_ch_py_ic);
SPIM_UINT32 const spim_ch_py_cnt_chs_size = SPIM_ARR_SIZE(spim_ch_py_cnt_chs);
SPIM_UINT32 const spim_ch_py_dict_cfg_size = SPIM_ARR_SIZE(spim_ch_py_dict_cfg);
SPIM_UINT32 const spim_ch_dict_cfg_size = SPIM_ARR_SIZE(spim_ch_dict_cfg);
SPIM_UINT32 const spim_ch_py_key_name_size = SPIM_ARR_SIZE(spim_ch_py_key_name);

#ifdef IM_TRAD_CHINESE_SUPPORT

// 可以接收的键值
SPIM_INT8 const spim_ch_zy_26key_name[] =
{
#include "..\\data\\chinese\\_zy_key_name_26key.src"
};

SPIM_INT8 const spim_ch_zy_9key_name[] =
{
#include "..\\data\\chinese\\_zy_key_name_9key.src"
};

// 与拼音一一对应的9key字符串
// 此处建立映射表，直接获取转化后的数字，减少检索中的操作，尝试增加速度
SPIM_INT8 const *spim_ch_zy_list_26k[] =
{
#include "..\\data\\chinese\\zhuyin_26k.src"
};

SPIM_INT8 const *spim_ch_zy_list_9k[] =
{
#include "..\\data\\chinese\\zhuyin_9k.src"
};

// valid zhuyin
SPIM_INT16 const *spim_ch_zy_list[] =
{
#include "..\\data\\chinese\\_zhuyin.src"
};

// initial consonant
SPIM_INT16 const *spim_ch_zy_ic[] =
{
    L"\x3105", L"\x3106", L"\x3107", L"\x3108",
    L"\x3109", L"\x310a", L"\x310b", L"\x310c",
    L"\x310d", L"\x310e", L"\x310f",
    L"\x3110", L"\x3111", L"\x3112",
    L"\x3113", L"\x3114", L"\x3115", L"\x3116",
    L"\x3117", L"\x3118", L"\x3119",
    L"\x311a", L"\x311b", L"\x311c", L"\x311d",
    L"\x311e", L"\x311f", L"\x3120", L"\x3121",
    L"\x3122", L"\x3123", L"\x3124", L"\x3125", L"\x3126",
    L"\x3127", L"\x3128", L"\x3129",
};

// count of the charaters in the same zhuyin
SPIM_INT16 const spim_ch_zy_cnt_chs[] =
{
#include "..\\data\\chinese\\_zy_cnt.src"
};

// 0x4e00..0x9fa5, zhuyin
static SPIM_PY_IDX_T const zy_all[] =
{
#include "..\\data\\chinese\\_chr_zy_idx.src"
};

// multi-pronouce, 2
static SPIM_PY_IDX_T const zy_mp2[][2] =
{
#include "..\\data\\chinese\\_chr_zy_mp2.src"
};

// multi-pronouce, 3
static SPIM_PY_IDX_T const zy_mp3[][3] =
{
#include "..\\data\\chinese\\_chr_zy_mp3.src"
};

// multi-pronouce, 4
static SPIM_PY_IDX_T const zy_mp4[][4] =
{
#include "..\\data\\chinese\\_chr_zy_mp4.src"
};

// multi-pronouce, 5
static SPIM_PY_IDX_T const zy_mp5[][5] =
{
#include "..\\data\\chinese\\_chr_zy_mp5.src"
};

// multi-pronouce, 6
static SPIM_PY_IDX_T const zy_mp6[][6] =
{
#include "..\\data\\chinese\\_chr_zy_mp6.src"
};

// word
static SPIM_UNICODE_T const zy_word_chs_f2[][2] =
{
#include "..\\data\\chinese\\zy_word_2.src"
};

static SPIM_UNICODE_T const zy_word_chs_f3[][3] =
{
#include "..\\data\\chinese\\zy_word_3.src"
};

static SPIM_UNICODE_T const zy_word_chs_f4[][4] =
{
#include "..\\data\\chinese\\zy_word_4.src"
};

static SPIM_UNICODE_T const zy_word_chs_f5[][5] =
{
#include "..\\data\\chinese\\zy_word_5.src"
};

static SPIM_UNICODE_T const zy_word_chs_f6[][6] =
{
#include "..\\data\\chinese\\zy_word_6.src"
};

// character list, chs, order by use frequence
static SPIM_UNICODE_T const ch_chs_zy_f[] =
{
#include "..\\data\\chinese\\_chrset_zy.src"
};


SPIM_CH_DICT_CFG_T const spim_ch_zy_key_dict_cfg[] =
{
    {(void*)zy_all, SPIM_ARR_SIZE(zy_all)},
    {(void*)zy_mp2, SPIM_ARR_SIZE(zy_mp2)},
    {(void*)zy_mp3, SPIM_ARR_SIZE(zy_mp3)},
    {(void*)zy_mp4, SPIM_ARR_SIZE(zy_mp4)},
    {(void*)zy_mp5, SPIM_ARR_SIZE(zy_mp5)},
    {(void*)zy_mp6, SPIM_ARR_SIZE(zy_mp6)},
};

SPIM_CH_DICT_CFG_T const spim_ch_zy_dict_cfg[] =
{
    {(void*)ch_chs_zy_f, SPIM_ARR_SIZE(ch_chs_zy_f)},
    {(void*)zy_word_chs_f2, SPIM_ARR_SIZE(zy_word_chs_f2)},
    {(void*)zy_word_chs_f3, SPIM_ARR_SIZE(zy_word_chs_f3)},
    {(void*)zy_word_chs_f4, SPIM_ARR_SIZE(zy_word_chs_f4)},
    {(void*)zy_word_chs_f5, SPIM_ARR_SIZE(zy_word_chs_f5)},
    {(void*)zy_word_chs_f6, SPIM_ARR_SIZE(zy_word_chs_f6)},
};


SPIM_UINT32 const spim_ch_zy_list_26k_size = SPIM_ARR_SIZE(spim_ch_zy_list_26k);
SPIM_UINT32 const spim_ch_zy_list_9k_size = SPIM_ARR_SIZE(spim_ch_zy_list_9k);
SPIM_UINT32 const spim_ch_zy_26key_name_size = SPIM_ARR_SIZE(spim_ch_zy_26key_name);
SPIM_UINT32 const spim_ch_zy_9key_name_size = SPIM_ARR_SIZE(spim_ch_zy_9key_name);
SPIM_UINT32 const spim_ch_zy_list_size = SPIM_ARR_SIZE(spim_ch_zy_list);
SPIM_UINT32 const spim_ch_zy_ic_size = SPIM_ARR_SIZE(spim_ch_zy_ic);
SPIM_UINT32 const spim_ch_zy_cnt_chs_size = SPIM_ARR_SIZE(spim_ch_zy_cnt_chs);
SPIM_UINT32 const spim_ch_zy_key_dict_cfg_size = SPIM_ARR_SIZE(spim_ch_zy_key_dict_cfg);
SPIM_UINT32 const spim_ch_zy_dict_cfg_size = SPIM_ARR_SIZE(spim_ch_zy_dict_cfg);

#endif

/*lint +e64*/



/*==============================================================================
Function Name: FindStringIndex
Description: 查找给定字符串在字符串列表中的索引
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 无相应字符串，则返回-1
==============================================================================*/
SPIM_INT32 FindStringIndex(
    SPIM_INT16 const *str_list[],
    SPIM_UINT32 list_size,
    SPIM_INT16 const * input
)
{
    SPIM_UINT32 str_idx = 0;

    for (str_idx = 0; str_idx < list_size; str_idx++)
    {
        if (0 == SPIM_WcsCmp((SPIM_UNICODE_T *)input, (SPIM_UNICODE_T *)str_list[str_idx]))
        {
            break;
        }
    }

    return (str_idx < list_size ? (SPIM_INT32)str_idx : -1);
}



/*==============================================================================
Function Name: IsPyIndexMatchChar
Description: 检查拼音索引是否与指定汉字匹配
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 索引字匹配
==============================================================================*/
SPIM_INT32 IsPyIndexMatchChar(
    SPIM_INT16 const *ch_pronc_ptr[],
    SPIM_PY_IDX_T input_py_idx,
    SPIM_CH_DICT_CFG_T const ch_key_dict_cfg[],
    SPIM_UNICODE_T chr_uni,
    SPIM_UINT8 ic_flag
)
{
    SPIM_PY_IDX_T chr_py = 0;
    SPIM_UINT16 mp_cnt = 0;
    SPIM_PY_IDX_T py_idx = 0;
    SPIM_PY_IDX_T const *mps_ptr = NULL;
    SPIM_UINT8 mp_i = 0;
    SPIM_INT32 is_match = 0;

    SPIM_ASSERT(PNULL != ch_pronc_ptr);
    SPIM_ASSERT(PNULL != ch_key_dict_cfg);

    chr_py = ((SPIM_PY_IDX_T*)(ch_key_dict_cfg[0].data_ptr))[chr_uni - 0x4e00];
    mp_cnt = (SPIM_UINT16)SPIM_GET_MP_CNT(chr_py);
    py_idx = (SPIM_PY_IDX_T)SPIM_GET_PY_IDX(chr_py);

    if (1 == mp_cnt)
    {
        mps_ptr = &py_idx;
    }
    else
    {
        SPIM_ASSERT(py_idx < ch_key_dict_cfg[mp_cnt-1].size);

        mps_ptr = ch_key_dict_cfg[mp_cnt-1].data_ptr;
        mps_ptr += py_idx * mp_cnt;
    }

    //输入的拼音索引与第char_i字的所有读音比较
    for (mp_i = 0; mp_i < mp_cnt; mp_i++)
    {
        if (input_py_idx == mps_ptr[mp_i])
        {
            is_match = 1;
            break;
        }

        //判定输入的拼音是不是该字的声母
        //先做简单的声母判定
        if (ic_flag)
        {
            //可以肯定会有首字母
            if (ch_pronc_ptr[input_py_idx][0] == ch_pronc_ptr[mps_ptr[mp_i]][0])
            {
                //有第二个字母，但不一样
                if (ch_pronc_ptr[input_py_idx][1]
                        && ch_pronc_ptr[input_py_idx][1] != ch_pronc_ptr[mps_ptr[mp_i]][1])
                {
                    continue;
                }

                is_match = 1;
                break;
            }
        }
    }


    return is_match;
}

/*==============================================================================
Description: 宽串比较
Author: hua.fang
Global resource dependence: NONE
Note: 
==============================================================================*/
SPIM_INT32 SPIM_WcsCmp(SPIM_UNICODE_T *str1, SPIM_UNICODE_T *str2)
{
    while (*str1)
    {
        if (*str1 > *str2)
        {
            return 1;
        }

        if (*str1++ < *str2++)
        {
            return -1;
        }
    }

    /* if we have not gotten to the end of str2 by the time we have
        gotten to the end of the str1, then str1 is less than str2 */
    if (*str2)
    {
        return -1;
    }

    return 0;
}

/*==============================================================================
Description: 宽串的字符串连接
Author: hua.fang
Global resource dependence: NONE
Note: 
==============================================================================*/
SPIM_UNICODE_T *SPIM_WcsCat(SPIM_UNICODE_T * str1, SPIM_UNICODE_T * str2)
{
   SPIM_UNICODE_T *cp = str1;

   while (*cp)    /* find null at the end of str1 */
   {
      cp++;
   }

   while (*str2)  /* copy str2 onto end */
   {
      *cp++ = *str2++;
   }

   *cp = 0;       /* null terminate */

   return str1;
}


/*==============================================================================
Description: 求宽串长度
Author: hua.fang
Global resource dependence: NONE
Note: 
==============================================================================*/
SPIM_UINT32 SPIM_WcsLen(SPIM_UNICODE_T *str)
{
    SPIM_UINT32 len = 0;

    if (NULL != str)
    {
        while (str[len]) len++;
    }

    return len;
}

