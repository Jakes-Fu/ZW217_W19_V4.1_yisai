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
commit - 提交的汉字，连续存储的
commit_cnt - 提交的汉字的个数
composite - 输入的字符的显示，由前面一部分的汉字后后面一部分未确认的拼音组合而成
    用'\0'隔开，形如"字字字\0pypypypy"，所以，如果是'\0'开头，则是无字。无拼音
    则自动提交，所以不会出现无拼音的情况
candidate - 候选字，用空格分开。形如"词组 词组 字 字 字 字"
page_flag - 翻页标识，提示是否有前后页
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
