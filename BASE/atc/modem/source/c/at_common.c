/******************************************************************************
** File Name:      at_common.c                                               *
** Author:         Xueliang.Wang                                             *
** Date:           19/04/2002                                                *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This file defines some common function for all of         *
**                 AT Command.                                               *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** DATE           NAME             Description:                              *
**---------------------------------------------------------------------------*
** 19/04/2002     Xueliang.Wang    Create.                                   *
** 31/05/2002     Xueliang.Wang    Modify.                                   *
******************************************************************************/

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>

#include "os_api.h"
#include "cmddef.h"
#include "at_common.h"

/**---------------------------------------------------------------------------*
**                         Debugging Flag                                    *
**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

    /**---------------------------------------------------------------------------*
     **                         Global variables                                  *
     **---------------------------------------------------------------------------*/
    /**---------------------------------------------------------------------------*
     **                         Function Definitions                              *
     **---------------------------------------------------------------------------*/

    /*****************************************************************************/
    //  Description:    Make a response packet, using the inputed message header.
    //  Global resource dependence: None
    //  Author: Xueliang.Wang
    //  Note:
    /*****************************************************************************/
    PUBLIC  void    ATCOMMON_GenerateRspMsg(
        uint8 **dest_ptr,       // Pointer of the response message.
        uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
        MSG_HEAD_T *msg_head_ptr)   // Inputed pointer of message header struct.
    {
        // Check input parameter.
        SCI_ASSERT(SCI_NULL != msg_head_ptr);/*assert verified: check null pointer*/

        // Malloc a memory block for response buffer.
        *dest_ptr = (uint8 *)SCI_ALLOC_BASE(sizeof(MSG_HEAD_T));

        msg_head_ptr->len = sizeof(MSG_HEAD_T);
        SCI_MEMCPY((void *)(*dest_ptr), (void *)msg_head_ptr, sizeof(MSG_HEAD_T));
        *dest_len_ptr = sizeof(MSG_HEAD_T);

        return;
    }

    /*****************************************************************************/
    //  Description:    Convert a string to uppercase.
    //  Global resource dependence: None
    //  Author: Xueliang.Wang
    //  Note:
    /*****************************************************************************/
    PUBLIC  uint8   *ATCOMMON_UpperStr( // Pointer to the string altered.
        uint8 *str_ptr)                 // Pointer to string to be capitalized.
    {
        uint8   *temp_ptr = SCI_NULL;

        // Check input parameter.
        SCI_ASSERT(SCI_NULL != str_ptr);/*assert verified: check null pointer*/

        temp_ptr = str_ptr;

        for(; *str_ptr != '\0'; str_ptr++)
        {
            if((*str_ptr >= 'a') && (*str_ptr <= 'z'))
            {
                *str_ptr = *str_ptr - 0x20;
            }
        }

        str_ptr = temp_ptr;

        return str_ptr;

    }

    /*****************************************************************************/
    //  Description:    Convert strings to an unsigned long-integer value.
    //                  The base of value can be 2, 8, 10, 16
    //  Global resource dependence: None
    //  Author: Xueliang.Wang
    //  Note:
    /*****************************************************************************/
    PUBLIC  uint32  ATCOMMON_ConvertStrToUl(   // Return if convert is successful.
        uint32 *value_ptr,  // Pointer to the value represented in the string.
        uint8 *src_ptr,     // Null-terminated value string to convert.
        uint8 **end_ptr)    // Pointer to character that stops scan.
    {
        uint8 base[3];

        // Check input parameter.
        SCI_ASSERT(SCI_NULL != src_ptr);/*assert verified: check null pointer*/

        // Convert it to uppercase.
        ATCOMMON_UpperStr(src_ptr);

        // Get base of the value.
        base[0] = *src_ptr;
        base[1] = *(src_ptr + 1);
        base[2] = '\0';

        // Check the base of value, then convert from string to uint32.
        if(0 == strcmp((char *)base, "0B"))
        {
            // Value base is 2.
            *value_ptr = strtoul((char *)(src_ptr + 2), (char **)end_ptr, 2);
        }
        else if(0 == strcmp((char *)base, "0O"))
        {
            // Value base is 8.
            *value_ptr = strtoul((char *)(src_ptr + 2), (char **)end_ptr, 8);
        }
        else if(0 == strcmp((char *)base, "0X"))
        {
            // Value base is 16.
            *value_ptr = strtoul((char *)(src_ptr + 2), (char **)end_ptr, 16);
        }
        else if(0 == strcmp((char *)base, "0D"))
        {
            // Value base is 10.
            *value_ptr = strtoul((char *)(src_ptr + 2), (char **)end_ptr, 10);
        }
        else if((base[0] >= '0') && (base[0] <= '9'))
        {
            // Value base is 10.
            *value_ptr = strtoul((char *)src_ptr, (char **)end_ptr, 10);
        }
        else
        {
            // Error src_ptr.
            *end_ptr = src_ptr;
            *value_ptr = 0;
            return ATCMD_BADPARAMETER;
        }

        return ATCMD_SUCCESS;
    }

    /**---------------------------------------------------------------------------*
     **                         Compiler Flag                                     *
     **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

