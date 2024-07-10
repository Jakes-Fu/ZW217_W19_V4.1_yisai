/******************************************************************************
 ** File Name:      atpgen_file.h                                             *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/15/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains functions' definitions used to generate*
 **                 output files according to the ATC info struct and template*
 **                 structs.                                                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/15/2002     Steven.Yao       Create.                                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include "atpgen_from_template.h"
#include "atp_atc_info.h"
#include "atpgen_file.h"

#define SCI_ASSERT(_bool)  assert(_bool)

ATC_DEFAULT_RULES_T g_atc_default_rules = {0};

void atpgen_one_info_from_tpl(
        FILE            * f_out,
        const ATC_INFO_T      * atc_info_ptr,
        const ATP_TPL_LINE_T  * tpl_line_ptr
        );

void atpgen_from_tpl(
        FILE            * f_out,
        const ATC_INFO_T      * atc_info_ptr,
        const ATP_TPL_FILE_T  * tpl_file_ptr,
        int             is_atc_lex
        );

void atpgen_info_file(
        const ATC_INFO_T *  atc_info_ptr,
        const ATP_TPL_FILE_T * tpl_file_ptr,
        const char *  info_file_name
        )
{
    const ATP_TPL_LINE_T  * cmd_name_tpl_line_ptr = NULL;
    const ATP_TPL_LINE_T  * num_param_tpl_line_ptr = NULL;
    const ATP_TPL_LINE_T  * str_param_tpl_line_ptr = NULL;
    const ATP_TPL_LINE_T  * param_info_tpl_line_ptr = NULL;
    const ATP_TPL_LINE_T  * cmd_info_tpl_line_ptr = NULL;

    const ATP_TPL_FILE_T  * cur_tpl_file_ptr = NULL;
    const ATP_FIX_LINE_T  * cur_fix_line_ptr = NULL;

    const ATC_INFO_T      * cur_atc_info_ptr = NULL;
    const ATC_PARAM_INFO_T* cur_param_info_ptr = NULL;
    const ATC_PARAM_STRING_ARRAY_T   * cur_str_ptr = NULL;
    const ATC_PARAM_NUMERIC_ARRAY_T  * cur_num_ptr = NULL;

    int         cur_param_num = 0;
    int         cur_cmd_index = 0;

    FILE *      f_info = NULL;
 
    char        str_tmp[255];  /* Contain a string which will be 
                                  written to info file soon */
    
    SCI_ASSERT( NULL != atc_info_ptr );
    SCI_ASSERT( NULL != tpl_file_ptr );
    SCI_ASSERT( NULL != info_file_name );
 
    f_info = fopen( info_file_name, "w+");
    if ( NULL == f_info )
    {
        printf("Error! Can not create file %s.\n", info_file_name);
        exit(1);
    }

    cur_tpl_file_ptr = tpl_file_ptr;
    while ( TPL_FILE_BLOCK_TYPE_TPL != cur_tpl_file_ptr->block_type )
    {
        cur_fix_line_ptr = cur_tpl_file_ptr->fix_line_ptr;
        
        while ( NULL != cur_fix_line_ptr )
        {
            fwrite( cur_fix_line_ptr->line_buf, 
                    1, 
                    cur_fix_line_ptr->line_len, 
                    f_info );

            cur_fix_line_ptr = cur_fix_line_ptr->next;
        }

        cur_tpl_file_ptr = cur_tpl_file_ptr->next;
    } 

    cmd_name_tpl_line_ptr = cur_tpl_file_ptr->tpl_line_ptr;
    SCI_ASSERT( NULL != cmd_name_tpl_line_ptr );
    cur_tpl_file_ptr = cur_tpl_file_ptr->next;
    
    while ( TPL_FILE_BLOCK_TYPE_TPL != cur_tpl_file_ptr->block_type )
    {
        cur_tpl_file_ptr = cur_tpl_file_ptr->next;
    }

    num_param_tpl_line_ptr = cur_tpl_file_ptr->tpl_line_ptr;
    SCI_ASSERT( NULL != num_param_tpl_line_ptr );
    cur_tpl_file_ptr = cur_tpl_file_ptr->next;

    while ( TPL_FILE_BLOCK_TYPE_TPL != cur_tpl_file_ptr->block_type )
    {
        cur_tpl_file_ptr = cur_tpl_file_ptr->next;
    }

    str_param_tpl_line_ptr = cur_tpl_file_ptr->tpl_line_ptr;
    SCI_ASSERT( NULL != str_param_tpl_line_ptr );
    cur_tpl_file_ptr = cur_tpl_file_ptr->next;

    while ( TPL_FILE_BLOCK_TYPE_TPL != cur_tpl_file_ptr->block_type )
    {
        cur_tpl_file_ptr = cur_tpl_file_ptr->next;
    }

    param_info_tpl_line_ptr = cur_tpl_file_ptr->tpl_line_ptr;
    SCI_ASSERT( NULL != param_info_tpl_line_ptr );
    cur_tpl_file_ptr = cur_tpl_file_ptr->next;

    
    cur_atc_info_ptr = atc_info_ptr;
    while ( NULL != cur_atc_info_ptr )
    {
        atpgen_one_info_from_tpl( 
                         f_info,
                         cur_atc_info_ptr,
                         cmd_name_tpl_line_ptr);
        
        sprintf( str_tmp,
                 "[1] = \n{\n    { %1d, (const uint8 *)\"%s\" }\n};\n",
                 cur_atc_info_ptr->cmd_name.len,
                 cur_atc_info_ptr->cmd_name.str);
        fwrite( str_tmp, 1, strlen(str_tmp), f_info);
                
        if ( 0 == cur_atc_info_ptr->param_info_num )
        {
            cur_atc_info_ptr = cur_atc_info_ptr->next;
            continue;
        }

        cur_param_info_ptr = cur_atc_info_ptr->param_head;
        cur_param_num = 0;

        while ( NULL != cur_param_info_ptr )
        {
            cur_param_num++;

            switch ( cur_param_info_ptr->param_type )
            {
            case ATC_PARAM_TYPE_NUMERIC:
                cur_num_ptr = cur_param_info_ptr->num_head;
                SCI_ASSERT( NULL != cur_num_ptr );

                atpgen_one_info_from_tpl( f_info, 
                                 cur_atc_info_ptr, 
                                 num_param_tpl_line_ptr);

                sprintf( str_tmp, 
                         "%1d[%1d] = \n{\n", 
                         cur_param_num,
                         cur_param_info_ptr->param_range_num);
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);

                while ( NULL != cur_num_ptr )
                {
                    if ( NULL != cur_num_ptr->next )
                    {
                        sprintf( str_tmp,
                                 "    { %4d, %4d },\n",
                                 cur_num_ptr->num.min_val,
                                 cur_num_ptr->num.max_val);
                    }
                    else
                    {
                        sprintf( str_tmp,
                                 "    { %4d, %4d }\n",
                                 cur_num_ptr->num.min_val,
                                 cur_num_ptr->num.max_val);
                    }

                    fwrite( str_tmp, 1, strlen(str_tmp), f_info);
                    cur_num_ptr = cur_num_ptr->next;
                }
                fwrite( "};\n", 1, 3, f_info);
                break;

            case ATC_PARAM_TYPE_STRING:
                cur_str_ptr = cur_param_info_ptr->str_head;
                SCI_ASSERT( NULL != cur_str_ptr );

                atpgen_one_info_from_tpl( f_info,
                                 cur_atc_info_ptr,
                                 str_param_tpl_line_ptr);

                sprintf( str_tmp,
                         "%1d[%1d] = \n{\n",
                         cur_param_num,
                         cur_param_info_ptr->param_range_num);
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);

                while ( NULL != cur_str_ptr )
                {
                    if ( NULL != cur_str_ptr->next )
                    {
                        sprintf( str_tmp,
                                 "    { %4d, (const uint8 *)%s },\n",
                                 (cur_str_ptr->str.len - 2),
                                 cur_str_ptr->str.str);
                    }
                    else
                    {
                        sprintf( str_tmp,
                                 "    { %4d, (const uint8 *)%s }\n",
                                 (cur_str_ptr->str.len - 2),
                                 cur_str_ptr->str.str);
                    }

                    fwrite( str_tmp, 1, strlen(str_tmp), f_info);
                    cur_str_ptr = cur_str_ptr->next;
                }
                fwrite( "};\n", 1, 3, f_info);
                break;

            case ATC_PARAM_TYPE_NONE:
                break;

            default:
                printf("Error! No other parameter type");
                SCI_ASSERT( 1 );
                break;
            }

            cur_param_info_ptr = cur_param_info_ptr->next;
        }

        atpgen_one_info_from_tpl( f_info,
                         cur_atc_info_ptr,
                         param_info_tpl_line_ptr);

        sprintf( str_tmp,
                 "[%2d] = \n{\n", 
                 cur_param_num);
        fwrite( str_tmp, 1, strlen(str_tmp), f_info);

        cur_param_info_ptr = cur_atc_info_ptr->param_head;
        cur_param_num = 0;

        while ( NULL != cur_param_info_ptr )
        {
            cur_param_num++;

            switch ( cur_param_info_ptr->param_type )
            {
            case ATC_PARAM_TYPE_NUMERIC:
                if ( NULL != cur_param_info_ptr->next )
                {
                    sprintf( str_tmp,
                             "    { %2d, %2d, (const ATC_PARAM_STRING_T *)atc_param_numeric_%s%1d },\n",
                             cur_param_info_ptr->param_type,
                             cur_param_info_ptr->param_range_num,
                             cur_atc_info_ptr->cmd_name_no_plus.str,
                             cur_param_num);
                }
                else
                {
                    sprintf( str_tmp,
                             "    { %2d, %2d, (const ATC_PARAM_STRING_T *)atc_param_numeric_%s%1d }\n",
                             cur_param_info_ptr->param_type,
                             cur_param_info_ptr->param_range_num,
                             cur_atc_info_ptr->cmd_name_no_plus.str,
                             cur_param_num);
                }
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);
                break; 

            case ATC_PARAM_TYPE_STRING:
                if ( NULL != cur_param_info_ptr->next )
                {
                    sprintf( str_tmp,
                             "    { %2d, %2d, (const ATC_PARAM_STRING_T *)atc_param_string_%s%1d },\n",
                             cur_param_info_ptr->param_type,
                             cur_param_info_ptr->param_range_num,
                             cur_atc_info_ptr->cmd_name_no_plus.str,
                             cur_param_num);
                }
                else
                {
                    sprintf( str_tmp,
                             "    { %2d, %2d, (const ATC_PARAM_STRING_T *)atc_param_string_%s%1d }\n",
                             cur_param_info_ptr->param_type,
                             cur_param_info_ptr->param_range_num,
                             cur_atc_info_ptr->cmd_name_no_plus.str,
                             cur_param_num);
                }
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);
                break;

            case ATC_PARAM_TYPE_NONE:
                if ( NULL != cur_param_info_ptr->next )
                {
                    sprintf( str_tmp,
                             "    { %2d, %2d, (const ATC_PARAM_STRING_T *)NULL },\n",
                             cur_param_info_ptr->param_type,
                             cur_param_info_ptr->param_range_num);
                }
                else
                {
                    sprintf( str_tmp,
                             "    { %2d, %2d, (const ATC_PARAM_STRING_T *)NULL }\n",
                             cur_param_info_ptr->param_type,
                             cur_param_info_ptr->param_range_num);
                }
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);
                break;

            default:
                printf("Error! No other parameter type\n");
                SCI_ASSERT( 1 );
                break;
            }
            
            cur_param_info_ptr = cur_param_info_ptr->next;
        }

        fwrite( "};\n", 1, 3, f_info);

        cur_atc_info_ptr = cur_atc_info_ptr->next;
    }

    while ( TPL_FILE_BLOCK_TYPE_TPL != cur_tpl_file_ptr->block_type )
    {
        cur_fix_line_ptr = cur_tpl_file_ptr->fix_line_ptr;
        
        while ( NULL != cur_fix_line_ptr )
        {
            fwrite( cur_fix_line_ptr->line_buf, 
                    1, 
                    cur_fix_line_ptr->line_len, 
                    f_info );

            cur_fix_line_ptr = cur_fix_line_ptr->next;
        }

        cur_tpl_file_ptr = cur_tpl_file_ptr->next;
    }

    cur_cmd_index = 0;
    cur_atc_info_ptr = atc_info_ptr;
    while ( NULL != cur_atc_info_ptr )
    {
        if ( 0 != cur_atc_info_ptr->param_info_num )
        {
            if ( NULL != cur_atc_info_ptr->next )
            {
                sprintf( str_tmp,
                         "    { %3d, %3d, atc_cmd_name_%s, %2d, atc_param_info_%s },\n",
                         cur_cmd_index,
                         cur_atc_info_ptr->cmd_type,
                         cur_atc_info_ptr->cmd_name_no_plus.str,
                         cur_atc_info_ptr->param_info_num,
                         cur_atc_info_ptr->cmd_name_no_plus.str);
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);            
            }
            else
            {
                sprintf( str_tmp,
                         "    { %3d, %3d, atc_cmd_name_%s, %2d, atc_param_info_%s }\n",
                         cur_cmd_index,
                         cur_atc_info_ptr->cmd_type,
                         cur_atc_info_ptr->cmd_name_no_plus.str,
                         cur_atc_info_ptr->param_info_num,
                         cur_atc_info_ptr->cmd_name_no_plus.str);
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);            
            }
        }
        else
        {
            if ( NULL != cur_atc_info_ptr->next )
            {
                sprintf( str_tmp,
                         "    { %3d, %3d, atc_cmd_name_%s, %2d, NULL },\n",
                         cur_cmd_index,
                         cur_atc_info_ptr->cmd_type,
                         cur_atc_info_ptr->cmd_name_no_plus.str,
                         cur_atc_info_ptr->param_info_num);
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);            
            }
            else
            {
                sprintf( str_tmp,
                         "    { %3d, %3d, atc_cmd_name_%s, %2d, NULL }\n",
                         cur_cmd_index,
                         cur_atc_info_ptr->cmd_type,
                         cur_atc_info_ptr->cmd_name_no_plus.str,
                         cur_atc_info_ptr->param_info_num);
                fwrite( str_tmp, 1, strlen(str_tmp), f_info);            
            }
        }
        
        cur_cmd_index++;
        cur_atc_info_ptr = cur_atc_info_ptr->next;
    }

    cur_tpl_file_ptr = cur_tpl_file_ptr->next;
    atpgen_from_tpl( f_info,
                     atc_info_ptr,
                     cur_tpl_file_ptr,
                     0);

    fclose( f_info );
}

