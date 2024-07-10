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
File Name:    atc_stk_func.c from old name:phonesever_stk_func.c

Description:  This is the C file for the func to be used by phonesever
***************************************************************************/
//#include "scttypes_rtos.h"

#include "MN.h"
#include "sim_type.h"
#include "simat_data_object.h"
#include "atc_stk_enum.h"
#include "atc_stk_common_func.h"
#include "atc_stk_func.h"

#ifdef __cplusplus
extern "C"
{
#endif


extern BOOLEAN(*s_simat_do_encode_array[])(uint8 *, uint8 *, void *);

extern SIMAT_DO_DECODE_RESULT_E(*s_simat_do_decode_array[])(uint8 *, uint8 *, void *, uint16, BOOLEAN);
/********************************************************************
//  DESCRIPTION
//      Decode the unknow data object,get the length of the object.
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static SIMAT_DO_DECODE_RESULT_E SIMAT_DecodeUnknowObject(
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    uint16 data_len   //the left data buf length
)
{
    uint8 *ptemp = pdata;
    uint8 len, content_len;

    if((NULL == offset) || (NULL == pdata))
    {
        SCI_TRACE_LOW("SIMAT::SIMAT_DecodeUnknowObject point is NULL! \n");
        return SIMAT_DO_SERIOUS;
    }

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    ptemp += 1;
    *offset += 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//      do the optional simple data object decode
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static SIMAT_DO_DECODE_RESULT_E SIMAT_DecodeDataObject
(    //return value:the data object decode result
    SIMAT_B_DIRECTION_TAG_E object_type,    //the object data type
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    if((NULL == offset) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("SIMAT::SIMAT_DecodeDataObject point is NULL! \n");
        return SIMAT_DO_SERIOUS;
    }

    *offset = 0;

    if((SIMAT_BTAG_COMMAND_DETAIL > object_type) ||
            (SIMAT_BTAG_REGISTRY_APP_DATA < object_type))
    {
        SCI_TRACE_LOW("SIMAT::SIMAT_DecodeDataObject error! \n");

        return SIMAT_DO_ERROR_TAG;
    }

    //guisen.deng; remove the buf_id in all related structs
    return s_simat_do_decode_array[object_type - 1](pdata, offset, pobject, data_len, is_discard);
}
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
)
{
    SIMAT_DO_DECODE_RESULT_E de_result;
    uint8 *ptemp = pdata;

    if((NULL == is_com) || (NULL == pdata) || (NULL == pobject) || (NULL == object_type))
    {
        SCI_TRACE_LOW("SIMAT::ATC_EncodeDataObject point is NULL! \n");
        return SIMAT_TR_MISS_INFO;
    }

    //get the comprehension flag
    *is_com = IS_COMPREHESION(*ptemp);
    //get the simple object type
    *object_type = (SIMAT_B_DIRECTION_TAG_E) GET_TAG_VALUE(*ptemp);
    *offset = 0;

    //decode the object type
    if((SIMAT_BTAG_COMMAND_DETAIL <= *object_type) &&
            (SIMAT_BTAG_UNKNOW > *object_type))
    {
        de_result = SIMAT_DecodeDataObject(*object_type, ptemp, offset, pobject, data_len, FALSE);

        if(SIMAT_DO_DECODE_OK != de_result)
        {
            //process the simple object decode error
            switch(de_result)
            {
                case SIMAT_DO_SERIOUS:
                    return SIMAT_TR_DATA_NOT_UNDERSTOOD;
                case SIMAT_DO_LENGTH_ERROR:
                    *offset = (uint8)data_len;
                    return SIMAT_TR_MISS_INFO;
                    //the error process is the same,so the case will go through
                case SIMAT_DO_RFU:
                case SIMAT_DO_CONTENTS_NOT_UNDERSTOOD:

                    if(*is_com)
                    {
                        return SIMAT_TR_DATA_NOT_UNDERSTOOD;
                    }
                    else
                    {
                        return SIMAT_TR_PATRIAL_COM;
                    }

                case SIMAT_DO_ERROR_TAG:
                    return SIMAT_TR_DATA_NOT_UNDERSTOOD;
                default:
                    return SIMAT_TR_ME_UNABLE;
            }
        }
    }
    else
    {
        *object_type = SIMAT_BTAG_UNKNOW;

        //if the comprehension flag is '1',then report the command data not understood
        //by ME
        if(*is_com)
        {
            return SIMAT_TR_DATA_NOT_UNDERSTOOD;
        }
        //ignor the unknow simple data object
        else
        {
            de_result = SIMAT_DecodeUnknowObject(ptemp, offset, data_len);

            //the only one reason can cause
            if(SIMAT_DO_DECODE_OK != de_result)
            {
                //the data length error
                if(SIMAT_DO_SERIOUS == de_result)
                {
                    return SIMAT_TR_DATA_NOT_UNDERSTOOD;
                }
                else
                {
                    *offset = (uint8)data_len;
                }
            }

            return SIMAT_TR_PATRIAL_COM;
        }
    }

    return SIMAT_TR_SUCCESS;
}
#if 0 //defined by SIM already
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
)
{
    uint8 *ptemp = pdata;
    SIMAT_DO_DECODE_RESULT_E de_result = SIMAT_DO_DECODE_OK;

    //decode the simple data object.
    de_result = SIMAT_DecodeDataObject(object_type, ptemp, offset, pobject, data_len, FALSE);

    if(SIMAT_DO_DECODE_OK != de_result)
    {
        switch(de_result)
        {
                //the error process is the same,so the case will go through
            case SIMAT_DO_SERIOUS:
            case SIMAT_DO_RFU:
            case SIMAT_DO_CONTENTS_NOT_UNDERSTOOD:
                return SIMAT_TR_DATA_NOT_UNDERSTOOD;
            case SIMAT_DO_LENGTH_ERROR:
            case SIMAT_DO_ERROR_TAG:
                return SIMAT_TR_REQUIRE_V_MISSING;
            case SIMAT_DO_ERROR_SHARE_MEM:
                return SIMAT_TR_ME_UNABLE;
            default:
                return SIMAT_TR_ME_UNABLE;
        }
    }

    return SIMAT_TR_SUCCESS;
}
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
)
{
    if((NULL == pdata) || (NULL == data_len) || (NULL == pobject))
    {
        SCI_TRACE_LOW("SIMAT::ATC_EncodeDataObject point is NULL! \n");
        return FALSE;
    }

    //BinLi the condition compare with invalid  enum element

    if((SIMAT_BTAG_COMMAND_DETAIL > object_type) ||
            (SIMAT_BTAG_UNKNOW <= object_type))
    {
        SCI_TRACE_LOW("SIMAT::ATC_EncodeDataObject error! \n");
        return FALSE;
    }

    SCI_TRACE_LOW("SIMAT:object_type:%d", object_type);

    return s_simat_do_encode_array[object_type - 1](pdata, data_len, pobject);
}

#ifdef __cplusplus
}
#endif

