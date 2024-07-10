#ifndef _ATC_STK_COMMON_FUNC_H_
#define _ATC_STK_COMMON_FUNC_H_
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
File Name:    atc_stk_common_func.h
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
#include "sci_types.h"
#include "simat_outport.h"
#include "sim_type.h"
#include "atc_stk_enum.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define GET_TAG_VALUE(uint8_byte)           ((uint8_byte) & 0x7f)
#define IS_COMPREHESION(uint8_byte)         ((BOOLEAN)(((uint8_byte) & 0x80) == 0x80))
#define MAKE_COMPREHESION(uint8_byte)       ((uint8_byte) |= 0x80)
//check the Data object and to make there are sufficency space for a data object
#define CHECK_TLV_MIN_LEN(pdata,len) if (len < 2)\
{\
    return SIMAT_DO_LENGTH_ERROR;\
}\
else if ((2 == len) && (*(pdata + 1) != 0x0))\
{\
    return SIMAT_DO_LENGTH_ERROR;\
}\
 
#define SIM_IS_INVALIDE_FILE(file_name) ((file_name < SIM_FILE_MF) || (file_name >= SIM_FILE_TOTAL_NUM))


/********************************************************************
//  DESCRIPTION
//      Get the length of the BER-TLV data.the length rule ref to GSM
//    11.14 Annex D
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_GetTLVLength
(     //return:is the data really start with a TLV length
    uint8 *pdata,   //in:the data that the start with the length of TLV
    uint8 *offset,  //out:the length offset
    uint8 *tlv_len  //out:the length of the BER-TLV  object
);
/********************************************************************
//  DESCRIPTION
//      Set the length of the BER-TLV data.the length rule ref to GSM
//    11.14 Annex D
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
void SIMAT_SetTLVLength(
    uint8 *pdata,   //in/out:the data that in start with the TLV length
    uint8 *offset,  //out:the length of TLV length
    uint8 tlv_len   //the TLV length that will set to the BER-TLV data
);
#if 0
/**********************************************************************
//  DESCription:
//      Get SIM file name by a file id
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
 SIM_FILE_NAME_E SIMFS_GetFileName(
    uint32 cur_sim_task,
    uint16 file_id,  //the file id
    uint8  path_id_len,
    uint8  *path_id
);

/**********************************************************************
//  DESCription:
//      get the a file's type
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
SIM_FILE_TYPE_E SIMFS_GetFileType
(  //return value:the file's file type
    SIM_FILE_NAME_E file_name   //the file name
);

/**********************************************************************
//  DESCription:
//      get the a EF file's file ID
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
uint16  SIMFS_GetFileID(    //return value:the file ID
    SIM_FILE_NAME_E file_name     //the file name
);

/**********************************************************************
//  DESCription:
//      get the a file's file system level,the MF is SIM_FILE_LEVEL_E_1,
//    the child for MF is SIM_FILE_LEVEL_E_2,the child of child of MF is
//   SIM_FILE_LEVEL_E_3,and so on
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
STATIC SIM_FILE_LEVEL_E GetFileLevel(//return value:the file level the file
    uint16 file_id  //the file id
);

/**********************************************************************
//  DESCription:
//      Select a file in the SIM card file system,the file select rule
//    refference to GSM 11.11 section 6.5 "Methods for selecting a file"
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
int8 SIMFS_FileRouting( //reutrn value:the number of file in the routing to the destionation file
    SIM_FILE_NAME_E file_name,  //the desitination file
    SIM_FILE_NAME_E cur_file,   //the current selected direction file
    SIM_FILE_NAME_E *file_route //the file route to the destionation file
);
#endif
/**********************************************************************
//  DESCription:
//    Encode mcc mnc into three bytes.
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
void EncodeMNCMCCToThreeByte(
    uint16 mcc,
    uint16 mnc,
    uint16 mnc_digit_num,
    uint8 *mcc_mnc_array
);

#ifdef __cplusplus
}
#endif

#endif  //_ATC_STK_COMMON_FUNC_H_
