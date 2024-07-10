%{
/*******************************************************************************
 ** File Name:      AT.y                                                       *
 ** Author:         Steven.Yao                                                 *
 ** DATE:           04/12/2002                                                 *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.          *
 ** Description:    This file describe the grammar rules of AT commands. GNU   *
 **                 Bison reads this file to produce the AT commands parser we *
 **                 want.                                                      *
 *******************************************************************************
                                                                               
 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                *
 ** 04/12/2002     Steven.Yao       Create.                                    *
 ** 06/05/2002     Ivan.Yin         add the handle about "A/"                  *
 ******************************************************************************/
 
/*******************************************************************************
 **                        Common Header Files                                 *
 ******************************************************************************/
#include "sci_types.h"


/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include "atc.h"
#include "atc_common.h"
#include "atc_malloc.h"
#include "atc_yacc.h"

#define ATCINITDEPTH 50

extern uint8 * atctext;
extern int32   atcleng;
extern char * atc_c_buf_p;
int32 atclex(void);

void ATCY_ErrorHandle( ATC_STATUS err_code );

LOCAL int32 atc_convert_num_param(void); // Return numeric parameter value
LOCAL ATC_CUR_PARAM_STRING_T * atc_convert_str_param(void); 
LOCAL ATC_CUR_PARAM_STRING_T * atc_convert_dialstr_param(void); 

LOCAL ATC_STATUS ATCY_NewNumParam( // Return S_ATC_SUCCESS if success,
                                     // otherwise return error code.
        ATC_CUR_PARAM_INFO_LIST_T ** param_ptr_ptr // OUT argument, return a
                                                   // param info list struct.
        );

LOCAL ATC_STATUS ATCY_NewStrParam( // Return S_ATC_SUCCESS if success,
                                     // otherwise return error code.
        ATC_CUR_PARAM_INFO_LIST_T ** param_ptr_ptr  // OUT argument, return a
                                                    // param info list struct.
        );

LOCAL ATC_STATUS ATCY_NewDialStrParam( // Return S_ATC_SUCCESS if success,
                                         // otherwise return error code.
        ATC_CUR_PARAM_INFO_LIST_T ** param_ptr_ptr  // OUT argument, return a
                                                    // param info list struct.
        );

LOCAL ATC_STATUS ATCY_NewDefaultParam( // Return S_ATC_SUCCESS if success,
                                         // otherwise return error code.
        ATC_CUR_PARAM_INFO_LIST_T ** param_ptr_ptr  // OUT argument, return a
                                                    // param info list struct.
        );

LOCAL ATC_CUR_PARAM_INFO_LIST_T * ATCY_LinkTwoParams(// return the linked list
        ATC_CUR_PARAM_INFO_LIST_T * param_first_ptr,
        ATC_CUR_PARAM_INFO_LIST_T * param_last_ptr
        );

LOCAL ATC_STATUS ATCY_NewCommand( // Return S_ATC_SUCCESS if success,
                                    // otherwise return error code
        ATC_CUR_INFO_LIST_T ** info_ptr_ptr, // OUT argument
        uint    cmd_type,           // AT command type
        uint    cmd_index,          // AT command index
        ATC_CUR_PARAM_INFO_LIST_T * param_list_ptr // AT command param info
        );

LOCAL ATC_CUR_INFO_LIST_T * ATCY_LinkTwoCmdInfo(// return the linked list
        ATC_CUR_INFO_LIST_T * info_list_first_ptr,
        ATC_CUR_INFO_LIST_T * info_list_last_ptr
        );

LOCAL ATC_STATUS ATCY_ParseError(// @Steven
        ATC_CUR_INFO_LIST_T * info_ptr
        );
        
ATC_STATUS  parse_status = S_ATC_SUCCESS;

%}

%no_lines

%token          AT_CMD_PREFIX
%token          AT_CMD_REPEAT
%token          AT_EXTENSION_CMD AT_BASIC_CMD
%token          AT_CMD_END  AT_CMD_DIVIDE
%token          NUMBER      HNUMBER     STRING    DIALSTRING  SHARPSTRING  DTMFSTRING

%union          {
                    uint                    cmd_index;
                    ATC_CUR_PARAM_INFO_LIST_T * cur_param_ptr;
                    ATC_CUR_INFO_LIST_T   * atc_cur_info_ptr;
                }

