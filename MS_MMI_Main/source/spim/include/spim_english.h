/*==============================================================================
File Name: spim_english.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: Ӣ�����뷨����ĺ���
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_SP_ENGLISH_H_
#define _SPIM_SP_ENGLISH_H_

#include "spim_spells.h"

//init
/*==============================================================================
Function Name: SPIM_SPELLS_InitEnglish
Description: ��ʼ��ʵ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_HANDLE_T SPIM_SPELLS_InitEnglish(SPIM_INIT_PARAMETER_T const *para_ptr);
//term
/*==============================================================================
Function Name: SPIM_SPELLS_Term
Description: �ͷ�ʵ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �˴����е�ƴдʽ������һ�µ�
==============================================================================*/
void SPIM_SPELLS_Term(SPIM_HANDLE_T handle);
//handle
/*==============================================================================
Function Name: SPIM_SPELLS_Handle
Description: ʵ���¼�����
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �˴����е�ƴдʽ������һ�µ�
==============================================================================*/
SPIM_RESULT_T SPIM_SPELLS_Handle(
    SPIM_HANDLE_T * handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    SPIM_CONTENT_T * content_ptr
);

#endif //_SPIM_SP_ENGLISH_H_





