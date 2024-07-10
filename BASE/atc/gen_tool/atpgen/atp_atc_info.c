/******************************************************************************
 ** File Name:      atp_atc_info.c                                            *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/15/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains function definitions used by grammar   *
 **                 analyzer to store ATC infomation when parsing ATC         *
 **                 infomation definition file.                               *
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
#include <assert.h>
#include "atp_atc_info.h"
#include "atp_malloc.h"
#include "atpgen_yacc.h"

/* external variables declared in atpgen_lex.c */
extern FILE * atpin;
extern char * atptext;
extern int  atpleng;

/* external variables declared in atpgen_yacc.c */
extern ATPSTYPE atplval;
extern int atpparse();

#define SCI_ASSERT(_bool)  assert(_bool)

ATC_INFO_T *  atc_info_ptr = NULL; /* store the whole ATC info parsed */

ATC_INFO_T * atp_parse_atc_info_file(const char * file_name)
{
    FILE * f_info = NULL;

    SCI_ASSERT( NULL != file_name );

    f_info = fopen( file_name, "r" );
    if ( NULL == f_info )
    {
        printf("Error! Can not open atc info definition file.\n");
        exit(1);
    }

    atpin = f_info;
    atpparse();

    SCI_ASSERT( NULL != atc_info_ptr );

    return atc_info_ptr;
}

void atp_store_numeric_param(void)
{
    atplval.val = atoi( atptext );
}

void atp_store_string_param(void)
{
    char * str = NULL;

    str = atp_malloc( (atpleng + 1) );
    memcpy( str, atptext, (atpleng + 1) );

    atplval.str_t.len = atpleng;
    atplval.str_t.str = str;
}

ATC_PARAM_NUMERIC_ARRAY_T * atp_store_one_numeric_param_range( 
        int min, 
        int max 
        )
{
    ATC_PARAM_NUMERIC_ARRAY_T * num_array_ptr = NULL;

    num_array_ptr = atp_malloc( sizeof( ATC_PARAM_NUMERIC_ARRAY_T ) );

    num_array_ptr->num.min_val = min;
    num_array_ptr->num.max_val = max;

    num_array_ptr->next = NULL;

    return num_array_ptr;
}

ATC_PARAM_STRING_ARRAY_T * atp_store_one_string_param_range( 
        ATC_PARAM_STRING_T str
        )
{
    ATC_PARAM_STRING_ARRAY_T * str_array_ptr = NULL;

    str_array_ptr = atp_malloc( sizeof( ATC_PARAM_STRING_ARRAY_T ) );

    str_array_ptr->str.len = str.len;
    str_array_ptr->str.str = str.str;

    str_array_ptr->next = NULL;

    return str_array_ptr;
}

ATC_PARAM_STRING_ARRAY_T * atp_link_string_param_ranges( 
        ATC_PARAM_STRING_ARRAY_T * str_array_ptr1, 
        ATC_PARAM_STRING_ARRAY_T * str_array_ptr2
        )
{
    ATC_PARAM_STRING_ARRAY_T * str_array_ptr = NULL;

    SCI_ASSERT( NULL != str_array_ptr1 );

    str_array_ptr = str_array_ptr1;

    /* find the tail of str_array_ptr1 */
    while ( NULL != str_array_ptr1->next )
    {
        str_array_ptr1 = str_array_ptr1->next;
    }

    str_array_ptr1->next = str_array_ptr2;

    return str_array_ptr;
}

ATC_PARAM_NUMERIC_ARRAY_T * atp_link_numeric_param_ranges( 
        ATC_PARAM_NUMERIC_ARRAY_T * num_array_ptr1,
        ATC_PARAM_NUMERIC_ARRAY_T * num_array_ptr2
        )
{
    ATC_PARAM_NUMERIC_ARRAY_T * num_array_ptr = NULL;

    SCI_ASSERT( NULL != num_array_ptr1 );

    num_array_ptr = num_array_ptr1;

    /* find the tail of num_array_ptr1 */
    while ( NULL != num_array_ptr1->next )
    {
        num_array_ptr1 = num_array_ptr1->next;
    }

    num_array_ptr1->next = num_array_ptr2;

    return num_array_ptr;
}

