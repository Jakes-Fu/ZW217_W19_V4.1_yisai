#ifndef _ATC_STK_ENCODE_H_
#define _ATC_STK_ENCODE_H_
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
File Name:    atc_stk_encode.h
Version:      0.1
Author:       Bin.Li
Date:         2009.4
Description:  This is the header file have the phoneserver stk common func
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
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeAddress(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject  //in:the object to encode to buf
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
/********************************************************************
//  DESCRIPTION
//  Encode TLV Data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN EncodeGetAccessTech(
    uint8 *pdata,   //out:that data buf that save the data object
    uint8 *data_len,//out:the data object length in the buf.
    void *pobject   //in:the object to encode to buf
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);

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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);

#ifdef __cplusplus
}
#endif

#endif  //_ATC_STK_ENCODE_H_