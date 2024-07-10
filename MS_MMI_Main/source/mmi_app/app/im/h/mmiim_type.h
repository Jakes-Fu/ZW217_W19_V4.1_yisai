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

//���������ҳ��
#define SPIM_IDX_BUF_PAGE_CNT (100)


//���Ͷ���
//unicode��16λ�޷�����
typedef unsigned short SPIM_UNICODE_T;
//ͨ�õ�handle����
typedef void* SPIM_HANDLE_T;


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
typedef unsigned int SPIM_RESULT_T;


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
    SPIM_EVENT_GET_CONTENT,

    SPIM_EVENT_MAX
};
typedef unsigned int SPIM_EVENT_T;


//�¼�����
typedef union
{
    char append;
    char select;
} SPIM_EVENT_PARA_T;


//��ҳ��ʶ����
enum
{
    SPIM_PAGE_FLAG_NEXT = 0x01,
    SPIM_PAGE_FLAG_PREVIOUS = 0x02,
};
typedef unsigned int SPIM_PAGE_FLAG_T;


#endif //_SPIM_TYPE_H_


