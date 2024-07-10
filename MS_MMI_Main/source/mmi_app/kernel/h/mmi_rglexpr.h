/*****************************************************************************
** File Name:      mmi_rglexpr.h                                                   *
** Author:                                                                   *
** Date:           24/03/2007                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2007       Bin.Ji       Create
******************************************************************************/

#ifndef _MMIRGLEXPR_H_
#define _MMIRGLEXPR_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

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

#define MAX_TOKEN_LEN 25
#define MAX_ACTION_NUM 2
#define MAX_STATE_NUM 75
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define RGLEXPR_URL_MAX_COUNT   6

typedef struct
{
    uint16 start_pos;
    uint16 length;
}RGLEXPR_STR_T;

typedef struct
{
    BOOLEAN is_wstring; //是否是双字节字符串
    uint8 *str_ptr;
    uint16 str_length;
    uint32 str_count;
    RGLEXPR_STR_T str_arr[RGLEXPR_URL_MAX_COUNT];
}RGLEXPR_URL_RESULT_T;


typedef enum
{
    RGLEXPR_ANY_SYMBOL,        //?
    RGLEXPR_SINGEL_SYMBOL,     //0,1,...,a,b,c...
    RGLEXPR_MULTI_SYMBOL,      //[]
    RGLEXPR_STAR_SYMBOL,       //*
    RGLEXPR_PLUS_SYMBOL,       //+
    RGLEXPR_END_SYMBOL       //END
}RGLEXPR_TOKEN_TYPE_E;

typedef struct 
{
    uint8 *str_ptr;
    uint32 current_pos;
}RGLEXPR_EXPR_T;

typedef struct 
{
    BOOLEAN is_end;         //'\0'
    BOOLEAN is_extend;      //+, *, ?
    BOOLEAN is_bracket;     //[abc]
    BOOLEAN is_trans;       //\a
    BOOLEAN is_range;       //a-b
    uint32 bracket_start;
    uint32 bracket_length;
    uint8 range_start_ch;
    uint8 range_end_ch;
    uint8 character;
    uint8 *expr_ptr;


}RGLEXPR_TOKEN_T;

typedef struct 
{
    RGLEXPR_TOKEN_T token;
    uint32 next_state;
}RGLEXPR_ACTION_T;

typedef struct 
{
    uint32 id;
    BOOLEAN is_accept_end;
    uint32 action_num;
    RGLEXPR_ACTION_T action[MAX_ACTION_NUM];
}RGLEXPR_STATE_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 查找http格式的URL
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMI_FindHttp(
                           RGLEXPR_URL_RESULT_T *url_result_ptr
                           );

/*****************************************************************************/
// 	Description : 查找rtsp格式的URL
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMI_FindRtsp(
                           RGLEXPR_URL_RESULT_T *url_result_ptr
                           );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