void atpgen_other_file(
        const ATC_INFO_T     * atc_info_ptr,
        const ATP_TPL_FILE_T * tpl_file_ptr,
        const char           * other_file_name
        )
{
    FILE *      f_other = NULL;
    int is_atc_lex = 0;
 
    SCI_ASSERT( NULL != atc_info_ptr );
    SCI_ASSERT( NULL != tpl_file_ptr );
    SCI_ASSERT( NULL != other_file_name );

    if ((strstr(other_file_name, "atc"))&&(strstr(other_file_name, ".lex")))
    {
        is_atc_lex = 1;
    }

    f_other = fopen( other_file_name, "w+");
    if ( NULL == f_other )
    {
        printf("Error! Can not create file %s.\n", other_file_name);
        exit(1);
    }

    atpgen_from_tpl( f_other,
                     atc_info_ptr,
                     tpl_file_ptr, 
                     is_atc_lex);

    fclose( f_other );
}

void atpgen_one_info_from_tpl(
        FILE            * f_out,
        const ATC_INFO_T      * atc_info_ptr,
        const ATP_TPL_LINE_T  * tpl_line_ptr
        )
{
    const ATP_TPL_LINE_T      * cur_tpl_line_ptr = NULL;

    SCI_ASSERT( NULL != f_out );
    SCI_ASSERT( NULL != atc_info_ptr );
    SCI_ASSERT( NULL != tpl_line_ptr );

    cur_tpl_line_ptr = tpl_line_ptr;

    while ( NULL != cur_tpl_line_ptr )
    {
        switch ( cur_tpl_line_ptr->type )
        {
        case TPL_TYPE_FIX_STR:
            fwrite( cur_tpl_line_ptr->fix_str, 
                    1, 
                    cur_tpl_line_ptr->length, 
                    f_out);
            break; 

        case TPL_TYPE_ATCNAME:
            fwrite( atc_info_ptr->cmd_name.str,
                    1,
                    atc_info_ptr->cmd_name.len,
                    f_out);
            break;

        case TPL_TYPE_ATCNAMENOPLUS:
            fwrite( atc_info_ptr->cmd_name_no_plus.str,
                    1,
                    atc_info_ptr->cmd_name_no_plus.len,
                    f_out);
            break;

        default:
            printf("Error! No other tpl types!");
            SCI_ASSERT( 1 );
            break;
        }
        
        cur_tpl_line_ptr = cur_tpl_line_ptr->next;
    }
}