ATC_PARAM_INFO_T * atp_store_numeric_param_range(
        ATC_PARAM_NUMERIC_ARRAY_T * num_array_ptr
        )
{
    ATC_PARAM_INFO_T * param_info_ptr = NULL;

    SCI_ASSERT( NULL != num_array_ptr );

    param_info_ptr = atp_malloc( sizeof( ATC_PARAM_INFO_T ) );

    param_info_ptr->param_type = ATC_PARAM_TYPE_NUMERIC;
    param_info_ptr->param_range_num = 1;
    param_info_ptr->num_head = num_array_ptr;

    /* search num_array_ptr and find the number of param_range */
    while ( NULL != num_array_ptr->next )
    {
        param_info_ptr->param_range_num++;
        num_array_ptr = num_array_ptr->next;
    }

    param_info_ptr->num_tail = num_array_ptr;

    param_info_ptr->next = NULL;

    return param_info_ptr;
}

ATC_PARAM_INFO_T * atp_store_string_param_range(
        ATC_PARAM_STRING_ARRAY_T * str_array_ptr
        )
{
    ATC_PARAM_INFO_T * param_info_ptr = NULL;

    SCI_ASSERT( NULL != str_array_ptr );

    param_info_ptr = atp_malloc( sizeof( ATC_PARAM_INFO_T ) );

    param_info_ptr->param_type = ATC_PARAM_TYPE_STRING;
    param_info_ptr->param_range_num = 1;
    param_info_ptr->str_head = str_array_ptr;

    /* search num_array_ptr and find the number of param_range */
    while ( NULL != str_array_ptr->next )
    {
        param_info_ptr->param_range_num++;
        str_array_ptr = str_array_ptr->next;
    }

    param_info_ptr->str_tail = str_array_ptr;

    param_info_ptr->next = NULL;

    return param_info_ptr;
}

ATC_PARAM_INFO_T * atp_store_unknown_param_range(void)
{
    ATC_PARAM_INFO_T * param_info_ptr = NULL;

    param_info_ptr = atp_malloc( sizeof( ATC_PARAM_INFO_T ) );

    param_info_ptr->param_type = ATC_PARAM_TYPE_NONE;
    param_info_ptr->param_range_num = 0;
    param_info_ptr->str_head = NULL;

    param_info_ptr->str_tail = NULL;

    param_info_ptr->next = NULL;

    return param_info_ptr;
}

ATC_PARAM_INFO_T * atp_link_param_ranges(
        ATC_PARAM_INFO_T * param_info_ptr1,
        ATC_PARAM_INFO_T * param_info_ptr2
        )
{
    ATC_PARAM_INFO_T * param_info_ptr = NULL;

    SCI_ASSERT( NULL != param_info_ptr1 );

    param_info_ptr = param_info_ptr1;

    while ( NULL != param_info_ptr1->next )
    {
        param_info_ptr1 = param_info_ptr1->next;
    }

    param_info_ptr1->next = param_info_ptr2;

    return param_info_ptr;
}

