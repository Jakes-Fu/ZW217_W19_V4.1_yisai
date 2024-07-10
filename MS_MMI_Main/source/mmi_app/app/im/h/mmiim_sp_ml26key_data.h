/***************************************************************************
** File Name:      mmiim_sp_ml26key_data.h                                 *
** Author:         hua.fang                                                *
** Date:           07/06/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)

#ifndef _MMIIM_SP_ML26KEY_DATA_H_
#define _MMIIM_SP_ML26KEY_DATA_H_

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
//only PDA 40 keys keyboard need to use this, if use PHYSICAL keyboard and TP keyboard, here can't use 40key
//use 26key layout or 40key layout,customer could choose it.
#if (defined MMI_IM_PDA_SUPPORT && defined IM_ENGINE_CSTAR && defined IM_TRAD_CHINESE_SUPPORT)		
#define IM_TRAD_CHINESE_QWERT_40KEY
#endif

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// ����һ���Լ��İ�������
enum
{
    MMIIM_SP_26KEY_INDEX_A,
    MMIIM_SP_26KEY_INDEX_B,
    MMIIM_SP_26KEY_INDEX_C,
    MMIIM_SP_26KEY_INDEX_D,
    MMIIM_SP_26KEY_INDEX_E,
    MMIIM_SP_26KEY_INDEX_F,
    MMIIM_SP_26KEY_INDEX_G,
    MMIIM_SP_26KEY_INDEX_H,
    MMIIM_SP_26KEY_INDEX_I,
    MMIIM_SP_26KEY_INDEX_J,
    MMIIM_SP_26KEY_INDEX_K,
    MMIIM_SP_26KEY_INDEX_L,
    MMIIM_SP_26KEY_INDEX_M,
    MMIIM_SP_26KEY_INDEX_N,
    MMIIM_SP_26KEY_INDEX_O,
    MMIIM_SP_26KEY_INDEX_P,
    MMIIM_SP_26KEY_INDEX_Q,
    MMIIM_SP_26KEY_INDEX_R,
    MMIIM_SP_26KEY_INDEX_S,
    MMIIM_SP_26KEY_INDEX_T,
    MMIIM_SP_26KEY_INDEX_U,
    MMIIM_SP_26KEY_INDEX_V,
    MMIIM_SP_26KEY_INDEX_W,
    MMIIM_SP_26KEY_INDEX_X,
    MMIIM_SP_26KEY_INDEX_Y,
    MMIIM_SP_26KEY_INDEX_Z,

//#if defined MMI_IM_PDA_SUPPORT
    //�˴�ΪĳЩ���Բ�ʹ����ͨ���̰������ã����޷�ʹ��abc�ȼ�����ʾ����
    //���һЩ�����򰴼�����
    //�˰���������ABCֵ��ͬ����ֹ�绰����ѯ��ƫ�����ȳ�������
    MMIIM_SP_KEY_IRREGULAR = MMIIM_SP_26KEY_INDEX_A,
    MMIIM_SP_KEY_IRREGULAR_1 = MMIIM_SP_KEY_IRREGULAR,      //MMIIM_SP_26KEY_INDEX_A
    MMIIM_SP_KEY_IRREGULAR_2,
    MMIIM_SP_KEY_IRREGULAR_3,
    MMIIM_SP_KEY_IRREGULAR_4,
    MMIIM_SP_KEY_IRREGULAR_5,
    MMIIM_SP_KEY_IRREGULAR_6,
    MMIIM_SP_KEY_IRREGULAR_7,
    MMIIM_SP_KEY_IRREGULAR_8,
    MMIIM_SP_KEY_IRREGULAR_9,
    MMIIM_SP_KEY_IRREGULAR_10,
    MMIIM_SP_KEY_IRREGULAR_11,
    MMIIM_SP_KEY_IRREGULAR_12,
    MMIIM_SP_KEY_IRREGULAR_13,
    MMIIM_SP_KEY_IRREGULAR_14,
    MMIIM_SP_KEY_IRREGULAR_15,
    MMIIM_SP_KEY_IRREGULAR_16,
    MMIIM_SP_KEY_IRREGULAR_17,
    MMIIM_SP_KEY_IRREGULAR_18,
    MMIIM_SP_KEY_IRREGULAR_19,
    MMIIM_SP_KEY_IRREGULAR_20,
    MMIIM_SP_KEY_IRREGULAR_21,
    MMIIM_SP_KEY_IRREGULAR_22,
    MMIIM_SP_KEY_IRREGULAR_23,
    MMIIM_SP_KEY_IRREGULAR_24,
    MMIIM_SP_KEY_IRREGULAR_25,
    MMIIM_SP_KEY_IRREGULAR_26,          //MMIIM_SP_26KEY_INDEX_Z
    MMIIM_SP_KEY_IRREGULAR_27,
    MMIIM_SP_KEY_IRREGULAR_28,
    MMIIM_SP_KEY_IRREGULAR_29,
    MMIIM_SP_KEY_IRREGULAR_30,
    MMIIM_SP_KEY_IRREGULAR_31,
    MMIIM_SP_KEY_IRREGULAR_32,
    MMIIM_SP_KEY_IRREGULAR_33,
    MMIIM_SP_KEY_IRREGULAR_34,
    MMIIM_SP_KEY_IRREGULAR_35,
    MMIIM_SP_KEY_IRREGULAR_36,
    MMIIM_SP_KEY_IRREGULAR_37,
    MMIIM_SP_KEY_IRREGULAR_38,
    MMIIM_SP_KEY_IRREGULAR_39,
    MMIIM_SP_KEY_IRREGULAR_40,
    MMIIM_SP_KEY_IRREGULAR_MAX = MMIIM_SP_KEY_IRREGULAR + 40,
//#endif

    MMIIM_SP_26KEY_COUNT,

    MMIIM_SP_26KEY_INDEX_DEL = MMIIM_SP_26KEY_COUNT,
    MMIIM_SP_26KEY_INDEX_COMMA,
    MMIIM_SP_26KEY_INDEX_PERIOD,
    MMIIM_SP_26KEY_INDEX_ENTER,
    MMIIM_SP_26KEY_INDEX_FN,
    MMIIM_SP_26KEY_INDEX_SHIFT,
    MMIIM_SP_26KEY_INDEX_AT_QWERTY,
    MMIIM_SP_26KEY_INDEX_SPACE,
    MMIIM_SP_26KEY_INDEX_AND,
    MMIIM_SP_26KEY_INDEX_QUESTION,
    MMIIM_SP_26KEY_INDEX_CTRL,

    MMIIM_SP_26KEY_INDEX_PLUS,                  // +
    MMIIM_SP_26KEY_INDEX_LEFT_PARENTHESIS,      // (
    MMIIM_SP_26KEY_INDEX_RIGHT_PARENTHESIS,     // )
    MMIIM_SP_26KEY_INDEX_MINUS,                 // -
    MMIIM_SP_26KEY_INDEX_DOUBLE_QUOTES,         // "
    MMIIM_SP_26KEY_INDEX_SEMICOLON,             // ;
    MMIIM_SP_26KEY_INDEX_COLON,                 // :
    MMIIM_SP_26KEY_INDEX_SLASH,                 // /
    MMIIM_SP_26KEY_INDEX_SHIFT_AND,             // &
    MMIIM_SP_26KEY_INDEX_EXCLAMATION,           // !
    MMIIM_SP_26KEY_INDEX_QUOTES,           // '
    MMIIM_SP_26KEY_INDEX_EQUAL,           // =
    MMIIM_SP_26KEY_INDEX_RIGHT_SLASH,           // \
    
    MMIIM_SP_26KEY_INDEX_0,
    MMIIM_SP_26KEY_INDEX_1,
    MMIIM_SP_26KEY_INDEX_2,
    MMIIM_SP_26KEY_INDEX_3,
    MMIIM_SP_26KEY_INDEX_4,
    MMIIM_SP_26KEY_INDEX_5,
    MMIIM_SP_26KEY_INDEX_6,
    MMIIM_SP_26KEY_INDEX_7,
    MMIIM_SP_26KEY_INDEX_8,
    MMIIM_SP_26KEY_INDEX_9,
    MMIIM_SP_26KEY_INDEX_STAR,
    MMIIM_SP_26KEY_INDEX_HASH,

    MMIIM_SP_26KEY_INDEX_MAX,
};

// һ������Ӧ������ĸ����
#define MMIIM_SP_26KEY_CHAR_MAX 19      //Խ���������ַ�����Ϊ18      

//Ϊ�ر����õļ���
#define MMIIM_SP_26KEY_COMMON_CNT (MMIIM_SP_26KEY_INDEX_MAX - MMIIM_SP_26KEY_COUNT)

//26������multitap��Ӧ��unicode��������
typedef struct
{
    uint16  key_index;                              // key index
    wchar   key_chars[MMIIM_SP_26KEY_CHAR_MAX+1];   // chars on this key. the first char is count of chars
} MMIIM_SP_26KEY_KEYCHAR_T;     /*lint !e770*/  //��ǰ�ڶ���ṹ���ּ����ԣ����뽫��ͷ�ļ��������˴�


