/******************************************************************************
 ** File Name:      atpgen_from_template.c                                    *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/13/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contain function definitions used to parse the  *
 **                 template files.                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/13/2002     Steven.Yao       Create.                                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "atp_malloc.h"
#include "atpgen_from_template.h"

#define SCI_ASSERT(_bool)  assert(_bool)

/* extern variables of atpgen_tpl_lex.c */
extern FILE * tplin;
extern int tpllex();

FILE * atp_open_template_file(const char * f_template)
{
    FILE * f_atp_template = NULL;

    SCI_ASSERT( NULL != f_template );

    f_atp_template = fopen( f_template, "r");
    if ( !f_atp_template )
    {
        printf("Error! Can not open template file %s\n", f_template);
        exit(1);
    }

    return f_atp_template;
}

ATP_TPL_FILE_T * atp_parse_template_file( const char * file_name)
{
    FILE * ftpl = NULL;

    ATP_FIX_LINE_T * fix_line_t_head = NULL;
    ATP_FIX_LINE_T * fix_line_t_tail = NULL;

    ATP_TPL_LINE_T * tpl_line_t_head = NULL;
    ATP_TPL_LINE_T * tpl_line_t_tail = NULL;
    
    ATP_TPL_FILE_T * tpl_file_t_head = NULL;
    ATP_TPL_FILE_T * tpl_file_t_tail = NULL;

    char  *  buf = NULL;
    int i = 0;
    int tpl_pattern = 0;

    int state = 0;  /* 0 - fix part 
                       1 - template part */

    SCI_ASSERT( NULL != file_name );
   
    ftpl = atp_open_template_file( file_name );

    buf = atp_malloc( MAX_BUFFER_SIZE ); 

    tplin = ftpl;

    tpl_pattern = tpllex();
    while ( 0 != tpl_pattern )
    {
        switch ( tpl_pattern )
        {
        case TPL_BEGIN:
            if ( NULL == fix_line_t_head )
            {
                fix_line_t_head = atp_new_fix_line( i, buf );
                fix_line_t_tail = fix_line_t_head;
            }
            else
            {
                fix_line_t_tail->next = atp_new_fix_line( i, buf );
                fix_line_t_tail = fix_line_t_tail->next;
            }

            if ( NULL == tpl_file_t_head )
            {
                tpl_file_t_head = atp_add_fix_line( fix_line_t_head );
                tpl_file_t_tail = tpl_file_t_head;
            }
            else
            {
                tpl_file_t_tail->next = atp_add_fix_line( fix_line_t_head );
                tpl_file_t_tail = tpl_file_t_tail->next;
            }

            fix_line_t_head = fix_line_t_tail = NULL;
            i = 0;

            state = 1;  /* it means the lines below are template part */
            break;

        case TPL_END:
            if ( 0 != i )
            {
                if ( NULL == tpl_line_t_head )
                {
                    tpl_line_t_head = atp_new_tpl_line( 1, i, buf );
                    tpl_line_t_tail = tpl_line_t_head;
                }
                else
                {
                    tpl_line_t_tail->next = atp_new_tpl_line( 1, i , buf );
                    tpl_line_t_tail = tpl_line_t_tail->next;
                }
            }

            if ( NULL == tpl_file_t_head )
            {
                tpl_file_t_head = atp_add_tpl_line( tpl_line_t_head );
                tpl_file_t_tail = tpl_file_t_head;
            }
            else
            {
                tpl_file_t_tail->next = atp_add_tpl_line( tpl_line_t_head );
                tpl_file_t_tail = tpl_file_t_tail->next;
            }

            i = 0;
            tpl_line_t_head = tpl_line_t_tail = NULL;

            state = 0;  /* template part end */
            break;

        case TPL_ATCNAME:
            if ( 0 != i )
            {
                if ( NULL == tpl_line_t_head )
                {
                    tpl_line_t_head = atp_new_tpl_line( 1, i, buf );
                    tpl_line_t_tail = tpl_line_t_head;
                }
                else
                {
                    tpl_line_t_tail->next = atp_new_tpl_line( 1, i , buf );
                    tpl_line_t_tail = tpl_line_t_tail->next;
                }
            }

            if ( NULL == tpl_line_t_head )
            {
                tpl_line_t_head = atp_new_tpl_line( TPL_TYPE_ATCNAME, 0, NULL );
                tpl_line_t_tail = tpl_line_t_head;
            }
            else
            {
                tpl_line_t_tail->next = atp_new_tpl_line( TPL_TYPE_ATCNAME, 0, NULL );
                tpl_line_t_tail = tpl_line_t_tail->next;
            }
            
            i = 0;
            break;

        case TPL_ATCNAMENOPLUS:
            if ( 0 != i )
            {
                if ( NULL == tpl_line_t_head )
                {
                    tpl_line_t_head = atp_new_tpl_line( 1, i, buf );
                    tpl_line_t_tail = tpl_line_t_head;
                }
                else
                {
                    tpl_line_t_tail->next = atp_new_tpl_line( 1, i , buf );
                    tpl_line_t_tail = tpl_line_t_tail->next;
                }
            }

            if ( NULL == tpl_line_t_head )
            {
                tpl_line_t_head = atp_new_tpl_line( TPL_TYPE_ATCNAMENOPLUS, 0, NULL );
                tpl_line_t_tail = tpl_line_t_head;
            }
            else
            {
                tpl_line_t_tail->next = atp_new_tpl_line( TPL_TYPE_ATCNAMENOPLUS, 0, NULL );
                tpl_line_t_tail = tpl_line_t_tail->next;
            }

            i = 0;
            break;

        case TPL_ATCINDEX:
            if ( 0 != i )
            {
                if ( NULL == tpl_line_t_head )
                {
                    tpl_line_t_head = atp_new_tpl_line( 1, i, buf );
                    tpl_line_t_tail = tpl_line_t_head;
                }
                else
                {
                    tpl_line_t_tail->next = atp_new_tpl_line( 1, i , buf );
                    tpl_line_t_tail = tpl_line_t_tail->next;
                }
            }

            if ( NULL == tpl_line_t_head )
            {
                tpl_line_t_head = atp_new_tpl_line( TPL_TYPE_ATCINDEX, 0, NULL );
                tpl_line_t_tail = tpl_line_t_head;
            }
            else
            {
                tpl_line_t_tail->next = atp_new_tpl_line( TPL_TYPE_ATCINDEX, 0, NULL );
                tpl_line_t_tail = tpl_line_t_tail->next;
            }
            
            i = 0;
            break;

        case TPL_ATCTYPE:
            if ( 0 != i )
            {
                if ( NULL == tpl_line_t_head )
                {
                    tpl_line_t_head = atp_new_tpl_line( 1, i, buf );
                    tpl_line_t_tail = tpl_line_t_head;
                }
                else
                {
                    tpl_line_t_tail->next = atp_new_tpl_line( 1, i , buf );
                    tpl_line_t_tail = tpl_line_t_tail->next;
                }
            }

            if ( NULL == tpl_line_t_head )
            {
                tpl_line_t_head = atp_new_tpl_line( TPL_TYPE_ATCTYPE, 0, NULL );
                tpl_line_t_tail = tpl_line_t_head;
            }
            else
            {
                tpl_line_t_tail->next = atp_new_tpl_line( TPL_TYPE_ATCTYPE, 0, NULL );
                tpl_line_t_tail = tpl_line_t_tail->next;
            }
            
            i = 0;
            break;

        case TPL_ATCPARAMNUM:
            if ( 0 != i )
            {
                if ( NULL == tpl_line_t_head )
                {
                    tpl_line_t_head = atp_new_tpl_line( 1, i, buf );
                    tpl_line_t_tail = tpl_line_t_head;
                }
                else
                {
                    tpl_line_t_tail->next = atp_new_tpl_line( 1, i , buf );
                    tpl_line_t_tail = tpl_line_t_tail->next;
                }
            }

            if ( NULL == tpl_line_t_head )
            {
                tpl_line_t_head = atp_new_tpl_line( TPL_TYPE_ATCPARAMNUM, 0, NULL );
                tpl_line_t_tail = tpl_line_t_head;
            }
            else
            {
                tpl_line_t_tail->next = atp_new_tpl_line( TPL_TYPE_ATCPARAMNUM, 0, NULL );
                tpl_line_t_tail = tpl_line_t_tail->next;
            }
            
            i = 0;
            break;

        default:
            buf[i++] = tpl_pattern;
            break;
        }
        tpl_pattern = tpllex();
    }

    if ( 0 != i )
    {
        if ( NULL == fix_line_t_head )
        {
            fix_line_t_head = atp_new_fix_line( i, buf );
            fix_line_t_tail = fix_line_t_head;
        }
        else
        {
            fix_line_t_tail->next = atp_new_fix_line( i, buf );
            fix_line_t_tail = fix_line_t_tail->next;
        }

        if ( NULL == tpl_file_t_head )
        {
            tpl_file_t_head = atp_add_fix_line( fix_line_t_head );
            tpl_file_t_tail = tpl_file_t_head;
        }
        else
        {
            tpl_file_t_tail->next = atp_add_fix_line( fix_line_t_head );
            tpl_file_t_tail = tpl_file_t_tail->next;
        }
    }

    return tpl_file_t_head;
}

