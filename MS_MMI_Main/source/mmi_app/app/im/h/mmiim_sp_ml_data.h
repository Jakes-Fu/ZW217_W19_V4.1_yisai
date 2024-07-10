/***************************************************************************
** File Name:      mmiim_sp_ml_data.h                                      *
** Author:         hua.fang                                                *
** Date:           07/22/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2010        hua.fang         Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIIM_SP_ML_DATA_H_
#define _MMIIM_SP_ML_DATA_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "mmiim_sp_ml26key_data.h"
#include "mmiim_sp_ml9key_data.h"

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
// 按键索引
typedef uint16 MMIIM_SP_KEY_INDEX_T;

// 多国语言数据
typedef struct
{
    GUIIM_METHOD_CFG_T *method_cfg_ptr;

    BOOLEAN is_26key;
    void const *key_table_ptr;

    void *custom_kb_ptr;
} MMIIM_SP_ML_DATA_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*==============================================================================
Description: 输入法数据的初始化
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_Init(
    MMIIM_SP_ML_DATA_T      *ml_data_ptr       // [in]
);

/*==============================================================================
Description: 销毁数据法数据
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_Term(
    MMIIM_SP_ML_DATA_T      *ml_data_ptr
);

/*==============================================================================
Description: Set Method
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_SetMethod(
    MMIIM_SP_ML_DATA_T      *ml_data_ptr,       // [in]
    GUIIM_METHOD_T           method
);

/*==============================================================================
Description: 获取对应按键下字母的个数
Author: hua.fang
Global resource dependence: g_26key_table_common
Note:
==============================================================================*/
PUBLIC uint16 MMIIM_SP_ML_GetCount(
    MMIIM_SP_ML_DATA_T       *ml_data_ptr,   // [in]
    MMIIM_SP_KEY_INDEX_T     key_index       // [in]
);

/*==============================================================================
Description: 根据字符得到对应的键值
Author: hua.fang
Global resource dependence: g_26key_table_common
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_GetKeyIndexByChar(
                                             MMIIM_SP_ML_DATA_T *ml_data_ptr,   // [in]
                                             wchar              key_char,       // [in]
                                             MMIIM_SP_KEY_INDEX_T*  key_index_ptr   // [out]
                                             );

/*==============================================================================
Description: 获取对应按键下指定索引的字母
Author: hua.fang
Global resource dependence: g_26key_table_common
Note: char_index从1开始计数
==============================================================================*/
PUBLIC wchar MMIIM_SP_ML_GetChar(
    MMIIM_SP_ML_DATA_T         *ml_data_ptr,   // [in]
    MMIIM_SP_KEY_INDEX_T       key_index,      // [in]
    uint16                     char_index      // [in]
);

/*==============================================================================
Description: 检查字母表
Author: hua.fang
Global resource dependence:
Note: 有些语言，某个字母后面只能跟特定的字母，这时就需要切换字母表。现通过这个
        方法检查字母表
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_CheckKeyTable(
    MMIIM_SP_ML_DATA_T     *ml_data_ptr,   // [in]
    CHECK_TABLE_FUNC_T     *checking_func_pptr
);

/*==============================================================================
Description: MMIIM_SP_ML_SetCustomKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML_SetCustomKey(
    MMIIM_SP_ML_DATA_T       *ml_data_ptr,
    GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr
);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
}
#endif

#endif  // _MMIIM_SP_ML_DATA_H_

