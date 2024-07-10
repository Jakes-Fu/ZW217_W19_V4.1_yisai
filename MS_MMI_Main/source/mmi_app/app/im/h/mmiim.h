/*****************************************************************************
** File Name:      mmiim.h                                                   *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/

#ifndef _MMIIM_H_
#define _MMIIM_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "mmiim_sp_ml9key_data.h"
#include "mmiim_sp_ml26key_data.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//�˴�����û�д�Сдʱ����lower��upper�����õ�
typedef struct _S_LANG_KEY_TABLE
{
	//���ڴ�Сд
	BOOLEAN has_caps;
	
	//9��
	MMIIM_SP_9KEY_TABLE_T *plower_9key;
	MMIIM_SP_9KEY_TABLE_T *pupper_9key;
	
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
	//26��
	MMIIM_SP_26KEY_KEYCHAR_T *plower_26key;
	MMIIM_SP_26KEY_KEYCHAR_T *pupper_26key;
#endif
}S_LANG_KEY_TABLE;


//���Ա�־��
typedef struct _S_LANG_CODE_STRING
{
    GUIIM_LANGUAGE_T im_lang;
    const wchar *code_str;
}S_LANG_CODE_STRING;

typedef struct _udb_entry
{
    void * udb_ptr;
    uint16 udb_len;
}UDB_ENTRY_T;

typedef struct _udb_lang_config
{
    GUIIM_LANGUAGE_T im_lang;
    uint16 lang_len;
}UDB_LANG_CONFIG_T;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init im module
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_InitModule(void);

/*****************************************************************************/
//  Description : open im set window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIIM_OpenSetWin(void);

/*****************************************************************************/
//  Description : init im nv
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_InitNv(void);

/*****************************************************************************/
//  Description : reset im nv
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_ResetNv(void);

/*****************************************************************************/
//  Description : clear user data im nv
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_ClearUserDataNv(void);

/*****************************************************************************/
//  Description : im init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_Init(void);

/*****************************************************************************/
//  Description : get text's letters
//  Global resource dependence : none
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetTextLetters(uint16 text_unicode, uint16 *letters_ptr);

/*==============================================================================
Description: �ж��Ƿ��Ƿ���
Global resource dependence: NONE
Author: 
Note:
�жϵĿ��ܲ�ȫ
==============================================================================*/
PUBLIC BOOLEAN MMIAPIIM_IsPunct(uint16 text_unicode);

#if defined MMI_IM_PDA_SUPPORT
/*****************************************************************************/
//  Description : ��ȡǰһ�εķ�Ӣ����������
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC GUIIM_LANGUAGE_T MMIAPIIM_GetPrevLanguage(void);

/*****************************************************************************/
//  Description : ����ǰһ�εķ�Ӣ����������
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetPrevLanguage(GUIIM_LANGUAGE_T lang);
#endif
/*****************************************************************************/
//  Description : ���õ�ǰ�������Ե�Ĭ������
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetLanguage(GUIIM_LANGUAGE_T lang);
/*****************************************************************************/
//  Description : ��ȡ��ǰ�������Ե�Ĭ������
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC GUIIM_LANGUAGE_T MMIAPIIM_GetLanguage(void);


/*****************************************************************************/
//  Description : �������뷨��Ĭ������
//  Global resource dependence : 
//  Author: Lianxiang.Zhou
//  Note: ���뷽ʽ�����뷨����Сдģʽ
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetDefaultType(GUIIM_METHOD_T method);

/*****************************************************************************/
//  Description : ��ȡ���뷨��Ĭ������
//  Global resource dependence : 
//  Author: Lianxiang.Zhou
//  Note: ���뷽ʽ�����뷨����Сдģʽ
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetDefaultType(GUIIM_METHOD_T *method_ptr);

/*****************************************************************************/
//  Description : ������������Լ��ַ�����ø��ַ����ڵļ�λ
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetKeyIndex(                                         // [ret] ��λ���ַ���ʽ��������"1"����1�����������0�������û�ҵ���Ӧ�ļ�λ
                                   MMISET_LANGUAGE_TYPE_E   set_lang_index, // [in] ��������
                                   BOOLEAN                  is_26key,       // [in] �Ƿ���26������
                                   uint16                   input_char      // [in] ���뷨��Ҫ��ѯ���ַ�
                                   );

/*****************************************************************************/
//  Description : ���ذ���ʱ��Ӧ���ַ�
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetKeyChar(                                          // [ret] ���ض�Ӧ���ַ�
                                  MMISET_LANGUAGE_TYPE_E    set_lang_index, // [in] ��������
                                  BOOLEAN                   is_26key,       // [in] �Ƿ���26������
                                  uint16                    key_index,      // [in] ��λ�����ַ���ʽ��������"1"����1��
                                  uint16                    input_count     // [in] ���ü�λ�Ĵ���
                                  );

/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_CapsExist

Func:   �ж������Ƿ���ڴ�Сд

Input:  
            lang:  ���뷨��Ӧ������

Return: TRUE:���ڴ�Сд��FALSE:�����ڴ�Сд

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPIIM_CapsExist(GUIIM_LANGUAGE_T lang);


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_InitLangKeyTable

Func:   1.��ʼ�����Զ�Ӧ�����뷨����˿ӡ���ݱ����ڵ绰�����ң���绰������ʱ������
             Ƶ�������ַ���Ӧ�������Һ��������ÿ�ζ����²��Ұ���˿ӡ���ݱ�Ļ���̫�˷�
             ��Դ���ٶ���
	    2.�˴���һ��Լ��������֧�ֱʻ������뷨��Ӧ�İ������ݣ�ֻ֧��9����26��
	    