%type <cmd_index> AT_EXTENSION_CMD AT_BASIC_CMD
%type <cur_param_ptr> Param ExtParams ExtParam
%type <atc_cur_info_ptr> AT_cmds AT_compound_cmd AT_basic_cmd AT_ext_cmd 

%start          AT_cmd_line

%%

AT_cmd_line         :   AT_CMD_PREFIX AT_cmds AT_CMD_END
                        {
                            *info_ptr_ptr = $2;
                            return parse_status;
                        }
                    |	AT_CMD_PREFIX AT_CMD_END
                    	{
                            parse_status = ATCY_NewCommand( 
                                                info_ptr_ptr,
                                                ATC_CMD_TYPE_EXECUTE | ATC_CMD_TYPE_BASIC,
                                                0,
                                                NULL);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                            
                            return parse_status;       
                    	}
                    ;

AT_cmds             :   AT_cmds AT_CMD_DIVIDE AT_compound_cmd
                        {
                            $$ = ATCY_LinkTwoCmdInfo( $1, $3);
                            
                            *info_ptr_ptr = $$; // @Steven
                        }
                    |   AT_cmds AT_CMD_DIVIDE AT_basic_cmd
                        {
                            $$ = ATCY_LinkTwoCmdInfo( $1, $3);
                            
                            *info_ptr_ptr = $$; // @Steven
                        }
                    |   AT_cmds AT_CMD_DIVIDE AT_ext_cmd
                        {
                            $$ = ATCY_LinkTwoCmdInfo( $1, $3);
                            
                            *info_ptr_ptr = $$; // @Steven
                        }
                    |   AT_compound_cmd AT_ext_cmd
                        {
                            $$ = ATCY_LinkTwoCmdInfo( $1, $2);
                            
                            *info_ptr_ptr = $$; // @Steven
                        }
                    |   AT_compound_cmd
                        {
                            $$ = $1;
                            
                            *info_ptr_ptr = $$; // @Steven
                        }
                    |   AT_basic_cmd
                        {
                            $$ = $1;
                            
                            *info_ptr_ptr = $$; // @Steven
                        }
                    |   AT_ext_cmd
                        {
                            $$ = $1;
                            
                            *info_ptr_ptr = $$; // @Steven
                        }
                    ;
                    
AT_compound_cmd     :   AT_compound_cmd AT_basic_cmd
                        {
                            $$ = ATCY_LinkTwoCmdInfo( $1, $2);
                            
                            *info_ptr_ptr = $$; // @Steven
                        }
                    |   AT_basic_cmd AT_basic_cmd
                    	{
                    	    $$ = ATCY_LinkTwoCmdInfo( $1, $2);
                            
                            *info_ptr_ptr = $$; // @Steven
                    	}
                    ;
                    
AT_basic_cmd        :   AT_BASIC_CMD 
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_EXECUTE | ATC_CMD_TYPE_BASIC,
                                                $1,
                                                NULL);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    |   AT_BASIC_CMD Param
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_EXECUTE | ATC_CMD_TYPE_BASIC,
                                                $1,
                                                $2);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    |   AT_BASIC_CMD '?'
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_READ | ATC_CMD_TYPE_BASIC,
                                                $1,
                                                NULL);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    |   AT_BASIC_CMD '=' '?'
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_TEST | ATC_CMD_TYPE_BASIC,
                                                $1,
                                                NULL);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    |   AT_BASIC_CMD '=' Param
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_SET | ATC_CMD_TYPE_BASIC,
                                                $1,
                                                $3);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    ;        

Param               :   NUMBER
                        {
                            parse_status = ATCY_NewNumParam(&$$);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                        }
                    |   STRING
                        {
                            parse_status = ATCY_NewStrParam(&$$);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                        }
                    |   DIALSTRING
                        {
                            parse_status = ATCY_NewDialStrParam(&$$);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                        }
                    |   SHARPSTRING
                        {
                            parse_status = ATCY_NewDialStrParam(&$$);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                        }
                    |   DTMFSTRING
                        {
                            parse_status = ATCY_NewDialStrParam(&$$);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                        }
                        
                    ;         
                        
