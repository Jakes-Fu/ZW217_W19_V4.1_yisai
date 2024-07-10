/****************************************************************************
** File Name:      mmi_rglexpr.c                                       *
** Author:                                                                 *
** Date:           2004.8.23                                               *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the MMS                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2007.7        Bin.Ji         Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                        Include Files                                     *
**--------------------------------------------------------------------------*/
//#include "std_header.h"

#include "mmi_app_kernel_trc.h"
#include "os_api.h"
#include "mmi_rglexpr.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


#define UCS2_CH         0xff        //UCS2字符



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
 /**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
 
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

//RGLEXPR_STATE_T s_rglexpr_state_arr[MAX_STATE_NUM];
LOCAL RGLEXPR_STATE_T *s_rglexpr_state_arr = PNULL;
LOCAL uint32 s_rglexpr_state_num = 0;


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 在一串文字中（多次）寻找模式，该文字为ASCII编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint32 FindMorePattern(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_start_pos_ptr,
                               uint32 *match_end_pos_ptr,
                               uint32 count
                               );

/*****************************************************************************/
// 	Description : 在一串文字中（多次）寻找模式，该文字为UCS2编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint32 FindMorePatternUCS2(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_start_pos_ptr,
                               uint32 *match_end_pos_ptr,
                               uint32 count
                               );

/*****************************************************************************/
// 	Description : 查找符合正则表达式hhexpr_str的URL
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void FindURL(
                     uint8 *hhexpr_str,
                     RGLEXPR_URL_RESULT_T *url_result_ptr
                     );

/*****************************************************************************/
// 	Description : 字符是否与Bracket类型token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchChInBracket(
                        uint8 *expr_ptr,
                        uint32 bracket_start,
                        uint32 bracket_length,
                        uint8 ch
                        );

/*****************************************************************************/
// 	Description : 字符是否与Range类型token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchChInRange(
                        uint8 range_start_ch,
                        uint8 range_end_ch,
                        uint8 ch
                        );

/*****************************************************************************/
// 	Description : 字符是否与Trans类型token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchChInTrans(
                        uint8 character,
                        uint8 ch
                        );

/*****************************************************************************/
// 	Description : 字符是否与Multi类型token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchChInCharacter(
                        uint8 character,
                        uint8 ch
                        );

/*****************************************************************************/
// 	Description : 从一串文字头开始匹配模式，该文字为ASCII编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN FindPatternFromHead(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_len_ptr
                               );

/*****************************************************************************/
// 	Description : 从一串文字头开始匹配模式，该文字为UCS2编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN FindPatternFromHeadUCS2(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_len_ptr
                               );

/*****************************************************************************/
// 	Description : 计算转义类型\a的字符值
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint8 TransValue(
                       uint8 ch
                       );

/*****************************************************************************/
// 	Description :  从字符串头部读取一个token（a, \a, a-b, [ab], x+, x*, x?）
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN gettoken(
                       RGLEXPR_EXPR_T *expr_ptr,
                       RGLEXPR_TOKEN_T *token_ptr
                       );

/*****************************************************************************/
// 	Description : 创建新状态
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint32 CreateNewState(void);

/*****************************************************************************/
// 	Description : 建立状态转换关系（接收token, A->B）
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void ConnectState(
                           uint32 old_state,
                           RGLEXPR_TOKEN_T *token_ptr,
                           uint32 new_state
                           );
                           
/*****************************************************************************/
// 	Description : 初始化状态机
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void InitialStateMachine(void);

/*****************************************************************************/
// 	Description : 销毁状态机，释放内存
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void DestroyStateMachine(void);

/*****************************************************************************/
// 	Description : 设为结束状态
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SetEndState(
                       uint32 state
                       );
                       
/*****************************************************************************/
// 	Description : 是否结束状态
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEndState(
                       uint32 state
                       );

/*****************************************************************************/
// 	Description : 字符是否与token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchCh(
                        RGLEXPR_TOKEN_T *token_ptr,
                        uint8 ch
                        );
                        
