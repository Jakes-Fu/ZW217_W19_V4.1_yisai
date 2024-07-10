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
 ** Description:    This file contains the function proto types called by main*
 **                 function to generate output files.                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/15/2002     Steven.Yao       Created.                                  *
 ******************************************************************************/

#ifndef ATPGEN_FILE_H
#define ATPGEN_FILE_H

void atpgen_info_file(
        const ATC_INFO_T     * atc_info_ptr,
        const ATP_TPL_FILE_T * tpl_file_ptr,
        const char           * info_file_name
        );

void atpgen_other_file(
        const ATC_INFO_T     * atc_info_ptr,
        const ATP_TPL_FILE_T * tpl_file_ptr,
        const char           * other_file_name
        );


typedef struct atc_default_rule_struct {
    int     rule_num;
    ATC_PARAM_STRING_T  rule_cmd_name[255];
}ATC_DEFAULT_RULES_T;

void ATC_GetDefaultRules(const char * file_name);
unsigned char ATC_IsDefaultCmdRulePresent(char * cmd_name_plus);

#endif /* End of #ifndef ATPGEN_FILE_H */
