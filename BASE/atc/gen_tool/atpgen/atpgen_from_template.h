/******************************************************************************
 ** File Name:      atpgen_from_template.h                                    *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/13/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file contains general types and macros that   *
 **                 are of use to atpgen_from_template.c which parses the     *
 **                 template files.                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/13/2002     Steven.Yao       Create.                                   *
 ******************************************************************************/


#ifndef ATPGEN_FROM_TEMPLATE_H
#define ATPGEN_FROM_TEMPLATE_H

/* Below are MICRO declarations used for template file lexical analyzer */
#define    TPL_BEGIN         (256+1)
#define    TPL_END           (256+2)
#define    TPL_ATCNAME       (256+3)
#define    TPL_ATCNAMENOPLUS (256+4)
#define    TPL_ATCINDEX      (256+5)
#define    TPL_ATCTYPE       (256+6)
#define    TPL_ATCPARAMNUM   (256+7)

#define    TPL_FILE_BLOCK_TYPE_FIX    1
#define    TPL_FILE_BLOCK_TYPE_TPL    2

#define    TPL_TYPE_FIX_STR           1
#define    TPL_TYPE_ATCNAME           2
#define    TPL_TYPE_ATCNAMENOPLUS     3
#define    TPL_TYPE_ATCINDEX          4
#define    TPL_TYPE_ATCTYPE           5
#define    TPL_TYPE_ATCPARAMNUM       6

#define    MAX_BUFFER_SIZE    (0x20*1024) 

typedef struct atp_fix_line_struct {
    int    line_len;
    char * line_buf;

    struct atp_fix_line_struct * next;
}ATP_FIX_LINE_T;

typedef struct atp_tpl_line_struct {
    int type;    /* 1 - fix string
                    2 - tpl ##atcname
                    3 - tpl ##atcnamenoplus  
                    4 - tpl ##atcindex
                    5 - tpl ##atctype
                    6 - tpl ##atcparamnum */
    int length;  /* length of fix_str */
    char * fix_str; 
    struct atp_tpl_line_struct * next;
}ATP_TPL_LINE_T;

typedef struct atp_tpl_file_struct {
    int block_type;   /*   1 - fix block 
                           2 - template block */

    union {
        ATP_FIX_LINE_T *  fix_line_ptr;
        ATP_TPL_LINE_T *  tpl_line_ptr;
    };

    struct atp_tpl_file_struct * next;
}ATP_TPL_FILE_T;

FILE * atp_open_template_file(const char * );
ATP_TPL_FILE_T * atp_parse_template_file( const char * );
ATP_FIX_LINE_T * atp_new_fix_line( int, const char * );
ATP_TPL_LINE_T * atp_new_tpl_line( int, int, const char *);
ATP_TPL_FILE_T * atp_add_fix_line( ATP_FIX_LINE_T * );
ATP_TPL_FILE_T * atp_add_tpl_line( ATP_TPL_LINE_T * );

#endif /* End of #ifndef ATPGEN_FROM_TEMPLATE */