// multitap language data type
typedef struct
{
    GUIIM_METHOD_T              method;            // method name
    const MMIIM_SP_26KEY_KEYCHAR_T   *key_tab_ptr;       // key table. when lead upper, its is upper table first, then set by lower_method below
} const MMIIM_SP_ML_26KEY_TABLE_CFG_T;

// custom key chars.
typedef MMIIM_SP_26KEY_KEYCHAR_T MMIIM_SP_26KEY_CUSTOM_CHAR_T[MMIIM_SP_26KEY_INDEX_MAX + 1];
/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*==============================================================================
Description: get key table by method index
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_ML26KEY_GetKeyTable(
    GUIIM_METHOD_T method      // [in] method type
);

PUBLIC void* MMIIM_SP_Smart28Key_GetKeyTable(
    GUIIM_METHOD_T method   // [in] method type
);

#if defined MMI_IM_PDA_SUPPORT	//���������������ʱ������Ҫ���ø����������˿ӡ����
/*==============================================================================
Description: ��ȡ����26��������Ӧ�����������ʾ�ַ�����ʱ�밴����unicodeֵ��ͬ��
			���ǽ���ȡ��һ���ַ���ʾ
Global resource dependence: NONE
Author:yaoguang.chen
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_ML26KEY_GetPrintKeyTable (
    GUIIM_METHOD_T method   // [in] method type
);

/*==============================================================================
Description: ��ȡmulti-tap 26��������Ӧ�����������ʾ�ַ�����ʱ�밴����unicodeֵ��ͬ��
			���ǽ���ȡ��һ���ַ���ʾ
Global resource dependence: NONE
Author:yaoguang.chen
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_SMART26KEY_GetPrintKeyTable (
    GUIIM_METHOD_T method   // [in] method type
);
#endif

/*==============================================================================
Description: ��ȡָ�������µ����ü�ֵ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
uint16 MMIIM_SP_ML26KEY_GetChrCnt(void const * table_ptr, uint16 key_idx);

/*==============================================================================
Description: ��ȡָ�������µ�ָ����ֵ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: char_index��1��ʼ����
==============================================================================*/
uint16 MMIIM_SP_ML26KEY_GetChr(void const * table_ptr, uint16 key_idx, uint16 chr_idx);