/*****************************************************************************/
// 	Description : 运行有限状态机：接收一个字符，状态转变为另一个状态
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN RunFSM(
                      uint32 old_state,
                      uint8 ch,
                      uint32 *new_state_ptr
                      );
                      
/*****************************************************************************/
// 	Description : 转换UCS2到ascii
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint8 ConvertCH(
                      uint8 ch1,
                      uint8 ch2
                      );                                                                                                                                                              
                       
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description :  从字符串头部读取一个token（a, \a, a-b, \a-\b, [ab], x+, x*, x?）
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN gettoken(
                       RGLEXPR_EXPR_T *expr_ptr,
                       RGLEXPR_TOKEN_T *token_ptr
                       )
{
    //uint8 *str_ptr = PNULL;
//    uint8 ch = 0;
    uint32 pos = 0;
    BOOLEAN result = FALSE;
    uint32 length = 0;
    uint32 next_pos = 0;

    SCI_ASSERT(token_ptr); /*assert verified*/
    SCI_ASSERT(expr_ptr); /*assert verified*/

    SCI_MEMSET(token_ptr, 0x00, sizeof(RGLEXPR_TOKEN_T));
    pos = expr_ptr->current_pos;

    token_ptr->expr_ptr = expr_ptr->str_ptr;
    switch (expr_ptr->str_ptr[pos])
    {
    case '\0':
        token_ptr->is_end = TRUE;
        break;

    case '[':
        token_ptr->bracket_start = pos;
        pos++;
        while ('\0' != expr_ptr->str_ptr[pos])
        {
            if (']' == expr_ptr->str_ptr[pos])
            {
                break;
            }
            pos++;
            length++;
        }
        token_ptr->is_bracket = TRUE;
        token_ptr->bracket_length = length;
        if (']' == expr_ptr->str_ptr[pos])
        {
            if ('+' == expr_ptr->str_ptr[pos+1])
            {
                token_ptr->is_extend = TRUE;
                next_pos = pos+2;
            }
            else
            {
                next_pos = pos+1;
            }
        }
        break;

    case '+':
    case ']':   //error
        result = FALSE;
        break;

    default:
        if ('\\' == expr_ptr->str_ptr[pos])
        {  
            if ('-' == expr_ptr->str_ptr[pos+2])
            {
                token_ptr->is_range = TRUE;
                token_ptr->range_start_ch = TransValue(expr_ptr->str_ptr[pos+1]);
                if ('\\' == expr_ptr->str_ptr[pos+3])
                {
                    token_ptr->range_end_ch = TransValue(expr_ptr->str_ptr[pos+4]);
                    next_pos = pos+5;
                }
                else
                {
                    token_ptr->range_end_ch = expr_ptr->str_ptr[pos+3];
                    next_pos = pos+4;
                }
            }
            else
            {
                token_ptr->is_trans = TRUE;
                token_ptr->character = TransValue(expr_ptr->str_ptr[pos+1]);
                next_pos = pos+2;
            }            
        }
        else
        {
            if ('-' == expr_ptr->str_ptr[pos+1])
            {
                token_ptr->is_range = TRUE;
                token_ptr->range_start_ch = expr_ptr->str_ptr[pos];
                if ('\\' == expr_ptr->str_ptr[pos+2])
                {
                    token_ptr->range_end_ch = TransValue(expr_ptr->str_ptr[pos+3]);
                    next_pos = pos + 4;
                }
                else
                {
                    token_ptr->range_end_ch = expr_ptr->str_ptr[pos+2];
                    next_pos = pos + 3;
                }
            }
            else
            {
                token_ptr->character = expr_ptr->str_ptr[pos];
                next_pos = pos + 1;
            }
        }
        break;
    }
    expr_ptr->current_pos = next_pos;
    
    return result;
}

/*****************************************************************************/
// 	Description : 创建新状态
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint32 CreateNewState(void)
{
    uint32 state_num = s_rglexpr_state_num;
    
    s_rglexpr_state_num++;
    s_rglexpr_state_arr[state_num].id = 0;
    s_rglexpr_state_arr[state_num].action_num = 0;
    return state_num;
}