Input:  
            lang:  ��ǰϵͳ���õ�����

Return: 

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void MMIAPIIM_InitLangKeyTable(MMISET_LANGUAGE_TYPE_E  language_type);


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_GetLangKeyTable

Func:   1.��ȡ���Զ�Ӧ�İ���˿ӡ���ݱ�
	    2.�˴���һ��Լ��������֧�ֱʻ������뷨��Ӧ�İ������ݣ�ֻ֧��9����26��
	    
Input:  

Return: 

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC S_LANG_KEY_TABLE *pMMIAPIIM_GetLangKeyTable (void);


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_GetKeyIndexByKeytable

Func:   1.����ָ�����ַ��ڰ������ݱ��е���������������ת��Ϊa~z��1~9����

Input:  
	    pkey_table: Ҫ���ҵİ������ݱ�ΪMMIIM_SP_26KEY_KEYCHAR_T��MMIIM_SP_9KEY_TABLE_DEF_T
    	    input_char:	Ҫ���ҵ��ַ�
    	    is_26key:  ��9����26�����ݱ����֯��ʽ��ͬ��û�а취ͨ��pkey_table����������
    	      �����һ�����ֲ���
Return: 
		���ҳɹ�����a~z��1~9���أ����򷵻�  < 0����

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIAPIIM_GetKeyIndexByKeytable(               // [ret] ��λ���ַ���ʽ��������"1"����1�����������0�������û�ҵ���Ӧ�ļ�λ                          
                                   wchar const *pkey_table,
                                   uint16                   input_char,      // [in] ���뷨��Ҫ��ѯ���ַ�
					BOOLEAN                  is_26key       // [in] �Ƿ���26������
                                   );



PUBLIC const wchar *MMIAPIIM_GetLangCodeString_ptr (GUIIM_LANGUAGE_T im_lang);

/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIIM_GetWcharKeyValueByTable

Func:   1.���������ַ���ת��Ϊ�ַ���Ӧ�İ������У�Ϊa~z��1~9
	    2.�˴������ڵ�ǰϵͳ��ʾ���Լ����õ����뷨����˿ӡ����
	    
Input:  wstr:�����ַ���
	    key_str:����ַ���
	    is_26key:�Ƿ�Ϊ26����������Ϊa~z or 1~9
    	    
Return: ת���ɹ�TRUE��else FALSE;

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIIM_GetWcharKeyValueByTable (wchar *wstr, wchar *key_str, BOOLEAN is_26key);

PUBLIC MMIIM_SP_VKEY_T MMIIM_PhysicalKeyMsgToImKey (MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : get word predictive state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_GetWordPredictiveState(void);

/*****************************************************************************/
//  Description : set word predictive state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetWordPredictiveState(BOOLEAN is_word_predic);

#ifdef MMI_CSTAR_UDB_SUPPORT
/*****************************************************************************/
//  Description : MMIIM_GetUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC UDB_ENTRY_T MMIIM_GetUdbEntry(void);

/*****************************************************************************/
//  Description : MMIIM_GetUdbTotalSize
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIIM_GetUdbTotalSize(void);

/*****************************************************************************/
//  Description : MMIIM_InitUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_InitUdbEntry(void);

/*****************************************************************************/
//  Description : MMIIM_FreeUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_FreeUdbEntry(void);

/*****************************************************************************/
//  Description : MMIIM_LoadUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_LoadUdbEntry(void);

/*****************************************************************************/
//  Description : MMIIM_SaveUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_SaveUdbEntry(void);
#endif

#ifdef MMI_IME_USER_DATABASE
/*****************************************************************************/
//  Description : load user database 
//  Global resource dependence : none
//  Author:haiwu.chen
//  Note: read from nv maybe just use one time when boot mobile
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_LoadUserData(void);

/*****************************************************************************/
//  Description : save user database
//  Global resource dependence : none
//  Author:haiwu.chen
//  Note: save data from memory to nv when is power off
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_SaveUserData(void);

/*****************************************************************************/
//  Description : get the alphabetic udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetAlphaUDBData(int8 * alpha_udb_data_ptr);

/*****************************************************************************/
//  Description : set the alphabetic udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetAlphaUDBData(int8 * alpha_udb_data_ptr);

#ifdef IM_SIMP_CHINESE_SUPPORT  
/*****************************************************************************/
//  Description : get the chs udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetChsUDBData(int8 * chs_udb_data_ptr);

/*****************************************************************************/
//  Description : set the chs udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetChsUDBData(int8 * chs_udb_data_ptr);
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT  
/*****************************************************************************/
//  Description : get the cht udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetChtUDBData(int8 * cht_udb_data_ptr);

/*****************************************************************************/
//  Description : set the cht udb data
//  Global resource dependence : 
//  Author: haiwu.chenaa
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetChtUDBData(int8 * cht_udb_data_ptr);
#endif
#endif

/*****************************************************************************/
//  Description : �õ��༭����ǰ���ǰһ���ַ��������뽫underline�ַ�����ȥ���ģ�
//				����Ҫ�ж�underline�ַ�������������Ͳ���Ҫ����underline�ַ���
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC wchar MMIIM_GetEditCursorChar (MMI_HANDLE_T edit_handle);
/*****************************************************************************/
//  Description : �жϽ�Ҫ������ַ���ǰһ���ַ��Ƿ���.?!
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsPreCurrentWordSeperateChar (MMI_HANDLE_T edit_handle);
/*****************************************************************************/
//  Description : �жϽ�Ҫ������ַ���ǰһ���ַ��Ƿ���.?!
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsPreUnderlineWordSeperateChar (MMI_HANDLE_T edit_handle);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
