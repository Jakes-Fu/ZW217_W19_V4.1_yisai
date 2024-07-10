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
File Name:    atc_stk_decode.c from old name:phonesever_stk_decode.c

Description:  This is the C file for the func to be used by phonesever
***************************************************************************/
//#include "scttypes_rtos.h"

#include "MN.h"
#include "sim_type.h"
#include "sim_to_simat.h"
#include "atc_stk_enum.h"
#include "atc_stk_common_func.h"
#include "atc_stk_decode.h"

#ifdef __cplusplus
extern "C"
{
#endif

SIMAT_DO_DECODE_RESULT_E(*s_simat_do_decode_array[])(uint8 *, uint8 *, void *, uint16, BOOLEAN) =
{
    DecodeCommandDetail,
    DecodeDeviceID,
    DecodeResult,
    DecodeDuration,
    DecodeAplhaID,
    DecodeAddress,
    DecodeCCP,
    DecodeCPS,
    DecodeSSString,
    DecodeUSSDString,
    DecodeSMSTPDU,
    DecodeCBPage,
    DecodeTextString,
    DecodeTone,
    DecodeItem,
    DecodeItemID,
    DecodeResponseLen,
    DecodeFileList,
    DecodeLocationInfo,
    DecodeIMEI,
    DecodeHelpReq,
    DecodeNMR,
    DecodeDefaultText,
    DecodeNextActionID,
    DecodeEventList,
    DecodeCause,
    DecodeLocationStatus,
    DecodeTransactionID,
    DecodeBCCHList,
    DecodeIconID,
    DecodeItemIconIDList,
    DecodeCardReaderStatus,
    DecodeCardATR,
    DecodeCAPDU,
    DecodeRAPDU,
    DecodeTimerID,
    DecodeTimerValue,
    DecodeDTandTZ,
    DecodeCCReqAction,
    DecodeATCommand,
    DecodeATResponse,
    DecodeBCRepeatInd,
    DecodeImmediateResponse,
    DecodeDTMFString,
    DecodeLanguage,
    DecodeTimeAdvance,

    /* Bin.Li add usat */
    DecodeAid,
    DecodeBrowserId,
    DecodeUrl,
    DecodeBearer,
    DecodeProvisionFileRef,
    DecodeBrowserTerminationCause,
    DecodeBearDesc,
    DecodeChannelData,
    DecodeChannelDataLen,
    DecodeChannelStatus,
    DecodeBufSize,
    DecodeCardReadId,
    DecodeFileUpdateInfo,
    DecodeTransLevel,
    DecodeNotUsed,
    DecodeOtherAddress,
    DecodeAccessTech,
    DecodeDisplayPara,
    DecodeServiceRecord,
    DecodeDeviceFilter,
    DecodeServiceSercher,
    DecodeAtribute_info,
    DecodeServiceAvailable,
    DecodeEsn,
    DecodeNetAccessName,

    DecodeCdmaSmsTpdu,
    DecodeRemoteEntityAddr,
    DecodeIWLANId,
    DecodeIWLANAccessStatus,
    DecodeFill1,      /*0x4C*/
    DecodeFill2,      /*0x4D*/
    DecodeFill3,      /*0x4E*/
    DecodeFill4,      /*0x4F*/
    DecodeTextAttibute,
    DecodeItemTextAttrList,
    DecodePdpActPara,    /* 0x52*/

    DecodeFill5,       /*0x53*/
    DecodeFill6,       /*0x54*/
    DecodeFill7,      /*0x55*/
    DecodeFill8,      /*0x56*/
    DecodeFill9,          /*0x57*/
    DecodeFill10,      /*0x58*/
    DecodeFill11,       /*0x59*/
    DecodeFill12,      /*0x5A*/
    DecodeFill13,      /*0x5B*/
    DecodeFill14,       /*0x5C*/
    DecodeFill15,      /*0x5D*/
    DecodeFill16,       /*0x5E*/
    DecodeFill17,      /*0x5F*/
    DecodeFill18,      /*0x60*/
    DecodeFill19,      /*0x61*/

    DecodeImsiSV,        /* 0x62*/
    DecodeBatteryState,
    DecodeBrowserState,
    DecodeNetWorkSearchMode,
    DecodeFrameLayOut,
    DecodeFrameInfo,
    DecodeFrameId,
    DecodeUtranMeasureQualfier,

    DecodeMultiMsgRef,
    DecodeMultiMsgId,
    DecodeMultiMsgTransStatus,
    DecodeMultiMsgMeid,
    DecodeMmContentId,
    DecodeMultiMsgNotification,
    DecodeLastEnvelop,
    DecodeRegistryAppData
};
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
void CalAlphaIDRealLen(uint8 text_len, uint8 *text_str_ptr, uint8 *real_len);

#if 0
/********************************************************************
//  DESCRIPTION
//  Decode the unknow data object,get the length of the object.
//  Global resource dependence :
//  Author:Raislin.kong
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E SIMAT_DecodeUnknowObject(
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    uint16 data_len   //the left data buf length
)
{
    uint8 *ptemp = pdata;
    uint8 len, content_len;

    SCI_ASSERT(NULL != pdata);
    SCI_ASSERT(NULL != offset);

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


SIMAT_DO_DECODE_RESULT_E SIMAT_DecodeDataObject
(    //return value:the data object decode result
    SIMAT_B_DIRECTION_TAG_E object_type,    //the object data type
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    SCI_ASSERT(NULL != pdata);
    SCI_ASSERT(NULL != offset);
    SCI_ASSERT(NULL != pobject);
    *offset = 0;

    if((SIMAT_BTAG_COMMAND_DETAIL > object_type) ||
            (SIMAT_BTAG_REGISTRY_APP_DATA < object_type))
    {
        SCI_TRACE_LOW("SIMAT::SIMAT_DecodeDataObject tag error!\n");
        SCI_ASSERT(FALSE);
        return SIMAT_DO_ERROR_TAG;
    }

    //guisen.deng; remove the buf_id in all related structs
    return s_simat_do_decode_array[object_type - 1](pdata, offset, pobject, data_len, is_discard);
}
#endif
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeAddress(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    MN_CALLED_NUMBER_T *address = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_ADDRESS) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        address->num_len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    address->num_len = content_len - 1;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }


    address->number_type = (MN_NUMBER_TYPE_E)((*ptemp & 0x70) >> 4);
    address->number_plan = (MN_NUMBER_PLAN_E)(*ptemp & 0x0f);

    ptemp += 1;
    content_len--;

    if(address->num_len <= MN_MAX_ADDR_BCD_LEN)
    {
        SCI_MEMCPY(address->party_num, ptemp, address->num_len);
    }
    else
    {
        SCI_TRACE_LOW("SIMAT:the address len longer than the maximum we support, error and cut it!!!\n");
        SCI_MEMCPY(address->party_num, ptemp, MN_MAX_ADDR_BCD_LEN);
    }

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeAplhaID(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_ALPHA_ID_T *alpha_id = pobject;
    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_ALPHA_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        alpha_id->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    alpha_id->len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //calculate the real length of the Alpha identifier,the Alpha identifier
    //is coded in the same way as for EFadn.
    //SCI_TRACE_LOW("SIMAT::The alpha id length card give is 0x%x.",alpha_id->len);
    //void CalAlphaIDRealLen(uint8 text_len,uint8* text_str_ptr,uint8* real_len);
    CalAlphaIDRealLen(alpha_id->len, ptemp, &alpha_id->len);
    SCI_TRACE_LOW("SIMAT::The real alpha id length is 0x%x.", alpha_id->len);

    SCI_MEMCPY(alpha_id->id_ptr, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCPS(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    MN_SUBADDR_T *cps = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_CPS) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        cps->addr_len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    cps->addr_len = content_len - 1;

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //decode subaddress
    cps->sub_addr_type = (MN_SUBADDR_TYPE_E)(((*ptemp) & 0x70) >> 4);
    cps->odd_even_type = (MN_LENGTH_TYPE_E)(((*ptemp) & 0x08) >> 3);


    //decode subaddress information
    ptemp += 1;
    SCI_MEMSET(cps->sub_num, 0x0, MN_MAX_SUBADDR_LEN);
    SCI_MEMCPY(cps->sub_num, ptemp, cps->addr_len);


    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCCP(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_CCP_T *ccp = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_CCP) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        ccp->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    ccp->len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    SCI_ASSERT(content_len <= SIMAT_MAX_CCP_BYTE_NUM);/*assert verified: check valid value*/
    SCI_MEMCPY(ccp->ccp_ptr, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCBPage(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_CB_PAGE_T *cb_page = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_CB_PAGE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        cb_page->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    cb_page->len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //alloc the memory needed from the share buf and copy the cell broadcast page

    SCI_ASSERT(MN_SIMAT_CB_PAGE_LEN >= content_len);/*assert verified: check valid value*/
    SCI_MEMCPY(cb_page->page_arr, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCommandDetail(    //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_ERROR_TAG;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeDeviceID
( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0;
    uint8 len = 0;
    uint8 *ptemp = pdata;
    SIMAT_DO_DEVICE_ID_T *device_id = pobject;

    SCI_ASSERT(NULL != pdata);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != offset);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != pobject);/*assert verified: check null pointer*/

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_DEVICE_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 2)
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the device identity
    device_id->source = (SIMAT_DEVICE_ID_E) * ptemp;
    ptemp += 1;
    device_id->destination = (SIMAT_DEVICE_ID_E) * ptemp;

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeDuration( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len, time_unit;
    uint8 *ptemp = pdata;
    uint32 *duration = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_DURATION) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 2)
    {
        SCI_TRACE_LOW("SIMAT::the length of the duration is not 2.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the duration
    time_unit = *ptemp;
    ptemp += 1;

    switch(time_unit)
    {
        case 0x00:
            *duration = (*ptemp) * 60 * 1000;
            break;
        case 0x01:
            *duration = (*ptemp) * 1000;
            break;
        case 0x02:
            *duration = (*ptemp) * 100;
            break;
        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
            break;
    }

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeItem( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_ITEMTEMPLATE_T *item_template = pobject;


    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_ITEM) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }


    if(content_len == 0)
    {
        item_template->len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeItem content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }


    item_template->len = content_len - 1;

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the text string of item
    item_template->id = *ptemp;

    ptemp += 1;

    //calculate the real length of the text string of item,the text string
    //is coded in the same way as the alpha identifier for EFadn. Any unused
    //bytes at the end of the value part shall be coded "FF"
    //SCI_TRACE_LOW("SIMAT::The item length card give is 0x%x.",item->len);
    //void CalAlphaIDRealLen(uint8 text_len,uint8* text_str_ptr,uint8* real_len);
    CalAlphaIDRealLen(item_template->len, ptemp, &item_template->len);
    //SCI_TRACE_LOW("SIMAT::The item length card give is 0x%x.",item_template->len);


    SCI_MEMCPY(item_template->item_content_entry_buf, ptemp, item_template->len);
    /*for(i = 0; i < item_template->len; i ++)
    {
        item_template->item_content_entry_buf[i] = *ptemp;
        *ptemp++;
    }*/

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeItemID(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_ITEM_ID *item_id = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_ITEM_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 1)
    {
        SCI_TRACE_LOW("SIMAT::the length of item id is not 1.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the identifier of item chosen
    *item_id = *ptemp;

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeResponseLen(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_RESPONSE_LEN_T *response_len = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_RESPONSE_LEN) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 2)
    {
        SCI_TRACE_LOW("SIMAT::the length of the Response length is not 2.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the length of response
    response_len->min_len = *ptemp;
    ptemp += 1;
    response_len->max_len = *ptemp;

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeResult(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_RESULT_T *presult = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len);

    //check the tag
    if((SIMAT_BTAG_RESULT) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    //guisen.deng
    if(content_len == 0)
    {
        presult->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    presult->len = content_len - 1;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    presult->gen_result = (SIMAT_GENERAL_PC_RESULT_E) * ptemp;

    ptemp += 1;

    if(presult->len != 0)
    {
        SCI_MEMCPY(presult->add_ptr, ptemp, content_len);
    }

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeSMSTPDU(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    MN_L3_MSG_UNIT_T *sms_tpdu = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_SMS_TPDU) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        sms_tpdu->length = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    sms_tpdu->length = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }


    SCI_ASSERT(251 >= content_len);/*assert verified: check valid value*/
    SCI_MEMCPY(sms_tpdu->l3_msg, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeSSString( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_SS_STRING_T *ss = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_SS_STRING) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len < 2)
    {
        ss->len = 0;
    }
    else
    {
        ss->len = content_len - 1;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    if(0 == ss->len)
    {
        return SIMAT_DO_DECODE_OK;
    }

    ss->ton = (MN_NUMBER_TYPE_E)((*ptemp & 0x70) >> 4);
    ss->npi = (MN_NUMBER_PLAN_E)(*ptemp & 0x0f);

    ptemp += 1;
    //content_len--;
    SCI_ASSERT(ss->len <= MN_MAX_SIMAT_DATA_OBJECT_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(ss->ss_ptr, ptemp, ss->len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTextString(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TEXT_STRING_T *text_string = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_TEXT_STRING) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        text_string->len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeTextString content_len is zero!!! \n");
        return SIMAT_DO_DECODE_OK;
    }

    text_string->len = content_len - 1;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the data coding scheme
    text_string->dcs = *ptemp;
    ptemp += 1;

    //calculate the real length of the text string
    //the coding of this data object is referenced to GSM11.14,section 12.15
    //if the text string is coded as 8bit data or UCS2 data,calculate real length
    if(((text_string->dcs & 0x0c) == 0x04) ||
            ((text_string->dcs & 0x0c) == 0x08))
    {

        void ATC_CalTextRealLen(uint8 dcs, uint8 text_len, uint8 * text_str_ptr, uint8 * real_len);
        ATC_CalTextRealLen(text_string->dcs, text_string->len, ptemp, &text_string->len);
    }

    //SCI_TRACE_LOW("SIMAT::The text string real length is 0x%x.",text_string->len);

    SCI_ASSERT(MN_MAX_SIMAT_DATA_OBJECT_LEN >= text_string->len);/*assert verified: check valid value*/
    SCI_MEMCPY(text_string->text_ptr, ptemp, text_string->len);


    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTone( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    //AUD_GENERIC_TONE_ID_E* tone = pobject;

    SCI_ASSERT(NULL != pdata);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != offset);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != pobject);/*assert verified: check null pointer*/

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_TONE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 1)
    {
        SCI_TRACE_LOW("SIMAT::the length of the tone is not 1.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the tone
    *(uint8 *)pobject = *ptemp;

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeUSSDString(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    MN_SS_USSD_DATA_T *ussd_string = pobject;
    //uint8 dcs = 0;
    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_USSD_STRING) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        ussd_string->str_len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    ussd_string->str_len = content_len - 1;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the data coding scheme
    //alloc the memory needed from the share buf and copy the ussd string
    ussd_string->data_code_scheme = (MN_SS_DCS_E)(*ptemp);
    ptemp += 1;

    SCI_ASSERT(ussd_string->str_len <= 182);/*assert verified: check valid value*/
    SCI_MEMCPY(ussd_string->ussd_str, ptemp, ussd_string->str_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFileList
( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    SIM_FILE_NAME_E file_name = SIM_FILE_TOTAL_NUM;
    uint8 content_len = 0, len = 0, id_len = 0, i = 0, sub_content_len = 0;
    uint8 *ptemp = pdata;
    SIMAT_DO_FILE_LIST_T *file_list = pobject;

    SCI_ASSERT(NULL != pdata);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != offset);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != pobject);/*assert verified: check null pointer*/

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_FILE_LIST) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        *offset += len;
        file_list->file_num = 0;
        return SIMAT_DO_DECODE_OK;
    }

    sub_content_len = content_len - 1;
    id_len = sub_content_len >> 1;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the number of files
    //alloc the memory needed from the share buf and copy the files
    file_list->file_num = *ptemp;

    if(file_list->file_num > SIMAT_MAX_FILE_NUM_IN_LIST)
    {
        file_list->file_num = SIMAT_MAX_FILE_NUM_IN_LIST;
    }

    ptemp += 1;

    for(len = 0; len < id_len; len ++)
    {
        //file_name = SIMFS_GetFileName((uint16)((*ptemp << 8) + *(ptemp + 1)));

        if(SIM_FILE_TOTAL_NUM == file_name)
        {
            continue;
        }

        //if(SIMFS_GetFileType(file_name) == SIM_FILE_T_EF)
        {
            file_list->file_arr[i] = file_name;
            i++;

            if(i >= file_list->file_num)
            {
                break;
            }
        }

        ptemp += 2;
    }

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeLocationInfo( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_LOCATION_INFO_T *location_info = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_LOCATION_INFO) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 7)
    {
        SCI_TRACE_LOW("SIMAT::length of LocationInfo is not 7!\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //copy the MCC & MNC
    SCI_MEMCPY(location_info->mcc_mnc_arr, ptemp, SIMAT_MCC_MNC_BCD_LEN);
    ptemp += SIMAT_MCC_MNC_BCD_LEN;

    //set the location area code and cell identity
    location_info->lac = (*ptemp << 8) + *(ptemp + 1);
    ptemp += 2;
    location_info->cell_id = (*ptemp << 8) + *(ptemp + 1);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeIMEI(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_IMEI_T *imei = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_IMEI) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 8)
    {
        SCI_TRACE_LOW("SIMAT::length of IMEI is not 8!\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //copy the IMEI of the ME
    SCI_MEMCPY(imei->imei_arr, ptemp, SIMAT_IMEI_LEN);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeHelpReq(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_HELP_REQ) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 0)
    {
        SCI_TRACE_LOW("SIMAT::length of Help request is not 0!\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeNMR(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_NMR_T *nmr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_NMR) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 16)
    {
        SCI_TRACE_LOW("SIMAT::length of NMR is not 16!\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //copy the network measurement results
    SCI_MEMCPY(nmr->nmr, ptemp, SIMAT_NMR_LEN);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeDefaultText(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_DEFAULT_TEXT_T *default_text = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_DEFAULT_TEXT) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        default_text->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    default_text->len = content_len - 1;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set data coding scheme
    default_text->dcs = *ptemp;
    ptemp += 1;

 
#if 0

    if(default_text->len > MN_MAX_SIMAT_DATA_OBJECT_LEN)
    {
        SCI_TRACE_LOW("SIMAT::DecodeDefaultText alloc from share buffer error!\n");
        SCI_ASSERT(FALSE);
        return SIMAT_DO_ERROR_SHARE_MEM;
    }

#endif

    SCI_MEMCPY(default_text->text_ptr, ptemp, default_text->len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeNextActionID( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_NEXT_ACTION_ID_T *next_action_id = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_ITEM_NEXT_ACTION_INDICATOR) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        next_action_id->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    next_action_id->len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }


    SCI_MEMCPY(next_action_id->next_action_list, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeEventList(    //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len, i;
    uint8 *ptemp = pdata;
    SIMAT_DO_EVENT_LIST_T *event_list = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_EVENT_LIST) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        event_list->num = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    event_list->num = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //alloc the memory needed from the share buf and copy the event list
    for(i = 0; i < content_len; i++)
    {

        event_list->events_arr[i] = (SIMAT_EVENT_E) * (ptemp + i);

    }

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCause(    //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_CAUSE_T *cause = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_CAUSE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        cause->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    cause->len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    SCI_MEMCPY(cause->cause_arr, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeLocationStatus(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_LOCATION_STATUS_T *location_status = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_LOCATION_STATUS) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 1)
    {
        SCI_TRACE_LOW("SIMAT::length of location status is not 1.\n");
        return SIMAT_DO_LENGTH_ERROR;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    if(2 < *ptemp)
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }
    else
    {
        //set the location status
        location_status->status = (SIMAT_LOCATION_STATUS_E) * ptemp;

    }

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTransactionID(    //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TI_LIST_T *ti_list = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_TRANSACTION_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        ti_list->ti_len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    ti_list->ti_len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //copy the transaction identifier list
    SCI_MEMCPY(ti_list->tis_ptr, ptemp, SIMAT_MAX_TI_NUM);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBCCHList( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_BCCH_LIST_T *bcch_list = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_BCCH_LIST) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        bcch_list->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    bcch_list->len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //Raistlin.Kong 2007-07-13
    //change the code to adapter the new memory mechanism---replace the share memory buffer
    //with arry buffer
    SCI_MEMCPY(bcch_list->bcch_list, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCCReqAction(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{

#if 0
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_CC_REQ_ACTION_T *cc_req_action = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_CC_REQ_ACTION) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        cc_req_action->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    cc_req_action->len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    if(NULL == cc_req_action->cc_req_ptr)
    {
        SCI_TRACE_LOW("SIMAT::DecodeCCReqAction alloc from share buffer error!\n");
        SCI_ASSERT(FALSE);
        return SIMAT_DO_ERROR_SHARE_MEM;
    }

    SCI_MEMCPY(cc_req_action->cc_req_ptr, ptemp, content_len);
#endif
    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeIconID(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_ICON_ID_T *icon_id = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_ICON_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 2)
    {
        SCI_TRACE_LOW("SIMAT::the length of icon id is not 2.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the icon is self-explanatory
    if(0 == *ptemp)
    {
        icon_id->is_self_explanatory = TRUE;
    }
    else
    {
        icon_id->is_self_explanatory = FALSE;
    }

    ptemp += 1;

    //set the icon identifier
    icon_id->icon_id = *ptemp;

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeItemIconIDList(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_ICON_LIST_T *icon_list = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_ICON_ID_LIST) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len < 1)
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    icon_list->num = content_len - 1;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the icon is self-explanatory
    if(0 == *ptemp)
    {
        icon_list->is_self_explanatory = TRUE;
    }
    else
    {
        icon_list->is_self_explanatory = FALSE;
    }

    ptemp += 1;

    SCI_MEMCPY(icon_list->icon_ids, ptemp, icon_list->num);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCardReaderStatus( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_ERROR_TAG;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCardATR(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_ERROR_TAG;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCAPDU(    //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_ERROR_TAG;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeRAPDU(    //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_ERROR_TAG;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTimerID(  //return value:the data object decode result`
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{

#if 0
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TIMER_ID_T *timer_id = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_TIMER_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 1)
    {
        SCI_TRACE_LOW("SIMAT::the length of timer id is not 1.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the timer identifier
    timer_id->timer_id = *ptemp;
#endif

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTimerValue(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TIMER_VALUE_T *timer_value = pobject;
    uint8 hour = 0;
    uint8 min = 0;
    uint8 sec = 0;
    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_TIMER_VALUE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 3)
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the timer value
    hour = (((*ptemp) >> 4) & 0x0F) + (((*ptemp) & 0x0F) * 10);
    min = (((*(ptemp + 1)) >> 4) & 0x0F) + (((*(ptemp + 1)) & 0x0F) * 10);
    sec = (((*(ptemp + 2)) >> 4) & 0x0F) + (((*(ptemp + 2)) & 0x0F) * 10);
    timer_value->timer_value = hour * 3600000 + min * 60000 + sec * 1000;

    SCI_TRACE_LOW("hour %d, min %d, sec %d, timer_value :%d ", hour, min, sec, timer_value->timer_value);
    return SIMAT_DO_DECODE_OK;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeDTandTZ(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_DT_TZ_T *dt_tz = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_DT_AND_TZ) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 7)
    {
        SCI_TRACE_LOW("SIMAT::the length of the DTandDZ is not 7.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //copy the date time and time zone
    SCI_MEMCPY(dt_tz->dt_tz_arr, ptemp, SIMAT_DT_TZ_LEN);

    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeATCommand(    //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_ERROR_TAG;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeATResponse(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_ERROR_TAG;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBCRepeatInd(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_BCR_INDICATOR_T *bcr_ind = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_BC_REPEAT_INDICATOR) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the bc repeat indicator
    bcr_ind->bcr_indicator = (SIMAT_BCR_INDICATOR_E) * ptemp;

    return SIMAT_DO_DECODE_OK;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeImmediateResponse(    //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_IMMEDIATE_RESPONSE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 0)
    {
        SCI_TRACE_LOW("SIMAT::the length of the ImmediateResponse is not 0.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    return SIMAT_DO_DECODE_OK;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeDTMFString(   //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_DTMF_T *dtmf_string = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_DTMF_STRING) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        dtmf_string->len = 0;
        *offset += len;
        return SIMAT_DO_DECODE_OK;
    }

    dtmf_string->len = content_len;
    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }


    SCI_ASSERT(content_len <= MN_MAX_SIMAT_DATA_OBJECT_LEN);/*assert verified: check valid value*/

    SCI_MEMCPY(dtmf_string->dtmf, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeLanguage( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_LANG_T *language = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_LANGUAGE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 2)
    {
        SCI_TRACE_LOW("SIMAT::the length of the Language is not 2.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the language
    SCI_MEMCPY(language->lang, ptemp, MN_SIMAT_LANGE_LEN);

    return SIMAT_DO_DECODE_OK;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTimeAdvance(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TIMING_AD_T *timing_ad = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_TIME_ADVANCE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 2)
    {
        SCI_TRACE_LOW("SIMAT::the length of the Time Advance is not 2.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //set the ME status and timing advance
    if(0x00 == *ptemp)
    {
        timing_ad->is_me_idle = TRUE;
    }
    else if(0x01 == *ptemp)
    {
        timing_ad->is_me_idle = FALSE;
    }

    ptemp += 1;

    timing_ad->timing_ad = *ptemp;

    return SIMAT_DO_DECODE_OK;
}


/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
/* Bin.Li add for usat */
SIMAT_DO_DECODE_RESULT_E DecodeChannelData(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{

    uint8 content_len, len;
    uint8 *ptemp = pdata;
    SIMAT_CHANNEL_DATA_T *channel_data = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_CHANNEL_DATA) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        channel_data->data_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeChannelData content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    if(content_len < MN_SIMAT_CHANNEL_DATA_LEN)
    {
        SCI_MEMCPY(channel_data->data, ptemp, content_len);
        channel_data->data_len = content_len;
    }

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeChannelDataLen(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len, len;
    uint8 *ptemp = pdata;
    uint8 *channel_data_len_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag


    if((SIMAT_BTAG_CHANNEL_DATA_LEN) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 1)
    {
        SCI_TRACE_LOW("SIMAT::the length of channel data len  is not 1.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }


    *channel_data_len_ptr = *ptemp;


    return SIMAT_DO_DECODE_OK;
}

/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBrowserId(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    uint8 *browser_id_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_BROWSER_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 1)
    {
        SCI_TRACE_LOW("SIMAT::the length of browser id  is not 1.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }


    *browser_id_ptr = *ptemp;


    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeUrl(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_URL_T *url_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_URL) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len > MN_SIMAT_URL_LEN)
    {
        SCI_TRACE_LOW("SIMAT::the length of URL overflow.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len == 0)
    {
        url_ptr->url_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeUrl content_len is zero!!! \n");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    url_ptr->url_len = content_len;
    SCI_MEMCPY(url_ptr->url, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBearer(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_BEAR_T *bear_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_BEARER) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len > MN_SIMAT_BEAR_LEN)
    {
        SCI_TRACE_LOW("SIMAT::the length of bear overflow.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        bear_ptr->bear_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeBearer content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    bear_ptr->bear_len = content_len;
    SCI_MEMCPY(bear_ptr->bear, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeProvisionFileRef(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_PROVISION_FILE_PATH_T *file_path_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_PROVISION_PRE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len > MN_SIMAT_FILE_PATH_LEN)
    {
        SCI_TRACE_LOW("SIMAT::the length of file path overflow.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        file_path_ptr->path_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeProvisionFileRef content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    file_path_ptr->path_len = content_len;
    SCI_MEMCPY(file_path_ptr->file_path, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBearDesc(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_BEAR_DESC_T *bear_desc_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_BEAR_DESC) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len > MN_SIMAT_BEAR_QOS_LEN)
    {
        SCI_TRACE_LOW("SIMAT::the length of bear qos overflow.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        bear_desc_ptr->bear_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeBearDesc content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    bear_desc_ptr->bear_type = (SIMAT_BEAR_TYPE_E)(*ptemp);
    bear_desc_ptr->bear_len = content_len - 1;

    ptemp += 1;

    SCI_MEMCPY(bear_desc_ptr->bear_desc_qos, ptemp, bear_desc_ptr->bear_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBufSize(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard)   //the will this data object be discard
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    uint16 *buf_size = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_BUFF_SIZE) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 2)
    {
        SCI_TRACE_LOW("SIMAT::the length of buffer size is wrong.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    //*buf_size = *((uint16*)ptemp);
    *buf_size = ((*ptemp) << 8) + *(ptemp + 1);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeNetAccessName(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard)   //the will this data object be discard
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_NET_ACCESS_NAME_T *net_access_name = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_NETWORK_ACCESS_NAME) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len > MN_SIMAT_NETWORK_ACCESS_NAME_LEN)
    {
        SCI_TRACE_LOW("SIMAT::the length of net access name overflow.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        net_access_name->name_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeNetAccessName content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    net_access_name->name_len = content_len;
    SCI_MEMCPY(net_access_name->net_access_name, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeOtherAddress(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard)   //the will this data object be discard
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_OTHER_ADDRESS_T *other_addr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_OTHER_ADDR) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len > MN_SIMAT_OTHER_ADDR_LEN)
    {
        SCI_TRACE_LOW("SIMAT::the length of other address overflow.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        other_addr->addr_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeOtherAddress content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    other_addr->addr_type = (SIMAT_ADDRESS_TYPE_E)(*ptemp);
    other_addr->addr_len = content_len - 1;
    SCI_MEMCPY(other_addr->other_addr, ptemp + 1, content_len - 1);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTransLevel(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard)   //the will this data object be discard
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_TRANS_LEVEL_T *trans_level = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_INTERFACE_TRANS_LEVEL) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 3)
    {
        SCI_TRACE_LOW("SIMAT::the length of trans level overflow.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    trans_level->trans_level_type = (SIMAT_TRANS_LEVEL_TYPE_E)(*ptemp) ;

    trans_level->port = ((*(ptemp + 1)) << 8) + *(ptemp + 2);
    SCI_TRACE_LOW("[DecodeTransLevel] port= %x  ", trans_level->port);

    return SIMAT_DO_DECODE_OK;
}

/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeMultiMsgRef(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_MULTIMEDIA_MSGREF_T *multi_media_msgref_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_MULTIMEDIA_MSG_REF) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        multi_media_msgref_ptr->data_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeMultiMsgRef content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    multi_media_msgref_ptr->data_len = content_len;
    SCI_ASSERT(content_len <= MN_SIMAT_MULTIMEDIA_MSGREF_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(multi_media_msgref_ptr->muiltimedia_msgref, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}

/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeMultiMsgId(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_MULTIMEDIA_MSG_IDENTITY_T *multi_media_msgid_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_MULTIMEDIA_MSG_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        multi_media_msgid_ptr->data_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeMultiMsgId content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    multi_media_msgid_ptr->data_len = content_len;
    SCI_ASSERT(content_len <= MN_SIMAT_MULTIMEDIA_MSGID_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(multi_media_msgid_ptr->muiltimedia_msg_id, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeMmContentId(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_MM_CONTENT_ID_T *mm_content_id_ptr = pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_MULTIMEDIA_MSG_CONTENT_ID) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(0 == content_len)
    {
        mm_content_id_ptr->data_len = 0;
        *offset += len;
        SCI_TRACE_LOW("SIMAT:: DecodeMmContentId content_len is zero!!! ");
        return SIMAT_DO_DECODE_OK;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    mm_content_id_ptr->data_len = content_len;
    SCI_ASSERT(content_len <= MN_SIMAT_MM_CONTENT_DATA_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(mm_content_id_ptr->mm_content_data, ptemp, content_len);

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
//calculate the real text string length
//this function is used in the decoding for Alpha identifier,Item,and Text String
void ATC_CalTextRealLen(uint8 dcs, uint8 text_len, uint8 *text_str_ptr, uint8 *real_len)
{
    uint8 i;
    uint8 *ptemp = text_str_ptr;
    SCI_ASSERT(text_str_ptr != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(real_len != NULL);/*assert verified: check null pointer*/

    if(text_len == 0)
    {
        *real_len = 0;
        return;
    }

    *real_len = text_len;

    //if the string is coding of text in 16 bits UCS2 alphabet fomat
    if((dcs & 0x0c) == 0x08)
    {
        for(i = 0; i < text_len; i++)
        {
            if(0xff == ptemp[i])
            {
                if((i != (text_len - 1)) && (0xff == ptemp[i+1])) //if the 0xFF 0xFF existed, then the alpha ended
                {
                    *real_len = i;
                    break;
                }
                else if(i == (text_len - 1))//if the last is 0xFF, then the length is entry_ptr->alpha_len - 1
                {
                    *real_len = i;
                    break;
                }
                else
                {
                    i++;//whether return FALSE or i++?if 0xFF ?? existed, then continue
                }
            }
            else
            {
                i++;// jumped once 2 octets
            }
        }
    }
    else if((dcs & 0x0c) == 0x04) //if the string used SMS default 7-bit coded apphabet
    {
        for(i = 0; i < text_len; i++)
        {
            if(0xff == ptemp[i])
            {
                *real_len = (uint8)i;
                break;
            }
            else if(0x0 == ptemp[i])
            {
                if((i != (text_len - 1)) && (0x0 == ptemp[i+1])) //if the 0x00 0x00 existed, then the alpha ended
                {
                    *real_len = (uint8)i;
                    break;
                }
            }
        }
    }

    return;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
void CalAlphaIDRealLen(uint8 text_len, uint8 *text_str_ptr, uint8 *real_len)
{
    uint8 i;
    uint8 *ptemp = text_str_ptr;
    SCI_ASSERT(text_str_ptr != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(real_len != NULL);/*assert verified: check null pointer*/

    if(text_len == 0)
    {
        *real_len = 0;
        return;
    }

    *real_len = text_len;

    //if the string use ucs2 coded
    if(0x80 == *ptemp)
    {
        for(i = 1; i < text_len; i++)
        {
            if(0xff == ptemp[i])
            {
                if((i != (text_len - 1)) && (0xff == ptemp[i+1])) //if the 0xFF 0xFF existed, then the alpha ended
                {
                    *real_len = i;
                    break;
                }
                else if(i == (text_len - 1))//if the last is 0xFF, then the length is entry_ptr->alpha_len - 1
                {
                    *real_len = i;
                    break;
                }
                else
                {
                    i++;//whether return FALSE or i++?if 0xFF ?? existed, then continue
                }
            }
            else
            {
                i++;// jumped once 2 octets
            }
        }
    }
    else if((*ptemp != 0x81) && (*ptemp != 0x82)) //if the string used SMS default 7-bit coded apphabet
    {
        for(i = 0; i < text_len; i++)
        {
            if(0xff == ptemp[i])
            {
                *real_len = (uint8)i;
                break;
            }
            else if(0x0 == ptemp[i])
            {
                if((i != (text_len - 1)) && (0x0 == ptemp[i+1])) //if the 0x00 0x00 existed, then the alpha ended
                {
                    *real_len = (uint8)i;
                    break;
                }
            }
        }
    }

    return;
}
/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeMultiMsgTransStatus(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeMultiMsgMeid(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeMultiMsgNotification(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeLastEnvelop(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeRegistryAppData(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeAid(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBrowserTerminationCause(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeChannelStatus(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    uint8 content_len = 0, len = 0;
    uint8 *ptemp = pdata;
    SIMAT_CHANNEL_STATUS_T *channel_status_ptr = (SIMAT_CHANNEL_STATUS_T *)pobject;

    CHECK_TLV_MIN_LEN(ptemp, data_len)

    //check the tag
    if((SIMAT_BTAG_CHANNEL_STATUS) != GET_TAG_VALUE(*ptemp))
    {
        return SIMAT_DO_ERROR_TAG;
    }

    ptemp += 1;
    *offset = 1;

    //get the length of data content and the length of the "length" block
    if(!SIMAT_GetTLVLength(ptemp, &len, &content_len))
    {
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    if(content_len != 2)
    {
        SCI_TRACE_LOW("SIMAT::the length of channel status is not 2.\n");
        return SIMAT_DO_CONTENTS_NOT_UNDERSTOOD;
    }

    ptemp += len;
    *offset += len + content_len;

    //the data length error
    if(*offset > data_len)
    {
        return SIMAT_DO_LENGTH_ERROR;
    }

    if(is_discard)
    {
        return SIMAT_DO_DECODE_OK;
    }

    if(NULL != channel_status_ptr)
    {
        /*decode byte3*/
        //bit 8: link status
        if(0x80 == (*ptemp & 0x80))
        {
            channel_status_ptr->is_link_establish = TRUE;
        }
        else
        {
            channel_status_ptr->is_link_establish = FALSE;
        }

        //bit 4 to 7: RFU
        //bit 1 to 3: channel id
        channel_status_ptr->channel_id = (uint8)(*ptemp & 0x7);
        /*decode byte 4: mode status*/
        ptemp += 1;
        channel_status_ptr->mode_info = (SIMAT_CHANNEL_MODE_INFO_E) * ptemp;
    }

    return SIMAT_DO_DECODE_OK;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCardReadId(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeNotUsed(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFileUpdateInfo(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeAccessTech(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeDisplayPara(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeServiceRecord(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeDeviceFilter(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeServiceSercher(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeAtribute_info(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeServiceAvailable(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeEsn(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeCdmaSmsTpdu(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeRemoteEntityAddr(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeIWLANId(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeIWLANAccessStatus(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill1(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}

/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill2(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill3(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill4(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTextAttibute(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeItemTextAttrList(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill5(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill6(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill7(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill8(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill9(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill10(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill11(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill12(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill13(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill14(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill15(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill16(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill17(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill18(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFill19(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeImsiSV(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBatteryState(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeBrowserState(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeNetWorkSearchMode(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFrameLayOut(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFrameInfo(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFrameId(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}
/********************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeUtranMeasureQualfier(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}

/***************************************************************
//  DESCRIPTION
//  Decode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodePdpActPara(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
)
{
    return SIMAT_DO_RFU;
}

#ifdef __cplusplus
}
#endif