/*****************************************************************************/
// 	Description : 建立状态转换关系（接收token, A->B）
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void ConnectState(
                           uint32 old_state,
                           RGLEXPR_TOKEN_T *token_ptr,
                           uint32 new_state
                           )
{
    uint32 action_num = 0;

    SCI_ASSERT(MAX_ACTION_NUM > action_num); /*assert verified*/
    action_num = s_rglexpr_state_arr[old_state].action_num;
    s_rglexpr_state_arr[old_state].action[action_num].next_state = new_state;
    s_rglexpr_state_arr[old_state].action[action_num].token = *token_ptr;
    s_rglexpr_state_arr[old_state].action_num++;
}

/*****************************************************************************/
// 	Description : 初始化状态机
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void InitialStateMachine(void)
{
//    uint32 i = 0;
    s_rglexpr_state_arr = SCI_ALLOC_APP(MAX_STATE_NUM *sizeof(RGLEXPR_STATE_T));

    SCI_MEMSET(s_rglexpr_state_arr, 0x00, (MAX_STATE_NUM *sizeof(RGLEXPR_STATE_T)));
    s_rglexpr_state_num = 0;
}

/*****************************************************************************/
// 	Description : 销毁状态机，释放内存
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void DestroyStateMachine(void)
{
//    uint32 i = 0;
    SCI_FREE(s_rglexpr_state_arr);
    s_rglexpr_state_num = 0;
}

/*****************************************************************************/
// 	Description : 设为结束状态
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SetEndState(
                       uint32 state
                       )
{
    s_rglexpr_state_arr[state].is_accept_end = TRUE;
}

/*****************************************************************************/
// 	Description : 是否结束状态
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEndState(
                       uint32 state
                       )
{
    return s_rglexpr_state_arr[state].is_accept_end;
}

/*****************************************************************************/
// 	Description : 正则表达式转换为FSM
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void ParseRegularExpression(
                                  RGLEXPR_EXPR_T *expr_ptr
                                  )
{
    RGLEXPR_TOKEN_T token = {0};
    uint32 new_state = 0, cur_state = 0;
    BOOLEAN is_end = FALSE;
    
    //InitialStateMachine();
    CreateNewState();
    while (!is_end)
    {
        gettoken(expr_ptr, &token);

        //SCI_TRACE_LOW("ParseRegularExpression: type = %d, old_state = %d", token.type, old_state);
        
        if (token.is_end)
        {
            SetEndState(cur_state);
            break;
        }
        else
        {
            if (token.is_extend)    //+
            {
                new_state = CreateNewState();
                ConnectState(cur_state, &token, new_state);                   
                ConnectState(new_state, &token, new_state);                
            }
            else
            {
                new_state = CreateNewState();
                ConnectState(cur_state, &token, new_state);
            }
            cur_state = new_state;
        }       
    }
}


/*****************************************************************************/
// 	Description : 字符是否与Bracket类型token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchChInBracket(
                        uint8 *expr_ptr,
                        uint32 bracket_start,
                        uint32 bracket_length,
                        uint8 ch
                        )
{
    BOOLEAN result = FALSE;
    uint32 pos = 0;
    uint32 next_pos = 0;
    uint8 range_start_ch = 0;
    uint8 range_end_ch = 0;
    uint8 character = 0;

    pos = bracket_start+1;
    while (1)/*lint !e716*/
    {
        if (']' == expr_ptr[pos])
        {
            break;
        }
        if ('\\' == expr_ptr[pos])
        {  
            if ('-' == expr_ptr[pos+2])
            {
                range_start_ch = TransValue(expr_ptr[pos+1]);
                if ('\\' == expr_ptr[pos+3])
                {
                    range_end_ch = TransValue(expr_ptr[pos+4]);
                    next_pos = pos+5;
                }
                else
                {
                    range_end_ch = expr_ptr[pos+3];
                    next_pos = pos+4;
                }
                if (IsMatchChInRange(range_start_ch, range_end_ch, ch))
                {
                    result = TRUE;
                    break;
                }
            }
            else
            {
                character = TransValue(expr_ptr[pos+1]);
                next_pos = pos+2;
                if (IsMatchChInTrans(character, ch))
                {
                    result = TRUE;
                    break;
                }
            }            
        }
        else
        {
            if ('-' == expr_ptr[pos+1])
            {
                range_start_ch = expr_ptr[pos];
                if ('\\' == expr_ptr[pos+2])
                {
                    range_end_ch = TransValue(expr_ptr[pos+3]);
                    next_pos = pos + 4;
                }
                else
                {
                    range_end_ch = expr_ptr[pos+2];
                    next_pos = pos + 3;
                }
                if (IsMatchChInRange(range_start_ch, range_end_ch, ch))
                {
                    result = TRUE;
                    break;
                }
            }
            else
            {
                if (IsMatchChInCharacter(expr_ptr[pos], ch))
                {
                    result = TRUE;
                    break;
                }
            }
        }  
        pos = next_pos;
    }
    return result;
}

