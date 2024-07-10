/******************************************************************************
 ** File Name:      atpgen.c                                                  *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/15/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This c program file is the main entrance of atpgen, it    *
 **                 generates output AT program files by calling other        *
 **                 functions.                                                *
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
#include <string.h>
#include "atpgen.h"
#include "atp_malloc.h"
#include "atp_atc_info.h"
#include "atpgen_from_template.h"
#include "atpgen_file.h"

int main(int argc, char* argv[])
{
    ATC_INFO_T * atc_info_ptr = NULL;
    ATP_TPL_FILE_T * tpl_file_ptr = NULL;

    char     pfname[255];

    int      file_index = 0;
    
    /* First string of argv is the command itself, second string of argv must 
     * be the AT commands definition file name , third string of argv must be
     * the atc_info.c template file name, then other strings are other template
     * file names.
     */
	//argv[1] = "XXX\\MS_Code\\BASE\\atc\\modem\\tools\\template\\atc.atd";
	//argv[2] = "XXX\\MS_Code\\BASE\\atc\\modem\\tools\\template\\atc_info.c.tpl";
	//argv[3] = "XXX\\MS_Code\\BASE\\atc\\modem\\tools\\template\\atc.lex.tpl";
	//argv[4] = "XXX\\MS_Code\\BASE\\atc\\modem\\tools\\template\\atc_def.h.tpl";
	//argc = 5;

    if ( 3 > argc )
    {
        printf("Error! No input atc info definition file.\n");
        return 1;
    }

    atp_mem_create();

    atc_info_ptr = atp_parse_atc_info_file( argv[1] );
    tpl_file_ptr = atp_parse_template_file( argv[2] );

    /* every template file name has the format : ****.tpl 
     * so the outout file name has the format : ****
     */
    strcpy( pfname, argv[2] );
    pfname[strlen(argv[2])-4] = '\0';

    atpgen_info_file( atc_info_ptr, tpl_file_ptr, pfname);

    ATC_GetDefaultRules(argv[3]);  //get some default rules from lex template file

    for ( file_index = 3; file_index < argc; file_index++ )
    {
        tpl_file_ptr = atp_parse_template_file( argv[file_index] );

        strcpy( pfname, argv[file_index] );
        pfname[strlen(pfname)-4] = '\0';

        atpgen_other_file( atc_info_ptr, tpl_file_ptr, pfname );
    }

    atp_mem_destroy();

    return 0;
}
