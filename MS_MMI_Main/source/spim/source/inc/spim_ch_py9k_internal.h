/*==============================================================================
File Name: spim_ch_py9k_internal.h
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

#ifndef _SPIM_CH_PY_9K_INTERNAL_H_
#define _SPIM_CH_PY_9K_INTERNAL_H_

#include "spim_type.h"

#include "spim_ch_py9k.h"


// idx_flag ����������ƥ�����ֳ��ȵĲ���
#define SPIM_CH_PYW_9K_IDX_LEN_MASK (0x1F)
// �� idx ��ƴ�������ţ�����ƥ������ִ��� 0 - ƴ������ 1-ƥ������
#define SPIM_CH_PYW_9K_IDX_NUM_FLAG_MASK (0x20)


/*
��Ϊֻ��ƥ��9k��ʱ�򣬳�����δ֪�ģ��Ҳ���ȷ����
match_idx_count - ƥ�������ĸ��������е�������һ��
*/
typedef struct
{
    SPIM_DICT_IDX_T dict;
    SPIM_UINT8 page;            // ��ʵ�Ǵʳ� - 1
    SPIM_UINT16 idx;
    SPIM_UINT8 match_idx_count;
    SPIM_UINT8 match_num_count;
} GUIME_CH_PYW_CAND_9K_T;

//core data(hide)
typedef struct
{
    SPIM_INT8 input[SPIM_INPUT_LEN_MAX];  //���ִ��������ԭʼ����
    SPIM_UINT8 input_len; //���볤�ȣ���input��len
    SPIM_PY_IDX_T idx[SPIM_INPUT_LEN_MAX]; //�Ѿ�ȷ�������֣����Ѿ�ת������ƴ�������� 1 ָ���ģ�ƴ�������� 2 ƥ��ģ��������
    SPIM_UINT8 idx_flag[SPIM_INPUT_LEN_MAX]; // ��ͨ��py candָ���ģ�����ƥ��Ĵ������ǵ�����
    SPIM_UINT8 idx_cnt; //����������ָ����һ����һ����ƥ���һ����һ��
    SPIM_UINT8 idx_confirm_len; //�������������볤��
    GUIME_CH_PYW_CAND_9K_T confirm[SPIM_INPUT_LEN_MAX]; //�ύ����  // �Ѿ�ȷ��Ϊ���ֵĺ�ѡ��
    SPIM_UINT8 confirm_cnt; //ȷ�ϵĺ��ֵĵ�λ����һ������һ����λ  // �Ѿ�ȷ��Ϊ���ֵĸ���
    SPIM_UINT8 confirm_idx_cnt; //ȷ�ϵ���������Ŀ��һ��ƴ����һ����һ��ģ��ƥ����ַ�Ҳ��һ��
} SPIM_CH_PYW_INPUT_9K_T;

typedef struct
{
    GUIME_CH_PYW_CAND_9K_T cand[SPIM_INPUT_LEN_MAX];
    SPIM_UINT8 cand_cnt;        // ��ѡ�ֵĸ���
    SPIM_UINT16 cand_win_width;
    SPIM_INT8 is_searching;     // 0�����Ѿ��ұ������е��֣�1��������û�б�ѡ��������´ο��Լ���
} SPIM_CH_PYW_OUTPUT_9K_T;


typedef struct
{
    SPIM_PY_IDX_T idx[SPIM_CH_PY9K_COMBINE_MAX];    // ��ѡƴ����9��ƴ�����е�����
    SPIM_UINT8 cnt;                                 // ���ҵ�������������Ҳ�������������ƫ��
} SPIM_CH_PYW_9K_PY_CAND_T;

/*
first idx ����ÿ����ʾҳ���׸��������������������棬ͨ�������õ�
*/
typedef struct
{
    GUIME_CH_PYW_CAND_9K_T first_idx[SPIM_IDX_BUF_PAGE_CNT];
    SPIM_UINT8 page_cnt;
    SPIM_UINT8 cur_page;
} SPIM_CH_PYW_BUFFER_9K_T;

typedef struct
{
    SPIM_INIT_PARAMETER_T init_para;        //��ʼ������

    SPIM_CH_PYW_INPUT_9K_T input;           //����

    SPIM_CH_PYW_9K_PY_CAND_T py_cand;       //��ѡ

    SPIM_CH_PYW_OUTPUT_9K_T output;         //���
    SPIM_CH_PYW_OUTPUT_9K_T test;

    SPIM_CH_PYW_BUFFER_9K_T buffer;         //��ҳ��

    SPIM_CH_DICT_CFG_T const *dict_cfg;     // �ֵ�����
    SPIM_UINT32 dict_size;                  // �ֵ���

    SPIM_INT16 const *py_cnt;               //��ǰ�ַ����ĸ�ƴ���¡��ֵĸ�����
    SPIM_INT16 const **ch_pronc_pptr;       //��ǰ�ַ����ĸ�����
    SPIM_INT8 const **ch_key_list_pptr;     //��ǰ�ַ����µĸ�����λ
    SPIM_UINT32 ch_pronc_size;              //�ж��ٵĶ������������ߵĸ�����Ӧ��ͬ

    SPIM_INT8 const *key_name_ptr;          //��ֵ
    SPIM_UINT32 key_name_size;              //��ֵ����

    SPIM_INT16 const **ch_ic_pptr;          //������
    SPIM_UINT32 ch_ic_size;                 //��������

    SPIM_CH_DICT_CFG_T const *ch_key_dict_cfg_ptr;  //�ַ����ļ�λ
    SPIM_UINT32 ch_dict_cfg_size;                   //����

    SPIM_INT8 separator_char;               //�ָ���

} SPIM_PY9K_INFO_T;






#endif //_SPIM_CH_PY_9K_INTERNAL_H_