ATC_INFO_T * atp_store_one_info_def( 
        ATC_PARAM_STRING_T str, 
        int cmdtype,
        ATC_PARAM_INFO_T * param_info_ptr
        )
{
    ATC_INFO_T * info_ptr = NULL;
    int          prefix_len = 0;
    int          no_plus_name_len = 0;
    char        *no_plus_name = NULL;

    info_ptr = atp_malloc( sizeof( ATC_INFO_T ) );

    info_ptr->cmd_type = cmdtype;

    info_ptr->cmd_name.len = str.len;
    info_ptr->cmd_name.str = str.str;

    switch (str.str[0])
    {
        case '+':
            no_plus_name_len = str.len - 1;
            no_plus_name = str.str + 1;
            break;

        case '$':
            prefix_len = strlen("dollar_");
            no_plus_name_len = prefix_len + str.len - 1;
            no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);

            strcpy(no_plus_name, "dollar_");
            memcpy((void *)&no_plus_name[prefix_len], (void *)&(str.str[1]), str.len - 1);
            no_plus_name[no_plus_name_len] = '\0';
            break;

        case '^':
            prefix_len = strlen("xor_");
            no_plus_name_len = prefix_len + str.len - 1;
            no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);

            strcpy(no_plus_name, "xor_");
            memcpy((void *)&no_plus_name[prefix_len], (void *)&(str.str[1]), str.len - 1);
            no_plus_name[no_plus_name_len] = '\0';
            break;

        case '&':
            prefix_len = strlen("and_");
            no_plus_name_len = prefix_len + str.len - 1;
            no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);

            strcpy(no_plus_name, "and_");
            memcpy((void *)&no_plus_name[prefix_len], (void *)&(str.str[1]), str.len - 1);
            no_plus_name[no_plus_name_len] = '\0';
            break;

        case '#':
            if (str.str[1] == '#')
            {
                no_plus_name_len = strlen("SHARP_SHARP");
                no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);
    
                strcpy(no_plus_name, "SHARP_SHARP");
            }
            else
            {
                prefix_len = strlen("sharp_");
                no_plus_name_len = prefix_len + str.len - 1;
                no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);
    
                strcpy(no_plus_name, "sharp_");
                memcpy((void *)&no_plus_name[prefix_len], (void *)&(str.str[1]), str.len - 1);
                no_plus_name[no_plus_name_len] = '\0';
            }
            break;

        case '*':
            prefix_len = strlen("star_");
            no_plus_name_len = prefix_len + str.len - 1;
            no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);

            strcpy(no_plus_name, "star_");
            memcpy((void *)&no_plus_name[prefix_len], (void *)&(str.str[1]), str.len - 1);
            no_plus_name[no_plus_name_len] = '\0';
            break;

        case '_':
            prefix_len = strlen("underline_");
            no_plus_name_len = prefix_len + str.len - 1;
            no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);

            strcpy(no_plus_name, "underline_");
            memcpy((void *)&no_plus_name[prefix_len], (void *)&(str.str[1]), str.len - 1);
            no_plus_name[no_plus_name_len] = '\0';
            break;

        case '%':
            if ((str.str[1] == '%')&&(str.str[2] == '%'))
            {
                no_plus_name_len = strlen("THREE_PERCENT");
                no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);
    
                strcpy(no_plus_name, "THREE_PERCENT");
            }
            else
            {
                prefix_len = strlen("percent_");
                no_plus_name_len = prefix_len + str.len - 1;
                no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);
    
                strcpy(no_plus_name, "percent_");
                memcpy((void *)&no_plus_name[prefix_len], (void *)&(str.str[1]), str.len - 1);
                no_plus_name[no_plus_name_len] = '\0';
            }

            break;

        case '\\':
            prefix_len = strlen("backslash_");
            no_plus_name_len = prefix_len + str.len - 2;
            no_plus_name = (char *)atp_malloc(no_plus_name_len + 1);

            strcpy(no_plus_name, "backslash_");
            memcpy((void *)&no_plus_name[prefix_len], (void *)&(str.str[2]), str.len - 2);
            no_plus_name[no_plus_name_len] = '\0';

            break;

        default :
            no_plus_name_len = str.len;
            no_plus_name = str.str;
            break;

    }

    info_ptr->cmd_name_no_plus.len = no_plus_name_len;
    info_ptr->cmd_name_no_plus.str = no_plus_name;

    info_ptr->param_head = param_info_ptr;

    if ( NULL != param_info_ptr )
    {
        info_ptr->param_info_num = 1;

        /* search param_info_ptr and find the tail */
        while ( NULL != param_info_ptr->next )
        {
            info_ptr->param_info_num++;
            param_info_ptr = param_info_ptr->next;
        }

        info_ptr->param_tail = param_info_ptr;
    }
    else 
    {
        info_ptr->param_info_num = 0;
        info_ptr->param_tail = NULL;
    }

    info_ptr->next = NULL;

    return info_ptr;
}

ATC_INFO_T * atp_link_info_defs(
        ATC_INFO_T * info_ptr1,
        ATC_INFO_T * info_ptr2
        )
{
    ATC_INFO_T * info_ptr = NULL;

    SCI_ASSERT( NULL != info_ptr1 );

    info_ptr = info_ptr1;

    /* find the tail of info_ptr1 */
    while ( NULL != info_ptr1->next )
    {
        info_ptr1 = info_ptr1->next;
    }

    info_ptr1->next = info_ptr2;

    return info_ptr;
}

void atp_store_all_defs(
        ATC_INFO_T * info_ptr
        )
{
    SCI_ASSERT( NULL != info_ptr );

    atc_info_ptr = info_ptr;
}
