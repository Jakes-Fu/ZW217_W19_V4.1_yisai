#ifndef _ATC_STK_FUNC_H_
#define _ATC_STK_FUNC_H_
/**************************************************************************

Copyright (C) 2001 SPREADTRUM COMMUNICATION CO.
All rights reserved.

This document contains proprietary information blonging to
SPREADTRUM. Passing on and copying this document, use and
communication of its contents is not permitted without prior
writed authorization.

***************************************************************************/

/***************************************************************************

Reversion information:
File Name:    atc_stk_func.h
Version:      0.1
Author:       Bin.Li
Date:         2009.4
Description:  This is the header file have the phoneserver stk command
            relative data and macro define
***************************************************************************/
/***************************************************************************

            Edit Histroy
Date          Name            Description
--------------------------------------------------------------------------
2009.4        Bin.Li    Create
***************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************
//  DESCRIPTION
//      do the optional simple data object decode
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_GENERAL_PC_RESULT_E DecodeOptionalObject(
    BOOLEAN *is_com,//out:is the comprehension needed for the data object
    uint8 *pdata,   //in:the data buf that including the simple tlv data object
    uint8 *offset,  //out:the simple tlv data object length in the data buf
    void *pobject, //out:the simple tlv data object
    uint16 data_len,    //the total length of the data buf
    SIMAT_B_DIRECTION_TAG_E *object_type    //out:the simple tlv data object type.
);
#if 0
/********************************************************************
//  DESCRIPTION
//      do the minimal simple data object decode
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_GENERAL_PC_RESULT_E DecodeMinimalObject(
    SIMAT_B_DIRECTION_TAG_E object_type,    //the object type of the simple object
    uint8 *pdata,   //in:the data buf that including the simple tlv data object
    uint8 *offset,  //out:the simple tlv data object length in the data buf
    void *pobject, //out:the simple tlv data object
    uint16 data_len   //the total length of the data buf
);
#endif
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN ATC_EncodeDataObject
(
    SIMAT_B_DIRECTION_TAG_E object_type,    //the object data type
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //out:the object decoded out
);


#ifdef __cplusplus
}
#endif

#endif  //_ATC_STK_FUNC_H_