AT_ext_cmd          :   AT_EXTENSION_CMD
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_EXECUTE | ATC_CMD_TYPE_EXTEND,
                                                $1,
                                                NULL);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    |   AT_EXTENSION_CMD '?'
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_READ | ATC_CMD_TYPE_EXTEND,
                                                $1,
                                                NULL);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    |   AT_EXTENSION_CMD '=' '?'
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_TEST | ATC_CMD_TYPE_EXTEND,
                                                $1,
                                                NULL);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    |   AT_EXTENSION_CMD '=' ExtParams
                        {
                            parse_status = ATCY_NewCommand( 
                                                &$$,
                                                ATC_CMD_TYPE_SET | ATC_CMD_TYPE_EXTEND,
                                                $1,
                                                $3);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }       
                        }
                    ;
                    
ExtParams           :   ExtParam
                        {
                            $$ = $1;
                        }
                    |   ExtParams ',' ExtParam
                        {
                            $$ = ATCY_LinkTwoParams( $1, $3 );
                        }
                    ;
                    
ExtParam            :   NUMBER
                        {
                            parse_status = ATCY_NewNumParam(&$$);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                        }
                    |   STRING
                        {
                            parse_status = ATCY_NewStrParam(&$$);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                        }
                    |
                        {
                            parse_status = ATCY_NewDefaultParam(&$$);
                            if ( S_ATC_SUCCESS != parse_status )
                            {
                                ATCY_ErrorHandle( parse_status );
                                return parse_status;
                            }
                        }
                    ;                 
%%

void ATCY_ErrorHandle( ATC_STATUS err_code )
{
    ATCS_TRACE_STATUS( err_code);

    ATCM_FreeAll();

    return;
}

/******************************************************************************/
// Description : This function converts the numeric parameter string to an 
//               int32 value.
// Global resource dependence : atctext
//                              atcleng
// Author : Steven.Yao 
// Note : The numeric parameter string is contain in atctext.
/******************************************************************************/
LOCAL int32 atc_convert_num_param(void) // Return numeric parameter value
{
    int32       num_val;
    uint8       hold_char;

    SCI_ASSERT( NULL != atctext ); /*assert verified*/
    SCI_ASSERT( 0 < atcleng ); /*assert verified*/

    hold_char = atctext[atcleng];
    atctext[atcleng] = '\0';

    num_val = atoi( (const char *)atctext );

    atctext[atcleng] = hold_char;

    return num_val;
}

/******************************************************************************/
// Description : This function stores a string parameter in an
//               ATC_CUR_PARAM_STRING_T struct type.
// Global resource dependence : atctext
//                              atcleng
// Author : Steven.Yao 
// Note : None
/******************************************************************************/
LOCAL ATC_CUR_PARAM_STRING_T * atc_convert_str_param(void) 
{
    ATC_CUR_PARAM_STRING_T  * str_ptr = NULL;

    ATC_STATUS  status = S_ATC_SUCCESS;

    uint8 hold_char;

    SCI_ASSERT( NULL != atctext ); /*assert verified*/
    SCI_ASSERT( 1 < atcleng ); /*assert verified*/

    hold_char = atctext[atcleng - 1];
    atctext[atcleng - 1] = '\0';

    status = ATCM_Alloc( (void **)&str_ptr, sizeof(ATC_CUR_PARAM_STRING_T) );
    if ( S_ATC_SUCCESS != status )
    {
        return NULL;
    }

    str_ptr->str_len = atcleng - 2;
    status = ATCM_Alloc( (void **)&str_ptr->str_ptr, (atcleng - 1) );
    if ( S_ATC_SUCCESS != status )
    {
        return NULL;
    }

    memcpy( str_ptr->str_ptr, (atctext + 1), (atcleng - 1) );

    atctext[atcleng - 1] = hold_char;

    return str_ptr;
}

