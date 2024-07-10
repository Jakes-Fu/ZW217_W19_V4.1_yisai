/******************************************************************************
 ** File Name:      atc_test_win32.c                                          *
 ** Author:         Xueli.lv                                                  *
 ** Date:           10/09/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 10/09/2002     Xueli.lv         Create.                                   *
 ** 15/07/2003     Liuhan           Modify.                                   *
 ******************************************************************************/

#include "atc.h"
#include "atc_common.h"
#include "atc_def.h"
#include "atc_malloc.h"

#include <stdio.h>
#include <stdlib.h>

ATC_STATUS ATC_Initialize(  // Retur S_ATC_SUCCESS if success,
                            // otherwise return error code.
        ATC_GLOBAL_INFO_T  ** atc_global_info_ptr  // Output argument, return a pointer 
                                        // which points an ATC_CONFIG_T struct.
        );

ATC_STATUS ATC_RecNewLineSig(  // Return S_ATC_SUCCESS if success,
                               // otherwise return error code.
        ATC_CONFIG_T  * atc_config_ptr, // point to the ATC config struct.
        const xSignalHeaderRec * sig_ptr // point to signal struct 
        );

ATC_STATUS ATC_ProcessCmds( // Return S_ATC_SUCCESS if success,
                            // otherwise return error code.
        ATC_GLOBAL_INFO_T  * atc_global_info_ptr // Point to the AT config struct.
        );

ATC_STATUS ATC_DespatchCmd(  // Return S_ATC_STATUS if success,
                             // otherwise return error code.
        ATC_CONFIG_T  * atc_config_ptr, // Point to ATC config struct.
        ATC_CUR_INFO_LIST_T * atc_cur_info_ptr // Point to the info struct of
                                               // AT command
        );

ATC_STATUS ATC_CheckCmd(  // Return S_ATC_SUCCESS if command is valid,
                           // otherwise return error code.
        ATC_CONFIG_T * atc_config_ptr, // Point to ATC config struct.
        ATC_CUR_INFO_LIST_T * atc_cur_info_ptr  // Point to the info struct of
                                                // AT command.
        );

ATC_STATUS ATC_BuildResponse(  // Return S_ATC_SUCCESS if success, 
                               // otherwise return error code.
        ATC_CONFIG_T  * atc_config_ptr, // Point to our ATC config struct
        ATC_STATUS  atc_status          // status used to contruct response
        );

ATC_STATUS ATC_BuildTestCmdResponse(  // Return S_ATC_SUCCESS if success,
                                      // otherwise return error code.
        const ATC_CONFIG_T * atc_config_ptr, // Point to our ATC config struct.
        const ATC_CUR_INFO_LIST_T * atc_cur_info_ptr // Point to the info of
                                                      // AT command.
        );

ATC_STATUS atcparse(
        const uint8 * atcbuf, 
        ATC_CUR_INFO_LIST_T ** info_ptr_ptr);


void main()
{
    ATC_STATUS      status = S_ATC_SUCCESS;

    ATC_CONFIG_T    * atc_config_ptr = NULL;
    ATC_GLOBAL_INFO_T *atc_global_info_ptr = NULL;

    char inbuf[256];
    int  i = 0;

    status = ATC_Initialize(&atc_global_info_ptr);

    SCI_ASSERT( S_ATC_SUCCESS == status );

    while ( TRUE ) 
    {
        inbuf[i] = getc(stdin);
        while ( '\n' != inbuf[i] ) 
        {
            inbuf[++i] = getc(stdin);
        }
        inbuf[i] = '\r';
        inbuf[++i] = 0; inbuf[++i] = 0;

        status = atcparse(inbuf, &atc_global_info_ptr->atc_info_ptr);
        if ( S_ATC_SUCCESS != status )
        {
            printf("ERROR\n");
            ATCS_TRACE_STATUS( status );
        }
        else
        {
            status = ATC_ProcessCmds( atc_global_info_ptr );
        }
        i = 0;
    }
    
    ATCM_DestroyHeap();
    return;
}
