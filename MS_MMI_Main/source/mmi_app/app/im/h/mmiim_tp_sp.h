/*****************************************************************************
** File Name:      mmiim_tp_sp.h                                             *
** Author:                                                                   *
** Date:           2010/08/16                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle pb menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/08/16     jian.ma            Create                                  *
******************************************************************************/

#ifndef _MMIIM_TP_CSTAR_H_
#define _MMIIM_TP_CSTAR_H_

#include "spim_type.h"

#define SPIM_PY_SEG_SYMBOL ('\'')


//output data
/*
commit - �ύ�ĺ��֣������洢��
commit_cnt - �ύ�ĺ��ֵĸ���
composite - ������ַ�����ʾ����ǰ��һ���ֵĺ��ֺ����һ����δȷ�ϵ�ƴ����϶���
    ��'\0'����������"������\0pypypypy"�����ԣ������'\0'��ͷ���������֡���ƴ��
    ���Զ��ύ�����Բ��������ƴ�������
candidate - ��ѡ�֣��ÿո�ֿ�������"���� ���� �� �� �� ��"
page_flag - ��ҳ��ʶ����ʾ�Ƿ���ǰ��ҳ
*/
// typedef struct
// {
//     SPIM_INT32 has_data;
//     SPIM_UNICODE_T commit[SPIM_INPUT_LEN_MAX];
//     SPIM_UINT32 commit_cnt;
//     SPIM_UNICODE_T composite[SPIM_INPUT_LEN_MAX];
//     SPIM_UNICODE_T candidate[SPIM_CH_PY26K_CAND_MAX];
//     SPIM_UINT32 cand_cnt;
//     SPIM_PAGE_FLAG_T page_flag;
// } SPIM_CONTENT_T;

typedef struct
{
    /* Commit content*/
    unsigned short pCommit[64];
    /* pre-edit content*/
    unsigned short pPreBuffer[64];      /* buffer of pre-edit window*/
    unsigned short nPreEdit;            /* number of pre-edit content*/
    unsigned short iHotPreEdit;         /* the hotspot of pre-edit*/

    /* Candidate content*/
    unsigned short pCandBuffer[64];     /* buffer of candidate window*/
    unsigned short nCand;               /* number of candidate content*/
    unsigned short iHotCand;            /* the hotspot of candidate*/

    /* pychoice content*/
    unsigned short pChoiceBuffer[64];
    unsigned short nChoice;
    unsigned short iHotChoice;

    /* underline content*/
    unsigned short pUnderlineBuffer[64];
    unsigned short nUnderline;
    unsigned short iHotUnderline;

    /* user word content*/
    unsigned short pUserWordBuffer[64];
    unsigned short nUserWord;
    unsigned short iHotUserWord;

    /* underline caret pos begin from 0,*/
    unsigned short iCaretPos;
    unsigned short bShowCaret;

    /* Page up/down flag of Candidate and Pinyin-Choice window*/
    unsigned short flagPageCand;
    unsigned short flagPageChoice;

    /* state*/
    unsigned short flagState;

    /* Timer operator*/
    unsigned short flagTimer;

} MMIIM_IMM_CONTENT, *PMMIIM_IMM_CONTENT;
#endif//_MMIIM_TP_CSTAR_H_