/******************************************************************************/
// Description : This function stores a dial string parameter in an 
//               ATC_CUR_PARAM_STRING_T struct type.
// Global resource dependence : atctext
//                              atcleng
// Author : Steven.Yao 
// Note : None
/******************************************************************************/
LOCAL ATC_CUR_PARAM_STRING_T * atc_convert_dialstr_param(void) 
{
    ATC_CUR_PARAM_STRING_T  * str_ptr = NULL;

    ATC_STATUS  status = S_ATC_SUCCESS;

    uint8 hold_char;

    SCI_ASSERT( NULL != atctext ); /*assert verified*/
    SCI_ASSERT( 0 < atcleng ); /*assert verified*/

    hold_char = atctext[atcleng];
    atctext[atcleng] = '\0';

    status = ATCM_Alloc( (void **)&str_ptr, sizeof(ATC_CUR_PARAM_STRING_T) );
    if ( S_ATC_SUCCESS != status )
    {
        return NULL;
    }

    str_ptr->str_len = atcleng;
    status = ATCM_Alloc( (void **)&str_ptr->str_ptr, (atcleng + 1) );
    if ( S_ATC_SUCCESS != status )
    {
        return NULL;
    }

    memcpy( str_ptr->str_ptr, atctext, (atcleng + 1) );

    atctext[atcleng] = hold_char;

    return str_ptr;
}

/******************************************************************************/
// Description : This function takes an numeric value and creates a new 
//               ATC_CUR_PARAM_INFO_LIST_T struct to include the value.
// Global resource dependence : None
// Author : Steven.Yao
// Note : This function is called when grammar parser do the following induce:
//        NUMBER ------> ExtParam 
//        NUMBER ------> Param
/******************************************************************************/
LOCAL ATC_STATUS ATCY_NewNumParam( // Return S_ATC_SUCCESS if success,
                                     // otherwise return error code.
        ATC_CUR_PARAM_INFO_LIST_T ** param_ptr_ptr // OUT argument, return a
                                                   // param info list struct.
        )
{
    ATC_STATUS  status = S_ATC_SUCCESS;

    ATC_CUR_PARAM_INFO_LIST_T * tmp_param_ptr = NULL;

    SCI_ASSERT( NULL != param_ptr_ptr ); /*assert verified*/

    status = ATCM_Alloc( (void **)&tmp_param_ptr, sizeof( ATC_CUR_PARAM_INFO_LIST_T ) );
    if ( S_ATC_SUCCESS != status )
    {
        return status;
    }

    tmp_param_ptr->param_type = ATC_CMD_PARAM_TYPE_NUMERIC;
    tmp_param_ptr->param_info.num = atc_convert_num_param();
    tmp_param_ptr->next = NULL;

    *param_ptr_ptr = tmp_param_ptr;

    return status;
}

/******************************************************************************/
// Description : This function takes an string value and creates a new 
//               ATC_CUR_PARAM_INFO_LIST_T struct to include the value.
// Global resource dependence : None
// Author : Steven.Yao
// Note : This function is called when grammar parser does the following induce:
//        STRING ------> ExtParam 
//        STRING ------> Param
//        DIALSTRING --> Param
/******************************************************************************/
LOCAL ATC_STATUS ATCY_NewStrParam( // Return S_ATC_SUCCESS if success,
                                     // otherwise return error code.
        ATC_CUR_PARAM_INFO_LIST_T ** param_ptr_ptr // OUT argument, return a
                                                   // param info list struct.
        )
{
    ATC_STATUS  status = S_ATC_SUCCESS;

    ATC_CUR_PARAM_INFO_LIST_T * tmp_param_ptr = NULL;

    SCI_ASSERT( NULL != param_ptr_ptr ); /*assert verified*/

    status = ATCM_Alloc( (void **)&tmp_param_ptr, sizeof( ATC_CUR_PARAM_INFO_LIST_T ) );
    if ( S_ATC_SUCCESS != status )
    {
        *param_ptr_ptr = NULL;

        return status;
    }

    tmp_param_ptr->param_type = ATC_CMD_PARAM_TYPE_STRING;
    tmp_param_ptr->param_info.str_ptr = atc_convert_str_param();
    tmp_param_ptr->next = NULL;

    if ( NULL == tmp_param_ptr->param_info.str_ptr )
    {
        *param_ptr_ptr = NULL;

        return ERR_MEMORY_FULL;
    }

    *param_ptr_ptr = tmp_param_ptr;

    return status;
}

