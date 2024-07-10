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
File Name:    atc_stk_encode.c from old name:phonesever_stk_encode.c

Description:  This is the C file for the func to be used by phonesever
***************************************************************************/
//#include "scttypes_rtos.h"

#include "MN.h"
#include "sim_type.h"
#include "sim_to_simat.h"
#include "atc_stk_enum.h"
#include "atc_stk_encode.h"
#include "atc_stk_common_func.h"

#ifdef __cplusplus
extern "C"
{
#endif


BOOLEAN(*s_simat_do_encode_array[])(uint8 *, uint8 *, void *) =
{
    EncodeCommandDetail,
    EncodeDeviceID,
    EncodeResult,
    EncodeDuration,
    EncodeAlphaID,
    EncodeAddress,
    EncodeCCP,
    EncodeCPS,
    EncodeSSString,
    EncodeUSSDString,
    EncodeSMSTPDU,
    EncodeCBPage,
    EncodeTextString,
    EncodeTone,
    EncodeItem,
    EncodeItemID,
    EncodeResponseLen,
    EncodeFileList,
    EncodeLocationInfo,
    EncodeIMEI,
    EncodeHelpReq,
    EncodeNMR,
    EncodeDefaultText,
    EncodeNextActionID,
    EncodeEventList,
    EncodeCause,
    EncodeLocationStatus,
    EncodeTransactionID,
    EncodeBCCHList,
    EncodeIconID,
    EncodeItemIconIDList,
    EncodeCardReaderStatus,
    EncodeCardATR,
    EncodeCAPDU,
    EncodeRAPDU,
    EncodeTimerID,
    EncodeTimerValue,
    EncodeDTandTZ,
    EncodeCCReqAction,
    EncodeATCommand,
    EncodeATResponse,
    EncodeBCRepeatInd,
    EncodeImmediateResponse,
    EncodeDTMFString,
    EncodeLanguage,
    EncodeTimeAdvance,

    EncodeAid,
    EncodeBrowserId,
    EncodeUrl,
    EncodeBearer,
    EncodeProvisionFileRef,
    EncodeBrowserTerminationCause,
    EncodeBearDesc,
    EncodeChannelData,
    EncodeChannelDataLen,
    EncodeChannelStatus,
    EncodeBufSize,
    EncodeCardReadId,
    EncodeFileUpdateInfo,
    EncodeTransLevel,
    EncodeNotUsed,
    EncodeOtherAddress,
    EncodeGetAccessTech,
    EncodeDisplayPara,
    EncodeServiceRecord,
    EncodeDeviceFilter,
    EncodeServiceSercher,
    EncodeAtribute_info,
    EncodeServiceAvailable,
    EncodeEsn,
    EncodeNetAccessName,



    EncodeCdmaSmsTpdu,
    EncodeRemoteEntityAddr,
    EncodeIWLANId,
    EncodeIWLANAccessStatus,
    EncodeFill1,      /*0x4C*/
    EncodeFill2,      /*0x4D*/
    EncodeFill3,      /*0x4E*/
    EncodeFill4,      /*0x4F*/
    EncodeTextAttibute,
    EncodeItemTextAttrList,
    EncodePdpActPara,    /* 0x52*/

    EncodeFill5,       /*0x53*/
    EncodeFill6,       /*0x54*/
    EncodeFill7,      /*0x55*/
    EncodeFill8,      /*0x56*/
    EncodeFill9,          /*0x57*/
    EncodeFill10,      /*0x58*/
    EncodeFill11,       /*0x59*/
    EncodeFill12,      /*0x5A*/
    EncodeFill13,      /*0x5B*/
    EncodeFill14,       /*0x5C*/
    EncodeFill15,      /*0x5D*/
    EncodeFill16,       /*0x5E*/
    EncodeFill17,      /*0x5F*/
    EncodeFill18,      /*0x60*/
    EncodeFill19,      /*0x61*/

    EncodeImsiSV,        /* 0x62*/
    EncodeBatteryState,
    EncodeBrowserState,
    EncodeNetWorkSearchMode,
    EncodeFrameLayOut,
    EncodeFrameInfo,
    EncodeFrameId,
    EncodeUtranMeasureQualfier,

    EncodeMultiMsgRef,
    EncodeMultiMsgId,
    EncodeMultiMsgTransStatus,
    EncodeMultiMsgMeid,
    EncodeMultiMsgRefContentId,
    EncodeMultiMsgNotification,
    EncodeLastEnvelop,
    EncodeRegistryAppData
};

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeAddress(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject  //in:the object to encode to buf
)
{
    uint8 *ptemp = pdata, offset;
    MN_CALLED_NUMBER_T *padd = pobject;

    *ptemp = (uint8)SIMAT_BTAG_ADDRESS;
    ptemp += 1;
    *data_len = 1;

    SIMAT_SetTLVLength(ptemp, &offset, (uint8)(padd->num_len + 1));
    ptemp += offset;

    *ptemp = ((uint8)((padd->number_type << 4) + padd->number_plan) | 0x80);
    ptemp += 1;

    if(padd->num_len <= MN_MAX_ADDR_BCD_LEN)
    {
        SCI_MEMCPY(ptemp, padd->party_num, padd->num_len);
        *data_len += offset + padd->num_len + 1;
    }
    else
    {
        SCI_TRACE_LOW("SIMAT:the address len longer than the maximum we support, error and cut it!!!\n");
        *data_len += offset + MN_MAX_ADDR_BCD_LEN + 1;
        SCI_MEMCPY(ptemp, padd->party_num, MN_MAX_ADDR_BCD_LEN);
    }

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBrowserTerminationCause(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_BROWSER_TERMINATION_CAUSE_E *cause = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_BROWSER_TERMINAT_CAUSE);
    ptemp += 1;

    //set the "length" block
    content_len = 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    *ptemp = *((uint8 *)cause);

    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeAlphaID(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_ALPHA_ID_T *alpha_id = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_ALPHA_ID);
    ptemp += 1;

    //set the "length" block
    content_len = alpha_id->len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the alpha identites
    SCI_MEMCPY(ptemp, alpha_id->id_ptr, alpha_id->len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCPS(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset = 0;
    uint8 content_len = 0;
    uint8 *ptemp = pdata;
    MN_SUBADDR_T *cps = pobject;

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeCPS occuring error point is NULL! \n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CPS);
    ptemp += 1;

    //set the "length" block
    content_len = cps->addr_len + 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the address type and odd/even type
    *ptemp = (uint8)(((cps->sub_addr_type << 4)
                      | (cps->odd_even_type << 3)
                      & 0xF8) | 0x80);
    ptemp += 1;
    //copy the called party subaddress
    SCI_ASSERT(*data_len <= MN_MAX_SUBADDR_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(ptemp, cps->sub_num, cps->addr_len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCCP(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset = 0;
    uint8 content_len = 0;
    uint8 *ptemp = pdata;
    SIMAT_DO_CCP_T *ccp = pobject;

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeCCP occuring error point is NULL!\n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CCP);
    ptemp += 1;

    //set the "length" block
    content_len = ccp->len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the capability configuration parameters
    SCI_ASSERT(ccp->len <= SIMAT_MAX_CCP_BYTE_NUM);/*assert verified: check valid value*/
    SCI_MEMCPY(ptemp, ccp->ccp_ptr, ccp->len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCBPage(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 content_len = 0;
    uint8 *ptemp = pdata;
    SIMAT_DO_CB_PAGE_T *cb_page = pobject;
    uint8 offset = 0;

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeCBPage occuring error point is NULL!\n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CB_PAGE);
    ptemp += 1;

    //set the "length" block
    content_len = cb_page->len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the cell broadcast page
    SCI_ASSERT(cb_page->len <= MN_SIMAT_CB_PAGE_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(ptemp, cb_page->page_arr, cb_page->len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCommandDetail(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeDeviceID(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset = 0;
    uint8 content_len = 0;
    uint8 *ptemp = pdata;
    SIMAT_DO_DEVICE_ID_T *device_id = pobject;

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeDeviceID occuring error point is NULL!\n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_DEVICE_ID);
    ptemp += 1;

    //set the "length" block
    content_len = 0x02;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the sourse device identity
    *ptemp = (uint8)device_id->source;
    ptemp += 1;

    //set the destination device identity
    *ptemp = (uint8)device_id->destination;

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeDuration(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint16 time;
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    uint32 *duration = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_DURATION);
    ptemp += 1;

    //set the "length" block
    content_len = 0x02;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the time unit and interval
    time = (uint16)((*duration) / 100);

    if(time < 1)
    {
        return FALSE;
    }
    else if((time <= 255) && (time % 10 != 0))
    {
        *ptemp = 0x02;
        ptemp += 1;
        *ptemp = (uint8)(time);
    }
    else if(((time / 10) <= 255) && (time % 600 != 0))
    {
        time = time / 10;
        *ptemp = 0x01;
        ptemp += 1;
        *ptemp = (uint8)time;
    }
    else if((time / 600) <= 255)
    {
        time = time / 600;
        *ptemp = 0x00;
        ptemp += 1;
        *ptemp = (uint8)time;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeItem(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_ITEMTEMPLATE_T *item_template = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_ITEM);
    ptemp += 1;

    //set the "length" block
    //content_len = item->len + 1;
    content_len = item_template->len + 1;

    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the item identiter and item text string
    *ptemp = (uint8)item_template->id;
    ptemp += 1;

    SCI_MEMCPY(ptemp, item_template->item_content_entry_buf, item_template->len);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeItemID(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset = 0;
    uint8 content_len = 0;
    uint8 *ptemp = NULL;
    SIMAT_DO_ITEM_ID *item_id = pobject;
    ptemp = pdata;

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeItemID occuring error point is NULL!\n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_ITEM_ID);
    ptemp += 1;

    //set the "length" block
    content_len = 0x01;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the item identifier
    *ptemp = *item_id;

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeResponseLen(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_RESPONSE_LEN_T *response_len = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_RESPONSE_LEN);
    ptemp += 1;

    //set the "length" block
    content_len = 0x02;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the minimum length of response
    *ptemp = (uint8)response_len->min_len;
    ptemp += 1;

    //set the maximum length of response
    *ptemp = (uint8)response_len->max_len;

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeResult(

    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeSMSTPDU(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{

    uint8 offset = 0;
    uint8 content_len = 0;
    uint8 *ptemp = pdata;
    MN_L3_MSG_UNIT_T *sms_tpdu = pobject;

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeSMSTPDU occuring error point is NULL!\n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_SMS_TPDU);
    ptemp += 1;

    //set the "length" block
    content_len = sms_tpdu->length;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the sms tpdu
    SCI_ASSERT(sms_tpdu->length <= 251);/*assert verified: check valid value*/
    SCI_MEMCPY(ptemp, sms_tpdu->l3_msg, sms_tpdu->length);

    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeSSString(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 *ptemp = pdata, offset;
    SIMAT_DO_SS_STRING_T *pss_str = pobject;

    *ptemp = (uint8)SIMAT_BTAG_SS_STRING;
    ptemp += 1;
    *data_len = 1;

    SIMAT_SetTLVLength(ptemp, &offset, (uint8)(pss_str->len + 1));
    ptemp += offset;
    *data_len += offset + pss_str->len + 1;

    *ptemp = ((uint8)((pss_str->ton << 4) + pss_str->npi) | 0x80);
    ptemp += 1;

    SCI_MEMCPY(ptemp, pss_str->ss_ptr, pss_str->len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeTextString(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TEXT_STRING_T *text_string = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_TEXT_STRING);
    ptemp += 1;

    //set the "length" block
    content_len = text_string->len + 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the data coding scheme and text string
    *ptemp = text_string->dcs;
    ptemp += 1;

    SCI_ASSERT(text_string->len <= MN_MAX_SIMAT_DATA_OBJECT_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(ptemp, text_string->text_ptr, text_string->len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeTone(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset = 0, content_len = 0;
    uint8 *ptemp = pdata;

    SCI_ASSERT(NULL != pdata);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != data_len);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != pobject);/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_TONE);
    ptemp += 1;

    //set the "length" block
    content_len = 0x01;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the tone
    *ptemp = *((uint8 *)pobject);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeUSSDString(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    MN_SS_USSD_DATA_T *ussd_string = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_USSD_STRING);
    ptemp += 1;

    //set the "length" block
    content_len = ussd_string->str_len + 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the date coding scheme
    *ptemp = ussd_string->data_code_scheme;
    ptemp += 1;

    //copy the USSD string
    SCI_ASSERT(ussd_string->str_len <= 182);/*assert verified: check null pointer*/
    SCI_MEMCPY(ptemp, ussd_string->ussd_str, ussd_string->str_len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFileList(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint16 len = 0, file_id;
    int8 route_num;
    uint8  buf[SIMAT_SHARE_BUF_LEN] = {0};
    uint8 *ptemp = pdata, offset, i, j, *pstart, *pbuf;

//#ifdef PRODUCT_DM
//    SIMAT_REFRESH_FILE_LIST_T  *pfile_list = pobject;
//#else
    SIMAT_DO_FILE_LIST_T  *pfile_list = pobject;
//#endif
    //SIMAT_DO_FILE_LIST_T* pfile_list = pobject;

    SIM_FILE_NAME_E file_route[SIM_FILE_LEVEL_E_NUM] = {SIM_FILE_MF};

    *data_len = 0;

    *ptemp = (uint8)SIMAT_BTAG_FILE_LIST;
    ptemp += 1;
    *data_len += 1;

    //open a share buf for temply using
    pstart = pbuf = buf;

    //add the file list file in to the file array
    for(i = 0; i < pfile_list->file_num; i++)
    {
        //get the file route from the SIM MF to the file
//#ifdef PRODUCT_DM
//        route_num = SIMFS_FileRouting((SIM_FILE_NAME_E)pfile_list->file_data_arr[i], SIM_FILE_MF, file_route);
//#else
        route_num = SIMFS_FileRouting(pfile_list->file_arr[i], SIM_FILE_MF, file_route);
//#endif

        if(route_num < 0)
        {
            return FALSE;
        }

        //add the SIM MF file to the file list files array
        file_id = SIMFS_GetFileID(0, SIM_FILE_MF);   
        *pbuf = (uint8)(file_id >> 8);
        pbuf += 1;
        *pbuf = (uint8)file_id;
        pbuf += 1;
        len += 2;

        //add the files in the file route to the list file array
        for(j = 0; j < route_num; j++)
        {
            file_id = SIMFS_GetFileID(0, file_route[j]);   
            *pbuf = (uint8)(file_id >> 8);
            pbuf += 1;
            *pbuf = (uint8)file_id;
            pbuf += 1;
            len += 2;
        }
    }

    if(len >= SIM_MAX_C_APDU_DATA_LEN)
    {
        return FALSE;
    }

    SIMAT_SetTLVLength(ptemp, &offset, (uint8)(len + 1));
    ptemp += offset;
    *data_len += (uint8)len + offset + 1;

    *ptemp = (uint8)pfile_list->file_num;
    ptemp += 1;

    SCI_MEMCPY(ptemp, pstart, len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeLocationInfo(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset = 0;
    uint8 content_len = 0;
    uint8 *ptemp = pdata;
    uint8 mccmnc[3];
    MN_PHONE_CURRENT_PLMN_INFO_T *location_info = pobject;

    SCI_MEMSET(mccmnc, 0xff, sizeof(mccmnc));

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeItemID occuring error point is NULL!\n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_LOCATION_INFO);
    ptemp += 1;

    //set the "length" block, why content_len = 0x07???
    content_len = 0x07;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the MCC & MNC
    //  SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);
    EncodeMNCMCCToThreeByte(location_info->mcc,
                            location_info->mnc,
                            location_info->mnc_digit_num,
                            mccmnc);
    SCI_MEMCPY(ptemp, mccmnc, SIMAT_MCC_MNC_BCD_LEN);
    ptemp += SIMAT_MCC_MNC_BCD_LEN;

    //set the location area code
    *ptemp = (uint8)(location_info->lac >> 8);
    ptemp += 1;
    *ptemp = (uint8)(location_info->lac & 0xff);
    ptemp += 1;

    //set the cell identity value
    *ptemp = (uint8)(location_info->cell_id >> 8);
    ptemp += 1;
    *ptemp = (uint8)(location_info->cell_id & 0xff);
    ptemp += 1;

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeIMEI(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_IMEI_T *imei = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_IMEI);
    ptemp += 1;

    //set the "length" block
    content_len = 0x08;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the IMEI of the ME

    SCI_MEMCPY(ptemp, imei->imei_arr, SIMAT_IMEI_LEN);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeHelpReq(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 *ptemp = pdata;

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeHelpReq occuring error point is NULL!\n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_HELP_REQ);
    ptemp += 1;

    //set the "length" block
    *ptemp = 0x00;
    *data_len = 2;

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeNMR(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    //nmr network measurement results
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_NMR_T *nmr = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_NMR);
    ptemp += 1;

    //set the "length" block
    content_len = 0x10;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the network measurement results
    SCI_MEMCPY(ptemp, nmr->nmr, SIMAT_NMR_LEN);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeDefaultText(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_DEFAULT_TEXT_T *default_text = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_DEFAULT_TEXT);
    ptemp += 1;

    //set the "length" block
    content_len = default_text->len + 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the data coding scheme and text string
    *ptemp = default_text->dcs;
    ptemp += 1;

    SCI_MEMCPY(ptemp, default_text->text_ptr, default_text->len);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeNextActionID(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_NEXT_ACTION_ID_T *next_action_id = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_ITEM_NEXT_ACTION_INDICATOR);
    ptemp += 1;

    //set the "length" block
    content_len = next_action_id->len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the items next action indicator list
    SCI_MEMCPY(ptemp, next_action_id->next_action_list, next_action_id->len);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeEventList(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset = 0;
    uint8 content_len = 0;
    uint8 i = 0;
    uint8 *ptemp = pdata;
    SIMAT_DO_EVENT_LIST_T *event_list = pobject;

    //Bin.Li add point check
    if((NULL == data_len) || (NULL == pdata) || (NULL == pobject))
    {
        SCI_TRACE_LOW("EncodeDeviceID occuring error point is NULL!\n"); //point is NULL

        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_EVENT_LIST);
    ptemp += 1;

    //set the "length" block
    content_len = event_list->num;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the event list
    for(i = 0; i < event_list->num; i++)
    {
        *(ptemp + i) = (uint8)event_list->events_arr[i];
    }

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCause(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_CAUSE_T *cause = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CAUSE);
    ptemp += 1;

    //set the "length" block
    content_len = cause->len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the cause
    SCI_MEMCPY(ptemp, cause->cause_arr, cause->len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeLocationStatus(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_LOCATION_STATUS_T *location_status = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_LOCATION_STATUS);
    ptemp += 1;

    //set the "length" block
    content_len = 0x01;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = 0x03;

    //set the location status
    *ptemp = (uint8)location_status->status;

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeTransactionID(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TI_LIST_T *ti = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_TRANSACTION_ID);
    ptemp += 1;

    /* +cetecom stk test bin.li */
    content_len = 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    *ptemp = ti->tis_ptr[0];

    //SCI_MEMCPY(ptemp,ti->tis_ptr,SIMAT_MAX_TI_NUM);

    /* -cetecom stk test bin.li */
    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBCCHList(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_BCCH_LIST_T *bcch_list = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_BCCH_LIST);
    ptemp += 1;

    //set the "length" block
    content_len = bcch_list->len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the BCCH channel list
    SCI_MEMCPY(ptemp, bcch_list->bcch_list, bcch_list->len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCCReqAction(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{

#if 0
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_CC_REQ_ACTION_T *ccreq_action = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CC_REQ_ACTION);
    ptemp += 1;

    //set the "length" block
    content_len = ccreq_action->len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the call control requested action
    SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);
    SCI_MEMCPY(ptemp, ccreq_action->cc_req_ptr, ccreq_action->len);
#endif
    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeIconID(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_ICON_ID_T *icon_id = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_ICON_ID);
    ptemp += 1;

    //set the "length" block
    content_len = 0x02;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the icon qualifier
    if(icon_id->is_self_explanatory)
    {
        *ptemp = 0x00;
    }
    else
    {
        *ptemp = 0x01;
    }

    ptemp += 1;

    //set the icon identifier
    *ptemp = icon_id->icon_id;

    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeItemIconIDList(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_ICON_LIST_T *icon_list = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_ICON_ID_LIST);
    ptemp += 1;

    //set the "length" block
    content_len = icon_list->num + 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the icon list qualifier
    if(icon_list->is_self_explanatory)
    {
        *ptemp = 0x00;
    }
    else
    {
        *ptemp = 0x01;
    }

    ptemp += 1;

    //set the icon identifier list
    SCI_MEMCPY(ptemp, icon_list->icon_ids, icon_list->num);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCardATR(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCAPDU(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeRAPDU(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeTimerID(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{

#if 0
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TIMER_ID_T *timer_id = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_TIMER_ID);
    ptemp += 1;

    //set the "length" block
    content_len = 0x01;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the timer identifier
    *ptemp = (uint8)timer_id->timer_id;
#endif
    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeTimerValue(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint32 sec = 0;
    uint32 min = 0;
    uint32 hour = 0;
    uint8 *ptemp = pdata;
    SIMAT_DO_TIMER_VALUE_T *timer_value = pobject;
    uint8 utemp = 0;
    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_TIMER_VALUE);
    ptemp += 1;

    //set the "length" block
    content_len = 0x03;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the Timer value
    sec = (timer_value->timer_value / 1000);

    if(sec > 60)
    {
        min = sec / 60;
        sec = sec % 60;
    }

    if(min > 60)
    {
        hour = min / 60;
        min = min % 60;
    }


    utemp = (((hour % 10) << 4) & 0xF0) + ((hour / 10) & 0x0F);
    *ptemp = utemp;
    ptemp += 1;
    utemp = (((min % 10) << 4) & 0xF0) + ((min / 10) & 0x0F);
    *ptemp = utemp;
    ptemp += 1;
    utemp = (((sec % 10) << 4) & 0xF0) + ((sec / 10) & 0x0F);

    *ptemp = utemp;

    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeDTandTZ(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_DT_TZ_T *dt_tz = pobject;

    /* bin.li modify for usat test */
    uint8 uTemp = 0x0;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_DT_AND_TZ);
    ptemp += 1;

    //set the "length" block
    content_len = SIMAT_DT_TZ_LEN;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the Data-Time and Time zone
    uTemp = ((((dt_tz->dt_tz_arr[0]) % 10) << 4) & 0xF0) + (((dt_tz->dt_tz_arr[0]) / 10) & 0x0F);
    *ptemp = uTemp;
    ptemp ++;
    uTemp = ((((dt_tz->dt_tz_arr[1]) % 10) << 4) & 0xF0) + (((dt_tz->dt_tz_arr[1]) / 10) & 0x0F);
    *ptemp = uTemp;
    ptemp ++;
    uTemp = ((((dt_tz->dt_tz_arr[2]) % 10) << 4) & 0xF0) + (((dt_tz->dt_tz_arr[2]) / 10) & 0x0F);
    *ptemp = uTemp;
    ptemp ++;
    uTemp = ((((dt_tz->dt_tz_arr[3]) % 10) << 4) & 0xF0) + (((dt_tz->dt_tz_arr[3]) / 10) & 0x0F);
    *ptemp = uTemp;
    ptemp ++;
    uTemp = ((((dt_tz->dt_tz_arr[4]) % 10) << 4) & 0xF0) + (((dt_tz->dt_tz_arr[4]) / 10) & 0x0F);
    *ptemp = uTemp;
    ptemp ++;

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeATCommand(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeATResponse(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBCRepeatInd(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_BCR_INDICATOR_T *bcr_ind = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_BC_REPEAT_INDICATOR);
    ptemp += 1;

    //set the "length" block
    content_len = 0x01;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the BC repeat indicatior
    *ptemp = (uint8)bcr_ind->bcr_indicator;

    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeImmediateResponse(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_IMMEDIATE_RESPONSE);
    ptemp += 1;

    //set the "length" block
    content_len = 0x00;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    *data_len = offset + content_len + 1;

    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeDTMFString(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_DTMF_T *dtmf = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_DTMF_STRING);
    ptemp += 1;

    //set the "length" block
    content_len = dtmf->len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the DTMF string
    SCI_MEMCPY(ptemp, dtmf->dtmf, dtmf->len);

    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeLanguage(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_LANG_T *language = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_LANGUAGE);
    ptemp += 1;

    //set the "length" block
    content_len = 0x02;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //copy the language
    SCI_MEMCPY(ptemp, language->lang, content_len);

    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeTimeAdvance(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_DO_TIMING_AD_T *timing_ad = pobject;

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_TIME_ADVANCE);
    ptemp += 1;

    //set the "length" block
    content_len = 0x02;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the ME status
    if(timing_ad->is_me_idle)
    {
        *ptemp = 0x00;
    }
    else
    {
        *ptemp = 0x01;
    }

    ptemp += 1;

    //set timing advance
    *ptemp = (uint8)timing_ad->timing_ad;

    return TRUE;
}



/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
/* Bin.Li add for usat */
BOOLEAN EncodeGetAccessTech(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/
    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_ACCESS_TECH);
    ptemp += 1;

    //set the "length" block
    content_len = 0x01;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    *ptemp = *(uint8 *)pobject;

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeChannelData(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_CHANNEL_DATA_T *channel_data_ptr = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CHANNEL_DATA);
    ptemp += 1;

    //set the "length" block
    content_len = channel_data_ptr->data_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    SCI_MEMCPY(ptemp, channel_data_ptr->data, content_len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeChannelDataLen(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CHANNEL_DATA_LEN);
    ptemp += 1;

    //set the "length" block
    content_len = 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;
    *ptemp = *(uint8 *)pobject;

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBearDesc(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_BEAR_DESC_T *bear_desc_ptr = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_BEAR_DESC);
    ptemp += 1;

    //set the "length" block
    content_len = 1 + bear_desc_ptr->bear_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;
    *ptemp = (uint8)bear_desc_ptr->bear_type;
    ptemp += 1;
    //set the "Bearer description" block
    SCI_MEMCPY(ptemp, bear_desc_ptr->bear_desc_qos, bear_desc_ptr->bear_len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBufSize(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_BUFF_SIZE);
    ptemp += 1;

    //set the "length" block
    content_len = 2;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    SCI_MEMCPY(ptemp, (uint8 *)pobject, sizeof(uint16));

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeOtherAddress(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_OTHER_ADDRESS_T *other_addr = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_OTHER_ADDR);
    ptemp += 1;

    //set the "length" block
    content_len = other_addr->addr_len + 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;
    *ptemp = (uint8)other_addr->addr_type;
    ptemp += 1;
    SCI_MEMCPY(ptemp, other_addr->other_addr, other_addr->addr_len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeChannelStatus(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{

#if 0
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_CHANNEL_STATUS_T *channel_status_ptr  = pobject;
    uint8 conversion = 0;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CHANNEL_STATUS);
    ptemp += 1;

    //set the "length" block
    content_len = 2;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    SCI_ASSERT(channel_status_ptr->channel_id  <= 7);
    conversion = channel_status_ptr->channel_id;

    if(channel_status_ptr->is_link_establish)
    {
        conversion |= 0x80;
    }

    *ptemp = conversion;
    ptemp += 1;
    *ptemp = (uint8)channel_status_ptr->mode_info;
#endif
    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBrowserState(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_BROWSER_STATUS_T *browser_status_ptr  = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_BROWSER_STATE);
    ptemp += 1;

    //set the "length" block
    content_len = browser_status_ptr->state_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    SCI_ASSERT(browser_status_ptr->state_len <= MN_SIMAT_BROWSER_STATUS_LEN);/*assert verified: check valid value*/

    SCI_MEMCPY(ptemp, browser_status_ptr->state_data, browser_status_ptr->state_len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCardReaderStatus(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{

#if 0
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_CARD_READ_STATUS_IND_T *card_reader_status_ptr  = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_CARD_READER_STATUS);
    ptemp += 1;

    //set the "length" block
    content_len = 1;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    *ptemp = card_reader_status_ptr->identity;
    *ptemp |= card_reader_status_ptr->status_mask;
#endif
    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeServiceRecord(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_SERVICE_RECORD_T *service_record_ptr  = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_SERVICE_RECORD);
    ptemp += 1;

    //set the "length" block
    content_len = 2 + service_record_ptr->data_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    *ptemp = service_record_ptr->local_bear_identity;
    ptemp += 1;
    *ptemp = service_record_ptr->service_id;
    ptemp += 1;

    SCI_ASSERT(service_record_ptr->data_len <= MN_SIMAT_SERVICE_RECORD_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(ptemp, service_record_ptr->service_record, service_record_ptr->data_len);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeRemoteEntityAddr(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_REMOTE_ENTITY_ADDRESS_T *remote_entity_addr_ptr  = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_REMOTE_ENTITY_ADDR);
    ptemp += 1;

    //set the "length" block
    content_len = 1 + remote_entity_addr_ptr->addr_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    *ptemp = remote_entity_addr_ptr->addr_code_type;
    ptemp += 1;

    SCI_ASSERT(remote_entity_addr_ptr->addr_len <= MN_SIMAT_SUB_ADDR_LEN);/*assert verified: check valid value*/
    SCI_MEMCPY(ptemp, remote_entity_addr_ptr->remote_addr, remote_entity_addr_ptr->addr_len);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeTransLevel(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_TRANS_LEVEL_T *trans_level_ptr = (SIMAT_TRANS_LEVEL_T *)pobject;

    if(NULL == pdata || NULL == data_len || NULL == trans_level_ptr)
    {
        SCI_TRACE_LOW("EncodeTransLevel occuring error point is NULL!\n");
        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_INTERFACE_TRANS_LEVEL);
    ptemp += 1;

    //set the "length" block
    content_len = 3;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the "UICC/terminal interface transport level" block
    *ptemp = (uint8)trans_level_ptr->trans_level_type;
    ptemp += 1;

    SCI_MEMCPY(ptemp, &(trans_level_ptr->port), sizeof(uint16));

    return TRUE;
}

/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeMultiMsgId(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_MULTIMEDIA_MSG_IDENTITY_T *multi_media_msg_id_ptr  = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_MULTIMEDIA_MSG_ID);
    ptemp += 1;

    //set the "length" block
    content_len = multi_media_msg_id_ptr->data_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    SCI_ASSERT(multi_media_msg_id_ptr->data_len <= MN_SIMAT_MULTIMEDIA_MSGID_LEN);/*assert verified: check valid value*/

    SCI_MEMCPY(ptemp, multi_media_msg_id_ptr->muiltimedia_msg_id,
               multi_media_msg_id_ptr->data_len);

    return TRUE;
}
/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeMultiMsgTransStatus(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_MULTIMEDIA_MSG_TRANS_STATUS_T *multi_media_msg_trans_status_ptr  = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_MULTIMEDIA_MSG_TRANS_STATUS);
    ptemp += 1;

    //set the "length" block
    content_len = multi_media_msg_trans_status_ptr->data_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    SCI_ASSERT(multi_media_msg_trans_status_ptr->data_len <= MN_SIMAT_MULTIMEDIA_MSG_TRANS_STATUS_LEN);/*assert verified: check valid value*/

    SCI_MEMCPY(ptemp, multi_media_msg_trans_status_ptr->muiltimedia_msg_trans_status,
               multi_media_msg_trans_status_ptr->data_len);

    return TRUE;
}

/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeMultiMsgNotification(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{

#if 0
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_MMS_NOTI_DOWNLOAD_IND_T *mms_noti_download  = pobject;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_MULTIMEDIA_MSG_NOTIFICATION);
    ptemp += 1;

    //set the "length" block
    content_len = mms_noti_download->data_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    SCI_ASSERT(mms_noti_download->data_len <= SIMAT_MULTIMEDIA_MSG_NOTIFY_DATA_LEN);

    SCI_MEMCPY(ptemp, mms_noti_download->msg_notify_data,
               mms_noti_download->data_len);
#endif
    return TRUE;
}

/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeLastEnvelop(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;

    SCI_ASSERT((NULL != pdata) && (NULL != data_len) && (NULL != pobject));/*assert verified: check null pointer*/

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_LAST_ENVELOP);
    ptemp += 1;

    //set the "length" block
    content_len = 0;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    return TRUE;
}

/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeRegistryAppData(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeMultiMsgMeid(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeMultiMsgRefContentId(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeNetWorkSearchMode(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFrameLayOut(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFrameInfo(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFrameId(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeUtranMeasureQualfier(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodePdpActPara(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/***************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeMultiMsgRef(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeIWLANId(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeIWLANAccessStatus(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill1(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill2(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill3(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill4(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeTextAttibute(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeItemTextAttrList(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill5(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill6(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill7(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill8(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill9(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill10(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill11(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill12(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill13(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill14(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill15(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill16(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill17(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill18(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFill19(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeImsiSV(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBatteryState(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCardReadId(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeFileUpdateInfo(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeNotUsed(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeDisplayPara(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeDeviceFilter(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeServiceSercher(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeAtribute_info(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeServiceAvailable(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeEsn(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeNetAccessName(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_NET_ACCESS_NAME_T *net_access_name_ptr = (SIMAT_NET_ACCESS_NAME_T *)pobject;

    if(NULL == pdata || NULL == data_len || NULL == net_access_name_ptr)
    {
        SCI_TRACE_LOW("EncodeNetAccessName occuring error point is NULL!\n");
        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_NETWORK_ACCESS_NAME);
    ptemp += 1;

    //set the "length" block
    content_len = net_access_name_ptr->name_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the "Network Access Name" block
    //SCI_ASSERT(*data_len <= MN_SIMAT_NETWORK_ACCESS_NAME_LEN);
    SCI_MEMCPY(ptemp, net_access_name_ptr->net_access_name, net_access_name_ptr->name_len);

    return TRUE;

}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeCdmaSmsTpdu(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}

/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeAid(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    return FALSE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBrowserId(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_BROWSER_IDENTITY_TYPE_E browser_id = *((SIMAT_BROWSER_IDENTITY_TYPE_E *)pobject);

    if(NULL == pdata || NULL == data_len || NULL == pobject)
    {
        SCI_TRACE_LOW("EncodeBrowserId occuring error point is NULL!\n");
        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_BROWSER_ID);
    ptemp += 1;

    //set the "length" block
    content_len = 0x01;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the "browser identifier" block
    *ptemp = (uint8)browser_id;

    return TRUE;

}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeUrl(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_URL_T   *url_ptr = (SIMAT_URL_T *)pobject;

    if(NULL == pdata || NULL == data_len || NULL == url_ptr)
    {
        SCI_TRACE_LOW("EncodeUrl occuring error point is NULL!\n");
        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_URL);
    ptemp += 1;

    //set the "length" block
    content_len = url_ptr->url_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the "url" block
    //SCI_ASSERT(*data_len <= MN_SIMAT_URL_LEN);
    SCI_MEMCPY(ptemp, url_ptr->url, url_ptr->url_len);

    return TRUE;

}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeBearer(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_BEAR_T   *bear_ptr = (SIMAT_BEAR_T *)pobject;

    if(NULL == pdata || NULL == data_len || NULL == bear_ptr)
    {
        SCI_TRACE_LOW("EncodeBearer occuring error point is NULL!\n");
        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_BEARER);
    ptemp += 1;

    //set the "length" block
    content_len = bear_ptr->bear_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the "bearer" block
    //SCI_ASSERT(*data_len <= MN_SIMAT_BEAR_LEN);
    SCI_MEMCPY(ptemp, bear_ptr->bear, bear_ptr->bear_len);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeProvisionFileRef(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
)
{
    uint8 offset, content_len;
    uint8 *ptemp = pdata;
    SIMAT_PROVISION_FILE_PATH_T   *provision_fr_ptr = (SIMAT_PROVISION_FILE_PATH_T *)pobject;

    if(NULL == pdata || NULL == data_len || NULL == provision_fr_ptr)
    {
        SCI_TRACE_LOW("EncodeProvisionFileRef occuring error point is NULL!\n");
        return FALSE;
    }

    //set the tag value
    *ptemp = (uint8)(SIMAT_BTAG_PROVISION_PRE);
    ptemp += 1;

    //set the "length" block
    content_len = provision_fr_ptr->path_len;
    SIMAT_SetTLVLength(ptemp, &offset, content_len);
    ptemp += offset;
    *data_len = offset + content_len + 1;

    //set the "Provisioning File Reference x" block
    //SCI_ASSERT(*data_len <= MN_SIMAT_FILE_PATH_LEN);
    SCI_MEMCPY(ptemp, provision_fr_ptr->file_path, provision_fr_ptr->path_len);

    return TRUE;
}

#ifdef __cplusplus
}
#endif
