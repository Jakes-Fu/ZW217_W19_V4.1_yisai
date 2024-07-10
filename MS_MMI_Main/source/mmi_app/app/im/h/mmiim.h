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

//此处假设没有大小写时，将lower和upper均设置掉
typedef struct _S_LANG_KEY_TABLE
{
	//存在大小写
	BOOLEAN has_caps;
	
	//9键
	MMIIM_SP_9KEY_TABLE_T *plower_9key;
	MMIIM_SP_9KEY_TABLE_T *pupper_9key;
	
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
	//26键
	MMIIM_SP_26KEY_KEYCHAR_T *plower_26key;
	MMIIM_SP_26KEY_KEYCHAR_T *pupper_26key;
#endif
}S_LANG_KEY_TABLE;


//语言标志码
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
Description: 判断是否是符号
Global resource dependence: NONE
Author: 
Note:
判断的可能不全
==============================================================================*/
PUBLIC BOOLEAN MMIAPIIM_IsPunct(uint16 text_unicode);

#if defined MMI_IM_PDA_SUPPORT
/*****************************************************************************/
//  Description : 获取前一次的非英语输入语言
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC GUIIM_LANGUAGE_T MMIAPIIM_GetPrevLanguage(void);

/*****************************************************************************/
//  Description : 设置前一次的非英语输入语言
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetPrevLanguage(GUIIM_LANGUAGE_T lang);
#endif
/*****************************************************************************/
//  Description : 设置当前输入语言的默认设置
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetLanguage(GUIIM_LANGUAGE_T lang);
/*****************************************************************************/
//  Description : 获取当前输入语言的默认设置
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC GUIIM_LANGUAGE_T MMIAPIIM_GetLanguage(void);


/*****************************************************************************/
//  Description : 设置输入法的默认设置
//  Global resource dependence : 
//  Author: Lianxiang.Zhou
//  Note: 输入方式，输入法，大小写模式
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetDefaultType(GUIIM_METHOD_T method);

/*****************************************************************************/
//  Description : 获取输入法的默认设置
//  Global resource dependence : 
//  Author: Lianxiang.Zhou
//  Note: 输入方式，输入法，大小写模式
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetDefaultType(GUIIM_METHOD_T *method_ptr);

/*****************************************************************************/
//  Description : 根据所设的语言及字符，获得该字符所在的键位
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetKeyIndex(                                         // [ret] 键位以字符形式给出，如"1"代表1键，如果返回0，则代表没找到相应的键位
                                   MMISET_LANGUAGE_TYPE_E   set_lang_index, // [in] 输入语言
                                   BOOLEAN                  is_26key,       // [in] 是否是26键键盘
                                   uint16                   input_char      // [in] 输入法需要查询的字符
                                   );

/*****************************************************************************/
//  Description : 返回按键时对应的字符
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetKeyChar(                                          // [ret] 返回对应的字符
                                  MMISET_LANGUAGE_TYPE_E    set_lang_index, // [in] 输入语言
                                  BOOLEAN                   is_26key,       // [in] 是否是26键键盘
                                  uint16                    key_index,      // [in] 键位，以字符形式给出，如"1"代表1键
                                  uint16                    input_count     // [in] 按该键位的次数
                                  );

/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_CapsExist

Func:   判断语言是否存在大小写

Input:  
            lang:  输入法对应的语言

Return: TRUE:存在大小写，FALSE:不存在大小写

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPIIM_CapsExist(GUIIM_LANGUAGE_T lang);


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_InitLangKeyTable

Func:   1.初始化语言对应的输入法按键丝印数据表，用于电话本查找；因电话本查找时，过于
             频繁调用字符对应按键查找函数，如果每次都重新查找按键丝印数据表的话，太浪费
             资源，速度慢
	    2.此处有一个约束，即不支持笔画等输入法对应的按键数据，只支持9键和26键
	    
Input:  
            lang:  当前系统设置的语言

Return: 

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void MMIAPIIM_InitLangKeyTable(MMISET_LANGUAGE_TYPE_E  language_type);


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_GetLangKeyTable

Func:   1.获取语言对应的按键丝印数据表
	    2.此处有一个约束，即不支持笔画等输入法对应的按键数据，只支持9键和26键
	    
Input:  

Return: 

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC S_LANG_KEY_TABLE *pMMIAPIIM_GetLangKeyTable (void);


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_GetKeyIndexByKeytable

Func:   1.查找指定的字符在按键数据表中的索引，并将索引转换为a~z或1~9返回

Input:  
	    pkey_table: 要查找的按键数据表，为MMIIM_SP_26KEY_KEYCHAR_T或MMIIM_SP_9KEY_TABLE_DEF_T
    	    input_char:	要查找的字符
    	    is_26key:  因9键和26键数据表的组织形式不同，没有办法通过pkey_table自身来区分
    	      故添加一个区分参数
Return: 
		查找成功，则a~z或1~9返回；否则返回  < 0数据

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIAPIIM_GetKeyIndexByKeytable(               // [ret] 键位以字符形式给出，如"1"代表1键，如果返回0，则代表没找到相应的键位                          
                                   wchar const *pkey_table,
                                   uint16                   input_char,      // [in] 输入法需要查询的字符
					BOOLEAN                  is_26key       // [in] 是否是26键键盘
                                   );



PUBLIC const wchar *MMIAPIIM_GetLangCodeString_ptr (GUIIM_LANGUAGE_T im_lang);

/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIIM_GetWcharKeyValueByTable

Func:   1.根据输入字符，转换为字符对应的按键序列，为a~z或1~9
	    2.此处依赖于当前系统显示语言及设置的输入法按键丝印数据
	    
Input:  wstr:输入字符串
	    key_str:输出字符串
	    is_26key:是否为26键，即返回为a~z or 1~9
    	    
Return: 转换成功TRUE，else FALSE;

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
//  Description : 得到编辑器当前光标前一个字符，本来想将underline字符长度去掉的，
//				因不需要判断underline字符，故这个函数就不需要考虑underline字符了
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC wchar MMIIM_GetEditCursorChar (MMI_HANDLE_T edit_handle);
/*****************************************************************************/
//  Description : 判断将要输入的字符的前一个字符是否是.?!
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsPreCurrentWordSeperateChar (MMI_HANDLE_T edit_handle);
/*****************************************************************************/
//  Description : 判断将要输入的字符的前一个字符是否是.?!
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