/******************************************************************************/
// Description : This function takes an dial string value and creates a new 
//               ATC_CUR_PARAM_INFO_LIST_T struct to include the value.
// Global resource dependence : None
// Author : Steven.Yao
// Note : This function is called when grammar parser does the following induce:
//        DIALSTRING --> Param
/******************************************************************************/
LOCAL ATC_STATUS ATCY_NewDialStrParam( // Return S_ATC_SUCCESS if success,
                                     // otherwise return error code.
        ATC_CUR_PARAM_INFO_LIST_T ** param_ptr_ptr // OUT argument, return a
                                                   // param info list struct.
        )
{
    ATC_STATUS  status = S_ATC_SUCCESS;

    ATC_CUR_PARAM_INFO_LIST_T * tmp_param_ptr = NULL;

    SCI_ASSERT( NULL != param_ptr_ptr ); /*assert verified*/

    status = ATCM_Alloc( (void **)&tmp_param_ptr, sizeof( ATC_CUR_PARAM_INFO_LIST_T ) );
    if ( S_ATC_SUCCESS != status )
    {
        *param_ptr_ptr = NULL;

        return status;
    }

    tmp_param_ptr->param_type = ATC_CMD_PARAM_TYPE_STRING;
    tmp_param_ptr->param_info.str_ptr = atc_convert_dialstr_param();
    tmp_param_ptr->next = NULL;

    if ( NULL == tmp_param_ptr->param_info.str_ptr )
    {
        *param_ptr_ptr = NULL;

        return ERR_MEMORY_FULL;
    }

    *param_ptr_ptr = tmp_param_ptr;

    return status;
}

/******************************************************************************/
// Description : This function creates a new ATC_CUR_PARAM_INFO_LIST_T struct 
//               whose parameter type is default.
// Global resource dependence : None
// Author : Steven.Yao
// Note : This function is called when grammar parser does the following induce:
//               ------> ExtParam 
/******************************************************************************/
LOCAL ATC_STATUS ATCY_NewDefaultParam( // Return S_ATC_SUCCESS if success,
                                         // otherwise return error code.
        ATC_CUR_PARAM_INFO_LIST_T ** param_ptr_ptr  // OUT argument, return a
                                                    // param info list struct.
        )
{
    ATC_STATUS  status = S_ATC_SUCCESS;

    ATC_CUR_PARAM_INFO_LIST_T * tmp_param_ptr = NULL;

    SCI_ASSERT( NULL != param_ptr_ptr );  /*assert verified*/

    status = ATCM_Alloc( (void **)&tmp_param_ptr, sizeof( ATC_CUR_PARAM_INFO_LIST_T ) );
    if ( S_ATC_SUCCESS != status )
    {
        return status;
    }

    tmp_param_ptr->param_type = ATC_CMD_PARAM_TYPE_DEFAULT;
    tmp_param_ptr->param_info.str_ptr = NULL;
    tmp_param_ptr->next = NULL;

    *param_ptr_ptr = tmp_param_ptr;

    return status;
}

/******************************************************************************/
// Description : This function links two parameter info list into one.
// Global resource dependence : None
// Author : Steven.Yao
// Note : This function is called when grammar parser does the following induce:
//        ExtParams ',' ExtParam ------> ExtParams 
/******************************************************************************/
LOCAL ATC_CUR_PARAM_INFO_LIST_T * ATCY_LinkTwoParams(// return the linked list
        ATC_CUR_PARAM_INFO_LIST_T * param_first_ptr,
        ATC_CUR_PARAM_INFO_LIST_T * param_last_ptr
        )
{
    ATC_CUR_PARAM_INFO_LIST_T * param_ptr = NULL;

    SCI_ASSERT( NULL != param_first_ptr ); /*assert verified*/
    SCI_ASSERT( NULL != param_last_ptr ); /*assert verified*/

    param_ptr = param_first_ptr;
    
    /* Find the last param of param_ptr */
    while ( NULL != param_ptr->next )
    {
        param_ptr = param_ptr->next;
    }

    param_ptr->next = param_last_ptr;

    return param_first_ptr;
}

