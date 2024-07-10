#ifndef _ATC_STK_DECODE_H_
#define _ATC_STK_DECODE_H_
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
File Name:    atc_stk_decode.h
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
);
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
);
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
);
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
);
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
);
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
);
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeDeviceID( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeFileList( //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
);
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
);
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
);
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
);
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
    void *pobject,   //out:the object decoded outT
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeTimerID(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
/********************************************************************
//  DESCRIPTION
//  Decode tlv data
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIMAT_DO_DECODE_RESULT_E DecodeChannelData(  //return value:the data object decode result
    uint8 *pdata,   //in:that data buf that save the data object
    uint8 *offset,     //out:the data object length in the buf.
    void *pobject,   //out:the object decoded out
    uint16 data_len,   //the left data buf length
    BOOLEAN is_discard   //the will this data object be discard
);
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
);
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
);
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
);
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
);
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
);
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
);
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
    BOOLEAN is_discard   //the will this data object be discard
);
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
    BOOLEAN is_discard   //the will this data object be discard
);
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
    BOOLEAN is_discard   //the will this data object be discard
);
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
    BOOLEAN is_discard   //the will this data object be discard
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);

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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);
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
);

#ifdef __cplusplus
}
#endif

#endif  //_ATC_STK_DECODE_H_