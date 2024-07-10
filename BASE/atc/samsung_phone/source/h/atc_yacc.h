/******************************************************************************
 ** File Name:      atc_yacc.h                                                *
 ** Author:         Steven.Yao                                                *
 ** DATE:           05/22/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains type and MICRO declarations that are of*
 **                 use to lex and yacc.                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/22/2002     Steven.Yao       Created.                                  *
 ******************************************************************************/
#ifndef _ATC_YACC_H
#define _ATC_YACC_H

typedef union   {
                    uint                    cmd_index;
                    ATC_CUR_PARAM_INFO_LIST_T * cur_param_ptr;
                    ATC_CUR_INFO_LIST_T   * atc_cur_info_ptr;
                } ATCSTYPE;

#define AT_CMD_PREFIX   257
#define AT_CMD_REPEAT   258
#define AT_EXTENSION_CMD    259
#define AT_BASIC_CMD    260
#define AT_CMD_END  261
#define AT_CMD_DIVIDE   262
#define NUMBER  263
#define HNUMBER 264
#define STRING  265
#define DIALSTRING  266
#define SHARPSTRING 267
#define DTMFSTRING  268
extern ATCSTYPE atclval;
#endif /* End of #ifndef _ATC_YACC_H */