void atpgen_from_tpl(
        FILE            * f_out,
        const ATC_INFO_T      * atc_info_ptr,
        const ATP_TPL_FILE_T  * tpl_file_ptr,
        int             is_atc_lex
        )
{
    const ATC_INFO_T          * cur_atc_info_ptr = NULL;
    const ATP_TPL_FILE_T      * cur_tpl_file_ptr = NULL;
    const ATP_TPL_LINE_T      * cur_tpl_line_ptr = NULL;
    const ATP_FIX_LINE_T      * cur_fix_line_ptr = NULL;

    int         atc_index = 0;
    char        str_tmp[255];
    char        *ext_fix_str = "; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }";
    char        *bsc_fix_str = "; BEGIN(AT_CMD); return AT_BASIC_CMD; }";

    SCI_ASSERT( NULL != f_out );
    SCI_ASSERT( NULL != atc_info_ptr );
    SCI_ASSERT( NULL != tpl_file_ptr );

    cur_tpl_file_ptr = tpl_file_ptr;

    while ( NULL != cur_tpl_file_ptr )
    {
        switch ( cur_tpl_file_ptr->block_type )
        {
        case TPL_FILE_BLOCK_TYPE_FIX:
            cur_fix_line_ptr = cur_tpl_file_ptr->fix_line_ptr;

            while ( NULL != cur_fix_line_ptr )
            {
                fwrite( cur_fix_line_ptr->line_buf,
                        1, 
                        cur_fix_line_ptr->line_len,
                        f_out);

                cur_fix_line_ptr = cur_fix_line_ptr->next;
            }
            break;

        case TPL_FILE_BLOCK_TYPE_TPL:
            cur_atc_info_ptr = atc_info_ptr;
            atc_index = 0;

            while ( NULL != cur_atc_info_ptr )
            {
                if (is_atc_lex && ATC_IsDefaultCmdRulePresent(cur_atc_info_ptr->cmd_name_no_plus.str))
                {
                    cur_atc_info_ptr = cur_atc_info_ptr->next;
                    atc_index++;
                    continue;
                }
                
                cur_tpl_line_ptr = cur_tpl_file_ptr->tpl_line_ptr;

                while ( NULL != cur_tpl_line_ptr )
                {
                    switch ( cur_tpl_line_ptr->type )
                    {
                    case TPL_TYPE_FIX_STR:
                        if (is_atc_lex 
                            && (strstr(cur_tpl_line_ptr->fix_str, "AT_EXTENSION_CMD")
                            ||  strstr(cur_tpl_line_ptr->fix_str, "AT_BASIC_CMD")))
                        {
                            if (Get_cmd_type(cur_atc_info_ptr->cmd_name_no_plus))
                            {
                                fwrite( bsc_fix_str, 1, strlen(bsc_fix_str), f_out);
                            }
                            else
                            {
                                fwrite( ext_fix_str, 1, strlen(ext_fix_str), f_out);
                            }
                        }
                        else
                        {
                            fwrite( cur_tpl_line_ptr->fix_str, 
                                    1, 
                                    cur_tpl_line_ptr->length, 
                                    f_out);
                        }
                        break; 
    
                    case TPL_TYPE_ATCNAME:
                        fwrite( cur_atc_info_ptr->cmd_name.str,
                                1,
                                cur_atc_info_ptr->cmd_name.len,
                                f_out);

                        break;
    
                    case TPL_TYPE_ATCNAMENOPLUS:
                        fwrite( cur_atc_info_ptr->cmd_name_no_plus.str,
                                1,
                                cur_atc_info_ptr->cmd_name_no_plus.len,
                                f_out);
                        break;
    
                    case TPL_TYPE_ATCINDEX:
                        sprintf( str_tmp, "%4d", atc_index );
                        fwrite( str_tmp, 1, strlen(str_tmp), f_out);
                        break;

                    case TPL_TYPE_ATCTYPE:
                        sprintf( str_tmp, "%3d", cur_atc_info_ptr->cmd_type );
                        fwrite( str_tmp, 1, strlen(str_tmp), f_out);
                        break;

                    case TPL_TYPE_ATCPARAMNUM:
                        sprintf( str_tmp, "%2d", cur_atc_info_ptr->param_info_num );
                        fwrite( str_tmp, 1, strlen(str_tmp), f_out);
                        break;

                    default:
                        printf("Error! No other tpl types!");
                        SCI_ASSERT( 1 );
                        break;
                    }
        
                    cur_tpl_line_ptr = cur_tpl_line_ptr->next;
                }

                cur_atc_info_ptr = cur_atc_info_ptr->next;
                atc_index++;
            }
            break;

        default:
            printf("Error! No other template type\n");
            SCI_ASSERT( 1 );
            break;
        }
        
        cur_tpl_file_ptr = cur_tpl_file_ptr->next;
    }

}