/******************************************************************************/
// Description : This function creates a new AT cmd info list struct according
//               to the input info.
// Global resource dependence : 
// Author : Steven.Yao
// Note : This function is called when grammar parser does the following induce:
//        AT_BASIC_CMD              ------> AT_basic_cmd 
//        AT_BASIC_CMD Param        ------> AT_basic_cmd 
//        AT_BASIC_CMD ?            ------> AT_basic_cmd 
//        AT_BASIC_CMD = ?          ------> AT_basic_cmd 
//        AT_BASIC_CMD = Param      ------> AT_basic_cmd 
//        AT_EXTEND_CMD             ------> AT_ext_cmd
//        AT_EXTEND_CMD ?           ------> AT_ext_cmd
//        AT_EXTEND_CMD = ?         ------> AT_ext_cmd
//        AT_EXTEND_CMD = ExtParams ------> AT_ext_cmd
/******************************************************************************/
LOCAL ATC_STATUS ATCY_NewCommand( // Return S_ATC_SUCCESS if success,
                                    // otherwise return error code
        ATC_CUR_INFO_LIST_T ** info_ptr_ptr, // OUT argument
        uint    cmd_type,           // AT command type
        uint    cmd_index,          // AT command index
        ATC_CUR_PARAM_INFO_LIST_T * param_list_ptr // AT command param info
        )
{
    ATC_STATUS  status = S_ATC_SUCCESS;
    
    uint    i;

    ATC_CUR_INFO_LIST_T * tmp_info_ptr = NULL;

    SCI_ASSERT( NULL != info_ptr_ptr ); /*assert verified*/
    
    status = ATCM_Alloc( (void **)&tmp_info_ptr, sizeof(ATC_CUR_INFO_LIST_T) );
    if ( S_ATC_SUCCESS != status )
    {
        return status;
    }

    tmp_info_ptr->cmd_type = cmd_type;
    tmp_info_ptr->cmd_index = cmd_index;
    tmp_info_ptr->cur_param_list_ptr = param_list_ptr;
    tmp_info_ptr->next = NULL;
    tmp_info_ptr->cur_param_num = 0;
    
    for( i = 0; i < ATC_MAX_PARAM_NUM; i++)
    {
        tmp_info_ptr->param_used[i] = FALSE;
    }
    
    while ( NULL != param_list_ptr )
    {
        if ( ATC_CMD_PARAM_TYPE_DEFAULT != param_list_ptr->param_type )
        {
            tmp_info_ptr->param_used[tmp_info_ptr->cur_param_num] = TRUE;
        }
        
        tmp_info_ptr->cur_param_num++;

        param_list_ptr = param_list_ptr->next;
    }

    *info_ptr_ptr = tmp_info_ptr;

    return status;
}

/******************************************************************************/
// Description : This function links two cmd info list into one.
// Global resource dependence : 
// Author : Steven.Yao
// Note : This function is called when grammar parser does the following induce:
//        AT_basic_cmd AT_ext_cmd               ------> AT_compound_cmd
//        AT_cmds AT_CMD_DIVIDE AT_compound_cmd ------> AT_cmds
//        AT_cmds AT_CMD_DIVIDE AT_basic_cmd    ------> AT_cmds
//        AT_cmds AT_CMD_DIVIDE AT_ext_cmd      ------> AT_cmds
/******************************************************************************/
LOCAL ATC_CUR_INFO_LIST_T * ATCY_LinkTwoCmdInfo(// return the linked list
        ATC_CUR_INFO_LIST_T * info_list_first_ptr,
        ATC_CUR_INFO_LIST_T * info_list_last_ptr
        )
{
    ATC_CUR_INFO_LIST_T *   info_list_ptr = NULL;

    SCI_ASSERT( NULL != info_list_first_ptr ); /*assert verified*/
    SCI_ASSERT( NULL != info_list_last_ptr ); /*assert verified*/

    info_list_ptr = info_list_first_ptr;

    /* Find the last cmd info in info_list_ptr */
    while ( NULL != info_list_ptr->next )
    {
        info_list_ptr = info_list_ptr->next;
    }

    info_list_ptr->next = info_list_last_ptr;

    return info_list_first_ptr;
}

LOCAL ATC_STATUS ATCY_ParseError(// @Steven
        ATC_CUR_INFO_LIST_T * info_ptr
        )
{
    //@Shijun.cui 2005-04-05 do not do anything here CR17325
    /*ATC_CUR_INFO_LIST_T *   error_info_ptr= NULL;
    
    if ( NULL == info_ptr )
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }
    
    parse_status = ATCY_NewCommand(
                        &error_info_ptr,
                        ATC_CMD_TYPE_ERROR,
                        0,
                        NULL);
                        
    if ( S_ATC_SUCCESS != parse_status )
    {
        ATCY_ErrorHandle( parse_status );
        return parse_status;
    }
    
    ATCY_LinkTwoCmdInfo( info_ptr, error_info_ptr);
    
    return S_ATC_SUCCESS;*/
    return ERR_OPERATION_NOT_SUPPORTED;
}