/*****************************************************************************/
// 	Description : 字符是否与Range类型token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchChInRange(
                        uint8 range_start_ch,
                        uint8 range_end_ch,
                        uint8 ch
                        )
{
    if (ch >= range_start_ch
        && ch <= range_end_ch)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 字符是否与Trans类型token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchChInTrans(
                        uint8 character,
                        uint8 ch
                        )
{
    if ('S' == character)
    {
        if (' ' != ch)
        {
            return TRUE;
        }
    }
    else if ('s' == character)
    {
        if (' ' == ch)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 字符是否与Multi类型token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchChInCharacter(
                        uint8 character,
                        uint8 ch
                        )
{
    if (ch == character)
    {
        return TRUE;
    }
    return FALSE;
}


/*****************************************************************************/
// 	Description : 字符是否与token匹配
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMatchCh(
                        RGLEXPR_TOKEN_T *token_ptr,
                        uint8 ch
                        )
{
    BOOLEAN result = FALSE;

    SCI_ASSERT(token_ptr); /*assert verified*/

    if (token_ptr->is_end)
    {
        result = FALSE;
    }
    else if (token_ptr->is_bracket)
    {
        result = IsMatchChInBracket(
            token_ptr->expr_ptr,
            token_ptr->bracket_start,
            token_ptr->bracket_length,
            ch);
    }
    else if (token_ptr->is_range)
    {
        result = IsMatchChInRange(token_ptr->range_start_ch, token_ptr->range_end_ch, ch);
    }
    else if (token_ptr->is_trans)
    {
        result = IsMatchChInTrans(token_ptr->character, ch);
    }
    else
    {
        result = IsMatchChInCharacter(token_ptr->character, ch);
        if (ch == token_ptr->character)
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : 运行有限状态机：接收一个字符，状态转变为另一个状态
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN RunFSM(
                      uint32 old_state,
                      uint8 ch,
                      uint32 *new_state_ptr
                      )
{
    uint32 i = 0;
    
    if ('\0' == ch || UCS2_CH == ch)    //@CR126162：遇到中文结束
    {
        return FALSE;
    }

    for (i = 0; i < s_rglexpr_state_arr[old_state].action_num; i++)
    {
        if (IsMatchCh(&s_rglexpr_state_arr[old_state].action[i].token, ch))
        {
            *new_state_ptr = s_rglexpr_state_arr[old_state].action[i].next_state;
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 从一串文字头开始匹配模式，该文字为ASCII编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN FindPatternFromHead(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_len_ptr
                               )
{
    uint32 new_state = 0, old_state = 0; //temp_new_state = 0;    
    uint8 *temp_str_ptr = (str_ptr + start_pos);
    uint8 ch = 0;
    uint32 i = 0, count = (end_pos+1-start_pos);

    for (i = 0; i < count; i++)
    {
        ch = *temp_str_ptr++;
        if (!RunFSM(old_state, ch, &new_state))
        {
            //SCI_TRACE_LOW:"---RunFSM break--"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RGLEXPR_729_112_2_18_2_0_26_248,(uint8*)"");
            break;
        }
        old_state = new_state;
    }

    if (IsEndState(new_state))
    {
        *match_len_ptr = i;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 在一串文字中寻找模式（找到一次即返回），该文字为ASCII编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN FindPattern(
                          uint8 *str_ptr,
                          uint32 start_pos,
                          uint32 end_pos,
                          uint32 *match_start_pos_ptr,
                          uint32 *match_end_pos_ptr
                          )
{
//    uint8 *temp_ptr = PNULL;
    uint32 i = 0;
    uint32 match_len = 0;

    for (i = start_pos; i < end_pos; i++)
    {
        if (FindPatternFromHead(str_ptr, i, end_pos, &match_len))
        {
            *match_start_pos_ptr = i;
            *match_end_pos_ptr = (i + match_len - 1);
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 在一串文字中（多次）寻找模式，该文字为ASCII编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint32 FindMorePattern(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_start_pos_ptr,
                               uint32 *match_end_pos_ptr,
                               uint32 count
                               )
{
    uint32 i = 0;
//    uint32 match_start_pos = 0;
//    uint32 match_end_pos = 0;
    BOOLEAN result = FALSE;

    for (i = 0; i < count; i++)
    {
        result = FindPattern(str_ptr, start_pos, end_pos, &match_start_pos_ptr[i], &match_end_pos_ptr[i]);
        if (!result)
        {
            break;
        }
        start_pos = match_end_pos_ptr[i]+1;
    }
    return i;
}

/********************************ucs2*************************************/

/*****************************************************************************/
// 	Description : 转换UCS2到ascii
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint8 ConvertCH(
                      uint8 ch1,
                      uint8 ch2
                      )
{
    if (0x00 == ch1)
    {
        return ch2;
    }
    return UCS2_CH;
}

/*****************************************************************************/
// 	Description : 从一串文字头开始匹配模式，该文字为UCS2编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN FindPatternFromHeadUCS2(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_len_ptr
                               )
{
    uint32 new_state = 0, old_state = 0; //temp_new_state = 0;    
    uint8 *temp_str_ptr = (str_ptr + start_pos);
    uint8 ch1 = 0, ch2 = 0, ch = 0;
    uint32 i = 0, count = (end_pos+1-start_pos);

    for (i = 0; i < count; i+=2)
    {
        ch1 = *temp_str_ptr;
        ch2 = *(temp_str_ptr+1);
        temp_str_ptr+=2;
        ch = ConvertCH(ch1, ch2);
        //SCI_TRACE_LOW:"---ConvertCH---%x---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RGLEXPR_843_112_2_18_2_0_26_249,(uint8*)"d", ch);
        if (!RunFSM(old_state, ch, &new_state))
        {
            //SCI_TRACE_LOW:"---RunFSM UCS2 break--"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RGLEXPR_846_112_2_18_2_0_26_250,(uint8*)"");
            break;
        }
        old_state = new_state;
    }
    
    if (IsEndState(new_state))
    {
        //SCI_TRACE_LOW:"---IsEndState success--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RGLEXPR_854_112_2_18_2_0_26_251,(uint8*)"");
        *match_len_ptr = i;
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"---IsEndState fail--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RGLEXPR_860_112_2_18_2_0_26_252,(uint8*)"");
        return FALSE;
    }
    
//    return FALSE;
}

/*****************************************************************************/
// 	Description : 在一串文字中寻找模式（找到一次即返回），该文字为UCS2编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN FindPatternUCS2(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_start_pos_ptr,
                               uint32 *match_end_pos_ptr
                               )
{
//    uint8 *temp_ptr = PNULL;
    uint32 i = 0;
    uint32 match_len = 0;

    for (i = start_pos; i < end_pos; i+=2)
    {
        //SCI_TRACE_LOW:"---FindPatternUCS2 %d--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RGLEXPR_885_112_2_18_2_0_26_253,(uint8*)"d", i);
        if (FindPatternFromHeadUCS2(str_ptr, i, end_pos, &match_len))
        {
            *match_start_pos_ptr = i;
            *match_end_pos_ptr = (i + match_len);
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 在一串文字中（多次）寻找模式，该文字为UCS2编码
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint32 FindMorePatternUCS2(
                               uint8 *str_ptr,
                               uint32 start_pos,
                               uint32 end_pos,
                               uint32 *match_start_pos_ptr,
                               uint32 *match_end_pos_ptr,
                               uint32 count
                               )
{
    uint32 i = 0;
//    uint32 match_start_pos = 0;
//    uint32 match_end_pos = 0;
    BOOLEAN result = FALSE;

    for (i = 0; i < count; i++)
    {
        //SCI_TRACE_LOW:"---FindMorePatternUCS2 %d--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RGLEXPR_916_112_2_18_2_0_26_254,(uint8*)"d", i);
        result = FindPatternUCS2(str_ptr, start_pos, end_pos, &match_start_pos_ptr[i], &match_end_pos_ptr[i]);
        if (!result)
        {
            //SCI_TRACE_LOW:"---FindMorePatternUCS2 break--"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RGLEXPR_920_112_2_18_2_0_26_255,(uint8*)"");
            break;
        }
        start_pos = match_end_pos_ptr[i];
    }
    return i;
}

/*****************************************************************************/
// 	Description : 计算转义类型\a的字符值
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL uint8 TransValue(
                       uint8 ch
                       )
{
    return ch;
}

/*****************************************************************************/
// 	Description : 查找rtsp格式的URL
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMI_FindRtsp(
                           RGLEXPR_URL_RESULT_T *url_result_ptr
                           )
{
    FindURL((uint8*)"rtsp://[\\S]+", url_result_ptr);
}

/*****************************************************************************/
// 	Description : 查找http格式的URL
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMI_FindHttp(
                           RGLEXPR_URL_RESULT_T *url_result_ptr
                           )
{
    FindURL((uint8*)"http://[\\S]+", url_result_ptr);
}

/*****************************************************************************/
// 	Description : 查找符合正则表达式hhexpr_str的URL
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void FindURL(
                     uint8 *hhexpr_str,
                     RGLEXPR_URL_RESULT_T *url_result_ptr
                     )
{
    uint32 start_pos[RGLEXPR_URL_MAX_COUNT] = {0};
    uint32 end_pos[RGLEXPR_URL_MAX_COUNT] = {0};
    uint32 i = 0;
    //uint8 hhexpr_str[]  = "rtsp://[\\S]+";
    RGLEXPR_EXPR_T expr_ptr = {0};
    uint32 find_count = 0;
    BOOLEAN is_wstring = FALSE;

    //is_ascii = MMIAPICOM_IsASCIIString(url_result_ptr->str_ptr, url_result_ptr->str_length/2);
    is_wstring = url_result_ptr->is_wstring;
    
    expr_ptr.str_ptr = hhexpr_str;
    expr_ptr.current_pos = 0;
    InitialStateMachine();
    ParseRegularExpression(&expr_ptr);
    
    if (PNULL == url_result_ptr->str_ptr
        || 0 == url_result_ptr->str_length)
    {
        url_result_ptr->str_count = 0;
    }
    else
    {
        if (is_wstring)
        {
            find_count = FindMorePatternUCS2(
                url_result_ptr->str_ptr,
                0,
                url_result_ptr->str_length-1,
                start_pos,
                end_pos,
                RGLEXPR_URL_MAX_COUNT
                );
        }
        else
        {
            find_count = FindMorePattern(
                url_result_ptr->str_ptr,
                0,
                url_result_ptr->str_length-1,
                start_pos,
                end_pos,
                RGLEXPR_URL_MAX_COUNT
                );
        }

        for (i = 0; i < RGLEXPR_URL_MAX_COUNT; i++)
        {
            url_result_ptr->str_arr[i].start_pos = start_pos[i];
            url_result_ptr->str_arr[i].length = end_pos[i] + 1 - start_pos[i];
        }
        url_result_ptr->str_count = find_count;
    }
    DestroyStateMachine();
}