unsigned int Get_cmd_type(ATC_PARAM_STRING_T cmd_name)
{
    unsigned int cmd_type = 0; // 1 BASIC_CMD_TYPE, 0 EXTENSION_CMD_TYPE

    SCI_ASSERT(NULL != cmd_name.str);

    if (cmd_name.str[0] != '+')
    {
        cmd_type |= !strncmp(cmd_name.str, "AT", 2) && (cmd_name.len == 2) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "SHARP_SHARP", 11) && (cmd_name.len == 11) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "D", 1) && (cmd_name.len == 1) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "Q", 1) && (cmd_name.len == 1) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "V", 1) && (cmd_name.len == 1) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "A", 1) && (cmd_name.len == 1) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "H", 1) && (cmd_name.len == 1) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "Z", 1) && (cmd_name.len == 1) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "E", 1) && (cmd_name.len == 1) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "W", 1) && (cmd_name.len == 1) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "S95", 3) && (cmd_name.len == 3) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "THREE_PERCENT", 13) && (cmd_name.len == 13) ? 1 : 0;
        cmd_type |= !strncmp(cmd_name.str, "and_", 4) ? 1 : 0;
    }
    else
    {
        cmd_type = 0;
    }

    return cmd_type;
}

void ATC_GetDefaultRules(const char * file_name)
{
    FILE * ftpl = NULL;
    char   line[255] = {0};
    char * str_pos1 = 0;
    char * str_pos2 = 0;
    char * cmd_name_no_plus = NULL;
    int    cmd_name_len = 0;
    int    tmp_len = 0;
    int    index = 0;
    unsigned char is_rule_area = 0;


    ftpl = atp_open_template_file( file_name );

    g_atc_default_rules.rule_num = 0;
    memset((void*)g_atc_default_rules.rule_cmd_name, 0, 255*sizeof(ATC_PARAM_STRING_T));

    cmd_name_no_plus = (char *)atp_malloc(3);
    strcpy(cmd_name_no_plus, "AT");

    g_atc_default_rules.rule_num++;
    g_atc_default_rules.rule_cmd_name[index].len = 2;
    g_atc_default_rules.rule_cmd_name[index++].str = cmd_name_no_plus;

    tmp_len = strlen("{ yylval.cmd_index = AT_CMD_");
    while(fgets(line, 255, ftpl) != NULL)
    {
        if (is_rule_area == 0)
        {
            if ((line[0] == '\%') && (line[1] == '\%'))
            {
                is_rule_area = 1;
                continue;
            }
            else
            {
                continue;
            }
        }

        if (strstr(line, "###atcbegin"))
        {
            break;
        }

        str_pos1 = strstr(line, "{ yylval.cmd_index = AT_CMD_");
        str_pos2 = strstr(line, "; BEGIN(AT_");
        cmd_name_len = (int)(str_pos2 - str_pos1 - tmp_len);

        if ((str_pos1 == NULL) || (str_pos2 == NULL)||(cmd_name_len <= 0))
        {
            continue;
        }

        cmd_name_no_plus = (char *)atp_malloc(cmd_name_len + 1);
        memcpy(cmd_name_no_plus, (str_pos1 + tmp_len), cmd_name_len);
        cmd_name_no_plus[cmd_name_len] = '\0';

        g_atc_default_rules.rule_num++;
        g_atc_default_rules.rule_cmd_name[index].len = cmd_name_len;
        g_atc_default_rules.rule_cmd_name[index++].str = cmd_name_no_plus;
    }

    fclose(ftpl);
}

unsigned char ATC_IsDefaultCmdRulePresent(char * cmd_name_plus)
{
    int i;

    if (cmd_name_plus == NULL)
    {
        return 0;
    }

    for (i = 0; i < g_atc_default_rules.rule_num; i++)
    {
        if (0 == strcmp(g_atc_default_rules.rule_cmd_name[i].str, cmd_name_plus))
        {
            return 1;
        }
    }

    return 0;
}