/*==============================================================================
Description: �����ַ��õ���Ӧ�ļ�ֵ
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML26KEY_GetKeyIndexByChar(
                                                  void const *table_ptr,     // [in]
                                                  wchar     key_char,       // [in]
                                                  uint16    *key_index_ptr  // [out]
                                                  );

/*==============================================================================
Description: alloc memory for custom key table
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML26KEY_AllocCustomKb(
                                              void const *table_ptr,
                                              void **custom_pptr
                                              );

/*==============================================================================
Description: free memory for custom key table
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML26KEY_FreeCustomKb(
                                             void *custom_ptr
                                             );

/*==============================================================================
Description: MMIIM_SP_ML26KEY_CustomKb
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ML26KEY_CustomKb(
    GUIIM_TYPE_T cur_im_type,
    GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr,
    void *custom_kb_ptr
);


extern PUBLIC void* MMIIM_SP_SMART26KEY_GetKeyTable(GUIIM_METHOD_T method);

/*==============================================================================
Description: get key table by method index
Author: yaoguang.chen
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_ML26KEY_GetShiftKeyTable(
    GUIIM_METHOD_T method   // [in] method type
);


/*==============================================================================
Description: ��ȡ����26��������Ӧ��SMARTֵ
Global resource dependence: NONE
Author:yaoguang.chen
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_SMART26KEY_GetShiftKeyTable(
    GUIIM_METHOD_T method   // [in] method type
);


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
}
#endif

#endif  // _MMIIM_SP_ML26KEY_DATA_H_
#endif
