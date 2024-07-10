/*==============================================================================
File Name: spim_type.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: spim�Ļ������ݺ����ݽṹ���壬�˴�����ʹ��spim������ϵͳ
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_TYPE_H_
#define _SPIM_TYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "spim_general.h"

/* �������� */
enum
{
    //���������ҳ��
    SPIM_IDX_BUF_PAGE_CNT = 100,

    //��������ַ���
    SPIM_INPUT_LEN_MAX = 64,

    //���ύ���ַ���
    SPIM_COMIT_LEN_MAX = SPIM_INPUT_LEN_MAX,
    //Ԥ�����ַ�ƴ����󳤶�
    SPIM_PRE_EDIT_LEN_MAX = 2*SPIM_INPUT_LEN_MAX,
    //��ѡ��ƴ����󳤶�
    SPIM_CANDIDATE_LEN_MAX = 64,
    //��ѡƴ��ƴ����󳤶�
    SPIM_PY_CHOICE_LEN_MAX = 140,
    
    //��ѡ�ָ���
    SPIM_CANDIDATE_CNT_MAX = 16,

    //ƴ���ָ���ʾ��
    SPIM_PY_SEG_SYMBOL = '\'',

    //�ύ�����ַ��ı�ʶ
    SPIM_COMMIT_INPUT_FLAG = -1,
};

// ���뷨����
typedef enum
{
    SPIM_TYPE_PINYIN_9KEY = 0,
    SPIM_TYPE_PINYIN_26KEY,
    SPIM_TYPE_ZHUYIN_9KEY,
    SPIM_TYPE_ZHUYIN_26KEY
} SPIM_TYPE_E;


//���Ͷ���
//unicode��16λ�޷�����
typedef SPIM_UINT16 SPIM_UNICODE_T;

//ͨ�õ�handle����
typedef void* SPIM_HANDLE_T;

//�ֵ���������
typedef SPIM_INT32 SPIM_DICT_INDEX_T;



//��ͨ��spim����ֵ
enum
{
    //���÷���ֵ
    SPIM_NO_ERROR,
    SPIM_ERROR,

    //�ض���Ϣ�ķ���ֵ
    SPIM_PARA_ERROR,

    SPIM_UNKNOW_ERROR
};
typedef SPIM_UINT32 SPIM_RESULT_T;


enum
{
    SPIM_CAPITAL_LOWER,
    SPIM_CAPITAL_LEAD_UPPER,
    SPIM_CAPITAL_UPPER,
};
typedef SPIM_UINT8 SPIM_CAPITAL_MODE_T;

//spim����ͨ�¼��б�
enum
{
    SPIM_EVENT_CHAR_APPEND,
    SPIM_EVENT_CHAR_REMOVE,
    SPIM_EVENT_CHAR_REMOVE_ALL,
    SPIM_EVENT_CAND_SELECT,
    SPIM_EVENT_PINYIN_SELECT,
    SPIM_EVENT_PAGE_PREVIOUS,
    SPIM_EVENT_PAGE_NEXT,
    SPIM_EVENT_COMMIT_INPUT,
    SPIM_EVENT_SET_CAPITAL,
    SPIM_EVENT_GET_CONTENT,

    SPIM_EVENT_MAX
};
typedef SPIM_UINT32 SPIM_EVENT_T;


//�¼�����
typedef union
{
    SPIM_INT8 append;
    SPIM_INT8 select;
    SPIM_CAPITAL_MODE_T capital;
} SPIM_EVENT_PARA_T;


//��ҳ��ʶ����
enum
{
    SPIM_PAGE_FLAG_PREVIOUS = 0x01,
    SPIM_PAGE_FLAG_NEXT = 0x02
};
typedef SPIM_UINT32 SPIM_PAGE_FLAG_T;

/*
�򵥵ĳ�ʼ������ģʽ
�����ȿ��ַ�ʱ������Ӧ���ṩ�Ļ�ȡ�ַ�����ȵĺ����������ַ������
����ȿ��ַ�ʱ��Ϊ��Ч�ʣ�һ������ʹ���ַ����
!!! ͨ����Ϊֻ�к����ǵȿ��
*/
typedef SPIM_UINT32 (*SPIM_STRING_WIDTH_F)(SPIM_UNICODE_T *ptr, SPIM_UINT32 len);

typedef struct
{
    SPIM_UINT16 width_win;
    SPIM_UINT8 width_chr;
    SPIM_UINT8 width_spc;
    SPIM_STRING_WIDTH_F string_width_f;
    SPIM_TYPE_E type;
} SPIM_INIT_PARAMETER_T;

/*
ͨ�õ����뷨�����Ľṹ
������������뷨�п��ܻ���ֵ���

*/
typedef struct
{
    /* ��ȷ��ʶ�Ƿ���������� */
    SPIM_INT32 has_data;

    /* �ύ���� */
    SPIM_UNICODE_T commit[SPIM_COMIT_LEN_MAX];
    SPIM_UINT32 commit_len;

    /* Ԥ���벿�֣���'\0'�ָ� */
    SPIM_UNICODE_T pre_edit[SPIM_PRE_EDIT_LEN_MAX];
    SPIM_UINT32 pre_edit_cnt;

    /* ��ѡ���'\0'�ָ� */
    SPIM_UNICODE_T candidate[SPIM_CANDIDATE_LEN_MAX];
    SPIM_UINT32 cand_cnt;

    /* ƴ����ѡ���'\0'�ָ� */
    /* !!!�˴�Ϊ���ں˵ļ򵥴�����ƴ����ѡ��һ�θ��� */
    SPIM_UNICODE_T py_choice[SPIM_PY_CHOICE_LEN_MAX];
    SPIM_UINT32 py_cnt;

    /* ��ѡ�ֵķ�ҳ��־ */
    SPIM_PAGE_FLAG_T page_flag;
} SPIM_CONTENT_T;



#endif //_SPIM_TYPE_H_