ATP_FIX_LINE_T * atp_new_fix_line( int len, const char * line_buf )
{
    ATP_FIX_LINE_T *  fix_line_ptr = NULL;

    SCI_ASSERT( NULL != line_buf );
    SCI_ASSERT( 0 != len );
    
    fix_line_ptr = atp_malloc( sizeof( ATP_FIX_LINE_T ) );

    fix_line_ptr->line_len = len;
    fix_line_ptr->line_buf = atp_malloc( len );
    memcpy( fix_line_ptr->line_buf, line_buf, len );
    
    fix_line_ptr->next = NULL;

    return fix_line_ptr;
}

ATP_TPL_FILE_T * atp_add_fix_line( ATP_FIX_LINE_T * fix_line_ptr )
{
    ATP_TPL_FILE_T * tpl_file_ptr = NULL;

    SCI_ASSERT( NULL != fix_line_ptr );

    tpl_file_ptr = atp_malloc( sizeof( ATP_TPL_FILE_T ) );

    tpl_file_ptr->block_type = TPL_FILE_BLOCK_TYPE_FIX;
    tpl_file_ptr->fix_line_ptr = fix_line_ptr;

    tpl_file_ptr->next = NULL;

    return tpl_file_ptr;
}

ATP_TPL_LINE_T * atp_new_tpl_line(int type, int len, const char * fix_str_buf )
{
    ATP_TPL_LINE_T *  tpl_line_ptr = NULL;

    tpl_line_ptr = atp_malloc( sizeof( ATP_TPL_LINE_T ) );
    tpl_line_ptr->type = type;  /* fix string */

    if ( TPL_TYPE_FIX_STR == type )
    {
        SCI_ASSERT( NULL != fix_str_buf );
        SCI_ASSERT( 0 != len );

        tpl_line_ptr->length = len;
        tpl_line_ptr->fix_str = atp_malloc( len );
        memcpy( tpl_line_ptr->fix_str, fix_str_buf, len );
    
        tpl_line_ptr->next = NULL;
    }
    else
    {
        tpl_line_ptr->length = 0;
        tpl_line_ptr->fix_str = NULL;
        tpl_line_ptr->next = NULL;
    }

    return tpl_line_ptr;
}

ATP_TPL_FILE_T * atp_add_tpl_line( ATP_TPL_LINE_T * tpl_line_ptr )
{
    ATP_TPL_FILE_T * tpl_file_ptr = NULL;

    SCI_ASSERT( NULL != tpl_line_ptr );

    tpl_file_ptr = atp_malloc( sizeof( ATP_TPL_FILE_T ) );

    tpl_file_ptr->block_type = TPL_FILE_BLOCK_TYPE_TPL;
    tpl_file_ptr->tpl_line_ptr = tpl_line_ptr;

    tpl_file_ptr->next = NULL;

    return tpl_file_ptr;
}
