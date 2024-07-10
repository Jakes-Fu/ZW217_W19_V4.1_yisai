/***************************************************************************
** File Name:      mmiim_sp_ml9key_data.h                                  *
** Author:         hua.fang                                                *
** Date:           07/06/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2010        hua.fang         Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIIM_SP_ML9KEY_DATA_H_
#define _MMIIM_SP_ML9KEY_DATA_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_msg.h"
#include "mmiim_sp_basic.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// ��������
enum
{
    MMIIM_SP_9KEY_INDEX_1,
    MMIIM_SP_9KEY_INDEX_2,
    MMIIM_SP_9KEY_INDEX_3,
    MMIIM_SP_9KEY_INDEX_4,
    MMIIM_SP_9KEY_INDEX_5,
    MMIIM_SP_9KEY_INDEX_6,
    MMIIM_SP_9KEY_INDEX_7,
    MMIIM_SP_9KEY_INDEX_8,
    MMIIM_SP_9KEY_INDEX_9,
    MMIIM_SP_9KEY_INDEX_0,
    MMIIM_SP_9KEY_INDEX_STAR,
    MMIIM_SP_9KEY_INDEX_HASH,

    MMIIM_SP_9KEY_INDEX_MAX,
};

//����ĳ���ʵ�ڲ���һ���ð취���˷ѿռ䣬�޸������ȽϷ�������ʱ��������
//�����ڽ����ݱ��Ϊ��̬���ȽϺ�
enum
{
#if (defined IM_MYANMAR_SUPPORT && defined IM_ENGINE_CSTAR)	//CSTAR �İ���������϶�
    MMIIM_SP_9KEY_CHAR_MAX = 38 + 1,        //ÿ������������multi-tap�ַ����� + 1(�׸��ֽ�������ʾ�ַ�����)
#elif defined IM_RUSSIAN_SUPPORT
    MMIIM_SP_9KEY_CHAR_MAX = 37 + 1,        //ÿ������������multi-tap�ַ����� + 1(�׸��ֽ�������ʾ�ַ�����)
#elif ((defined IM_PERSIAN_SUPPORT && !defined SPECIAL_ARABIC_PERSIAN) || defined IM_URDU_SUPPORT)
    MMIIM_SP_9KEY_CHAR_MAX = 29 + 1,        //ÿ������������multi-tap�ַ����� + 1(�׸��ֽ�������ʾ�ַ�����)
#elif defined IM_VIETNAMESE_SUPPORT
    MMIIM_SP_9KEY_CHAR_MAX = 26 + 1,        //ÿ������������multi-tap�ַ����� + 1(�׸��ֽ�������ʾ�ַ�����)
#else
    MMIIM_SP_9KEY_CHAR_MAX = 22 + 1,        //ÿ������������multi-tap�ַ����� + 1(�׸��ֽ�������ʾ�ַ�����)
#endif    
};

// +1��ż�ֵ����
typedef wchar MMIIM_SP_9KEY_TABLE_T[MMIIM_SP_9KEY_INDEX_MAX][MMIIM_SP_9KEY_CHAR_MAX+1];
typedef MMIIM_SP_9KEY_TABLE_T const MMIIM_SP_9KEY_TABLE_DEF_T;

// function pointer which checks letter and change key table
typedef MMIIM_SP_9KEY_TABLE_DEF_T*(*CHECK_TABLE_FUNC_T)(wchar);


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*==============================================================================
Description: get key table by method index
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void const * MMIIM_SP_ML9KEY_GetKeyTable(
    GUIIM_METHOD_T method      // [in] method type
);

//�������������ʱ������Ҫ����˿ӡ
#if defined MMI_IM_PDA_SUPPORT
/*==============================================================================
Description: get key table by method index
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void const * MMIIM_SP_ML9KEY_GetPrintKeyTable(
    GUIIM_METHOD_T method      // [in] method type
);
#endif

/*==============================================================================
Description: get key table by method index
Author: yaoguang.chen
Global resource dependence: NONE
Note:   ͨ�����뷨������ȡ��ǰ����������Ӧ�����������룬����multitap
    ʹ��ͬһ�ű���Ϊ�˺��ڳ���ά���������
==============================================================================*/
PUBLIC void const * MMIIM_SP_Smart9Key_GetKeyTable(
    GUIIM_METHOD_T method      // [in] method type
);

//�������������ʱ������Ҫ����˿ӡ
#if defined MMI_IM_PDA_SUPPORT
/*==============================================================================
Description: get key table by method index
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void const * MMIIM_SP_Smart9KEY_GetPrintKeyTable(
    GUIIM_METHOD_T method      // [in] method type
);
#endif

/*==============================================================================
Description: get function pointer which checks special language letter.
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC CHECK_TABLE_FUNC_T MMIIM_SP_ML9KEY_GetCheckingFunc(
    GUIIM_METHOD_T method // [in] method type
);

/*==============================================================================
Function Name: MMIIM_SP_Check9KeyTable
Description: У���ֵ�����ȷ��
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_Check9KeyTable(
    void *table_ptr   // [in] key table
);

/*==============================================================================
Description: ��ȡָ�������µ����ü�ֵ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
uint16 MMIIM_SP_ML9KEY_GetChrCnt(void const * table_ptr, uint16 key_idx);

/*==============================================================================
Description: ��ȡָ�������µ�ָ����ֵ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: char_index��1��ʼ����
==============================================================================*/
uint16 MMIIM_SP_ML9KEY_GetChr(void const * table_ptr, uint16 key_idx, uint16 chr_idx);

/*==============================================================================
Description: �����ַ��õ���Ӧ�ļ�ֵ
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML9KEY_GetKeyIndexByChar(
                                                 void const *table_ptr,     // [in]
                                                 wchar  key_char,       // [in]
                                                 uint16 *key_index_ptr  // [out]
                                                 );

/*==============================================================================
Description: alloc memory for custom key table
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML9KEY_AllocCustomKb(
                                             void const *table_ptr,
                                             void  **custom_pptr
                                             );

/*==============================================================================
Description: free memory for custom key table
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML9KEY_FreeCustomKb(
                                            void *custom_ptr
                                            );

/*==============================================================================
Description: MMIIM_SP_ML9KEY_CustomKb
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ML9KEY_CustomKb(
    GUIIM_TYPE_T cur_im_type,
    GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr,
    void *custom_kb_ptr
);





/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
}
#endif

#endif  // _MMIIM_SP_ML9KEY_DATA_H_

