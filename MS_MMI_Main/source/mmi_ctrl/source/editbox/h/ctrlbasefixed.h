/*****************************************************************************
** File Name:      ctrlbasefixed.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLBASEFIXED_H_
#define _CTRLBASEFIXED_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseedit.h"
#include "ctrlbasefixed_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// basefixed type info
#define CTRL_BASEFIXED_TYPE     (BASEFIXED_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// basefixed init param
typedef struct
{
    CTRLBASEEDIT_INIT_PARAM_T       baseedit_ctrl_param;

    // self inif param ...
} CTRLBASEFIXED_INIT_PARAM_T;

//basefixed control
typedef struct
{
    CTRLBASEEDIT_OBJ_T              baseedit_ctrl;

    // self member ...
} CTRLBASEFIXED_OBJ_T;

// basefixed class for public or protected method
typedef struct
{
    CTRLBASEEDIT_VTBL_T             baseedit_ctrl_vtbl;

    /**************************************************************************/
    //  Description : 获取指定偏移的字符是否有效
    // 该方法用于在TP消息点击后，判断是否点在有效的字符上
    /**************************************************************************/
    BOOLEAN (*IsValidOffset)(                               // [ret] TRUE if char is valid
        CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr,        // [in] ctrl ptr
        uint16                  *char_offset_ptr,           // [in/out] char offset, and may change the offset if valid
        uint16                  *highlight_len_ptr          // [out] highlight length if char valid
        );

    /**************************************************************************/
    //  Description : 将内容转换成字符串
    /**************************************************************************/
    void (*ConvertToStr)(
        CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr
        );

} CTRLBASEFIXED_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get basefixed type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEFIXED_GetType(void);

/*****************************************************************************/
//  Description : get bit value
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint8 BASEFIXED_CalculateBitValue (
    BOOLEAN  is_add,     //in
    uint8    bit_value,  //in
    uint8    bit_min,    //in
    uint8    bit_max
);

/*****************************************************************************/
//  Description : convert number key to number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFIXED_GetNumValueByIm (
    uint8                *num_value_ptr, //in/out:
    GUIEDIT_ADD_STR_T    *add_str_ptr    //in:
);

/*****************************************************************************/
//  Description : 获取指定偏移的字符是否有效
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 该方法用于在TP消息点击后，判断是否点在有效的字符上
/*****************************************************************************/
static __inline BOOLEAN VTLBASEFIXED_IsValidOffset(     // [ret] TRUE if char offset is valid
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr,        // [in] ctrl ptr
    uint16                  *char_offset_ptr,           // [in/out] char offset, and may change the offset if valid
    uint16                  *highlight_len_ptr          // [out] highlight length if char valid
    )
{
    BOOLEAN result = FALSE;
    CTRLBASEFIXED_VTBL_T *basefixed_vtbl_ptr = (CTRLBASEFIXED_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFIXED_TYPE, basefixed_ctrl_ptr);

    if (PNULL != basefixed_vtbl_ptr->IsValidOffset)
    {
        result = basefixed_vtbl_ptr->IsValidOffset(basefixed_ctrl_ptr, char_offset_ptr, highlight_len_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : 将内容转换成字符串
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEFIXED_ConvertToStr(
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr
    )
{
    CTRLBASEFIXED_VTBL_T *basefixed_vtbl_ptr = (CTRLBASEFIXED_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFIXED_TYPE, basefixed_ctrl_ptr);

    if (PNULL != basefixed_vtbl_ptr->ConvertToStr)
    {
        basefixed_vtbl_ptr->ConvertToStr(basefixed_ctrl_ptr);
    }
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
