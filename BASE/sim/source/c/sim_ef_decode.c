/******************************************************************************
** File Name:      sim_ef_decode.c                                           *
** Author:         Raislin.Kong                                              *
** Date:           2001.8                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the c file define the fucntions decode the ef     *
**                 content                                                   *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2001.8        Raislin.Kong    Create                                      *
** 2005.6        Yongxia.zhang   CR26616,Modify DecodeEXT1-4()               *
** 2005.9        Fancier.Fan     CR30902,MNC digit number should be 2 or 3   *
******************************************************************************/
#include "base_trc.h"
#include "sim_include.h"
#include "sci_codec.h"//SCI_Unpack7bitsTo8bits
#ifdef _SUPPORT_IPC_
#include "nvitem.h"//NVITEM_ERROR_E
#include "ps_nvitem_id.h"//NV_SIM_CFG
#endif

#ifdef   __cplusplus
extern   "C"
{
#endif

//-----------------------global varialble define-----------------------

//-----------------------local function declare----------------------------
//the functions that decode the whole Transparent EF content into the
//EF mapping structure,or a recode of Fixed Linear,Cyclic file to the
//record mapping sturcture.
/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeICCID(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeDIR(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeELP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeLP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeIMSI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/*****************************************************************************/
//  Description: get HPLMN and MSIN from the imsi
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note: 3GPP TS 23.003 and 24.008
/*****************************************************************************/
LOCAL BOOLEAN GetHplmnAndMsinFromImsi(
    uint32 cur_sim_task,
    PLMN_T        *hplmn_ptr,     //OUT:
    uint8         *msin_len_ptr,  //OUT:
    uint8         *msin_ptr,      //OUT:
    const uint8   imsi_len,       //IN:
    const uint8   *imsi_value     //IN:
);

/*****************************************************************************/
//  Description: get "length of MNC in the IMSI"
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL uint8 GetMncLen(
    uint32                cur_sim_task
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeKC(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePLMNsel(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeHPLMN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeACMmax(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSST(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeACM(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeGID1(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeGID2(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePUCT(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCBMI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSPN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCBMID(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeBCCH(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeACC(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeFPLMN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeLOCI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeAD(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePHASE(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVGCS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVGCSS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVBS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVBSS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeeMLPP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeAAem(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeECC(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCBMIR(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeDCK(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCNL(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeNIA(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeKcGRPS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeLOCIGPRS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSUME(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePNN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeOPL(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMBDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMBI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMWIS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCFIS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSPDI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMMSN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeEXT8(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMMSICP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMMSUP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMMSUCP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSAI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSLL(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeADN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeFDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSMS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCCP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMSISDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSMSP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSMSS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeLND(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSMSR(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeEXT(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeBDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeIMG(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeOPNameStr(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeOPNameShtf(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCPHSInfo(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVoiceMsgWtFlag(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMailboxnum(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:King.li
//    Note:New added for CPHS extended functions
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeInformationnum(
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:King.li
//    Note:New added for CPHS extended functions
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSSTCPHS(
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCSP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCCFlag(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      The common function to decode the MCC and MNC for EFloci,EFplmnsel,
//    EFfplmn,EFlocigprs
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMCCMNC(
    const uint16 data_len,  //the length of the data to be decode
    void *data,             //in:the data buf that start with MCC and MNC information
    uint16 *ptr_mcc,        //out:the result MCC
    uint16 *pte_mnc,        //out:the reuslt MNC
    uint16 *ptr_mnc_digit_num   //out:the MNC digit num,3 or 2
);

/**********************************************************************
//    Description:
//    the function decode the EF PLMNWACT content to the corresponding file information structure.
//  According to GSM11.11 v840.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF OPLMNWACT content to the corresponding file information structure.
//  According to GSM11.11 v840.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeOPLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF HPLMNACT content to the corresponding file information structure.
//  According to GSM11.11 v840.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeHPLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF CPBCCH content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCPBCCH(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF INVSCAN content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeINVSCAN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF ECCP content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeECCP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF CMI content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCMI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF MExEST content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMEXEST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF ORPK content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeORPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF ARPK content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeARPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF TPRPK content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeTPRPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EFarr under MF/ADFusim/DFtel content to the corresponding
//file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeARR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF UST content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMUST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF EST content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMEST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EFpsloci in ADFusim
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMPSLOCI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EFecc content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMECC(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EFpbr content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMPBR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EFIMSI_M of the cdma card
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCDMAIMSI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/********************************************************************************/
//  Description: the function decode the EF EHPLMN content to the corresponding
//               file information structure.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 31.102 Rel-7
/********************************************************************************/
static SIM_SERVICE_RESULT_E DecodeEHPLMN(                           //RETURN: the result of the encode operation
    uint32 cur_sim_task,
    const uint16   data_len,   //IN: the len of the data to be decode
    void           *data,      //IN: the data to be decoded
    void           *ef_mapping //OUT: the pointer of the struct that decode out
);

/********************************************************************************/
//  Description: the function decode the EF EHPLMNPI content to the corresponding
//               file information structure.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 31.102 Rel-7
/********************************************************************************/
static SIM_SERVICE_RESULT_E DecodeEHPLMNPI(                           //RETURN: the result of the encode operation
    uint32 cur_sim_task,
    const uint16   data_len,   //IN: the len of the data to be decode
    void           *data,      //IN: the data to be decoded
    void           *ef_mapping //OUT: the pointer of the struct that decode out
);

/********************************************************************************/
//  Description: the function decode the EF SPNI or PNNI content to the
//               corresponding file information structure.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 31.102 Rel-8
/********************************************************************************/
static SIM_SERVICE_RESULT_E DecodeIconInfo(                             //RETURN: the result of the encode operation
    uint32       cur_sim_task,
    const uint16 data_len,       //IN: the len of the data to be decode
    void         *data,          //IN: the data to be decoded
    void         *ef_mapping     //OUT: the pointer of the struct that decode out
);

/********************************************************************************/
//  Description: the function decode the EFcst (CDMA service table)
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
static SIM_SERVICE_RESULT_E DecodeCST(                           //RETURN: the result of the encode operation
    uint32 cur_sim_task,
    const uint16   data_len,   //IN: the len of the data to be decode
    void           *data,      //IN: the data to be decoded
    void           *ef_mapping //OUT: the pointer of the struct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeDN(   //return value:the result of decode operation
    const uint16           data_len,       //the len of the data to be decode
    void                   *data,          //in:the data to be decoded
    SIM_RECORD_EF_INFO_T   *dn_info_ptr,
    SIM_EF_MAPPING_DN_T    *dn_data_ptr    //out:the pointer of the struct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeORANGEDYN2FLAG(
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeORANGECSPLINE2(
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeORANGEDYNFLAG(
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EFnetpar content to the corresponding file information structure.
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeNetPar(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

/**********************************************************************
//    Description:
//    the function decode the EF ACL content to the corresponding file information structure.
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeACL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
);

//the decode functin array of all Ef file
static SIM_SERVICE_RESULT_E(*s_sim_decode_functions[])(uint32, uint16, void *, void *) =
{
    NULL,//MF
    NULL,//DFgsm
    NULL,//DFdcs1800
    NULL,//DFtelecom
    NULL,//DFis41
    NULL,//DFfpcts
    NULL,//ADFusim
    NULL,//ADFusim-DFgsm
    NULL,//DFiridium
    NULL,//DFglobst
    NULL,//DFico
    NULL,//DFaces
    NULL,//DFmexe
    NULL,//DFpcs1900
    NULL,//DFcts
    NULL,//DFsolsa
    NULL,//DFtelecom-DFphonebook
    NULL,//DFtelecom-DFgraphics
    NULL,//ADFusim-DFphonebook
    NULL,//ADFusim-DFsolsa
    NULL,//ADFusim-DFmexe
    NULL,//DFcdma
    NULL,//DForange
    DecodeICCID,
    DecodeDIR,
    DecodeELP,
    DecodeARR,
    DecodeLP,
    DecodeIMSI,
    DecodeKC,
    DecodePLMNsel,
    DecodeHPLMN,
    DecodeACMmax,
    DecodeSST,
    DecodeACM,
    DecodeGID1,
    DecodeGID2,
    DecodePUCT,
    DecodeCBMI,
    DecodeSPN,
    DecodeCBMID,
    DecodeBCCH,
    DecodeACC,
    DecodeFPLMN,
    DecodeLOCI,
    DecodeAD,
    DecodePHASE,
    DecodeVGCS,
    DecodeVGCSS,
    DecodeVBS,
    DecodeVBSS,
    DecodeeMLPP,
    DecodeAAem,
    DecodeECC,
    DecodeCBMIR,
    DecodeDCK,
    DecodeCNL,
    DecodeNIA,
    DecodeKcGRPS,
    DecodeLOCIGPRS,
    DecodeSUME,
    DecodePLMNWACT,
    DecodeOPLMNWACT,
    DecodeHPLMNWACT,
    DecodeCPBCCH,
    DecodeINVSCAN,
    DecodePNN,
    DecodeOPL,
    DecodeMBDN,
    DecodeEXT,
    DecodeMBI,
    DecodeMWIS,
    DecodeCFIS,
    DecodeEXT,
    DecodeSPDI,
    DecodeMMSN,
    DecodeEXT8,
    DecodeMMSICP,
    DecodeMMSUP,
    DecodeMMSUCP,
    DecodeSAI,
    DecodeSLL,
    DecodeADN,
    DecodeFDN,
    DecodeSMS,
    DecodeCCP,
    DecodeECCP,//SIM_RELEASE99_UPDATE
    DecodeMSISDN,
    DecodeSMSP,
    DecodeSMSS,
    DecodeLND,
    DecodeSMSR,
    DecodeSDN,
    DecodeEXT,
    DecodeEXT,
    DecodeEXT,
    DecodeBDN,
    DecodeEXT,
    DecodeSUME,//SIM_FILE_EF_TELECOM_SUME
    DecodeCMI,//SIM_RELEASE99_UPDATE
    DecodeIMG,
    NULL,//IIDF
    DecodeCDMAIMSI,
    DecodeOPNameStr,
    DecodeOPNameShtf,
    DecodeCPHSInfo,
    DecodeVoiceMsgWtFlag,
    DecodeCCFlag,
    DecodeCSP,
    DecodeMailboxnum,
    DecodeInformationnum,       // SIM_FILE_EF_INFONUM
    DecodeSSTCPHS,              // SIM_FILE_EF_CPHS_SST
    DecodeMEXEST,
    DecodeORPK,
    DecodeARPK,
    DecodeTPRPK,
    DecodeARR,
    NULL,//SIM_FILE_EF_TELECOM_PB_PSC,
    NULL,//SIM_FILE_EF_TELECOM_PB_CC,
    NULL,//SIM_FILE_EF_TELECOM_PB_PUID,
    DecodeUSIMPBR,//SIM_FILE_EF_TELECOM_PB_PBR,
    NULL,//SIM_FILE_EF_TELECOM_PB_IAP,
    DecodeADN,//SIM_FILE_EF_TELECOM_PB_ADN,
    DecodeEXT,//SIM_FILE_EF_TELECOM_PB_EXT1,
    NULL,//SIM_FILE_EF_TELECOM_PB_PBC,
    NULL,//SIM_FILE_EF_TELECOM_PB_GRP,
    NULL,//SIM_FILE_EF_TELECOM_PB_AAS,
    NULL,//SIM_FILE_EF_TELECOM_PB_GAS,
    NULL,//SIM_FILE_EF_TELECOM_PB_PBC,
    NULL,//SIM_FILE_EF_TELECOM_PB_GRP,
    NULL,//SIM_FILE_EF_TELECOM_PB_AAS,
    NULL,//SIM_FILE_EF_TELECOM_PB_GAS,
    NULL,//SIM_FILE_EF_TELECOM_PB_ANR,
    NULL,//SIM_FILE_EF_TELECOM_PB_SNE,
    NULL,//SIM_FILE_EF_TELECOM_PB_CCP1,
    NULL,//SIM_FILE_EF_TELECOM_PB_UID,
    NULL,//SIM_FILE_EF_TELECOM_PB_EMAIL,
    DecodeADN,//SIM_FILE_EF_TELECOM_PB1_ADN,
    NULL,//SIM_FILE_EF_TELECOM_PB1_PBC,
    NULL,//SIM_FILE_EF_TELECOM_PB1_GRP,
    NULL,//SIM_FILE_EF_TELECOM_PB1_ANRA,
    NULL,//SIM_FILE_EF_TELECOM_PB1_ANRB,
    NULL,//SIM_FILE_EF_TELECOM_PB1_ANRC,
    NULL,//SIM_FILE_EF_TELECOM_PB1_SNE,
    NULL,//SIM_FILE_EF_TELECOM_PB1_UID,
    NULL,//SIM_FILE_EF_TELECOM_PB1_EMAILA,
    NULL,//SIM_FILE_EF_TELECOM_PB1_EMAILB,
    NULL,//SIM_FILE_EF_TELECOM_PB2_IAP,
    NULL,//SIM_FILE_EF_TELECOM_PB2_ADN,
    NULL,//SIM_FILE_EF_TELECOM_PB2_PBC,
    NULL,//SIM_FILE_EF_TELECOM_PB2_GRP,
    NULL,//SIM_FILE_EF_TELECOM_PB2_ANRA,
    NULL,//SIM_FILE_EF_TELECOM_PB2_ANRB,
    NULL,//SIM_FILE_EF_TELECOM_PB2_ANRC,
    NULL,//SIM_FILE_EF_TELECOM_PB2_SNE,
    NULL,//SIM_FILE_EF_TELECOM_PB2_UID,
    NULL,//SIM_FILE_EF_TELECOM_PB2_EMAILA,
    NULL,//SIM_FILE_EF_TELECOM_PB2_EMAILB,
    NULL,//SIM_FILE_EF_TELECOM_PB3_IAP,
    NULL,//SIM_FILE_EF_TELECOM_PB3_ADN,
    NULL,//SIM_FILE_EF_TELECOM_PB3_PBC,
    NULL,//SIM_FILE_EF_TELECOM_PB3_GRP,
    NULL,//SIM_FILE_EF_TELECOM_PB3_ANRA,
    NULL,//SIM_FILE_EF_TELECOM_PB3_ANRB,
    NULL,//SIM_FILE_EF_TELECOM_PB3_ANRC,
    NULL,//SIM_FILE_EF_TELECOM_PB3_SNE,
    NULL,//SIM_FILE_EF_TELECOM_PB3_UID,
    NULL,//SIM_FILE_EF_TELECOM_PB3_EMAILA,
    NULL,//SIM_FILE_EF_TELECOM_PB3_EMAILB,
    DecodeKC,//GSMAccess Kc
    DecodeKcGRPS,//GSMAccess Kcgprs,
    DecodeCPBCCH,//GSMAccess CPBCCH,
    DecodeINVSCAN,//GSMAccess INVSCAN,
    NULL,//SIM_FILE_EF_USIM_PB_PSC,
    NULL,//SIM_FILE_EF_USIM_PB_CC,
    NULL,//SIM_FILE_EF_USIM_PB_PUID,
    DecodeUSIMPBR,//SIM_FILE_EF_USIM_PB_PBR,
    NULL,//SIM_FILE_EF_USIM_PB_IAP,
    DecodeADN,//SIM_FILE_EF_USIM_PB_ADN,
    DecodeEXT,//SIM_FILE_EF_USIM_PB_EXT1,
    NULL,//SIM_FILE_EF_USIM_PB_PBC,
    NULL,//SIM_FILE_EF_USIM_PB_GRP,
    NULL,//SIM_FILE_EF_USIM_PB_AAS,
    NULL,//SIM_FILE_EF_USIM_PB_GAS,
    NULL,//SIM_FILE_EF_USIM_PB_ANR,
    NULL,//SIM_FILE_EF_USIM_PB_SNE,
    NULL,//SIM_FILE_EF_USIM_PB_CCP1,
    NULL,//SIM_FILE_EF_USIM_PB_UID,
    NULL,//SIM_FILE_EF_USIM_PB_EMAIL,
    NULL,//SIM_FILE_EF_USIM_PB_UID,
    NULL,//SIM_FILE_EF_USIM_PB_EMAILA,
    NULL,//SIM_FILE_EF_USIM_PB_EMAILB,
    NULL,//SIM_FILE_EF_USIM_PB1_IAP,
    DecodeADN,//SIM_FILE_EF_USIM_PB1_ADN,
    NULL,//SIM_FILE_EF_USIM_PB1_PBC,
    NULL,//SIM_FILE_EF_USIM_PB1_GRP,
    NULL,//SIM_FILE_EF_USIM_PB1_ANRA,
    NULL,//SIM_FILE_EF_USIM_PB1_ANRB,
    NULL,//SIM_FILE_EF_USIM_PB1_ANRC,
    NULL,//SIM_FILE_EF_USIM_PB1_SNE,
    NULL,//SIM_FILE_EF_USIM_PB1_UID,
    NULL,//SIM_FILE_EF_USIM_PB1_EMAILA,
    NULL,//SIM_FILE_EF_USIM_PB1_EMAILB,
    NULL,//SIM_FILE_EF_USIM_PB2_IAP,
    NULL,//SIM_FILE_EF_USIM_PB2_ADN,
    NULL,//SIM_FILE_EF_USIM_PB2_PBC,
    NULL,//SIM_FILE_EF_USIM_PB2_GRP,
    NULL,//SIM_FILE_EF_USIM_PB2_ANRA,
    NULL,//SIM_FILE_EF_USIM_PB2_ANRB,
    NULL,//SIM_FILE_EF_USIM_PB2_ANRC,
    NULL,//SIM_FILE_EF_USIM_PB2_SNE,
    NULL,//SIM_FILE_EF_USIM_PB2_UID,
    NULL,//SIM_FILE_EF_USIM_PB2_EMAILA,
    NULL,//SIM_FILE_EF_USIM_PB2_EMAILB,
    NULL,//SIM_FILE_EF_USIM_PB3_IAP,
    NULL,//SIM_FILE_EF_USIM_PB3_ADN,
    NULL,//SIM_FILE_EF_USIM_PB3_PBC,
    NULL,//SIM_FILE_EF_USIM_PB3_GRP,
    NULL,//SIM_FILE_EF_USIM_PB3_ANRA,
    NULL,//SIM_FILE_EF_USIM_PB3_ANRB,
    NULL,//SIM_FILE_EF_USIM_PB3_ANRC,
    NULL,//SIM_FILE_EF_USIM_PB3_SNE,
    NULL,//SIM_FILE_EF_USIM_PB3_UID,
    NULL,//SIM_FILE_EF_USIM_PB3_EMAILA,
    NULL,//SIM_FILE_EF_USIM_PB3_EMAILB,
    DecodeELP,//SIM_FILE_EF_USIM_LI
    DecodeARR,
    DecodeIMSI,
    NULL,
    NULL,
    DecodeVoiceMsgWtFlag,//SIM_FILE_EF_USIM_VMWFLAG,
    DecodeSSTCPHS,//SIM_FILE_EF_USIM_CPHS_SST,
    DecodeCCFlag,//SIM_FILE_EF_USIM_CFFLAG,
    DecodeOPNameStr,//SIM_FILE_EF_USIM_ONSTRING,
    DecodeCSP,//SIM_FILE_EF_USIM_CSP,
    DecodeCPHSInfo,//SIM_FILE_EF_USIM_CPHSINFO,
    DecodeMailboxnum,//SIM_FILE_EF_USIM_MBNUM,
    DecodeOPNameShtf,//SIM_FILE_EF_USIM_ONSTHORT,
    DecodeInformationnum,//SIM_FILE_EF_USIM_INFONUM,
    DecodeDCK,
    DecodeHPLMN,
    DecodeCNL,
    DecodeACMmax,
    DecodeUSIMUST,
    DecodeACM,
    DecodeFDN,//SIM_FILE_EF_USIM_FDN
    DecodeSMS,//SIM_FILE_EF_USIM_SMS
    DecodeGID1,
    DecodeGID2,
    DecodeMSISDN,//SIM_FILE_EF_USIM_MSISDN
    DecodePUCT,
    DecodeSMSP,//SIM_FILE_EF_USIM_SMSP
    DecodeSMSS,//SIM_FILE_EF_USIM_SMSS
    DecodeCBMI,
    DecodeSPN,
    DecodeSMSR,//SIM_FILE_EF_USIM_SMSR
    DecodeCBMID,
    DecodeSDN,
    DecodeEXT,//SIM_FILE_EF_USIM_EXT2
    DecodeEXT,//SIM_FILE_EF_USIM_EXT3
    DecodeBDN,//SIM_FILE_EF_USIM_BDN
    DecodeEXT,//SIM_FILE_EF_USIM_EXT5
    DecodeCBMIR,
    DecodeEXT,//SIM_FILE_EF_USIM_EXT4
    DecodeUSIMEST,
    DecodeACL,
    DecodeCMI,
    DecodePLMNWACT,
    DecodeOPLMNWACT,
    DecodeHPLMNWACT,
    DecodeUSIMPSLOCI,
    DecodeACC,
    DecodeFPLMN,
    DecodeLOCI,
    DecodeAD,
    DecodeVGCS,
    DecodeVGCSS,
    DecodeVBS,
    DecodeVBSS,
    DecodeeMLPP,
    DecodeAAem,
    DecodeUSIMECC,
    DecodeNetPar,
    DecodePNN,
    DecodeOPL,
    DecodeMBDN,
    DecodeEXT,
    DecodeMBI,
    DecodeMWIS,
    DecodeCFIS,
    DecodeEXT,
    DecodeSPDI,
    DecodeMMSN,
    DecodeEXT8,
    DecodeMMSICP,
    DecodeMMSUP,
    DecodeMMSUCP,
    DecodeNIA,
    DecodeEHPLMN,
    DecodeEHPLMNPI,
    DecodeIconInfo, //SPNI
    DecodeIconInfo, //PNNI
    DecodeSAI,
    DecodeSLL,
    DecodeMEXEST,
    DecodeORPK,
    DecodeARPK,
    DecodeTPRPK,
    DecodeCST,
    NULL,
    DecodeORANGEDYN2FLAG,
    DecodeORANGECSPLINE2,
    NULL,
    DecodeACM,
    DecodeORANGEDYNFLAG
};

/**********************************************************************
//    Description:
//      the function that dispatch the decode function for differenct
//    EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMFS_DecodeEFFile(    //return value:the result of the operation
    uint32 cur_sim_task,
    const SIM_FILE_NAME_E file_name,    //the file that the decode data come from
    const uint16 data_len,  //the decode data length
    void *data,             //in:the decode data
    void *ef_mapping        //out:the file structure decode to
)
{

    //the file must be a valid file name
    if(SIM_IS_INVALIDE_FILE(file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //select the corresponding decode function to decode the ef mapping
    if(NULL == s_sim_decode_functions[file_name])
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }
    else
    {
        SCI_ASSERT(NULL != data);            /*assert verified: Null_pointer*/
        SCI_ASSERT(NULL != ef_mapping); /*assert verified: Null_pointer*/
        return s_sim_decode_functions[file_name](cur_sim_task, data_len, data, ef_mapping);
    }
}



/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeICCID(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_ICCID_T *iccid = (SIM_EF_MAPPING_ICCID_T *)ef_mapping;

    if(SIM_EF_ICCID_ID_NUM_LEN != data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMCPY(iccid->id_num, data, SIM_EF_ICCID_ID_NUM_LEN);

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeDIR(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E result = SIM_SERVICE_OK;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_DIR_T *dir = (SIM_EF_MAPPING_DIR_T *)ef_mapping;
    uint8 empty_data[255] = {0};

    if(0x61 == *ptemp)
    {
        dir->adf_tag = *ptemp;
        ptemp++;
        dir->adf_len = *ptemp;
        ptemp++;
        dir->aid_tag = *ptemp;

        if(0x4F == dir->aid_tag)
        {
            ptemp++;
            dir->aid_len = *ptemp;

            if(dir->aid_len > 16)
            {
                dir->aid_len = 16;
            }
            else if(dir->aid_len < 12)
            {
                result = SIM_SERVICE_DECODE_EF_ERROR;
                return result;
            }

            ptemp++;

            //ETSI 101.220 4.1 RID and Annex.E
            if ((0xA0 == *ptemp) && (0x00 == *(ptemp+1)) 
                && (0x00 == *(ptemp+2)) && (0x00 == *(ptemp+3)) 
                && (0x87 == *(ptemp+4)))//for the 3GPP
            {
                //ETSI 101.220 4.2 PIX and Annex.E
                if ((0x10 == *(ptemp+5)) && (0x02 == *(ptemp+6))) 
                {
                    SCI_MEMCPY(dir->aid_value, ptemp, dir->aid_len);
                    result = SIM_SERVICE_OK;
                }
                else
                {
                    result = SIM_SERVICE_DECODE_EF_ERROR;
                }
            }
            else
            {
                result = SIM_SERVICE_DECODE_EF_ERROR;
            }
        }
        else
        {
            result = SIM_SERVICE_DECODE_EF_ERROR;
        }
    }
    else
    {
        SCI_MEMSET(empty_data, 0xFF, 255); //set as all 0xFF

        if(0 == memcmp(empty_data, ptemp, data_len))
        {
            result = SIM_SERVICE_DECODE_EMPTY_RECORD;
        }
        else
        {
            result = SIM_SERVICE_DECODE_EF_ERROR;
        }
    }

    return result;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeELP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint16 length = data_len;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ELP_T *elp = ef_mapping;

    elp->valid_num = 0;
    elp->max_num = (uint8)(length / 2);

    if(elp->max_num > SIM_MAX_VALIDATE_LANGUAGE_NUM)
    {
        elp->max_num = SIM_MAX_VALIDATE_LANGUAGE_NUM;
    }

    while((length >= 2) && (elp->valid_num < SIM_MAX_VALIDATE_LANGUAGE_NUM))
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM%d:DecodeELP 0x%x 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_1826_112_2_17_22_58_28_4377, (uint8 *)"ddd", cur_sim_task, *ptemp, *(ptemp + 1));
#endif

        if((0xff == *ptemp) && (0xff == *(ptemp + 1)))
        {
            break;
        }

        SCI_MEMCPY(&(elp->lang_codes[elp->valid_num]), ptemp, 2);
        ptemp += 2;
        length -= 2;
        elp->valid_num++;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeLP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint16 length = data_len;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_LP_T *lp = ef_mapping;

    lp->valid_num = 0;
    lp->max_num = (uint8)length;

    if(lp->max_num > SIM_MAX_VALIDATE_LANGUAGE_NUM)
    {
        lp->max_num = SIM_MAX_VALIDATE_LANGUAGE_NUM;
    }

    while((length > 0) && (lp->valid_num < SIM_MAX_VALIDATE_LANGUAGE_NUM))
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM%d:DecodeLP 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_1871_112_2_17_22_58_28_4378, (uint8 *)"dd", cur_sim_task, *ptemp);
#endif

        if(0xff == *ptemp)
        {
            break;
        }

        lp->lang_codes[lp->valid_num] = *ptemp;
        ptemp += 1;
        length -= 1;
        lp->valid_num++;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeIMSI(                         //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,   //the len of the data to be decode
    void         *data,      //in:the data to be decoded
    void         *ef_mapping //out:the pointer of the struct that decode out
)
{
    uint8                   len             = 0;
    uint8                   *ptemp          = data;
    SIM_EF_MAPPING_IMSI_T   *imsi           = ef_mapping;
    uint8                   imsi_is_ff_buf[9];
#ifdef _SUPPORT_IPC_
    MULTIPS_CREATE_PARAM_T multips_param;
#endif

    //the imsi file length should be 9 byte length
    if(9 > data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMSET(imsi_is_ff_buf, 0xFF, 9);

    if(0 == memcmp(imsi_is_ff_buf, ptemp, 9))
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::The IMSI contains 8 bytes of 0xFF."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_1914_112_2_17_22_58_28_4379, (uint8 *)"");
#endif

#ifdef _SUPPORT_IPC_//imsi replace
        if (NVERR_NONE == EFS_NvitemRead(NV_SIM_CFG, (uint16)sizeof(MULTIPS_CREATE_PARAM_T), (uint8*)&multips_param))
        {
            if (1 == multips_param.reserve1)
            {
                imsi->imsi.imsi_value[0] = 0x09;
                imsi->imsi.imsi_value[1] = 0x10;
                imsi->imsi.imsi_value[2] = 0x10;
                imsi->imsi.imsi_value[3] = 0x00;
                imsi->imsi.imsi_value[4] = 0x00;
                imsi->imsi.imsi_value[5] = 0x00;
                imsi->imsi.imsi_value[6] = 0x00;
                imsi->imsi.imsi_value[7] = 0x00;
                imsi->imsi.length = 4;
                imsi->imsi.valid_flag = TRUE;
                SCI_TRACE_LOW("SIM Decode Imsi. imsi replace");
            }
        }
#else
        SCI_MEMSET(imsi->imsi.imsi_value, 0XFF, 8);
        imsi->imsi.length     = 8;
        imsi->imsi.valid_flag = FALSE;
#endif
        return SIM_SERVICE_OK;
    }

#ifdef GSM_CUSTOMER_AFP_SUPPORT
    SCI_MEMSET(imsi_is_ff_buf, 0x00, 9);

    if(0 == memcmp(imsi_is_ff_buf, ptemp, 9))
    {
#ifndef _LESSEN_TRACE_SUPPORT
        SCI_TRACE_LOW("SIM::The IMSI contains 8 bytes of 0x00.");
#endif
        SCI_MEMSET(imsi->imsi.imsi_value, 0X00, 8);
        imsi->imsi.length     = 8;
        imsi->imsi.valid_flag = FALSE;
        return SIM_SERVICE_OK;
    }

#endif

    len = *ptemp;

    //the IMSI byte should not exceed 8
    if(len > 8)
    {
		SCI_TRACE_LOW("SIM:warning! the imsi length is %d.",len);
		len = 8;
        //return SIM_SERVICE_DECODE_EF_ERROR;
    }

    ptemp++;

#ifdef GSM_CUSTOMER_AFP_SUPPORT

    if((0xFF == (*(ptemp + 1))) && (0xFF == (*(ptemp + 2))) && (0xFF == (*(ptemp + 3))))
    {
        //SCI_TRACE_LOW:"SIM::ss imsi The MCC&MNC are 0xFF."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_1935_112_2_17_22_58_28_4380, (uint8 *)"");
        SCI_MEMSET(imsi->imsi.imsi_value, 0XFF, 8);
        imsi->imsi.length     = len;
        imsi->imsi.valid_flag = FALSE;
        return SIM_SERVICE_OK;
    }
    else if((0x00 == (*(ptemp + 1))) && (0x00 == (*(ptemp + 2))) && (0x00 == (*(ptemp + 3))))
    {
        SCI_TRACE_LOW("SIM::imsi The MCC&MNC are 0x00.");
        SCI_MEMSET(imsi->imsi.imsi_value, 0X00, 8);
        imsi->imsi.length     = len;
        imsi->imsi.valid_flag = FALSE;
        return SIM_SERVICE_OK;
    }

#endif

    if(GetHplmnAndMsinFromImsi(cur_sim_task, &(imsi->hplmn), &(imsi->msin_digit_num), imsi->msin, len, ptemp))
    {
        imsi->mcc = imsi->hplmn.mcc;
        imsi->mnc = imsi->hplmn.mnc;

        //copy the imsi
        SCI_MEMCPY(imsi->imsi.imsi_value, ((uint8 *)data + 1), 8);
        imsi->imsi.length = len;

        imsi->imsi.valid_flag = TRUE;
        return SIM_SERVICE_OK;
    }
    else
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }
}

// North America PCS1900
#define NA_MCC_LOW       310
#define NA_MCC_HIGH      316

/*****************************************************************************/
//  Description: get HPLMN and MSIN from the IMSI
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note: 3GPP TS 23.003, 23.122 and 24.008
/*****************************************************************************/
LOCAL BOOLEAN GetHplmnAndMsinFromImsi(
    uint32 cur_sim_task,
    PLMN_T        *hplmn_ptr,     //OUT:
    uint8         *msin_len_ptr,  //OUT:
    uint8         *msin_ptr,      //OUT:
    const uint8   imsi_len,       //IN:
    const uint8   *imsi_value     //IN:
)
{
    uint8   i           = 0;
    uint8   mnc_len     = GetMncLen(cur_sim_task);
    uint8   msin_len    = 0;
    uint8   bcd_num     = 0;
    uint8   bcd_str[32] = {0};

    SCI_MEMSET(hplmn_ptr, 0, sizeof(PLMN_T));
    SCI_MEMSET(msin_ptr,  0, MAX_MSIN_BYTE_NUM);

    for(i = 0; i < imsi_len; i++)
    {
        bcd_str[bcd_num] = (imsi_value[i] & 0x0f);
        bcd_num++;
        bcd_str[bcd_num] = (imsi_value[i] & 0xf0) >> 4;
        bcd_num++;
    }

    if((0x01 == bcd_str[0]) && (0x0f == bcd_str[bcd_num-1]))
    {
        bcd_num--;
    }
    else if((0x09 == bcd_str[0]) && (0x0f != bcd_str[bcd_num-1]))
    {

    }
    else
    {
        SCI_TRACE_LOW("SIM%d:bcd_str[0]=0x%x error,imsi(1-8)=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x.",cur_sim_task,bcd_str[0],
        imsi_value[0],imsi_value[1],imsi_value[2],imsi_value[3],imsi_value[4],imsi_value[5],imsi_value[6],imsi_value[7]);
//#if defined (_SUPPORT_IPC_) || defined (GSM_CUSTOMER_AFP_SUPPORT)
        //do nothing now
//#else
        //return (FALSE);
//#endif
    }

    // MCC
    for(i = 1; i < 4; i++)
    {
        if(bcd_str[i] <= 0x09)
        {
            hplmn_ptr->mcc = (hplmn_ptr->mcc) * 10 + bcd_str[i];
        }
        else
        {
            SCI_TRACE_LOW("SIM%d:mcc error,imsi(1-8)=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x.",cur_sim_task,
            imsi_value[0],imsi_value[1],imsi_value[2],imsi_value[3],imsi_value[4],imsi_value[5],imsi_value[6],imsi_value[7]);
//#if defined (_SUPPORT_IPC_) || defined (GSM_CUSTOMER_AFP_SUPPORT)
            //do nothing now
//#else
            //return (FALSE);
//#endif
        }
    }

    // default value
    hplmn_ptr->mnc_digit_num = 2;

    // MNC
    // first two digits
    for(i = 4; i < 6; i++)
    {
        if(bcd_str[i] <= 0x09)
        {
            hplmn_ptr->mnc = (hplmn_ptr->mnc) * 10 + bcd_str[i];
        }
        else
        {
            SCI_TRACE_LOW("SIM%d:mnc error,imsi(1-8)=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x.",cur_sim_task,
            imsi_value[0],imsi_value[1],imsi_value[2],imsi_value[3],imsi_value[4],imsi_value[5],imsi_value[6],imsi_value[7]);
//#if defined (_SUPPORT_IPC_) || defined (GSM_CUSTOMER_AFP_SUPPORT)
            //do nothing now
//#else
            //return (FALSE);
//#endif
        }
    }

    // the third digit
    if((3 == mnc_len)
        || ((NA_MCC_LOW <= hplmn_ptr->mcc) && (hplmn_ptr->mcc <= NA_MCC_HIGH)))
    {
        mnc_len = 3;

        if(bcd_str[6] <= 0x09)
        {
            hplmn_ptr->mnc           = (hplmn_ptr->mnc) * 10 + bcd_str[6];
            hplmn_ptr->mnc_digit_num = 3;
        }
        else
        {
            SCI_TRACE_LOW("SIM%d:3rd mnc error,imsi(1-8)=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x.",cur_sim_task,
            imsi_value[0],imsi_value[1],imsi_value[2],imsi_value[3],imsi_value[4],imsi_value[5],imsi_value[6],imsi_value[7]);
//#if defined (_SUPPORT_IPC_) || defined (GSM_CUSTOMER_AFP_SUPPORT)
            //do nothing now
//#else
            //return (FALSE);
//#endif
        }
    }

    // MSIN
    for(i = 4 + mnc_len; i < MIN(30, bcd_num); i += 2)
    {
        msin_ptr[msin_len]  = (bcd_str[i] & 0x0f) << 4;

        if(i < (uint8)(bcd_num - 1))
        {
            msin_ptr[msin_len] += (bcd_str[i+1] & 0x0f);
        }

        msin_len++;
    }

    // digit num
    *msin_len_ptr = (uint8)(bcd_num - 4 - mnc_len);

    return (TRUE);
}

/*****************************************************************************/
//  Description: get "length of MNC in the IMSI"
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL uint8 GetMncLen(
    uint32                cur_sim_task
)
{
    SIM_MNC_LEN_INFO_T  *mnc_info_ptr = &(g_sim_card_setting[cur_sim_task].mnc_len_info);
    uint8               mnc_digit_num = 2;

    if(mnc_info_ptr->mnc_len_exist)
    {
        mnc_digit_num = mnc_info_ptr->mnc_digit_num;
    }

    return (mnc_digit_num);
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeKC(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_KC_T *kc = ef_mapping;

    if(9 != data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMCPY(kc->kc, data, 8);
    kc->cksn = ((*((uint8 *)data + 8)) & 0x07);

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePLMNsel(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint16 i, mcc, mnc, mnc_digit_num;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_PLMNSEL_T *plmn_list = ef_mapping;
    uint8 *ptemp = data;

    plmn_list->validate_num = 0;
    plmn_list->max_num = data_len / SIM_EF_PLMNSEL_ARR_SIZE;

    if(plmn_list->max_num > SIM_EF_PLMNSEL_ARR_NUM)
    {
        plmn_list->max_num = SIM_EF_PLMNSEL_ARR_NUM;
    }

    for(i = 0; i < plmn_list->max_num; i++)
    {
        mcc = 0;
        mnc = 0;
        mnc_digit_num = 0;

        if(((0xFF == ptemp[0]) && (0xFF == ptemp[1]) && (0xFF == ptemp[2]))
            || ((0 == ptemp[0]) && (0 == ptemp[1]) && (0 == ptemp[2])))
        {
            ptemp += 3;
            continue;
        }

#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM%d:DecodePLMNsel i=%d, 0x%x 0x%x 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2158_112_2_17_22_58_28_4381, (uint8 *)"ddddd", cur_sim_task, i, *ptemp, *(ptemp + 1), *(ptemp + 2));
#endif

        ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }

        plmn_list->plmn_arr[plmn_list->validate_num].mnc = mnc;
        plmn_list->plmn_arr[plmn_list->validate_num].mcc = mcc;
        plmn_list->plmn_arr[plmn_list->validate_num].mnc_digit_num = mnc_digit_num;
        ptemp += 3;
        plmn_list->validate_num++;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeHPLMN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    HPLMN_TIMER_VALUE_T *time_int = ef_mapping;

    if(data_len != 1)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    *time_int = *ptemp;

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeACMmax(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ACMMAX_T *acm_max = ef_mapping;

    if(data_len < 3)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFacmmax is less than 3."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2225_112_2_17_22_58_28_4382, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    acm_max->acm_max_value = ((uint32)(*ptemp) << 16) + ((uint32)(*(ptemp + 1)) << 8) + *(ptemp + 2);
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSST(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    int i = 0;
    uint8 *ptemp = data;
    uint8 max_service_num;
    SIM_EF_MAPPING_SST_T *sst = ef_mapping;

    //set all the sst to zero.that mean the service that do not pointer out in the
    //file content is set to not allocated and not actived.
    SCI_MEMSET(sst, 0x0, sizeof(SIM_EF_MAPPING_SST_T));

    if(data_len < 2)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFsst is less than 2."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2261_112_2_17_22_58_28_4383, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

#ifndef SIM_TEST_IN_WIN32

    if(SIM_PH_1 == g_sim_card_setting[cur_sim_task].card_phase)
    {
        max_service_num = SIMSER_PH1_NUM;
    }
    else
    {
        max_service_num = SIMSER_PH2_2G_SST_END_NUM;
    }

#else
    max_service_num = SIMSER_PH2_2G_SST_END_NUM;
#endif

    //SCI_TRACE_LOW:"SIM%d SST len %d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2281_112_2_17_22_58_28_4384, (uint8 *)"ddddddddddddddddd", cur_sim_task, data_len, (*ptemp), (*(ptemp + 1)), (*(ptemp + 2)), (*(ptemp + 3)), (*(ptemp + 4)), (*(ptemp + 5)), (*(ptemp + 6)), (*(ptemp + 7)), (*(ptemp + 8)), (*(ptemp + 9)), (*(ptemp + 10)), (*(ptemp + 11)), (*(ptemp + 12)), (*(ptemp + 13)), (*(ptemp + 14)));

    sst->service_num = 0;

    for(i = 0; i < 4 * data_len;)
    {
        //decode the first service status in a byte
        sst->service_table[i].allocated = (BOOLEAN)((*ptemp & 0x01) != 0);
        sst->service_table[i].activated = (BOOLEAN)((*ptemp & 0x02) != 0);
        sst->service_num++;
        i++;

        if(i >= max_service_num)
        {
            return SIM_SERVICE_OK;
        }

        //decode the second service status in a byte
        sst->service_table[i].allocated = (BOOLEAN)((*ptemp & 0x04) != 0);
        sst->service_table[i].activated = (BOOLEAN)((*ptemp & 0x08) != 0);
        sst->service_num++;
        i++;

        if(i >= max_service_num)
        {
            return SIM_SERVICE_OK;
        }

        //decode the third service status in a byte
        sst->service_table[i].allocated = (BOOLEAN)((*ptemp & 0x10) != 0);
        sst->service_table[i].activated = (BOOLEAN)((*ptemp & 0x20) != 0);
        sst->service_num++;
        i++;

        if(i >= max_service_num)
        {
            return SIM_SERVICE_OK;
        }

        //decode the fourth service status in a byte
        sst->service_table[i].allocated = (BOOLEAN)((*ptemp & 0x40) != 0);
        sst->service_table[i].activated = (BOOLEAN)((*ptemp & 0x80) != 0);
        sst->service_num++;
        i++;

        if(i >= max_service_num)
        {
            return SIM_SERVICE_OK;
        }

        ptemp += 1;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeACM(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ACM_T *acm = ef_mapping;

    if(data_len < 3)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFacm is less than 3."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2350_112_2_17_22_58_29_4385, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    acm->acm_value = ((uint32)(*ptemp) << 16) +
                     ((uint32)(*(ptemp + 1)) << 8) +
                     *(ptemp + 2);
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeGID1(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_GID_T *gid = ef_mapping;

    //check length of the GID
    gid->len = (uint8)data_len;

    if(data_len > SIM_MAX_GID_LEN)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of the GID1 is out of range."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2383_112_2_17_22_58_29_4386, (uint8 *)"");
#endif
        gid->len = SIM_MAX_GID_LEN;
    }

    SCI_MEMCPY(gid->gid, data, gid->len);

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeGID2(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_GID_T *gid = ef_mapping;

    //check length of the GID
    gid->len = (uint8)data_len;

    if(data_len > SIM_MAX_GID_LEN)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of the GID2 is out of range."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2415_112_2_17_22_58_29_4387, (uint8 *)"");
#endif
        gid->len = SIM_MAX_GID_LEN;
    }

    SCI_MEMCPY(gid->gid, data, gid->len);

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePUCT(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_PUCT_T *puct = ef_mapping;

    if(data_len < 5)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFpuct is less than 5."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2446_112_2_17_22_58_29_4388, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //get the currency code
    SCI_MEMCPY(puct->cur_code.cur_code, ptemp, 3);
    ptemp += 3;

    //get the Price per unit
    puct->eppu.eppu = (((uint16)(*ptemp)) << 4) + (*(ptemp + 1) & SIM_EF_PUCT_EPPU_MARK);
    ptemp += 1;
    puct->eppu.ex = ((*ptemp) & SIM_EF_PUCT_EX_MARK) >> 5;
    puct->eppu.ex_sign = (BOOLEAN)((*ptemp & SIM_EF_PUCT_EX_SIGN_MARK) == 0);

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCBMI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint16 num, i;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CBMI_T *cbmi  = ef_mapping;

    SCI_MEMSET(cbmi, 0xff, sizeof(SIM_EF_MAPPING_CBMI_T));
    cbmi->validate_num = 0;
    num = data_len / SIM_EF_CBMI_CB_SIZE;
    num = num > SIM_MAX_CBMID_CBMI_NUM ? SIM_MAX_CBMID_CBMI_NUM : num;

    for(i = 0; i < num; i++)
    {
        if((0xff == *ptemp) &&
            (0xff == *(ptemp + 1)))
        {
            ptemp += SIM_EF_CBMI_CB_SIZE;
            continue;
        }

        SCI_MEMCPY(&(cbmi->cbmis[i]), ptemp, SIM_EF_CBMI_CB_SIZE);
        ptemp += SIM_EF_CBMI_CB_SIZE;
        cbmi->validate_num++;
    }

    return SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSPN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_SPN_T *pspn = ef_mapping;

    if(data_len != 17)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning the len of the EFspn is %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2525_112_2_17_22_58_29_4389, (uint8 *)"d", data_len);
#endif
        //return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //get the display condition
    pspn->display_condition = (*ptemp);
    ptemp += 1;

    //get the service provide name
    SCI_MEMCPY(pspn->spn, ptemp, SIM_EF_SPN_SPN_LEN);
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCBMID(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint16 num, i;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CBMID_T *cbmid  = ef_mapping;

    SCI_MEMSET(cbmid, 0xff, sizeof(SIM_EF_MAPPING_CBMID_T));
    num = data_len / SIM_EF_CBMID_CB_SIZE;
    num = num > SIM_MAX_CBMID_CBMI_NUM ? SIM_MAX_CBMID_CBMI_NUM : num;
    cbmid->validate_num = 0;

    for(i = 0; i < num; i++)
    {
        if((0xff == *ptemp) &&
            (0xff == *(ptemp + 1)))
        {
            ptemp += SIM_EF_CBMID_CB_SIZE;
            continue;
        }

        SCI_MEMCPY(&(cbmid->cbmi_for_dowload[i]), ptemp, SIM_EF_CBMID_CB_SIZE);
        ptemp += SIM_EF_CBMID_CB_SIZE;
        cbmid->validate_num++;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeBCCH(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_BCCH_T *bcch = ef_mapping;

    if(data_len < 16)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFbcch is less than 16."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2599_112_2_17_22_58_29_4390, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    bcch->is_usim = FALSE;
    SCI_MEMSET(&bcch->usim_netpar,0x0,sizeof(bcch->usim_netpar));

    SCI_MEMCPY(&bcch->ba_list, ptemp, 16);
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeACC(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    ACC_CONTROL_CLASS_T *acc = ef_mapping;

    if(data_len < 2)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFpuct is less than 2."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2629_112_2_17_22_58_29_4391, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    *acc = (((uint16) * ptemp) << 8) + *(ptemp + 1);

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeFPLMN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    int32                   i           = 0;
    SIM_SERVICE_RESULT_E    ser_result  = SIM_SERVICE_OK;
    uint16                  mcc         = 0;
    uint16                  mnc         = 0;
    uint16                  mnc_digit_num = 0;
    uint16                  max_num     = 0;
    FPLMN_LIST_T            *fplmn_list = ef_mapping;
    uint8                   *ptemp      = data;

    max_num = data_len / 3;

    SCI_MEMSET(fplmn_list, 0x0, sizeof(FPLMN_LIST_T));

    fplmn_list->max_num = max_num;

    for(i = 0; i < max_num; i++)
    {
        mcc             = 0;
        mnc             = 0;
        mnc_digit_num   = 0;

        if((0xFF == ptemp[0])
            && (0xFF == ptemp[1])
            && (0xFF == ptemp[2]))
        {
            ptemp += 3;
            fplmn_list->fplmn_arr[i].present = FALSE;
            continue;
        }

        ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

        if(SIM_SERVICE_OK == ser_result)
        {
            fplmn_list->fplmn_arr[i].present            = TRUE;
            fplmn_list->fplmn_arr[i].fplmn.mcc          = mcc;
            fplmn_list->fplmn_arr[i].fplmn.mnc          = mnc;
            fplmn_list->fplmn_arr[i].fplmn.mnc_digit_num = mnc_digit_num;
            fplmn_list->length++;
            ptemp += 3;
        }
        else
        {
            break;
        }
    }

    //SCI_TRACE_LOW:"SIM: DecodeFPLMN data_len = %d, max_num = %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2700_112_2_17_22_58_29_4392, (uint8 *)"dd", data_len, max_num);
    return (ser_result);
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeLOCI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    uint16 mnc_digit_num = 0;
    uint8 *ptemp = data, digit;
    SIM_EF_MAPPING_LOCI_T *loci = ef_mapping;

    if(data_len != 11)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFloci is not 11."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2727_112_2_17_22_58_29_4393, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    loci->tmsi.valid_flag = TRUE;
    SCI_MEMCPY(loci->tmsi.tmsi_value, ptemp, 4);

    if((0xff == loci->tmsi.tmsi_value[0]) &&
        (0xff == loci->tmsi.tmsi_value[1]) &&
        (0xff == loci->tmsi.tmsi_value[2]) &&
        (0xff == loci->tmsi.tmsi_value[3]))
    {
        loci->tmsi.valid_flag = FALSE;
    }

    ptemp += 4;

    loci->lai.mcc = 0;
    loci->lai.mnc = 0;
    //share_cr7116,begin
    loci->lai.mnc_digit_num = 0;
    //cr7116,end

    if((0xFF == ptemp[0]) &&
        (0xFF == ptemp[1]) &&
        (0xFF == ptemp[2]))
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::DecodeLOCI mcc-mnc is 0xff 0xff 0xff."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2754_112_2_17_22_58_29_4394, (uint8 *)"");
#endif
        loci->lai.mnc_digit_num = 0;
    }
    else
    {
        ser_result = DecodeMCCMNC(3, ptemp, &(loci->lai.mcc), &(loci->lai.mnc), &mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }

        loci->lai.mnc_digit_num = mnc_digit_num;
    }

    ptemp += 3;

    loci->lai.lac = (((uint16) * ptemp) << 8) + *(ptemp + 1);
    ptemp += 2;

    loci->tmsi_time = *ptemp;
    ptemp += 1;

    digit = *ptemp & SIM_EF_LOCI_UPDATE_STATUS_MARK;

    switch(digit)
    {
        case 0x00:
            loci->update_status = MM_UPDATED;
            break;
        case 0x01:
            loci->update_status = MM_NOT_UPDATED;
            break;
        case 0x02:
            loci->update_status = MM_PLMN_NOT_ALLOWED;
            break;
        case 0x03:
            loci->update_status = MM_LAI_NOT_ALLOWED;
            break;
        case 0x07:
            loci->update_status = MM_REVERSE_STATE;
            break;
        default:
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::DecodeLOCI update_status byte is 0x%x and is invalid."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2796_112_2_17_22_58_30_4395, (uint8 *)"d", digit);
#endif
            loci->update_status = MM_REVERSE_STATE;
            break;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeAD(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of the struct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_AD_T *ad = ef_mapping;
    uint8 operation_mode = 0;

    if(data_len < 3)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFad is less than 3."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2827_112_2_17_22_58_30_4396, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    ad->is_ad_exist = TRUE;
    operation_mode = *ptemp;

    switch(operation_mode)
    {
        case SIM_MS_NORMAL_OPERATION:
            ad->operation_mode = SIM_MS_NORMAL_OPERATION;
            break;
        case SIM_MS_TYPE_APPROVAL_OPERATION:
            ad->operation_mode = SIM_MS_TYPE_APPROVAL_OPERATION;
            break;
        case SIM_MS_NORMAL_SPECIFIC_FACILITIES:
            ad->operation_mode = SIM_MS_NORMAL_SPECIFIC_FACILITIES;
            break;
        case SIM_MS_TYPE_APPROVAL_SPECIFIC_FACILITIES:
            ad->operation_mode = SIM_MS_TYPE_APPROVAL_SPECIFIC_FACILITIES;
            break;
        case SIM_MS_MAINTENANCE:
            ad->operation_mode = SIM_MS_MAINTENANCE;
            break;
        case SIM_CELL_TEST:
            ad->operation_mode = SIM_CELL_TEST;
            break;
        default:
            ad->operation_mode = SIM_MS_MODE_RFU;
            break;
    }

    ptemp += 2;

    if((SIM_MS_NORMAL_SPECIFIC_FACILITIES == operation_mode)
        || (SIM_MS_TYPE_APPROVAL_SPECIFIC_FACILITIES == operation_mode))
    {
        ad->ofm_actived = (BOOLEAN)(((*ptemp) & SIM_EF_AD_OFM_ENABLE_MARK) != 0);
    }
    else
    {
        ad->ofm_actived = FALSE;
    }

    if(data_len >= 4)
    {
        ptemp++;

        if((2 == *ptemp) || (3 == *ptemp))
        {
            // length of MNC in the IMSI
            ad->mnc_digit_num = *ptemp;
            ad->mnc_len_exist = TRUE;
        }
        else
        {
            ad->mnc_len_exist = FALSE;
            ad->mnc_digit_num = 2;
        }
    }
    else
    {
        ad->mnc_len_exist = FALSE;
        ad->mnc_digit_num = 2;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePHASE(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_PHASE_T *phase = ef_mapping;

    if(data_len < 1)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFphase is less than 1."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_2914_112_2_17_22_58_30_4397, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    switch(*ptemp)
    {
        case 0x00:
        case 0x01:
            phase->phase = SIM_PH_1;
            break;
        case 0x02:
            phase->phase = SIM_PH_2;
            break;
        case 0x03:
        default:
            phase->phase = SIM_PH_2_PD;
            break;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVGCS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_VGCS_T *vgcs = ef_mapping;
    uint16 i = 0;
    uint16 vsGidCount = data_len / SIM_EF_VGCS_GID_SIZE;

    SCI_MEMSET(vgcs, 0xff, sizeof(SIM_EF_MAPPING_VGCS_T));
    vgcs->validate_num = 0;
    vsGidCount = vsGidCount > SIM_EF_VGCS_VGI_NUM ? SIM_EF_VGCS_VGI_NUM : vsGidCount;

    for(i = 0; i < vsGidCount; i++)
    {
        if((*ptemp == 0xff) && (*(ptemp + 1) == 0xff)
            && (*(ptemp + 2) == 0xff) && (*(ptemp + 3) == 0xff))
        {
            ptemp += SIM_EF_VGCS_GID_SIZE;
            continue;
        }

        SCI_MEMCPY(&(vgcs->bgis[i]), ptemp, SIM_EF_VGCS_GID_SIZE);
        ptemp += SIM_EF_VGCS_GID_SIZE;
        vgcs->validate_num++;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVGCSS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_VGCSS_T *vgcss = ef_mapping;
    uint16 vsGidCount = 0;
    uint8 bitCount = 0;
    uint8 byteCount = 0;
    uint8 newGid = *ptemp;

    SCI_MEMSET(vgcss, 0x0, sizeof(SIM_EF_MAPPING_VGCSS_T));

    for(vsGidCount = 0; vsGidCount < SIM_EF_VGCS_VGI_NUM; vsGidCount++)
    {
        if(newGid & 0x01)
        {
            vgcss->actived[vsGidCount] = TRUE;//set activation flag of Group ID = activated
        }

        newGid >>= 1;//Position next GID in bottom bit of the byte
        bitCount++;

        if(bitCount == 8)
        {
            bitCount = 0;//Move to next byte in VGCSS file
            byteCount++;
            newGid = ptemp[byteCount];
        }
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVBS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_VBS_T *vbs = ef_mapping;
    uint16 bsGidCount = data_len / SIM_EF_VBS_GID_SIZE;
    uint16 i = 0;

    SCI_MEMSET(vbs, 0xff, sizeof(SIM_EF_MAPPING_VBS_T));
    vbs->validate_num = 0;
    bsGidCount = bsGidCount > SIM_EF_VBS_VBI_NUM ? SIM_EF_VBS_VBI_NUM : bsGidCount;

    for(i = 0; i < bsGidCount; i++)
    {
        if((*ptemp == 0xff) && (*(ptemp + 1) == 0xff)
            && (*(ptemp + 2) == 0xff) && (*(ptemp + 3) == 0xff))
        {
            ptemp += SIM_EF_VBS_GID_SIZE;
            continue;
        }

        SCI_MEMCPY(&(vbs->vbis[i]), ptemp, SIM_EF_VBS_GID_SIZE);
        ptemp += SIM_EF_VBS_GID_SIZE;
        vbs->validate_num++;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVBSS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_VBSS_T *vbss = ef_mapping;
    uint16 vbsGidCount = 0;
    uint8 bitCount = 0;
    uint8 byteCount = 0;
    uint8 newGid = *ptemp;

    SCI_MEMSET(vbss, 0x0, sizeof(SIM_EF_MAPPING_VBSS_T));

    for(vbsGidCount = 0; vbsGidCount < SIM_EF_VBS_VBI_NUM; vbsGidCount++)
    {
        if(newGid & 0x01)
        {
            vbss->avtived[vbsGidCount] = TRUE;//set activation flag of Group ID = activated
        }

        newGid >>= 1;//Position next GID in bottom bit of the byte
        bitCount++;

        if(bitCount == 8)
        {
            bitCount = 0;//Move to next byte in VBSS file
            byteCount++;
            newGid = ptemp[byteCount];
        }
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeeMLPP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_EMLPP_T *emlpp = ef_mapping;
    uint8 *ptemp = data;
    uint8 newPriority = *ptemp;
    uint8 newFastCall = *(ptemp + 1);

    SCI_MEMSET(emlpp, 0, sizeof(SIM_EF_MAPPING_EMLPP_T));

    //Decode Priority levels
    if(newPriority & 0x1)
    {
        emlpp->priority_level.priority_l_A = TRUE;
    }

    if(newPriority & 0x2)
    {
        emlpp->priority_level.priority_l_B = TRUE;
    }

    if(newPriority & 0x4)
    {
        emlpp->priority_level.priority_l_0 = TRUE;
    }

    if(newPriority & 0x8)
    {
        emlpp->priority_level.priority_l_1 = TRUE;
    }

    if(newPriority & 0x10)
    {
        emlpp->priority_level.priority_l_2 = TRUE;
    }

    if(newPriority & 0x20)
    {
        emlpp->priority_level.priority_l_3 = TRUE;
    }

    if(newPriority & 0x40)
    {
        emlpp->priority_level.priority_l_4 = TRUE;
    }

    //Decode Fast Call set up conditions
    if(newFastCall & 0x1)
    {
        emlpp->fsc_condition.priority_l_A = TRUE;
    }

    if(newFastCall & 0x2)
    {
        emlpp->fsc_condition.priority_l_B = TRUE;
    }

    if(newFastCall & 0x4)
    {
        emlpp->fsc_condition.priority_l_0 = TRUE;
    }

    if(newFastCall & 0x8)
    {
        emlpp->fsc_condition.priority_l_1 = TRUE;
    }

    if(newFastCall & 0x10)
    {
        emlpp->fsc_condition.priority_l_2 = TRUE;
    }

    if(newFastCall & 0x20)
    {
        emlpp->fsc_condition.priority_l_3 = TRUE;
    }

    if(newFastCall & 0x40)
    {
        emlpp->fsc_condition.priority_l_4 = TRUE;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeAAem(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_AAEM_T *aaem = ef_mapping;
    uint8 *ptemp = data;
    uint8 newAaem = *ptemp;

    SCI_MEMSET(aaem, 0, sizeof(SIM_EF_MAPPING_EMLPP_T));

    //Decode Automatic answer priority levels
    if(newAaem & 0x1)
    {
        aaem->aa_priority_level.priority_l_A = TRUE;
    }

    if(newAaem & 0x2)
    {
        aaem->aa_priority_level.priority_l_B = TRUE;
    }

    if(newAaem & 0x4)
    {
        aaem->aa_priority_level.priority_l_0 = TRUE;
    }

    if(newAaem & 0x8)
    {
        aaem->aa_priority_level.priority_l_1 = TRUE;
    }

    if(newAaem & 0x10)
    {
        aaem->aa_priority_level.priority_l_2 = TRUE;
    }

    if(newAaem & 0x20)
    {
        aaem->aa_priority_level.priority_l_3 = TRUE;
    }

    if(newAaem & 0x40)
    {
        aaem->aa_priority_level.priority_l_4 = TRUE;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeECC(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint16 length = data_len;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ECC_T *ecc = ef_mapping;

    ecc->validate_num = 0;

    while((length >= 3) && (ecc->validate_num < SIM_MAX_ECC_NUM))
    {
        if((0xff == *ptemp) &&
            (0xff == *(ptemp + 1)) &&
            (0xff == *(ptemp + 2)))
        {
            return SIM_SERVICE_OK;
        }

        //copy one emergency call code to the call code array
        SCI_MEMCPY(&(ecc->call_codes[ecc->validate_num].ecc_code[0]), ptemp, 3);
        ptemp = ptemp + 3;
        length -= 3;
        ecc->call_codes[ecc->validate_num].alpha_id_flag = FALSE;// 2G Ecc has no this field
        ecc->call_codes[ecc->validate_num].ecc_category_flag = FALSE;// 2G Ecc has no this field
        ecc->validate_num++;
    }

    return SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCBMIR(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint16 num, i;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CBMIR_T *cbmir  = ef_mapping;

    SCI_MEMSET(cbmir, 0xff, sizeof(SIM_EF_MAPPING_CBMIR_T));
    num = data_len / SIM_EF_CBMIR_CB_SIZE;
    num = num > SIM_MAX_CBMIR_RANGE_NUM ? SIM_MAX_CBMIR_RANGE_NUM : num;
    cbmir->validate_num = 0;

    for(i = 0; i < num; i++)
    {
        if((0xff == *ptemp) && (0xff == *(ptemp + 1)) &&
            (0xff == *(ptemp + 2)) && (0xff == *(ptemp + 3)))
        {
            ptemp += SIM_EF_CBMIR_CB_SIZE;
            continue;
        }

        SCI_MEMCPY(&(cbmir->cbmi_ranges[i]), ptemp, SIM_EF_CBMIR_CB_SIZE);
        ptemp += SIM_EF_CBMIR_CB_SIZE;
        cbmir->validate_num++;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeDCK(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_DCK_T *dck = ef_mapping;

    if(data_len < 16)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFdck is less than 16."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_3361_112_2_17_22_58_31_4398, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    if((0xff == *ptemp) &&
        (0xff == *(ptemp + 1)) &&
        (0xff == *(ptemp + 2)) &&
        (0xff == *(ptemp + 3)))
    {
        dck->b_network_dck = FALSE;
    }
    else
    {
        dck->b_network_dck = TRUE;
        SCI_MEMCPY(dck->network_dck, ptemp, 4);
    }

    ptemp += 4;

    if((0xff == *ptemp) &&
        (0xff == *(ptemp + 1)) &&
        (0xff == *(ptemp + 2)) &&
        (0xff == *(ptemp + 3)))
    {
        dck->b_network_sub_dck  = FALSE;
    }
    else
    {
        dck->b_network_sub_dck = TRUE;
        SCI_MEMCPY(dck->network_sub_dck, ptemp, 4);
    }

    ptemp += 4;

    if((0xff == *ptemp) &&
        (0xff == *(ptemp + 1)) &&
        (0xff == *(ptemp + 2)) &&
        (0xff == *(ptemp + 3)))
    {
        dck->b_sp_dck = FALSE;
    }
    else
    {
        dck->b_sp_dck = TRUE;
        SCI_MEMCPY(dck->sp_dck, ptemp, 4);
    }

    ptemp += 4;

    if((0xff == *ptemp) &&
        (0xff == *(ptemp + 1)) &&
        (0xff == *(ptemp + 2)) &&
        (0xff == *(ptemp + 3)))
    {
        dck->b_corperate_dck = FALSE;
    }
    else
    {
        dck->b_corperate_dck = TRUE;
        SCI_MEMCPY(dck->corperate_dck, ptemp, 4);
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCNL(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_CNL_T *cnl = ef_mapping;
    uint16 i = 0;
    uint8 *ptemp = data;
    uint16 num = data_len / SIM_EF_CNL_ELEMENT_SIZE;

    cnl->validate_num = 0;
    num = num > SIM_MAX_CO_OPERATIVE_NET_LIST_NUM ? SIM_MAX_CO_OPERATIVE_NET_LIST_NUM : num;

    for(i = 0; i < num; i++)
    {
        if(0xff != *ptemp)//The end of the list is delimited by the first MCC field coded 'fff'.
        {
            cnl->validate_num++;
            SCI_MEMCPY(&(cnl->net_lists[i]), ptemp, SIM_EF_CNL_ELEMENT_SIZE);
            ptemp += SIM_EF_CNL_ELEMENT_SIZE;
        }
        else
        {
            return SIM_SERVICE_OK;
        }
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeNIA(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_NIA_T *nia = ef_mapping;
    uint8 *ptemp = data;
    uint16 iformative_len = data_len - SIM_EF_NIA_CATEGORY_LEN;

    nia->alerting_cat = *ptemp;

    if(0xff != nia->alerting_cat)
    {
        ptemp++;
        iformative_len = iformative_len > SIM_EF_NIA_MAX_INFORMATIVE_TEXT_LEN ? SIM_EF_NIA_MAX_INFORMATIVE_TEXT_LEN : iformative_len;
        nia->iformative_text_len = (uint8)iformative_len;
        SCI_MEMCPY(nia->iformative_text, ptemp, iformative_len);
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeKcGRPS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef _SUPPORT_GPRS_
    uint8 *ptemp = data;
    SIM_EF_MAPPING_KC_GPRS_T *kc_gprs = ef_mapping;

    if(data_len != 9)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFkcgprs is not 9."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_3524_112_2_17_22_58_31_4399, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMCPY(kc_gprs->kc, ptemp, 8);
    ptemp += 8;

    kc_gprs->cksn = (*ptemp & 0x07);
#endif
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeLOCIGPRS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef _SUPPORT_GPRS_
    uint16 mnc_digit_num = 0;
    SIM_SERVICE_RESULT_E ser_result;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_LOCI_GPRS_T *loci_gprs = ef_mapping;

    if(data_len != 14)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFlocigprs is not 14."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_3561_112_2_17_22_58_31_4400, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //the P-TMSI
    if((0xff == ptemp[0]) &&
        (0xff == ptemp[1]) &&
        (0xff == ptemp[2]) &&
        (0xff == ptemp[3]))
    {
        loci_gprs->p_tmsi.valid_flag = FALSE;
        SCI_MEMCPY(loci_gprs->p_tmsi.value, ptemp, 4);
    }
    else
    {
        loci_gprs->p_tmsi.valid_flag = TRUE;
        SCI_MEMCPY(loci_gprs->p_tmsi.value, ptemp, 4);
    }

    ptemp += 4;

    //the P-TMSI signature
    if((0xff == ptemp[0]) &&
        (0xff == ptemp[1]) &&
        (0xff == ptemp[2]))
    {
        loci_gprs->p_tmsi_signature.valid_flag = FALSE;
        loci_gprs->p_tmsi_signature.value = 0xFFFFFF;
    }
    else
    {
        loci_gprs->p_tmsi_signature.valid_flag = TRUE;
        loci_gprs->p_tmsi_signature.value = (*ptemp << 16) +
                                            (*(ptemp + 1) << 8) +
                                            (*(ptemp + 2)) ;
    }

    ptemp += 3;

    loci_gprs->rai.mcc = 0;
    loci_gprs->rai.mnc = 0;
    //share_cr7116,begin
    loci_gprs->rai.mnc_digit_num = 0;
    //cr7116,end

    //decode the RAI
    //get a loci_gprs->rai.mcc and loci_gprs->rai.mnc code
    if((0xFF == ptemp[0]) &&
        (0xFF == ptemp[1]) &&
        (0xFF == ptemp[2]))
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::DecodeLOCIGPRS mcc-mnc is 0xff 0xff 0xff."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_3613_112_2_17_22_58_31_4401, (uint8 *)"");
#endif
        //init the rai plmn
        loci_gprs->rai.mnc_digit_num = 0xf;
    }
    else
    {
        ser_result = DecodeMCCMNC(3, ptemp, &(loci_gprs->rai.mcc),
                                  &(loci_gprs->rai.mnc),
                                  &mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }

        loci_gprs->rai.mnc_digit_num = mnc_digit_num;
    }

    ptemp += 3;

    loci_gprs->rai.lac = (((uint16) * ptemp) << 8) + *(ptemp + 1);
    ptemp += 2;
    loci_gprs->rai.rac = (uint8)(*ptemp);
    ptemp += 1;

    loci_gprs->update_status = (SIM_R_UPDATE_STATUS_E)((*ptemp) & SIM_EF_LOCI_GPRS_UPDATE_STATUS_MARK);
#endif
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSUME(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_SUME_T *sume = ef_mapping;
    uint8 *ptemp = data;
    uint16 len = 0;

    sume->alpha_id_tag = *ptemp;
    ptemp++;
    len++;

    //GSM11.14 AnnexD, decode the alpha id len
    if(*ptemp < 0x80) //there is only 1 byte to indicate the alpha id len
    {
        sume->alpha_id_len = *ptemp;
    }
    else//there are 2 bytes to indicate the alpha id len
    {
        if(0x81 == *ptemp)
        {
            ptemp++;
            len++;

            if(*ptemp >= 0x80)
            {
                sume->alpha_id_len = *ptemp;
            }
            else
            {
                return SIM_SERVICE_DECODE_EF_ERROR;
            }
        }
        else
        {
            return SIM_SERVICE_DECODE_EF_ERROR;
        }
    }

    ptemp++;
    len += (sume->alpha_id_len);
    SCI_MEMCPY(sume->alpha_id, ptemp, sume->alpha_id_len);

    sume->icon_flag = FALSE;

    if((data_len - len) >= SIM_EF_SUME_ICONID_SIZE)
    {
        ptemp += (sume->alpha_id_len);
        sume->icon_id_tag = *ptemp;
        ptemp++;
        sume->icon_id_len = *ptemp;

        if(sume->icon_id_len != SIM_EF_SUME_ICONID_REST_LEN)
        {
            return SIM_SERVICE_DECODE_EF_ERROR;
        }

        ptemp++;
        sume->icon_qualifier = *ptemp;
        ptemp++;
        sume->icon_id = *ptemp;
        sume->icon_flag = TRUE;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePNN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_PNN_T *pnn = ef_mapping;
    uint8 *ptemp = data;
    uint8 spare_bits, dcs = 0;
    uint8 nt_name_len = 0;
    uint8 seven_bits_data_ptr[NETWORK_NAME_ARR_LEN];
    int16 seven_bits_data_len = 0;
    int16 eight_bits_data_len = 0;

    SCI_MEMSET(pnn, 0, sizeof(SIM_EF_MAPPING_PNN_T));

    //decode according to the Figure 10.5.80/3GPP TS 24.008
    if(SIM_FULL_NETWORK_NAME_IEI == *ptemp) //Full Network Name IEI
    {
        ptemp++;
        nt_name_len = *ptemp;//Length of Network Name contents

        if(nt_name_len > 1)
        {
            pnn->full_nt_name.length = nt_name_len - 1;// 1 is the 3rd octet
            ptemp++;

            //{ext|coding scheme|AddCI|Number of sparebits in last octet}octet 3
            if(BIT_3 == (BIT_3 & (*ptemp)))
            {
                pnn->full_nt_name.is_add_ci = TRUE;
            }
            else
            {
                pnn->full_nt_name.is_add_ci = FALSE;
            }

            dcs = (uint8)((BIT_6 | BIT_5 | BIT_4) & (*ptemp));
            dcs = dcs >> 4;

            if(dcs == 1)
            {
                pnn->full_nt_name.code_scheme = UCS_DCS_VALUE;

                if(pnn->full_nt_name.length > NETWORK_NAME_ARR_LEN)
                {
                    pnn->full_nt_name.length = NETWORK_NAME_ARR_LEN;
                }

                ptemp++;//move the ptr to the TextString
                SCI_MEMCPY(pnn->full_nt_name.name, ptemp, pnn->full_nt_name.length);
            }
            else
            {
                pnn->full_nt_name.code_scheme = DEFAULT_DCS_VALUE;

                spare_bits = (uint8)((BIT_2 | BIT_1 | BIT_0) & (*ptemp));
                ptemp++;//move the ptr to the TextString

                if(7 == spare_bits)
                {
                    //fill the last byte 0  0   0   1   1   0   1   7a
                    // see 03.38,section 6.1.2.3.1  Packing of 7 bit characters
                    ptemp[pnn->full_nt_name.length-1] &= 0x01;
                    ptemp[pnn->full_nt_name.length-1] |= 0x1a;
                }

                SCI_MEMSET((void *)seven_bits_data_ptr, 0, NETWORK_NAME_ARR_LEN);

                //the most compressed date length is NETWORK_NAME_ARR_LEN*7/8 +( NETWORK_NAME_ARR_LEN*7%8 ? 1: 0)
                if(pnn->full_nt_name.length > (NETWORK_NAME_ARR_LEN * 7 / 8))
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"MM:The length of the network name is too long"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_3787_112_2_17_22_58_32_4402, (uint8 *)"");
#endif
                    seven_bits_data_len = (NETWORK_NAME_ARR_LEN * 7 / 8);
                }
                else
                {
                    seven_bits_data_len = (int16)pnn->full_nt_name.length;
                }

                SCI_MEMCPY((void *)seven_bits_data_ptr, ptemp, seven_bits_data_len);

                eight_bits_data_len = seven_bits_data_len * 8 / 7;
                SCI_Unpack7bitsTo8bits(seven_bits_data_ptr, seven_bits_data_len, 0, (uint8 *)(&(pnn->full_nt_name.name[0])), &eight_bits_data_len);
                pnn->full_nt_name.length = (uint16)eight_bits_data_len;
            }

            ptemp += (nt_name_len - 1);//move the ptr from "TextStrig" to the Short Network Name
        }
        else
        {
            //If nt_name_len 0 or 1,we should continue to move to the "Short Network Name".
            ptemp += (nt_name_len + 1);
        }

        pnn->is_short_nt_name_flag = FALSE;//clear

        if(SIM_SHORT_NETWORK_NAME_IEI == *ptemp) //Short Network Name IEI
        {
            ptemp++;
            nt_name_len = *ptemp;//Length of Network Name contents
            pnn->short_nt_name.length = nt_name_len - 1;// 1 is the 3rd octet

            if(nt_name_len > 1)
            {
                pnn->is_short_nt_name_flag = TRUE;
                ptemp++;

                //{ext|coding scheme|AddCI|Number of sparebits in last octet}octet 3
                if(BIT_3 == (BIT_3 & (*ptemp)))
                {
                    pnn->short_nt_name.is_add_ci = TRUE;
                }
                else
                {
                    pnn->short_nt_name.is_add_ci = FALSE;
                }

                dcs = (uint8)((BIT_6 | BIT_5 | BIT_4) & (*ptemp));
                dcs = dcs >> 4;

                if(dcs == 1)
                {
                    pnn->short_nt_name.code_scheme = UCS_DCS_VALUE;

                    if(pnn->short_nt_name.length > NETWORK_NAME_ARR_LEN)
                    {
                        pnn->short_nt_name.length = NETWORK_NAME_ARR_LEN;
                    }

                    ptemp++;//move the ptr to the TextString
                    SCI_MEMCPY(pnn->short_nt_name.name, ptemp, pnn->short_nt_name.length);
                }
                else
                {
                    pnn->short_nt_name.code_scheme = DEFAULT_DCS_VALUE;

                    spare_bits = (uint8)((BIT_2 | BIT_1 | BIT_0) & (*ptemp));
                    ptemp++;//move the ptr to the TextString

                    if(7 == spare_bits)
                    {
                        //fill the last byte 0  0   0   1   1   0   1   7a
                        // see 03.38,section 6.1.2.3.1  Packing of 7 bit characters
                        ptemp[pnn->short_nt_name.length-1] &= 0x01;
                        ptemp[pnn->short_nt_name.length-1] |= 0x1a;
                    }

                    SCI_MEMSET((void *)seven_bits_data_ptr, 0, NETWORK_NAME_ARR_LEN);

                    //the most compressed date length is NETWORK_NAME_ARR_LEN*7/8 +( NETWORK_NAME_ARR_LEN*7%8 ? 1: 0)
                    if(pnn->short_nt_name.length > (NETWORK_NAME_ARR_LEN * 7 / 8))
                    {
#ifndef _LESSEN_TRACE_SUPPORT
                        //SCI_TRACE_LOW:"MM:The length of the network name is too long"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_3861_112_2_17_22_58_32_4403, (uint8 *)"");
#endif
                        seven_bits_data_len = (NETWORK_NAME_ARR_LEN * 7 / 8);
                    }
                    else
                    {
                        seven_bits_data_len = (int16)pnn->short_nt_name.length;
                    }

                    SCI_MEMCPY((void *)seven_bits_data_ptr, ptemp, seven_bits_data_len);

                    eight_bits_data_len = seven_bits_data_len * 8 / 7;
                    SCI_Unpack7bitsTo8bits(seven_bits_data_ptr, seven_bits_data_len, 0, (uint8 *)(&(pnn->short_nt_name.name[0])), &eight_bits_data_len);
                    pnn->short_nt_name.length = (uint16)eight_bits_data_len;
                }
            }
        }
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeOPL(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_OPL_T *opl = ef_mapping;
    uint8 *ptemp = data;
    uint16 temp = 0;
    uint16 mcc = 0;
    uint16 mnc = 0;
    uint16 mnc_digit_num = 0;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;

    if(data_len < 8)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

    if(SIM_SERVICE_OK == ser_result)
    {
        opl->plmn.mcc = mcc;
        opl->plmn.mnc = mnc;
        opl->plmn.mnc_digit_num = mnc_digit_num;
        ptemp += 3;
    }
    else
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    temp = *ptemp;
    ptemp++;
    //In the LAC field bit 8 of octet 5 is the most significant bit and bit 1 of octet 6 the least significant bit.
    temp = (temp << 8) | (*ptemp);
    opl->lac_1 = temp;

    ptemp++;
    temp = *ptemp;
    ptemp++;
    temp = (temp << 8) | (*ptemp);
    opl->lac_2 = temp;

    ptemp++;
    opl->pnn_index = *ptemp;

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMBDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    uint8 *temp_ptr;
    SIM_EF_MAPPING_MBNUM_T  *mailbox_num;

    temp_ptr = data;
    mailbox_num = ef_mapping;

    if(data_len < 14)
    {
        ser_result = SIM_SERVICE_DECODE_EF_ERROR;
        return ser_result;
    }

    //record the data_len from the select file for the latter updating file
    g_sim_r4_vm_record_len[cur_sim_task] = (uint8)data_len;

    //the alpha id
    mailbox_num->alpha_id_len = data_len - 14;

    if(mailbox_num->alpha_id_len > 0)
    {
        if(mailbox_num->alpha_id_len <= SIM_MAX_DN_ALPHA_IDENTIFY_LEN)
        {
            SCI_MEMCPY(mailbox_num->alpha_id, temp_ptr, mailbox_num->alpha_id_len);
            temp_ptr += mailbox_num->alpha_id_len;
        }
        else
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::the alpha id len is bigger than 60."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_3986_112_2_17_22_58_32_4404, (uint8 *)"");
#endif
            SCI_MEMCPY(mailbox_num->alpha_id, temp_ptr, SIM_MAX_DN_ALPHA_IDENTIFY_LEN);
            temp_ptr += mailbox_num->alpha_id_len;
            mailbox_num->alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
        }
    }

    //get the mailbox number length
    if((*temp_ptr > 0) && (*temp_ptr <=  MN_MAX_ADDR_BCD_LEN))
    {
        mailbox_num->mailbox_number_len = *temp_ptr - 1;
    }
    else
    {
        mailbox_num->mailbox_number_len = 0;
    }

    temp_ptr += 1;

    //the TON and the NPI
    if(UINT8_INVALIDE != *temp_ptr)
    {
        mailbox_num->is_ton_npi_exist = TRUE;
        mailbox_num->ton = ((*temp_ptr) & SIM_EF_DN_TON_MARK) >> 4;
        mailbox_num->npi = (*temp_ptr) & SIM_EF_DN_NPI_MARK;
    }
    else
    {
        mailbox_num->is_ton_npi_exist = FALSE;
    }

    temp_ptr += 1;

    //the mailbox number
    SCI_MEMCPY(mailbox_num->mailbox_number, temp_ptr, SIM_EF_DN_DN_LEN);
    temp_ptr += SIM_EF_DN_DN_LEN;

    //the Capability/Configuration Identifier
    if((UINT8_INVALIDE != *temp_ptr) && (0 != *temp_ptr))
    {
        mailbox_num->is_ccp_exist = TRUE;
        mailbox_num->ccp_id = *temp_ptr;
    }
    else
    {
        mailbox_num->is_ccp_exist = FALSE;
    }

    temp_ptr += 1;

    //the Extension6 Record Identifier
    //We cannnot support EXT6...
    mailbox_num->is_ext_exist = FALSE;
    mailbox_num->ext_id = 0xFF;

    return ser_result;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMBI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_MBI_T  *mailbox_id;

    temp_ptr = data;
    mailbox_id = ef_mapping;

    if(data_len < 4)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    g_sim_r4_vm_mbi_record_len[cur_sim_task] = data_len;

    mailbox_id->voicemail_index = *temp_ptr;
    temp_ptr++;
    mailbox_id->fax_index = *temp_ptr;
    temp_ptr++;
    mailbox_id->email_index = *temp_ptr;
    temp_ptr++;
    mailbox_id->other_index = *temp_ptr;

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMWIS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_MWIS_T *mwis_mapping = ef_mapping;
    BOOLEAN voice_flag = FALSE;
    BOOLEAN fax_flag = FALSE;
    BOOLEAN email_flag = FALSE;
    BOOLEAN other_flag = FALSE;

    g_sim_r4_vm_mwis_record_len[cur_sim_task] = data_len;

    SCI_MEMSET(mwis_mapping, 0x0, sizeof(SIM_EF_MAPPING_MWIS_T));

    if(data_len >= 5)
    {
        if(BIT_0 == ((*ptemp)&BIT_0))
        {
            voice_flag = TRUE;
        }

        if(BIT_1 == ((*ptemp)&BIT_1))
        {
            fax_flag = TRUE;
        }

        if(BIT_2 == ((*ptemp)&BIT_2))
        {
            email_flag = TRUE;
        }

        if(BIT_3 == ((*ptemp)&BIT_3))
        {
            other_flag = TRUE;
        }

        ptemp++;

        if(voice_flag)
        {
            if(0 == *ptemp)
            {
                //flag status indicate that message waiting exists
                mwis_mapping->vm_waiting_count = 1;
            }
            else
            {
                mwis_mapping->vm_waiting_count = *ptemp;
            }
        }

        ptemp++;

        if(fax_flag)
        {
            if(0 == *ptemp)
            {
                //flag status indicate that message waiting exists
                mwis_mapping->fax_waiting_count = 1;
            }
            else
            {
                mwis_mapping->fax_waiting_count = *ptemp;
            }
        }

        ptemp++;

        if(email_flag)
        {
            if(0 == *ptemp)
            {
                //flag status indicate that message waiting exists
                mwis_mapping->data_waiting_count = 1;
            }
            else
            {
                mwis_mapping->data_waiting_count = *ptemp;
            }
        }

        ptemp++;

        if(other_flag)
        {
            if(0 == *ptemp)
            {
                //flag status indicate that message waiting exists
                mwis_mapping->other_waiting_count = 1;
            }
            else
            {
                mwis_mapping->other_waiting_count = *ptemp;
            }
        }

        return SIM_SERVICE_OK;
    }
    else
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCFIS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_CFIS_T *cfis = ef_mapping;
    uint8 *ptemp = data;

    cfis->msp_id = *ptemp;
    ptemp++;

    if(BIT_0 == ((*ptemp)&BIT_0))
    {
        cfis->voice_callfrd_uncond = TRUE;
    }
    else
    {
        cfis->voice_callfrd_uncond = FALSE;
    }

    if(BIT_1 == ((*ptemp)&BIT_1))
    {
        cfis->fax_callfrd_uncond = TRUE;
    }
    else
    {
        cfis->fax_callfrd_uncond = FALSE;
    }

    if(BIT_2 == ((*ptemp)&BIT_2))
    {
        cfis->data_callfrd_uncond = TRUE;
    }
    else
    {
        cfis->data_callfrd_uncond = FALSE;
    }

    SCI_TRACE_LOW("SIM%d:DecodeCFIS %d, %d %d %d", cur_sim_task, cfis->msp_id, cfis->voice_callfrd_uncond, cfis->fax_callfrd_uncond, cfis->data_callfrd_uncond);

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSPDI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_SPDI_T *spdi = ef_mapping;
    uint8 *ptemp = data;
    uint16 i, len = 0;
    uint16 mcc = 0;
    uint16 mnc = 0;
    uint16 mnc_digit_num = 0;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;

    SCI_MEMSET(spdi, 0xFF, sizeof(SIM_EF_MAPPING_SPDI_T));
    spdi->num = 0;

    if(SIM_SP_DISPLAY_TAG == *ptemp)
    {
        ptemp++;
        len = *ptemp;

        if(0xFF == len) //invalid len
        {
            return SIM_SERVICE_OK;
        }

        ptemp++;
    }

    if(SIM_SP_PLMN_LIST_TAG == *ptemp)
    {
        ptemp++;
        len = *ptemp;//length

        if(0xFF != len)
        {
            len = len / 3;

            if(len > SIM_SPDI_PLMN_MAX_NUM)
            {
                len = SIM_SPDI_PLMN_MAX_NUM;
            }

            ptemp++;

            for(i = 0; i < len; i++)
            {
                if((0xFF == ptemp[0])
                    && (0xFF == ptemp[1])
                    && (0xFF == ptemp[2]))
                {
                    ptemp += 3;
                    spdi->plmn_arr[i].present = FALSE;
                    continue;
                }

                ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

                if(SIM_SERVICE_OK == ser_result)
                {
                    spdi->plmn_arr[i].present = TRUE;
                    spdi->plmn_arr[i].plmn.mcc = mcc;
                    spdi->plmn_arr[i].plmn.mnc = mnc;
                    spdi->plmn_arr[i].plmn.mnc_digit_num = mnc_digit_num;
                    spdi->num++;
                    ptemp += 3;
                }
                else
                {
                    break;
                }
            }
        }
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMMSN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    return SIM_SERVICE_DECODE_EF_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeEXT8(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    return SIM_SERVICE_DECODE_EF_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMMSICP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    return SIM_SERVICE_DECODE_EF_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMMSUP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    return SIM_SERVICE_DECODE_EF_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMMSUCP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    return SIM_SERVICE_DECODE_EF_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSAI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_SAI_T *sai = ef_mapping;
    uint8 *ptemp = data;
    uint16 len = 0;

    if((*ptemp) & SIM_EF_SAI_LSA_ACTIVE_MASK)
    {
        sai->lsa_only_access = TRUE;
        len = data_len - SIM_EF_SAI_LSA_INDICATOR_LEN;
        len = len > SIM_EF_SAI_LSA_TEXT_MAX_LEN ? SIM_EF_SAI_LSA_TEXT_MAX_LEN : len;
        sai->lsa_lsa_only_access_len = (uint8)len;
        ptemp++;
        SCI_MEMCPY(sai->lsa_only_access_text, ptemp, len);
    }
    else
    {
        sai->lsa_only_access = FALSE;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSLL(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_SLL_T *sll = ef_mapping;
    uint8 *ptemp = data;
    int16 len = 0;
    uint8 cof_parameter = 0;
    uint16 mcc, mnc, mnc_digit_num;
    SIM_SERVICE_RESULT_E ser_result;

    len = data_len - SIM_EF_SLL_MANDATORY_LEN;

    if(len >= 1)
    {
        sll->lsa_name_len = len > SIM_EF_SLL_LSA_NAME_MAX_LEN ? SIM_EF_SLL_LSA_NAME_MAX_LEN : len;
        SCI_MEMCPY(sll->lsa_name, ptemp, sll->lsa_name_len);
        ptemp += len;
    }
    else
    {
        sll->lsa_name_len = 0;
    }

    //The Configuration Parameters
    cof_parameter = *ptemp;
    ptemp++;
    sll->icon_qualifier = cof_parameter & SIM_EF_SLL_ICONID_MASK;

    if(cof_parameter & SIM_EF_SLL_IDLE_SUPPORT_MASK)
    {
        sll->idle_mode_support = TRUE;
    }
    else
    {
        sll->idle_mode_support = FALSE;
    }

    if(cof_parameter & SIM_EF_SLL_LSA_IDLE_SUPPORT_MASK)
    {
        sll->lsa_indication_idle_mode = TRUE;
    }
    else
    {
        sll->lsa_indication_idle_mode = FALSE;
    }

    //RFU
    sll->rfu = *ptemp;
    ptemp++;

    //The Icon Identifier
    sll->icon_identifier = *ptemp;
    ptemp++;

    //The Priority
    sll->priority = *ptemp;
    ptemp++;

    //The PLMN code
    ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    sll->plmn.mcc = mcc;
    sll->plmn.mnc = mnc;
    sll->plmn.mnc_digit_num = mnc_digit_num;
    ptemp += 3;

    //The LSA Descriptor File Identifier
    sll->lsa_file_id = *ptemp;
    sll->lsa_file_id <<= 8;
    ptemp++;
    sll->lsa_file_id |= *ptemp;
    ptemp++;

    //The LSA Descriptor Record Identifier
    sll->lsa_rec_number = *ptemp;

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeADN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_DN_T *pdn = ef_mapping;

    //the ADN record info still not got yet,get the record info
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].adn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_ADN, &(g_sim_card_setting[cur_sim_task].adn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return DecodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].adn_info), pdn);
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeFDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_DN_T *pdn = ef_mapping;

    //the ADN record info still not got yet,get the record info
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].fdn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_FDN, &(g_sim_card_setting[cur_sim_task].fdn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return DecodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].fdn_info), pdn);
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSMS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_SMS_T *psms = ef_mapping;

    //the record len is not right
    if(SIM_EF_SMS_RECORD_LEN != data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    switch(*ptemp)
    {
        case SIM_EF_SMS_FREE:
            psms->sms_status = MN_SMS_FREE_SPACE;
            break;
        case SIM_EF_SMS_MT_READED:
            psms->sms_status = MN_SMS_MT_READED;
            break;
        case SIM_EF_SMS_MT_TO_BE_READ:
            psms->sms_status = MN_SMS_MT_TO_BE_READ;
            break;
        case SIM_EF_SMS_MO_TO_BE_SENT:
            psms->sms_status = MN_SMS_MO_TO_BE_SEND;
            break;
        case SIM_EF_SMS_MO_NO_SR:
            psms->sms_status = MN_SMS_MO_SR_NOT_REQUEST;
            break;
        case SIM_EF_SMS_MO_SR_NOT_RECV:
            psms->sms_status = MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED;
            break;
        case SIM_EF_SMS_MO_SR_NOT_SAVE:
            psms->sms_status = MN_SMS_MO_SR_RECEIVED_NOT_STORE;
            break;
        case SIM_EF_SMS_MO_SR_SAVED:
            psms->sms_status = MN_SMS_MO_SR_RECEIVED_AND_STORED;
            break;
        default:
            psms->sms_status = MN_SMS_FREE_SPACE;
            break;
    }

    ptemp += 1;
    //copy the SMS content
    SCI_MEMCPY(psms->sms_sim_sm_arr, ptemp, (SIM_EF_SMS_RECORD_LEN - 1));

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCCP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CCP_T *pccp = ef_mapping;

    //check the record length
    if(data_len < SIM_EF_CCP_RECORD_LEN)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning:the length of the EFccp is %d.fill data in it."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_4529_112_2_17_22_58_33_4405, (uint8 *)"d", data_len);
#endif
        SCI_MEMSET((void *)((uint32)data + data_len), 0xff, (SIM_EF_CCP_RECORD_LEN - data_len));
    }

    //the Bear capability information element
    SCI_MEMCPY(pccp->bc_info, ptemp, SIM_EF_CCP_BC_INFO_LEN);
    ptemp += SIM_EF_CCP_BC_INFO_LEN;

    //Bytes reserved
    SCI_MEMCPY(pccp->revers_bytes, ptemp, SIM_EF_CCP_RESERVED_BYTE_LEN);

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMSISDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_DN_T *pdn = ef_mapping;

    //the ADN record info still not got yet,get the record info
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].msisdn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_MSISDN, &(g_sim_card_setting[cur_sim_task].msisdn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return DecodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].msisdn_info), pdn);
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSMSP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SMS_PARAM_T *param = ef_mapping;
    SIM_SERVICE_RESULT_E ser_result;

    //the record must not
    if(data_len < SIM_EF_SMSP_MIN_RECORD_LEN)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFsmsp is less than 28."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_4598_112_2_17_22_58_33_4406, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //still not get the record info for the EFsmsp
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_SMSP, &(g_sim_card_setting[cur_sim_task].smsp_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    //check the data len
    if(g_sim_card_setting[cur_sim_task].smsp_info.original_alpha_id_len + SIM_EF_SMSP_MIN_RECORD_LEN != data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //the alpha id is not present
    if(0 == g_sim_card_setting[cur_sim_task].smsp_info.original_alpha_id_len)
    {
        param->alpha_id_present = FALSE;
    }
    //the alpha id is present,copy down the alpha id
    else
    {
        param->alpha_id_present = TRUE;

        if(g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len > SIM_MAX_SMSP_ALPHA_ID_LEN)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::warning the alpha id len of the EFsmsp is %d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_4632_112_2_17_22_58_33_4407, (uint8 *)"d", g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len);
#endif
            g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len = SIM_MAX_SMSP_ALPHA_ID_LEN;
        }

        SCI_MEMSET(param->alpha_id_t.id, 0xFF, SIM_MAX_SMSP_ALPHA_ID_LEN);
        param->alpha_id_t.id_len = g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len;
        SCI_MEMCPY(param->alpha_id_t.id, ptemp, param->alpha_id_t.id_len);
        ptemp += g_sim_card_setting[cur_sim_task].smsp_info.original_alpha_id_len;
    }

    //decode the Parameter Indicator
    if(0 == ((*ptemp) & SIM_EF_SMSP_TP_DA_INDICATOR))
    {
        param->tp_dest_addr_present = TRUE;
    }
    else
    {
        param->tp_dest_addr_present = FALSE;
    }

    if(0 == ((*ptemp) & SIM_EF_SMSP_TP_SC_INDICATOR))
    {
        param->tp_sc_addr_present = TRUE;
    }
    else
    {
        param->tp_sc_addr_present = FALSE;
    }

    if(0 == ((*ptemp) & SIM_EF_SMSP_TP_PI_INDICATOR))
    {
        param->tp_protocol_id_present = TRUE;
    }
    else
    {
        param->tp_protocol_id_present = FALSE;
    }

    if(0 == ((*ptemp) & SIM_EF_SMSP_TP_DCS_INDICATOR))
    {
        param->tp_dcs_present = TRUE;
    }
    else
    {
        param->tp_dcs_present = FALSE;
    }

    if(0 == ((*ptemp) & SIM_EF_SMSP_TP_VP_INDICATOR))
    {
        param->tp_vp_present = TRUE;
    }
    else
    {
        param->tp_vp_present = FALSE;
    }

    ptemp += 1;

    //decode the TP-Destination Address
    if(param->tp_dest_addr_present)
    {
        SCI_MEMCPY(param->tp_dest_addr_arr, ptemp, SMS_ADDR_MAX_LEN);
    }

    ptemp += SIM_EF_SMSP_ADD_OBJECT_LEN;

    //decode the TS-Service Centre Address
    if(param->tp_sc_addr_present)
    {
        SCI_MEMCPY(param->tp_sc_addr_arr, ptemp, SMS_ADDR_MAX_LEN);
    }

    ptemp += SIM_EF_SMSP_ADD_OBJECT_LEN;

    //decode the TP-Protocol Identifier
    if(param->tp_protocol_id_present)
    {
        param->tp_protocol_id = *ptemp;
    }

    ptemp += 1;

    //decode the TP-Data Code Scheme
    if(param->tp_dcs_present)
    {
        param->tp_dcs = *ptemp;
    }

    ptemp += 1;

    //decode the TP-Validity Period
    if(param->tp_vp_present)
    {
        param->relative_format_vp = *ptemp;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSMSS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_SMSS_T *psmss = ef_mapping;

    //the file size should not mini than the mini filesize
    if(data_len < SIM_EF_SMSS_MIN_FILE_SIZE)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFsmss is less than 2."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_4744_112_2_17_22_58_34_4408, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //get the Last Used TP-Message Reference
    psmss->tp_mr = *ptemp;
    ptemp += 1;

    //check the Memory Exceeded Flag
    if(*ptemp & SIM_EF_SMSS_MEM_EXCEED_MARK)
    {
        psmss->mem_exeed = FALSE;
    }
    else
    {
        psmss->mem_exeed = TRUE;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeLND(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_DN_T *pdn = ef_mapping;

    //the ADN record info still not got yet,get the record info
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].lnd_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_LND, &(g_sim_card_setting[cur_sim_task].lnd_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return DecodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].lnd_info), pdn);
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSMSR(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_SMSR_T *smsr_mapping = ef_mapping;

    //check the record len of the SMSR
    if(SIM_EF_SMSR_RECORD_LEN > data_len)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFsmsr is less than 30."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_4819_112_2_17_22_58_34_4409, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    smsr_mapping->sm_record_id = *ptemp;
    ptemp += 1;

    //copy the status report
    SCI_MEMCPY(smsr_mapping->status_report, ptemp, SMS_SIM_STATUS_REPORT_LEN);

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_DN_T *pdn = ef_mapping;

    //the ADN record info still not got yet,get the record info
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].sdn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_SDN, &(g_sim_card_setting[cur_sim_task].sdn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return DecodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].sdn_info), pdn);
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeEXT(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_EXT_T *pextension = ef_mapping;

    if(SIM_EF_EXT_RECORD_LEN > data_len)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFext1 is less than 13."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_4885_112_2_17_22_58_34_4410, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //the extension record type
    switch(*ptemp)
    {
        case SIM_EXT_RECORD_UNKNOW:
            pextension->record_type = SIM_EXT_RECORD_UNKNOW;
            break;
        case SIM_EXT_RECORD_CPS:
            pextension->record_type = SIM_EXT_RECORD_CPS;
            break;
        case SIM_EXT_RECORD_ADDTIONAL_DATA:
            pextension->record_type = SIM_EXT_RECORD_ADDTIONAL_DATA;
            break;
        default:
            pextension->record_type = SIM_EXT_RECORD_UNKNOW;
            break;
    }

    ptemp += 1;

    //the record extension data
    if(pextension->record_type == SIM_EXT_RECORD_ADDTIONAL_DATA)
    {
        if((*ptemp) > SIM_EF_EXT_ADDITIONAL_DATA_LEN)
        {
            pextension->extension_data_len = SIM_EF_EXT_ADDITIONAL_DATA_LEN;
        }
        else
        {
            pextension->extension_data_len = *ptemp;
        }

        //the first byte is the number of bytes of the remainder of ADC/SSC
        SCI_MEMCPY(pextension->extension_data, ptemp + 1, SIM_EF_EXT_ADDITIONAL_DATA_LEN);
    }
    else
    {
        pextension->extension_data_len = SIM_EF_EXT_DATA_LEN;
        SCI_MEMCPY(pextension->extension_data, ptemp, SIM_EF_EXT_DATA_LEN);
    }

    ptemp += SIM_EF_EXT_DATA_LEN;

    //is there are next record id
    if((UINT8_INVALIDE == *ptemp) || (0 ==  *ptemp))
    {
        pextension->more_data = FALSE;
    }
    else
    {
        pextension->more_data = TRUE;
        pextension->next_record_id = *ptemp;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeBDN(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_DN_T *pdn = ef_mapping;

    //the ADN record info still not got yet,get the record info
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_BDN, &(g_sim_card_setting[cur_sim_task].bdn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    //the record length is not correct
    if(g_sim_card_setting[cur_sim_task].bdn_info.original_alpha_id_len + SIM_EF_DN_MIN_RECORD_LEN != data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //the alpha id
    if(g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len > SIM_MAX_DN_ALPHA_IDENTIFY_LEN)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning the alpha id len of the EFlnd is %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_4981_112_2_17_22_58_34_4411, (uint8 *)"d", g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len);
#endif
        g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    }

    SCI_MEMSET(pdn->alpha_id, 0xFF, SIM_MAX_DN_ALPHA_IDENTIFY_LEN); //initialise the alpha_id with 0xFF
    pdn->alpha_id_len = g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len;
    SCI_MEMCPY(pdn->alpha_id, ptemp, pdn->alpha_id_len);
    ptemp += g_sim_card_setting[cur_sim_task].bdn_info.original_alpha_id_len;

    //get the dialling num length

    /*king.li modify for cr93009 the same with the modify in DecodeADN function*/
    if((0 == *ptemp) || (0xFF == *ptemp))
    {
        pdn->dn_len = 0;
    }
    else if(*ptemp > 11)
    {
        pdn->dn_len = SIM_EF_DN_DN_LEN;
    }
    else
    {
        pdn->dn_len = *ptemp - 1;
    }

    ptemp += 1;

    //the TON and the NPI
    if(UINT8_INVALIDE != *ptemp)
    {
        pdn->is_ton_npi_exist = TRUE;
        pdn->ton = (((*ptemp) & SIM_EF_DN_TON_MARK) >> 4);
        pdn->npi = (*ptemp) & SIM_EF_DN_NPI_MARK;
    }
    else
    {
        pdn->is_ton_npi_exist = FALSE;
    }

    ptemp += 1;

    //the dialling num
    SCI_MEMCPY(pdn->dialling_num, ptemp, SIM_EF_DN_DN_LEN);
    ptemp += SIM_EF_DN_DN_LEN;

    //the Capability/Configuration Identifier
    if((UINT8_INVALIDE != *ptemp)
        && (0 != *ptemp))
    {
        pdn->is_ccp_exist = TRUE;
        pdn->ccp_id = *ptemp;
    }
    else
    {
        pdn->is_ccp_exist = FALSE;
    }

    ptemp += 1;

    //the Extension1 Record Identifier
    if((UINT8_INVALIDE != *ptemp)
        && (0 != *ptemp))
    {
        pdn->is_ext_exist = TRUE;
        pdn->ext_id = *ptemp;
    }
    else
    {
        pdn->is_ext_exist = FALSE;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeIMG(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_IMG_T *img = ef_mapping;
    uint8 *ptemp = data;
    uint8 *ptemp_rfu = data;
    uint16 i, num, before_rfu_len = 0;

    //REAL NUM
    num = *ptemp;

    //RFU
    before_rfu_len = 1 + num * SIM_EF_IMG_INSTANCE_SIZE;

    if(before_rfu_len == (data_len - 1))
    {
        ptemp_rfu += before_rfu_len;
        img->rfu_flag = TRUE;
        img->rfu = *ptemp_rfu;
    }
    else
    {
        img->rfu_flag = FALSE;
    }

    //NUM we support
    num = num > SIM_EF_IMG_INSTANCE_NUM ? SIM_EF_IMG_INSTANCE_NUM : num;
    img->num = (uint8)num;

    //INSTANCE
    ptemp++;

    for(i = 0; i < num; i++)
    {
        img->descriptor[i].width = *ptemp;
        ptemp++;
        img->descriptor[i].height = *ptemp;
        ptemp++;
        img->descriptor[i].coding_scheme = *ptemp;
        ptemp++;

        img->descriptor[i].fileid_of_data = *ptemp;
        img->descriptor[i].fileid_of_data <<= 8;
        ptemp++;
        img->descriptor[i].fileid_of_data |= *ptemp;
        ptemp++;

        img->descriptor[i].offset_of_data = *ptemp;
        img->descriptor[i].offset_of_data <<= 8;
        ptemp++;
        img->descriptor[i].offset_of_data |= *ptemp;
        ptemp++;

        img->descriptor[i].len_of_data = *ptemp;
        img->descriptor[i].len_of_data <<= 8;
        ptemp++;
        img->descriptor[i].len_of_data |= *ptemp;
        ptemp++;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      The common function to decode the MCC and MNC for EFloci,EFplmnsel,
//    EFfplmn,EFlocigprs
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMCCMNC(
    const uint16 data_len,  //the length of the data to be decode
    void *data,             //in:the data buf that start with MCC and MNC information
    uint16 *ptr_mcc,        //out:the result MCC
    uint16 *ptr_mnc,        //out:the reuslt MNC
    uint16 *ptr_mnc_digit_num   //out:the MNC digit num,3 or 2
)
{
#define MCC_MNC_DATA_LEN    3       //the mcc/mnc data length
#define MAX_VALIDATE_DIGIT  9       //the max digit value

    uint8 mcc_digit_1, mcc_digit_2, mcc_digit_3 = 0;
    uint8 mnc_digit_1, mnc_digit_2, mnc_digit_3 = 0;
    uint8 *ptemp = (uint8 *)data;

    *ptr_mcc = 0;
    *ptr_mnc = 0;
    *ptr_mnc_digit_num = 3;

    if(data_len < MCC_MNC_DATA_LEN)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //get the all the digit from the data buffer
    mcc_digit_1 = *ptemp & 0x0F;
    mcc_digit_2 = (*ptemp & 0xF0) >> 4;
    ptemp += 1;
    mcc_digit_3 = *ptemp & 0x0F;
    mnc_digit_3 = (*ptemp & 0xF0) >> 4;
    ptemp += 1;
    mnc_digit_1 = *ptemp & 0x0F;
    mnc_digit_2 = (*ptemp & 0xF0) >> 4;

    //check the digit for validate
    if((mcc_digit_1 > MAX_VALIDATE_DIGIT) ||
        (mcc_digit_2 > MAX_VALIDATE_DIGIT) ||
        (mcc_digit_3 > MAX_VALIDATE_DIGIT) ||
        (mnc_digit_1 > MAX_VALIDATE_DIGIT) ||
        (mnc_digit_2 > MAX_VALIDATE_DIGIT) ||
        ((mnc_digit_3 > MAX_VALIDATE_DIGIT) &&
         (mnc_digit_3 != 0xF)))
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::decode mcc&mnc error."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_5178_112_2_17_22_58_34_4412, (uint8 *)"");
#endif
        *ptr_mnc_digit_num = 0;
        return SIM_SERVICE_OK;
    }

    //count the MCC
    *ptr_mcc = mcc_digit_1 * 100 + mcc_digit_2 * 10 + mcc_digit_3;

    //count the MNC according the digit number is 2 or 3
    if(mnc_digit_3 != 0xF)
    {
        *ptr_mnc = mnc_digit_1 * 100 + mnc_digit_2 * 10 + mnc_digit_3;
        *ptr_mnc_digit_num = 3;
    }
    else
    {
        *ptr_mnc = mnc_digit_1 * 10 + mnc_digit_2;
        *ptr_mnc_digit_num = 2;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EFIMSI_M of the cdma card
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCDMAIMSI(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef SIM_CDMA_CARD_SUPPORT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_IMSI_M_T *imsi_m = ef_mapping;
    uint16 m_11_12p = 0;
    uint16 mcc_mp = 0;
    uint16 d1, d2, d3 = 0;

    SCI_MEMSET(imsi_m, 0x0, sizeof(SIM_EF_MAPPING_IMSI_M_T));

    if(0 == data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMCPY(imsi_m->imsi_m_data, ptemp, 10);
    //structure: 00 AA AA BB BB BB CC 80 DD DD
    //CC is related to MNC, and DD DD is related to MCC
    m_11_12p = imsi_m->imsi_m_data[6];//CC = D1*10+D2-11 (D1,D2 change to 10 when it is 0)
    m_11_12p += 11;

    if(m_11_12p >= 100)
    {
        imsi_m->mnc = m_11_12p - 100;
    }
    else
    {
        imsi_m->mnc = m_11_12p;
    }

    //DD DD = D1*100+D2*10+D3-111 (D1,D2,D3 change to 10 when it is 0)
    mcc_mp = imsi_m->imsi_m_data[9];
    mcc_mp <<= 8;
    mcc_mp |= imsi_m->imsi_m_data[8];
    mcc_mp += 111;
    d1 = mcc_mp / 100;
    d2 = (mcc_mp - d1 * 100) / 10;
    d3 = mcc_mp - d1 * 100 - d2 * 10;
    mcc_mp = d1 * 100 + d2 * 10 + d3;

    if(0 == d3)
    {
        mcc_mp -= 10;
    }

    d1 = mcc_mp / 100;
    d2 = (mcc_mp - d1 * 100) / 10;

    if(0 == d2)
    {
        mcc_mp -= 100;
    }

    d1 = mcc_mp / 100;

    if(0 == d1)
    {
        mcc_mp -= 1000;
    }

    imsi_m->mcc = mcc_mp;

    SCI_TRACE_LOW("SIM CDMA imsi len=%d mcc=%d mnc=%d, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x", \
                  data_len, imsi_m->mcc, imsi_m->mnc, \
                  imsi_m->imsi_m_data[0], imsi_m->imsi_m_data[1], imsi_m->imsi_m_data[2], imsi_m->imsi_m_data[3], imsi_m->imsi_m_data[4], \
                  imsi_m->imsi_m_data[5], imsi_m->imsi_m_data[6], imsi_m->imsi_m_data[7], imsi_m->imsi_m_data[8], imsi_m->imsi_m_data[9]);

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_ERROR_UNKNOW;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
//    refer to  CPHS Phase 2  B.4.1.2
//
  Bytes |Description|M/O| Length  |
 _______|___________|___|_________|
 1 to n |  PLMN name| M | n Bytes |
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeOPNameStr(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp;
    SIM_EF_MAPPING_ONSTRING_T *onstring;

    ptemp = data;
    onstring = ef_mapping;

    // SCI_TRACE_LOW("SIM:: DecodeOPNameStr, data_len is %d ", data_len);

    if(data_len > SIM_EF_MAX_ONSTRING_LEN)
    {
        //SCI_TRACE_LOW("SIM:: the operator string name length beyond the max len! ");
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    onstring->len = (uint8)data_len;

    //decode the string,
    SCI_MEMCPY(onstring->onstring, ptemp, onstring->len);

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
//    refer to  CPHS Phase 2  B.4.1.2

  Bytes |   Description         |M/O| Length   |
________|_______________________|___|__________|
 1 to 10|Operator Name Shortform| M | 10 Bytes |
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeOPNameShtf(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *ptemp;
    SIM_EF_MAPPING_ONSHORT_T *onshort;

    ptemp = data;
    onshort = ef_mapping;


    //   SCI_TRACE_LOW("SIM:: DecodeOPNameShtf, data_len is %d ", data_len);

    if(data_len > SIM_EF_MAX_ONSHORTFORM_LEN)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //decode the string,
    SCI_MEMCPY(onshort->onshort, ptemp, SIM_EF_MAX_ONSHORTFORM_LEN);

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
//    refer to  CPHS Phase 2  B.4.1.2

  Bytes    |  Description     |M/O|Length|
  _________|__________________|___|______|
   1       |CPHS Phase        | M |  1   |
  _________|__________________|___|______|
  2-3      |CPHS Service Table| M |  2   |
  _________|__________________|___|______|
  4-n      |RFU               | O | n-3  |
-----------------------------------------|

   01: phase1   02: phase2

   bit8    | bit7 | bit6 | bit5   | bit4 | bit3 |  bit2  |   bit1   |
 __________|______|______|________|______|______|________|__________|
 OpName Shortform |Mailbox Numbers|      SST    |        CSP        |
 _________________|_______________|_____________|___________________|
        RFU       |     RFU       |      RFU    |Information numbers|
------------------|---------------|-------------|-------------------|

***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCPHSInfo(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of the struct that decode out
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_CPHSINFO_T *cphs_info;

    temp_ptr = data;
    cphs_info = ef_mapping;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM:: DecodeCPHSInfo, data_len is %d, 0x%x 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_5360_112_2_17_22_58_35_4413, (uint8 *)"dddd", data_len, *temp_ptr, *(temp_ptr + 1), *(temp_ptr + 2));
#endif

    //decode the CPHS phase
    if((1 == *temp_ptr) || (2 == *temp_ptr))
    {
        cphs_info->cphs_phase = (SIM_CPHS_PHASE_E) * temp_ptr;
        temp_ptr++;
    }
    else
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //decode the indicators
    SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 2, cphs_info->cphs_service_table.csp)
    SIM_GET_DATA_BETWEEN((*temp_ptr), 2, 2, cphs_info->cphs_service_table.sst)
    SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 2, cphs_info->cphs_service_table.mailboxnum)
    SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 2, cphs_info->cphs_service_table.opnameshort)

    if(data_len > 2)
    {
        temp_ptr++;
        SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 2, cphs_info->cphs_service_table.infonum)
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
//    refer to  CPHS Phase 2  B.4.2.3

  Bytes    |          Description                    |M/O|Length|
  _________|_________________________________________|___|______|
   1       |Voice Message Waiting Indicator flags    | M |  1   |
  _________|_________________________________________|___|______|
   2       |Fax/Data Message Waiting Indicator flags | O |  1   |
  _________|_________________________________________|___|______|
  3-n      |RFU                                      | O | n-2  |
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeVoiceMsgWtFlag(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of the struct that decode out
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_VMWFLAG_T *voice_waiting_flag;

    temp_ptr = data;
    voice_waiting_flag = ef_mapping;
    g_sim_r4_vm_vmwflag_len[cur_sim_task] = data_len;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM:: DecodeVoiceMsgWtFlag, data_len is %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_5418_112_2_17_22_58_35_4414, (uint8 *)"d", data_len);
#endif

    if(data_len < 1)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //decode the flags
    SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 4, voice_waiting_flag->line2_status)
    SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 4, voice_waiting_flag->line1_status)

    voice_waiting_flag->is_datafax_flag_present = 0;

    //存在data/fax等待消息的flag
    if(data_len >= 2)
    {
        voice_waiting_flag->is_datafax_flag_present = 1;
        temp_ptr = temp_ptr + 1;
        SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 4, voice_waiting_flag->data_status)
        SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 4, voice_waiting_flag->fax_status)
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
//    refer to  CPHS Phase 2  B.4.2.2

  Bytes    |          Description              |M/O| Length |
___________|___________________________________|___|________|
 1 to X    |Alpha Identifier                   | O |X Byte  |
___________|___________________________________|___|________|
  X+1      |Length of BCD number contents      | M |1 Byte  |
___________|___________________________________|___|________|
  X+2      |TON and NPI                        | M |1 Byte  |
___________|___________________________________|___|________|
X+3 to X+12|Mailbox Number string              | M |10 Bytes|
___________|___________________________________|___|________|
  X+13     |Capability/Configuration Identifier| M |1 Byte  |
___________|___________________________________|___|________|
  X+14     |Extension1 Record Identifier       | M |1 Byte  |

***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMailboxnum(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of the struct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    uint8 *temp_ptr;
    SIM_EF_MAPPING_MBNUM_T  *mailbox_num;

    temp_ptr = data;
    mailbox_num = ef_mapping;

    // 04/24/2008 delete the trace
    //  SCI_TRACE_LOW("SIM:: call func DecodeMailboxnum() ,data_len is %d ", data_len);

    if(data_len < 14)
    {
        ser_result = SIM_SERVICE_DECODE_EF_ERROR;
        return ser_result;
    }

    //record the data_len from the select file for the latter updating file
    g_sim_cphs_vm_record_len[cur_sim_task] = (uint8)data_len;

    //the alpha id
    mailbox_num->alpha_id_len = data_len - 14;

    if(mailbox_num->alpha_id_len > 0)
    {
        if(mailbox_num->alpha_id_len <= SIM_MAX_DN_ALPHA_IDENTIFY_LEN)
        {
            memcpy(mailbox_num->alpha_id, temp_ptr, mailbox_num->alpha_id_len);
            temp_ptr += mailbox_num->alpha_id_len;
        }
        else
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::the alpha id len is bigger than 20."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_5507_112_2_17_22_58_35_4415, (uint8 *)"");
#endif
            memcpy(mailbox_num->alpha_id, temp_ptr, SIM_MAX_DN_ALPHA_IDENTIFY_LEN);
            temp_ptr += mailbox_num->alpha_id_len;
            mailbox_num->alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
        }
    }

    //get the mailbox number length
    if((*temp_ptr > 0) && (*temp_ptr <=  MN_MAX_ADDR_BCD_LEN))
    {
        mailbox_num->mailbox_number_len = *temp_ptr - 1;
    }
    else
    {
        mailbox_num->mailbox_number_len = 0;
    }

    temp_ptr += 1;

    //the TON and the NPI
    if(UINT8_INVALIDE != *temp_ptr)
    {
        mailbox_num->is_ton_npi_exist = TRUE;
        mailbox_num->ton = ((*temp_ptr) & SIM_EF_DN_TON_MARK) >> 4;
        mailbox_num->npi = (*temp_ptr) & SIM_EF_DN_NPI_MARK;
    }
    else
    {
        mailbox_num->is_ton_npi_exist = FALSE;
    }

    temp_ptr += 1;

    //the mailbox number
    SCI_MEMCPY(mailbox_num->mailbox_number, temp_ptr, SIM_EF_DN_DN_LEN);
    temp_ptr += SIM_EF_DN_DN_LEN;

    //the Capability/Configuration Identifier
    if((UINT8_INVALIDE != *temp_ptr)
        && (0 != *temp_ptr))
    {
        mailbox_num->is_ccp_exist = TRUE;
        mailbox_num->ccp_id = *temp_ptr;
    }
    else
    {
        mailbox_num->is_ccp_exist = FALSE;
    }

    temp_ptr += 1;

    //the Extension1 Record Identifier
    if((UINT8_INVALIDE != *temp_ptr)
        && (0 != *temp_ptr))
    {
        mailbox_num->is_ext_exist = TRUE;
        mailbox_num->ext_id = *temp_ptr;
    }
    else
    {
        mailbox_num->is_ext_exist = FALSE;
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:King.li
//    Note: new added for CPHS extended function
//    refer to  CPHS Phase 2  B.4.6.2
**********************************************************************/
static SIM_SERVICE_RESULT_E DecodeInformationnum(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef _CPHS_SUPPORT
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    uint8 *temp_ptr;
    SIM_EF_MAPPING_INFONUM_T  *information_num;

    temp_ptr = data;
    information_num = ef_mapping;

    // 04/24/2008 delete the trace
    //  SCI_TRACE_LOW("SIM:: call func DecodeInformationnum() ,data_len is %d ", data_len);
    /*the Alpha length field*/
    information_num->alpha_id_len = *temp_ptr;
    information_num->information_digits_number_len = data_len - 5 - (information_num->alpha_id_len);
    temp_ptr += 1;
    /*the Entry Status field*/
    information_num->entry_status = *temp_ptr;
    temp_ptr += 1;

    /*the Alpha identifier field*/
    if(information_num->alpha_id_len <= SIM_MAX_DN_ALPHA_IDENTIFY_LEN)
    {
        memcpy(information_num->alpha_id, temp_ptr, information_num->alpha_id_len);
        temp_ptr += information_num->alpha_id_len;
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::the alpha id len is bigger than 20."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_5611_112_2_17_22_58_35_4416, (uint8 *)"");
#endif
        memcpy(information_num->alpha_id, temp_ptr, SIM_MAX_DN_ALPHA_IDENTIFY_LEN);
        temp_ptr += information_num->alpha_id_len;
        information_num->alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    }

    /*the BCD Number length field*/
    information_num->number_len = *temp_ptr;
    temp_ptr += 1;

    /*the TON & NPI field*/
    if(UINT8_INVALIDE != *temp_ptr)
    {
        information_num->is_ton_npi_exist = TRUE;
        information_num->ton = ((*temp_ptr) & SIM_EF_DN_TON_MARK) >> 4;
        information_num->npi = (*temp_ptr) & SIM_EF_DN_NPI_MARK;
    }
    else
    {
        information_num->is_ton_npi_exist = FALSE;
    }

    temp_ptr += 1;

    /*the Digits section field*/
    if(information_num->information_digits_number_len <= SIM_MAX_DN_ALPHA_IDENTIFY_LEN)
    {
        memcpy(information_num->information_digits_number, temp_ptr, information_num->information_digits_number_len);
        temp_ptr += information_num->information_digits_number_len;
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::the alpha id len is bigger than 20."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_5641_112_2_17_22_58_35_4417, (uint8 *)"");
#endif
        memcpy(information_num->information_digits_number, temp_ptr, SIM_MAX_DN_ALPHA_IDENTIFY_LEN);
        temp_ptr += information_num->information_digits_number_len;
        information_num->information_digits_number_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    }

    /*the extention id field*/
    if((UINT8_INVALIDE != *temp_ptr)
        && (0 != *temp_ptr))
    {
        information_num->is_ext_exist = TRUE;
        information_num->ext_id = *temp_ptr;
    }
    else
    {
        information_num->is_ext_exist = FALSE;
    }

    return ser_result;
#else
    return SIM_SERVICE_NOT_ENABLE;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:King.li
//    Note: new added for CPHS extended function
//    refer to  CPHS Phase 2  B.4.6.2
**********************************************************************/
static SIM_SERVICE_RESULT_E DecodeSSTCPHS(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    return SIM_SERVICE_NOT_ENABLE;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
//    refer to  CPHS Phase 2  B.4.7

  BYTE A    |                                                       BYTE B                                            |
____________|_________________________________________________________________________________________________________|
service code|service           | bit8    | bit7 |   bit6  |   bit5    |  bit4   |   bit3    |   bit2        |  bit1   |
____________|__________________|_________|______|_________|___________|_________|___________|_______________|_________|
      01    |Call offering     | CFU     |CFB   |CFNRy    |CFNRc      |CT       |           |               |         |
      02    |call restriction  | BOAC    |BOIC  |Boic exHC|BAIC       |BIC Roam |           |               |         |
      03    |other suppp serv  | MPTY    |CUG   |AoC      |Pref.CUG   |CUG OA   |           |               |         |
      04    |call completion   | HOLD    |CW    |CCBS     |User sig   |         |           |               |         |
      05    |teleservices      | SM/MT   |SM/MO |SM/CB    |Reply path |Del.Conf |Protocol ID|Validity Period|         |
      06    |CPHS Teleservices | ALS     |      |         |           |         |           |               |         |
      07    |Cphs Feature      |Reserved |      |         |           |         |           |               |         |
      08    |Number identif    | CLIP    |Reserv|CoLP     |CoLP       |MCI      |           |CLI send       |CLI block|
      09    |Phase 2+ services | GPRS    |HSCSD |Voic Brd |Voic brdcst|Mlti subs|Multi Band |               |         |
     _______|__________________|_________|______|_________|___________|_________|___________|_______________|_________|
      C0    |ValueAdded Servic |PLMN MODE|VPS   |SM/MO PAG|SM/MO Email|Fax      |   data    |Reserved       |Language |
     _______|__________________|_________|______|_________|___________|_________|___________|_______________|_________|
      D5    |Information Number|            Information Numbers                                                       |
     _______|__________________|______________________________________________________________________________________|
we set the last two item as optional
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCSP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_CSP_T *csp;
    uint8 service_num;

    temp_ptr = data;
    csp = ef_mapping;

    // 04/24/2008 delete the trace
    //SCI_TRACE_LOW("SIM:: call func DecodeCSP(),data_len is %d ", data_len);

    csp->is_call_offering_present = FALSE;
    csp->is_call_restriction_present = FALSE;
    csp->is_cphs_teleserv_present = FALSE;
    csp->is_value_added_serv_present = FALSE;
    csp->is_info_numbers_present = FALSE;

    if(data_len < 18)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    service_num = data_len / 2;

    while(service_num >= 1)
    {
        switch(*temp_ptr)
        {
            case SIM_CALL_OFFERING:
                csp->is_call_offering_present = TRUE;
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->call_offering.is_cfu)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->call_offering.is_cfb)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->call_offering.is_cfnry)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->call_offering.is_cfnrc)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->call_offering.is_ct)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_CALL_RESTRICTION:
                csp->is_call_restriction_present = TRUE;
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->call_restriction.is_boac)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->call_restriction.is_boic)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->call_restriction.is_boic_exhc)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->call_restriction.is_baic)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->call_restriction.is_bic_roam)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_OTHER_SERVICES:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->other_supp_serv.is_mpty)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->other_supp_serv.is_cug)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->other_supp_serv.is_aoc)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->other_supp_serv.is_pref_cug)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->other_supp_serv.is_cug_oa)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_CALL_COMPLETION:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->call_completion.is_hold)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->call_completion.is_cw)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->call_completion.is_ccbs)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->call_completion.is_u2u_signalling)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_TELESERVICES:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->teleservices.is_sm_mt)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->teleservices.is_sm_mo)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->teleservices.is_sm_cb)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->teleservices.is_reply_path)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->teleservices.is_del_conf)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 2, 1, csp->teleservices.is_protocol_id)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 1, 1, csp->teleservices.is_validity_period)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_CPHS_TELE:
                csp->is_cphs_teleserv_present = TRUE;
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->cphs_teleserv.is_als)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_CPHS_FEATURES:
                temp_ptr = temp_ptr + 2;
                service_num--;
                break;
            case SIM_NUM_ID:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->number_identif.is_clip)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->number_identif.is_colr)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->number_identif.is_colp)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->number_identif.is_mci)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 1, 1, csp->number_identif.is_cli_send)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 1, csp->number_identif.is_cli_block)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_PHASE2_SERVICE:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->phase_service.is_gprs)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->phase_service.is_hscsd)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->phase_service.is_vgc)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->phase_service.is_vbs)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->phase_service.is_msp)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 2, 1, csp->phase_service.is_multi_band)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_VALUEADDED_SERV:
                temp_ptr = temp_ptr + 1;
                csp->is_value_added_serv_present = TRUE;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->value_added_service.is_plmn_mode)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->value_added_service.is_vps)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->value_added_service.is_sm_mo_paging)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->value_added_service.is_sm_mo_email)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->value_added_service.is_fax)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 2, 1, csp->value_added_service.is_data)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 1, csp->value_added_service.is_language)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_INFO_NUM:
                temp_ptr = temp_ptr + 1;
                csp->is_info_numbers_present = TRUE;
                csp->info_numbers = *temp_ptr;
                service_num--;
                break;

            default:
                //SCI_TRACE_LOW("There are unrecognized service code when decode CSP!");
                return SIM_SERVICE_OK;
        }
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
//    refer to  CPHS Phase 2  B.4.5
                                                 |   4 bits    |     4 bits   |
_________________________________________________|_____________|______________|
byte1(Voice Call forward unconditional flags)    |line1  status|line2  status |
_________________________________________________|_____________|______________|
byte2(Data/fax ....  (Optional))                 | fax   status| data  status |

***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCCFlag(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_CFFLAG_T *callfrd_flag;

    temp_ptr = data;
    callfrd_flag = ef_mapping;


    // 04/24/2008 delete the trace
    //     SCI_TRACE_LOW("SIM:: call func DecodeCCFlag() ");
    if(data_len < 1)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //decode the flags
    SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 4, callfrd_flag->line1_callfrd_uncond)
    SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 4, callfrd_flag->line2_callfrd_uncond)

    callfrd_flag->is_datafax_flag_present = 0;

    //there is a data/fax service flag
    if(data_len >= 2)
    {
        callfrd_flag->is_datafax_flag_present = 1;
        temp_ptr = temp_ptr + 1;
        SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 4, callfrd_flag->fax_callfrd_uncond)
        SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 4, callfrd_flag->data_callfrd_uncond)
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function decode the EF PLMNWACT content to the corresponding file information structure.
//  According to GSM11.11 v840.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodePLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_PLMNWACT_T *plmnwact = ef_mapping;
    uint8 *ptemp = data;
    uint16 i = 0;
    uint16 j = 0;
    uint16 len = data_len;
    uint16 mcc, mnc, mnc_digit_num;

    //the data len must be more than 5 bytes and must be 5's mutliple.
    if(data_len < SIM_EF_PLMNWACT_ARR_SIZE)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    len = data_len % SIM_EF_PLMNWACT_ARR_SIZE;
    len = data_len - len;

    SCI_MEMSET(plmnwact, 0x0, sizeof(SIM_EF_MAPPING_PLMNWACT_T));

    //count the max num of EFplmnwact
    plmnwact->max_num = (uint8)(len / SIM_EF_PLMNWACT_ARR_SIZE);

    if(plmnwact->max_num > SIM_EF_PLMNWACT_ARR_NUM)
    {
        plmnwact->max_num = SIM_EF_PLMNWACT_ARR_NUM;
    }

    for(i = 0; i < (len / SIM_EF_PLMNWACT_ARR_SIZE); i++)
    {
        if(i >= SIM_EF_PLMNWACT_ARR_NUM)
        {
            break;
        }

        if((0xFF == ptemp[0]) && (0xFF == ptemp[1]) && (0xFF == ptemp[2]))
        {
            ptemp += 5;
            continue;
        }

#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM%d:DecodePLMNWACT i=%d, 0x%x 0x%x 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_5960_112_2_17_22_58_36_4418, (uint8 *)"ddddd", cur_sim_task, i, *ptemp, *(ptemp + 1), *(ptemp + 2));
#endif

        ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return SIM_SERVICE_DECODE_EF_ERROR;
        }
        else
        {
            plmnwact->plmn[j].plmn.mcc = mcc;
            plmnwact->plmn[j].plmn.mnc = mnc;
            plmnwact->plmn[j].plmn.mnc_digit_num = mnc_digit_num;
            ptemp += 3;

            if(0x80 == (*ptemp & 0x80))
            {
                plmnwact->plmn[j].utran_flag = TRUE;
            }

            if(0x40 == (*ptemp & 0x40))
            {
                plmnwact->plmn[j].e_utran_flag = TRUE;
            }

            ptemp++;

            if(0x80 == (*ptemp & 0x80))
            {
                plmnwact->plmn[j].gsm_flag = TRUE;
            }

            if(0x40 == (*ptemp & 0x40))
            {
                plmnwact->plmn[j].compact_gsm_flag = TRUE;
            }

            if(0x20 == (*ptemp & 0x20))
            {
                plmnwact->plmn[j].cdma2000_hrpd_flag = TRUE;
            }

            if(0x10 == (*ptemp & 0x10))
            {
                plmnwact->plmn[j].cdma2000_1xrtt_flag = TRUE;
            }

            ptemp++;
            plmnwact->validate_num++;
            j++;

            if(j >= SIM_EF_PLMNWACT_ARR_NUM)
            {
                break;
            }
        }
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function decode the EF OPLMNWACT content to the corresponding file information structure.
//  According to GSM11.11 v840.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeOPLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_OPLMNWACT_T *oplmnwact = ef_mapping;
    uint8 *ptemp = data;
    uint16 i = 0;
    uint16 j = 0;
    uint16 len = data_len;
    uint16 mcc, mnc, mnc_digit_num;

    //the data len must be more than 5 bytes and must be 5's mutliple.
    if(data_len < SIM_EF_OPLMNWACT_ARR_SIZE)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    // We found a 500 bytes oplmnwact, SIM lower layer will send at most 256 bytes data.
    // For decoding, we shorten some data...
    len = data_len % SIM_EF_OPLMNWACT_ARR_SIZE;
    len = data_len - len;

    SCI_MEMSET(oplmnwact, 0x0, sizeof(SIM_EF_MAPPING_OPLMNWACT_T));

    //count the max num
    oplmnwact->max_num = (uint8)(len / SIM_EF_OPLMNWACT_ARR_SIZE);

    if(oplmnwact->max_num > SIM_EF_OPLMNWACT_ARR_NUM)
    {
        oplmnwact->max_num = SIM_EF_OPLMNWACT_ARR_NUM;
    }

    for(i = 0; i < oplmnwact->max_num; i++)
    {
        if(i >= SIM_EF_OPLMNWACT_ARR_NUM)
        {
            break;
        }

        if((0xFF == ptemp[0]) && (0xFF == ptemp[1]) && (0xFF == ptemp[2]))
        {
            ptemp += 5;
            continue;
        }

#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM%d:DecodeOPLMNWACT i=%d, 0x%x 0x%x 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_6065_112_2_17_22_58_36_4419, (uint8 *)"ddddd", cur_sim_task, i, *ptemp, *(ptemp + 1), *(ptemp + 2));
#endif
        ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return SIM_SERVICE_DECODE_EF_ERROR;
        }
        else
        {
            oplmnwact->plmn[j].plmn.mcc = mcc;
            oplmnwact->plmn[j].plmn.mnc = mnc;
            oplmnwact->plmn[j].plmn.mnc_digit_num = mnc_digit_num;
            ptemp += 3;

            if(0x80 == (*ptemp & 0x80))
            {
                oplmnwact->plmn[j].utran_flag = TRUE;
            }

            if(0x40 == (*ptemp & 0x40))
            {
                oplmnwact->plmn[j].e_utran_flag = TRUE;
            }

            ptemp++;

            if(0x80 == (*ptemp & 0x80))
            {
                oplmnwact->plmn[j].gsm_flag = TRUE;
            }

            if(0x40 == (*ptemp & 0x40))
            {
                oplmnwact->plmn[j].compact_gsm_flag = TRUE;
            }

            if(0x20 == (*ptemp & 0x20))
            {
                oplmnwact->plmn[j].cdma2000_hrpd_flag = TRUE;
            }

            if(0x10 == (*ptemp & 0x10))
            {
                oplmnwact->plmn[j].cdma2000_1xrtt_flag = TRUE;
            }

            ptemp++;
            oplmnwact->validate_num++;
            j++;

            if(j >= SIM_EF_OPLMNWACT_ARR_NUM)
            {
                break;
            }
        }
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function decode the EF HPLMNACT content to the corresponding file information structure.
//  According to GSM11.11 v840.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeHPLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_HPLMNWACT_T *hplmnwact = ef_mapping;
    uint8 *ptemp = data;
    uint16 i = 0;
    uint16 j = 0;
    uint16 len = data_len;
    uint16 mcc, mnc, mnc_digit_num;

    //the data len must be more than 5 bytes and must be 5's mutliple.
    if(data_len < SIM_EF_HPLMNWACT_ARR_SIZE)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    len = data_len % SIM_EF_HPLMNWACT_ARR_SIZE;
    len = data_len - len;

    SCI_MEMSET(hplmnwact, 0x0, sizeof(SIM_EF_MAPPING_HPLMNWACT_T));

    for(i = 0; i < (len / SIM_EF_HPLMNWACT_ARR_SIZE); i++)
    {
        if(i >= SIM_EF_HPLMNWACT_ARR_NUM)
        {
            break;
        }

        if((0xFF == ptemp[0]) && (0xFF == ptemp[1]) && (0xFF == ptemp[2]))
        {
            ptemp += 5;
            continue;
        }

        ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return SIM_SERVICE_DECODE_EF_ERROR;
        }
        else
        {
            hplmnwact->plmn[j].plmn.mcc = mcc;
            hplmnwact->plmn[j].plmn.mnc = mnc;
            hplmnwact->plmn[j].plmn.mnc_digit_num = mnc_digit_num;
            ptemp += 3;

            if(0x80 == (*ptemp & 0x80))
            {
                hplmnwact->plmn[j].utran_flag = TRUE;
            }

            if(0x40 == (*ptemp & 0x40))
            {
                hplmnwact->plmn[j].e_utran_flag = TRUE;
            }

            ptemp++;

            if(0x80 == (*ptemp & 0x80))
            {
                hplmnwact->plmn[j].gsm_flag = TRUE;
            }

            if(0x40 == (*ptemp & 0x40))
            {
                hplmnwact->plmn[j].compact_gsm_flag = TRUE;
            }

            if(0x20 == (*ptemp & 0x20))
            {
                hplmnwact->plmn[j].cdma2000_hrpd_flag = TRUE;
            }

            if(0x10 == (*ptemp & 0x10))
            {
                hplmnwact->plmn[j].cdma2000_1xrtt_flag = TRUE;
            }

            ptemp++;
            hplmnwact->validate_num++;
            j++;

            if(j >= SIM_EF_HPLMNWACT_ARR_NUM)
            {
                break;
            }
        }
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function decode the EF CPBCCH content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCPBCCH(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
#ifdef _SUPPORT_GPRS_
    SIM_EF_MAPPING_CPBCCH_T *cpbcch = ef_mapping;
    uint8 *ptemp = data;
    uint16 len = data_len / SIM_EF_CPBCCH_ELEMENT_SIZE;

    len = len > SIM_EF_CPBCCH_ARR_NUM ? SIM_EF_CPBCCH_ARR_NUM : len;
    SCI_MEMCPY(cpbcch, ptemp, (len * SIM_EF_CPBCCH_ELEMENT_SIZE));
#endif
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//    the function decode the EF INVSCAN content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeINVSCAN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_INVSCAN_T *invscan = ef_mapping;
    uint8 *ptemp = data;

    invscan->invscan_flag = *ptemp;
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//    the function decode the EF ECCP content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeECCP(   //return value:the result of decode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_ECCP_T *eccp = ef_mapping;
    uint8 *ptemp = data;

    if(data_len < SIM_EF_ECCP_MANDATORY_LEN)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }
    else
    {
        eccp->bc_info_len = data_len > SIM_EF_ECCP_MAX_LEN ? SIM_EF_ECCP_MAX_LEN : data_len;
        SCI_MEMCPY(eccp->bc_info, ptemp, eccp->bc_info_len);
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//    the function decode the EF CMI content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeCMI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_CMI_T *cmi = ef_mapping;
    uint8 *ptemp = data;
    int16 alpha_len = 0;

    alpha_len = data_len - SIM_EF_CMI_CMI_LEN;

    if(alpha_len >= 1)
    {
        cmi->alpha_id_len = alpha_len > SIM_MAX_DN_ALPHA_IDENTIFY_LEN ? SIM_MAX_DN_ALPHA_IDENTIFY_LEN : alpha_len;
        SCI_MEMCPY(cmi->alpha_id, ptemp, cmi->alpha_id_len);
        ptemp += alpha_len;
        cmi->cmi = *ptemp;
        return SIM_SERVICE_OK;
    }
    else
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//    the function decode the EF MExEST content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeMEXEST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_MEXEST_T *mexest = ef_mapping;
    uint16 i, len = 0;
    uint8 *ptemp = data;

    SCI_MEMSET(mexest, 0x0, sizeof(SIM_EF_MAPPING_MEXEST_T));
    len = 4 * data_len;

    for(i = 0; i < len;)
    {
        //decode the first service status in a byte
        mexest->service_table[i].allocated = (BOOLEAN)((*ptemp & 0x01) != 0);
        mexest->service_table[i].activated = (BOOLEAN)((*ptemp & 0x02) != 0);
        mexest->service_num++;
        i++;

        if(i >= len || i >= SIMSER_MEXE_NUM)
        {
            return SIM_SERVICE_OK;
        }

        //decode the second service status in a byte
        mexest->service_table[i].allocated = (BOOLEAN)((*ptemp & 0x04) != 0);
        mexest->service_table[i].activated = (BOOLEAN)((*ptemp & 0x08) != 0);
        mexest->service_num++;
        i++;

        if(i >= len || i >= SIMSER_MEXE_NUM)
        {
            return SIM_SERVICE_OK;
        }

        //decode the third service status in a byte
        mexest->service_table[i].allocated = (BOOLEAN)((*ptemp & 0x10) != 0);
        mexest->service_table[i].activated = (BOOLEAN)((*ptemp & 0x20) != 0);
        mexest->service_num++;
        i++;

        if(i >= len || i >= SIMSER_MEXE_NUM)
        {
            return SIM_SERVICE_OK;
        }

        //decode the fourth service status in a byte
        mexest->service_table[i].allocated = (BOOLEAN)((*ptemp & 0x40) != 0);
        mexest->service_table[i].activated = (BOOLEAN)((*ptemp & 0x80) != 0);
        mexest->service_num++;
        i++;

        if(i >= len || i >= SIMSER_MEXE_NUM)
        {
            return SIM_SERVICE_OK;
        }

        ptemp += 1;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//    the function decode the EF ORPK content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeORPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_ORPK_T *orpk = ef_mapping;
    uint8 *ptemp = data;
    uint8 len = 0;

    if(data_len - SIM_EF_ORPK_MANDATORY_LEN > SIM_EF_ORPK_KEYID_MAX_LEN)
    {
        len = SIM_EF_ORPK_KEYID_MAX_LEN + SIM_EF_ORPK_MANDATORY_LEN;
        SCI_MEMCPY(orpk, ptemp, len);
        orpk->key_len = SIM_EF_ORPK_KEYID_MAX_LEN;
    }
    else
    {
        SCI_MEMCPY(orpk, ptemp, data_len);
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//    the function decode the EF ARPK content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeARPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_ARPK_T *arpk = ef_mapping;
    uint8 *ptemp = data;
    uint8 len = 0;

    if(data_len - SIM_EF_ARPK_MANDATORY_LEN > SIM_EF_ARPK_KEYID_MAX_LEN)
    {
        len = SIM_EF_ARPK_KEYID_MAX_LEN + SIM_EF_ARPK_MANDATORY_LEN;
        SCI_MEMCPY(arpk, ptemp, len);
        arpk->key_len = SIM_EF_ARPK_KEYID_MAX_LEN;
    }
    else
    {
        SCI_MEMCPY(arpk, ptemp, data_len);
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//    the function decode the EF TPRPK content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeTPRPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_TPRPK_T *tprpk = ef_mapping;
    uint8 *ptemp = data;
    uint8 len = 0;

    //Parameters indicator
    tprpk->para_indicator = *ptemp;
    ptemp++;
    //Flags
    tprpk->flags = *ptemp;
    ptemp++;
    //type of certificate
    tprpk->type = *ptemp;
    ptemp++;
    //key/certificate file id
    tprpk->key_file_id = *ptemp;
    ptemp++;
    tprpk->para_indicator <<= 8;
    tprpk->para_indicator |= *ptemp;
    ptemp++;
    //offset into key/certificate file id
    tprpk->offset_into_file = *ptemp;
    ptemp++;
    tprpk->offset_into_file <<= 8;
    tprpk->offset_into_file |= *ptemp;
    ptemp++;
    //length of key/certificate data
    tprpk->data_len = *ptemp;
    ptemp++;
    tprpk->data_len <<= 8;
    tprpk->data_len |= *ptemp;
    ptemp++;
    //key identifier length
    len = *ptemp;
    ptemp++;
    //key identifier
    tprpk->key_len = len > SIM_EF_TPRPK_KEYID_MAX_LEN ? SIM_EF_TPRPK_KEYID_MAX_LEN : len;
    SCI_MEMCPY(tprpk->key_id, ptemp, tprpk->key_len);
    ptemp += len;
    //certificate identifier length
    len = *ptemp;
    ptemp++;
    //certificate identifier
    tprpk->cer_len = len > SIM_EF_TPRPK_CERID_MAX_LEN ? SIM_EF_TPRPK_CERID_MAX_LEN : len;
    SCI_MEMCPY(tprpk->key_id, ptemp, tprpk->cer_len);

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//    the function decode the EF ARR content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeARR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    //return SIM_SERVICE_OK;
    return SIM_SERVICE_DECODE_EF_ERROR;
}

/**********************************************************************
//    Description:
//    the function decode the EF UST content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMUST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef SIM_3G_USIM_SUPPORT
    uint8 *ptemp = data;//*data is 3G USTformat
    SIM_EF_MAPPING_SST_T *sst = ef_mapping;//re-map 3G UST to 2G SST

    SCI_MEMSET(sst, 0, sizeof(SIM_EF_MAPPING_SST_T)); //clear

    if(0 == data_len) // M has at least 1 bytes
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //SCI_TRACE_LOW:"SIM%d UST len %d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_6573_112_2_17_22_58_37_4420, (uint8 *)"ddddddddddddddddd", cur_sim_task, data_len, (*ptemp), (*(ptemp + 1)), (*(ptemp + 2)), (*(ptemp + 3)), (*(ptemp + 4)), (*(ptemp + 5)), (*(ptemp + 6)), (*(ptemp + 7)), (*(ptemp + 8)), (*(ptemp + 9)), (*(ptemp + 10)), (*(ptemp + 11)), (*(ptemp + 12)), (*(ptemp + 13)), (*(ptemp + 14)));

    sst->service_table[SIMSER_PH2_CHV1_DISABLE].allocated = TRUE;// 2G SIM always support
    sst->service_table[SIMSER_PH2_CHV1_DISABLE].activated = TRUE;

    //[wuding] We will decide the ADN and EXT1 after decode PBR later.

    if(BIT_1 == (*ptemp & BIT_1))
    {
        sst->service_table[SIMSER_PH2_FDN].allocated = TRUE;
        sst->service_table[SIMSER_PH2_FDN].activated = TRUE;
    }

    if(data_len >= 2) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_1 == (*(ptemp + 1) & BIT_1))
        {
            sst->service_table[SIMSER_PH2_SMS].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SMS].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 1) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_AOC].allocated = TRUE;
            sst->service_table[SIMSER_PH2_AOC].activated = TRUE;
        }
    }

    // 3G UST has no CCP but a CCP2...
    sst->service_table[SIMSER_PH2_CCP].allocated = FALSE;
    sst->service_table[SIMSER_PH2_CCP].activated = FALSE;

    // 3G UST has no PLMNsel
    //目前思路是，有EFplmnwact就不读EFplmnsel
    //如果USIM 没有EFplmnwact(Orange Usim),那就要去读DFgsm下EFplmnsel
    //处理是否妥当还要考虑，为过case先这样
    //为了确保这一思路，这里只好强行置为TRUE.
    //sst->service_table[SIMSER_PH2_PLMN_SEL].allocated = FALSE;
    //sst->service_table[SIMSER_PH2_PLMN_SEL].activated = FALSE;
    sst->service_table[SIMSER_PH2_PLMN_SEL].allocated = TRUE;
    sst->service_table[SIMSER_PH2_PLMN_SEL].activated = TRUE;

    if(data_len >= 3) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_4 == (*(ptemp + 2) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_MSISDN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_MSISDN].activated = TRUE;
        }
    }

    // 3G UST has no ext1 (EXT1 is associated with ADN)
    sst->service_table[SIMSER_PH2_EXT1].allocated = FALSE;
    sst->service_table[SIMSER_PH2_EXT1].activated = FALSE;

    if(BIT_2 == (*ptemp & BIT_2))
    {
        sst->service_table[SIMSER_PH2_EXT2].allocated = TRUE;
        sst->service_table[SIMSER_PH2_EXT2].activated = TRUE;
    }

    if(data_len >= 2) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_3 == (*(ptemp + 1) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_SMSP].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SMSP].activated = TRUE;
        }
    }

    sst->service_table[SIMSER_PH2_LND].allocated = TRUE;//set it true firstly,will check when get record info of DFtelecom/EFldn //bug639449
    sst->service_table[SIMSER_PH2_LND].activated = TRUE;//set it true firstly,will check when get record info of DFtelecom/EFldn //bug639449

    if(data_len >= 2) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_6 == (*(ptemp + 1) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_CBMI].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CBMI].activated = TRUE;
        }
    }

    if(data_len >= 3) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_0 == (*(ptemp + 2) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_GID1].allocated = TRUE;
            sst->service_table[SIMSER_PH2_GID1].activated = TRUE;
        }

        if(BIT_1 == (*(ptemp + 2) & BIT_1))
        {
            sst->service_table[SIMSER_PH2_GID2].allocated = TRUE;
            sst->service_table[SIMSER_PH2_GID2].activated = TRUE;
        }

        if(BIT_2 == (*(ptemp + 2) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_SPN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SPN].activated = TRUE;
        }
    }

    if(BIT_3 == (*ptemp & BIT_3))
    {
        sst->service_table[SIMSER_PH2_SDN].allocated = TRUE;
        sst->service_table[SIMSER_PH2_SDN].activated = TRUE;
    }

    if(BIT_4 == (*ptemp & BIT_4))
    {
        sst->service_table[SIMSER_PH2_EXT3].allocated = TRUE;
        sst->service_table[SIMSER_PH2_EXT3].activated = TRUE;
    }

    if(data_len >= 8) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_0 == (*(ptemp + 7) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_VGCS].allocated = TRUE;
            sst->service_table[SIMSER_PH2_VGCS].activated = TRUE;
        }

        if(BIT_1 == (*(ptemp + 7) & BIT_1))
        {
            sst->service_table[SIMSER_PH2_VBS].allocated = TRUE;
            sst->service_table[SIMSER_PH2_VBS].activated = TRUE;
        }
    }

    if(data_len >= 3) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_7 == (*(ptemp + 2) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_EMLPP].allocated = TRUE;
            sst->service_table[SIMSER_PH2_EMLPP].activated = TRUE;
        }
    }

    if(data_len >= 4) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_0 == (*(ptemp + 3) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_AAEM].allocated = TRUE;
            sst->service_table[SIMSER_PH2_AAEM].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 3) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_SMS_CB_DL].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SMS_CB_DL].activated = TRUE;
        }

        if(BIT_3 == (*(ptemp + 3) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_SMS_PP_DL].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SMS_PP_DL].activated = TRUE;
        }
    }

    // 3G UST has no this item
    sst->service_table[SIMSER_PH2_MENU_SEL].allocated = TRUE;
    sst->service_table[SIMSER_PH2_MENU_SEL].activated = TRUE;

    if(data_len >= 4) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_5 == (*(ptemp + 3) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_CALL_CON].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CALL_CON].activated = TRUE;
        }
    }

    // 3G UST  has no this item, we assume 3G always support STK
    sst->service_table[SIMSER_PH2_PROACTIVE_SIM].allocated = TRUE;
    sst->service_table[SIMSER_PH2_PROACTIVE_SIM].activated = TRUE;

    if(data_len >= 2) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_7 == (*(ptemp + 1) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_CBMIR].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CBMIR].activated = TRUE;
        }
    }

    if(BIT_5 == (*ptemp & BIT_5))
    {
        sst->service_table[SIMSER_PH2_BDN].allocated = TRUE;
        sst->service_table[SIMSER_PH2_BDN].activated = TRUE;
    }

    if(BIT_6 == (*ptemp & BIT_6))
    {
        sst->service_table[SIMSER_PH2_EXT4].allocated = TRUE;
        sst->service_table[SIMSER_PH2_EXT4].activated = TRUE;
    }

    if(data_len >= 5) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_3 == (*(ptemp + 4) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_DCK].allocated = TRUE;
            sst->service_table[SIMSER_PH2_DCK].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 4) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_CO_NL].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CO_NL].activated = TRUE;
        }
    }

    if(data_len >= 2) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_2 == (*(ptemp + 1) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_SMS_STATUS_REPORT].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SMS_STATUS_REPORT].activated = TRUE;
        }
    }

    if(data_len >= 7) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_7 == (*(ptemp + 6) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_NIA].allocated = TRUE;
            sst->service_table[SIMSER_PH2_NIA].activated = TRUE;
        }
    }

    if(data_len >= 4) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_6 == (*(ptemp + 3) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_SIM_CON_MO_SMS].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SIM_CON_MO_SMS].activated = TRUE;
        }
    }

    sst->service_table[SIMSER_PH2_GPRS].allocated = TRUE;
    sst->service_table[SIMSER_PH2_GPRS].activated = TRUE;
    sst->service_num++;

    if(data_len >= 3) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_5 == (*(ptemp + 2) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_IMAGE].allocated = TRUE;
            sst->service_table[SIMSER_PH2_IMAGE].activated = TRUE;
        }

        if(BIT_6 == (*(ptemp + 2) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_SOLSA].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SOLSA].activated = TRUE;
        }
    }

    // 3G UST  has no this item, and this item cannot be supported in many 2G SIM.
    sst->service_table[SIMSER_PH2_USSD_IN_CC].allocated = FALSE;
    sst->service_table[SIMSER_PH2_USSD_IN_CC].activated = FALSE;

    if(data_len >= 4) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_7 == (*(ptemp + 3) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_RUN_AT_COMMAND].allocated = TRUE;
            sst->service_table[SIMSER_PH2_RUN_AT_COMMAND].activated = TRUE;
        }
    }

    if(data_len >= 3) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_3 == (*(ptemp + 2) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_PLMN_WACT].allocated = TRUE;
            sst->service_table[SIMSER_PH2_PLMN_WACT].activated = TRUE;
        }
    }

    if(data_len >= 6) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_1 == (*(ptemp + 5) & BIT_1))
        {
            sst->service_table[SIMSER_PH2_OPLMN_WACT].allocated = TRUE;
            sst->service_table[SIMSER_PH2_OPLMN_WACT].activated = TRUE;
        }

        if(BIT_2 == (*(ptemp + 5) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_HPLMN_WACT].allocated = TRUE;
            sst->service_table[SIMSER_PH2_HPLMN_WACT].activated = TRUE;
        }
    }

    if(data_len >= 5) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_6 == (*(ptemp + 4) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_CPBCCH].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CPBCCH].activated = TRUE;
        }

        if(BIT_7 == (*(ptemp + 4) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_INV_SCAN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_INV_SCAN].activated = TRUE;
        }
    }

    // 3G UST has no this item but a CCP2 ....
    sst->service_table[SIMSER_PH2_EXT_CCP].allocated = FALSE;
    sst->service_table[SIMSER_PH2_EXT_CCP].activated = FALSE;

    if(data_len >= 6) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_0 == (*(ptemp + 5) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_MEXE].allocated = TRUE;
            sst->service_table[SIMSER_PH2_MEXE].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 5) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_PNN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_PNN].activated = TRUE;
        }

        if(BIT_5 == (*(ptemp + 5) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_OPL].allocated = TRUE;
            sst->service_table[SIMSER_PH2_OPL].activated = TRUE;
        }

        if(BIT_6 == (*(ptemp + 5) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_MDN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_MDN].activated = TRUE;
        }

        if(BIT_7 == (*(ptemp + 5) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_MWIS].allocated = TRUE;
            sst->service_table[SIMSER_PH2_MWIS].activated = TRUE;
        }
    }

    if(data_len >= 7) //UST M has only 1 bytes, so we should check the real length.
    {
        if(BIT_0 == (*(ptemp + 6) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_CFIS].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CFIS].activated = TRUE;
        }

        if(BIT_2 == (*(ptemp + 6) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_SPDI].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SPDI].activated = TRUE;
        }

        if(BIT_3 == (*(ptemp + 6) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_MMS].allocated = TRUE;
            sst->service_table[SIMSER_PH2_MMS].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 6) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_EXT8].allocated = TRUE;
            sst->service_table[SIMSER_PH2_EXT8].activated = TRUE;
        }

        if(BIT_6 == (*(ptemp + 6) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_MMSUCP].allocated = TRUE;
            sst->service_table[SIMSER_PH2_MMSUCP].activated = TRUE;
        }
    }

    if(BIT_0 == (*ptemp & BIT_0))
    {
        sst->service_table[SIMSER_PH2_LPB].allocated = TRUE;
        sst->service_table[SIMSER_PH2_LPB].activated = TRUE;
    }

    if(BIT_7 == (*ptemp & BIT_7))
    {
        sst->service_table[SIMSER_PH2_OCI].allocated = TRUE;
        sst->service_table[SIMSER_PH2_OCI].activated = TRUE;
    }

    if(data_len >= 1)
    {
        if(BIT_0 == (*(ptemp + 1) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_ICI].allocated = TRUE;
            sst->service_table[SIMSER_PH2_ICI].activated = TRUE;
        }

        if(BIT_5 == (*(ptemp + 1) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_CCP2].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CCP2].activated = TRUE;
        }
    }

    if(data_len >= 4)
    {
        if(BIT_2 == (*(ptemp + 3) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_GSM_ACCESS].allocated = TRUE;
            sst->service_table[SIMSER_PH2_GSM_ACCESS].activated = TRUE;
        }
    }

    if(data_len >= 5)
    {
        if(BIT_1 == (*(ptemp + 4) & BIT_1))
        {
            sst->service_table[SIMSER_PH2_EST].allocated = TRUE;
            sst->service_table[SIMSER_PH2_EST].activated = TRUE;
        }

        if(BIT_2 == (*(ptemp + 4) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_ACL].allocated = TRUE;
            sst->service_table[SIMSER_PH2_ACL].activated = TRUE;
        }

        if(BIT_5 == (*(ptemp + 4) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_GSM_SEC_CONTEXT].allocated = TRUE;
            sst->service_table[SIMSER_PH2_GSM_SEC_CONTEXT].activated = TRUE;
        }
    }

    if(data_len >= 6)
    {
        if(BIT_3 == (*(ptemp + 5) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_EXT5].allocated = TRUE;
            sst->service_table[SIMSER_PH2_EXT5].activated = TRUE;
        }
    }

    if(data_len >= 7)
    {
        if(BIT_5 == (*(ptemp + 6) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_CC_GPRS_BY_USIM].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CC_GPRS_BY_USIM].activated = TRUE;
        }
    }

    if(data_len >= 8)
    {
        if(BIT_2 == (*(ptemp + 7) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_PSEUDONYM].allocated = TRUE;
            sst->service_table[SIMSER_PH2_PSEUDONYM].activated = TRUE;
        }

        if(BIT_3 == (*(ptemp + 7) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_UPLMN_WLAN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_UPLMN_WLAN].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 7) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_OPLMN_WLAN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_OPLMN_WLAN].activated = TRUE;
        }

        if(BIT_5 == (*(ptemp + 7) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_UC_WSID].allocated = TRUE;
            sst->service_table[SIMSER_PH2_UC_WSID].activated = TRUE;
        }

        if(BIT_6 == (*(ptemp + 7) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_OC_WSID].allocated = TRUE;
            sst->service_table[SIMSER_PH2_OC_WSID].activated = TRUE;
        }

        if(BIT_7 == (*(ptemp + 7) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_VGCS_SEC].allocated = TRUE;
            sst->service_table[SIMSER_PH2_VGCS_SEC].activated = TRUE;
        }
    }

    if(data_len >= 9)
    {
        if(BIT_0 == (*(ptemp + 8) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_VBS_SEC].allocated = TRUE;
            sst->service_table[SIMSER_PH2_VBS_SEC].activated = TRUE;
        }

        if(BIT_1 == (*(ptemp + 8) & BIT_1))
        {
            sst->service_table[SIMSER_PH2_WLAN_REAUTH_ID].allocated = TRUE;
            sst->service_table[SIMSER_PH2_WLAN_REAUTH_ID].activated = TRUE;
        }

        if(BIT_2 == (*(ptemp + 8) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_MMS_STORAGE].allocated = TRUE;
            sst->service_table[SIMSER_PH2_MMS_STORAGE].activated = TRUE;
        }

        if(BIT_3 == (*(ptemp + 8) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_GBA].allocated = TRUE;
            sst->service_table[SIMSER_PH2_GBA].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 8) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_MBMS_SEC].allocated = TRUE;
            sst->service_table[SIMSER_PH2_MBMS_SEC].activated = TRUE;
        }

        if(BIT_5 == (*(ptemp + 8) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_DL_USSD].allocated = TRUE;
            sst->service_table[SIMSER_PH2_DL_USSD].activated = TRUE;
        }

        if(BIT_6 == (*(ptemp + 8) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_EHPLMN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_EHPLMN].activated = TRUE;
        }

        if(BIT_7 == (*(ptemp + 8) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_ADD_TERMINAL_PROFILE].allocated = TRUE;
            sst->service_table[SIMSER_PH2_ADD_TERMINAL_PROFILE].activated = TRUE;
        }
    }

    if(data_len >= 10)
    {
        if(BIT_0 == (*(ptemp + 9) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_EHPLMNPI].allocated = TRUE;
            sst->service_table[SIMSER_PH2_EHPLMNPI].activated = TRUE;
        }

        if(BIT_1 == (*(ptemp + 9) & BIT_1))
        {
            sst->service_table[SIMSER_PH2_LAST_RPLMN_SI].allocated = TRUE;
            sst->service_table[SIMSER_PH2_LAST_RPLMN_SI].activated = TRUE;
        }

        if(BIT_2 == (*(ptemp + 9) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_OMA_BCAST_PROFILE].allocated = TRUE;
            sst->service_table[SIMSER_PH2_OMA_BCAST_PROFILE].activated = TRUE;
        }

        if(BIT_3 == (*(ptemp + 9) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_GBA_LOCAL_KEY].allocated = TRUE;
            sst->service_table[SIMSER_PH2_GBA_LOCAL_KEY].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 9) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_TERMINAL_APP].allocated = TRUE;
            sst->service_table[SIMSER_PH2_TERMINAL_APP].activated = TRUE;
        }

        if(BIT_5 == (*(ptemp + 9) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_SPN_ICON].allocated = TRUE;
            sst->service_table[SIMSER_PH2_SPN_ICON].activated = TRUE;
        }

        if(BIT_6 == (*(ptemp + 9) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_PNN_ICON].allocated = TRUE;
            sst->service_table[SIMSER_PH2_PNN_ICON].activated = TRUE;
        }

        if(BIT_7 == (*(ptemp + 9) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_CP_USIM_IP].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CP_USIM_IP].activated = TRUE;
        }
    }

    if(data_len >= 11)
    {
        if(BIT_0 == (*(ptemp + 10) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_HIWLAN_ID_LIST].allocated = TRUE;
            sst->service_table[SIMSER_PH2_HIWLAN_ID_LIST].activated = TRUE;
        }

        if(BIT_1 == (*(ptemp + 10) & BIT_1))
        {
            sst->service_table[SIMSER_PH2_IWLAN_EHPLMN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_IWLAN_EHPLMN].activated = TRUE;
        }

        if(BIT_2 == (*(ptemp + 10) & BIT_2))
        {
            sst->service_table[SIMSER_PH2_IWLAN_HPLMN_PRIORITY].allocated = TRUE;
            sst->service_table[SIMSER_PH2_IWLAN_HPLMN_PRIORITY].activated = TRUE;
        }

        if(BIT_3 == (*(ptemp + 10) & BIT_3))
        {
            sst->service_table[SIMSER_PH2_IWLAN_LAST_RPLMN].allocated = TRUE;
            sst->service_table[SIMSER_PH2_IWLAN_LAST_RPLMN].activated = TRUE;
        }

        if(BIT_4 == (*(ptemp + 10) & BIT_4))
        {
            sst->service_table[SIMSER_PH2_EPS_MM].allocated = TRUE;
            sst->service_table[SIMSER_PH2_EPS_MM].activated = TRUE;
        }

        if(BIT_5 == (*(ptemp + 10) & BIT_5))
        {
            sst->service_table[SIMSER_PH2_CSG_INDICATION].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CSG_INDICATION].activated = TRUE;
        }

        if(BIT_6 == (*(ptemp + 10) & BIT_6))
        {
            sst->service_table[SIMSER_PH2_CC_EPS_BY_USIM].allocated = TRUE;
            sst->service_table[SIMSER_PH2_CC_EPS_BY_USIM].activated = TRUE;
        }

        if(BIT_7 == (*(ptemp + 10) & BIT_7))
        {
            sst->service_table[SIMSER_PH2_HPLMN_DA].allocated = TRUE;
            sst->service_table[SIMSER_PH2_HPLMN_DA].activated = TRUE;
        }
    }

    if(data_len >= 12)
    {
        if(BIT_0 == (*(ptemp + 11) & BIT_0))
        {
            sst->service_table[SIMSER_PH2_ECALL_DATA].allocated = TRUE;
            sst->service_table[SIMSER_PH2_ECALL_DATA].activated = TRUE;
        }
    }

    sst->service_num = SIMSER_PH2_NUM;
#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function decode the EF EST content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMEST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef SIM_3G_USIM_SUPPORT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_EST_T *est = ef_mapping;

    if(0 == data_len) // M has at least 1 bytes
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    if(BIT_0 == ((*ptemp) & BIT_0))
    {
        est->is_FDN_enable = TRUE;
    }
    else
    {
        est->is_FDN_enable = FALSE;
    }

    if(BIT_1 == ((*ptemp) & BIT_1))
    {
        est->is_BDN_enable = TRUE;
    }
    else
    {
        est->is_BDN_enable = FALSE;
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function decode the EFpsloci in ADFusim
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMPSLOCI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    //this EFpsloci's structrue is the same as EFlocigprs.
#ifdef SIM_3G_USIM_SUPPORT
#ifdef _SUPPORT_GPRS_
    uint16 mnc_digit_num = 0;
    SIM_SERVICE_RESULT_E ser_result;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_LOCI_GPRS_T *loci_gprs = ef_mapping;

    if(data_len != 14)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFpsloci is not 14."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_7275_112_2_17_22_58_39_4421, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    //the P-TMSI
    if((0xff == ptemp[0])
        && (0xff == ptemp[1])
        && (0xff == ptemp[2])
        && (0xff == ptemp[3]))
    {
        loci_gprs->p_tmsi.valid_flag = FALSE;
        SCI_MEMCPY(loci_gprs->p_tmsi.value, ptemp, 4);
    }
    else
    {
        loci_gprs->p_tmsi.valid_flag = TRUE;
        SCI_MEMCPY(loci_gprs->p_tmsi.value, ptemp, 4);
    }

    ptemp += 4;

    //the P-TMSI signature
    if((0xff == ptemp[0])
        && (0xff == ptemp[1])
        && (0xff == ptemp[2]))
    {
        loci_gprs->p_tmsi_signature.valid_flag = FALSE;
        loci_gprs->p_tmsi_signature.value = 0xFFFFFF;
    }
    else
    {
        loci_gprs->p_tmsi_signature.valid_flag = TRUE;
        loci_gprs->p_tmsi_signature.value = (*ptemp << 16) +
                                            (*(ptemp + 1) << 8) +
                                            (*(ptemp + 2)) ;
    }

    ptemp += 3;

    loci_gprs->rai.mcc = 0;
    loci_gprs->rai.mnc = 0;
    loci_gprs->rai.mnc_digit_num = 0;

    //decode the RAI
    //get a loci_gprs->rai.mcc and loci_gprs->rai.mnc code
    if((0xFF == ptemp[0])
        && (0xFF == ptemp[1])
        && (0xFF == ptemp[2]))
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::DecodeUsimPSLOCI mcc-mnc is 0xff 0xff 0xff."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_7325_112_2_17_22_58_39_4422, (uint8 *)"");
#endif
        //init the rai plmn
        loci_gprs->rai.mnc_digit_num = 0xf;
    }
    else
    {
        ser_result = DecodeMCCMNC(3, ptemp, &(loci_gprs->rai.mcc),
                                  &(loci_gprs->rai.mnc),
                                  &mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }

        loci_gprs->rai.mnc_digit_num = mnc_digit_num;
    }

    ptemp += 3;

    loci_gprs->rai.lac = (((uint16) * ptemp) << 8) + *(ptemp + 1);
    ptemp += 2;
    loci_gprs->rai.rac = (uint8)(*ptemp);
    ptemp += 1;

    loci_gprs->update_status = (SIM_R_UPDATE_STATUS_E)((*ptemp) & SIM_EF_LOCI_GPRS_UPDATE_STATUS_MARK);
#endif
#endif
    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function decode the EFecc content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMECC(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef SIM_3G_USIM_SUPPORT
    uint8 *ptemp = data;
    SIM_ECC_T *ecc = ef_mapping;
    uint8 ecc_alpha_id_len = s_sim_usim_ecc_alpha_id_len[cur_sim_task];

    //copy the 3 byte ecc code
    ecc->ecc_code[0] = *ptemp;
    ptemp++;
    ecc->ecc_code[1] = *ptemp;
    ptemp++;
    ecc->ecc_code[2] = *ptemp;
    ptemp++;

    if(0 != ecc_alpha_id_len)
    {
        ecc->alpha_id_flag = TRUE;

        if(ecc_alpha_id_len > SIM_MAX_ECC_ALPHA_ID_LEN)
        {
            ecc_alpha_id_len = SIM_MAX_ECC_ALPHA_ID_LEN;
        }

        ecc->alpha_id_len = ecc_alpha_id_len;
        SCI_MEMCPY(ecc->alpha_id, ptemp, ecc_alpha_id_len);
        ptemp += s_sim_usim_ecc_alpha_id_len[cur_sim_task];
    }
    else
    {
        ecc->alpha_id_flag = FALSE;
    }

    ecc->ecc_category_flag = TRUE;
    ecc->ecc_category = *ptemp;
#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function decode the EFpbr content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeUSIMPBR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef SIM_3G_USIM_SUPPORT
    uint16 i, j, file_id = 0;
    uint8 len = 0;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_PBR_T *pbr = ef_mapping;
    uint8 anr_times = 0;
    uint8 email_times = 0;
    uint8 item_len = 0;

    pbr->adn_file_id = 0;
    pbr->ext1_file_id = 0;
    i = 0;

    while(i < data_len)
    {
        if (USIM_PBR_TYPE_1 == ptemp[i])
        {
            i++;
            len = ptemp[i];

            for(j = 1; j <= len; j++)
            {
                item_len = 0;
                SCI_TRACE_LOW("SIM PBR TYPE1 0x%x, %d %d, %d",ptemp[i+j],i,j,len);
                if (USIM_PBR_EF_ADN == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->adn_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else if (USIM_PBR_EF_ANR == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }

                    if (0 == anr_times)
                    {
                        pbr->anra_file_id = file_id;
                    }
                    else if (1 == anr_times)
                    {
                        pbr->anrb_file_id = file_id;
                    }
                    else if (2 == anr_times)
                    {
                        pbr->anrc_file_id = file_id;
                    }
                    anr_times++;
                }
                else if (USIM_PBR_EF_EMAIL == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }

                    if (0 == email_times)
                    {
                        pbr->emaila_file_id = file_id;
                    }
                    else if (1 == email_times)
                    {
                        pbr->emailb_file_id = file_id;
                    }
                    email_times++;
                }
                else if (USIM_PBR_EF_GRP == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->grp_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else if (USIM_PBR_EF_IAP == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->iap_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else if (USIM_PBR_EF_PBC == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->pbc_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else if (USIM_PBR_EF_SNE == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->sne_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else if (USIM_PBR_EF_UID == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->uid_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else//other tags
                {
                    j++;//len
                    j += ptemp[i+j];//move to the next tag
                }
            }

            i += len;
        }

        if (USIM_PBR_TYPE_2 == ptemp[i])
        {
            i++;
            len = ptemp[i];

            for(j = 1; j <= len; j++)
            {
                item_len = 0;
                SCI_TRACE_LOW("SIM PBR TYPE2 0x%x, %d %d, %d",ptemp[i+j],i,j,len);
                if (USIM_PBR_EF_ANR == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }

                    if (0 == anr_times)
                    {
                        pbr->anra_file_id = file_id;
                    }
                    else if (1 == anr_times)
                    {
                        pbr->anrb_file_id = file_id;
                    }
                    else if (2 == anr_times)
                    {
                        pbr->anrc_file_id = file_id;
                    }
                    anr_times++;
                }
                else if (USIM_PBR_EF_EMAIL == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }

                    if (0 == email_times)
                    {
                        pbr->emaila_file_id = file_id;
                    }
                    else if (1 == email_times)
                    {
                        pbr->emailb_file_id = file_id;
                    }
                    email_times++;
                }
                else if (USIM_PBR_EF_SNE == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->sne_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
            }

            i += len;
        }

        if (USIM_PBR_TYPE_3 == ptemp[i])
        {
            i++;
            len = ptemp[i];

            for(j = 1; j <= len; j++)
            {
                item_len = 0;
                SCI_TRACE_LOW("SIM PBR TYPE3 0x%x, %d %d, len %d", ptemp[i+j], i, j, len);
                if (USIM_PBR_EF_EXT1 == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->ext1_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else if (USIM_PBR_EF_AAS == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->aas_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else if (USIM_PBR_EF_GAS == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->gas_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else if (USIM_PBR_EF_CCP1 == ptemp[i+j])
                {
                    j++;//len
                    item_len = ptemp[i+j];
                    j++;//move to the file_id byte1
                    file_id = ptemp[i+j];
                    j++;//move to the file_id byte2
                    file_id <<= 8;
                    file_id |= ptemp[i+j];
                    pbr->ccp1_file_id = file_id;
                    if (item_len > 2)
                    {
                        item_len = item_len - 2;
                        j += item_len;
                    }
                }
                else//other tags
                {
                    j++;//len
                    j += ptemp[i+j];
                }
            }

            i += len;
        }

        //if((0 != (pbr->adn_file_id)) && (0 != (pbr->ext1_file_id)))
        {
            //Once we get the ADN and EXT1 file id, we can quit.
            //return SIM_SERVICE_OK;
        }

        i++;//continue the loop, move to the next tag
    }

#endif

    return SIM_SERVICE_OK;
}

/********************************************************************************/
//  Description: the function decode the EF EHPLMN content to the corresponding
//               file information structure.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 31.102 Rel-7
/********************************************************************************/
static SIM_SERVICE_RESULT_E DecodeEHPLMN(                           //RETURN: the result of the encode operation
    uint32 cur_sim_task,
    const uint16   data_len,   //IN: the len of the data to be decode
    void           *data,      //IN: the data to be decoded
    void           *ef_mapping //OUT: the pointer of the struct that decode out
)
{
    SIM_SERVICE_RESULT_E    ser_result      = SIM_SERVICE_OK;
    SIM_EF_MAPPING_EHPLMN_T *ehplmn_ptr     = ef_mapping;
    uint8                   *ptemp          = data;
    uint16                  i               = 0;
    uint16                  mcc             = 0;
    uint16                  mnc             = 0;
    uint16                  mnc_digit_num   = 0;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"Decode EHPLMN"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_7538_112_2_17_22_58_39_4424, (uint8 *)"");
#endif

    //the data len must be more than 3 bytes and must be 3's multiple.
    if((data_len < SIM_EF_EHPLMN_ARR_SIZE)
        || (0 != (data_len % SIM_EF_EHPLMN_ARR_SIZE)))
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMSET(ehplmn_ptr, 0x0, sizeof(SIM_EF_MAPPING_EHPLMN_T));
    ehplmn_ptr->max_num = data_len / SIM_EF_EHPLMN_ARR_SIZE;

    for(i = 0; i < ehplmn_ptr->max_num; i++)
    {
        if((i >= SIM_EF_EHPLMN_ARR_NUM)
            || ((0XFF == ptemp[0] && (0XFF == ptemp[1]) && (0XFF == ptemp[2]))))
        {
            break;
        }

        ser_result = DecodeMCCMNC(3, ptemp, &mcc, &mnc, &mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return SIM_SERVICE_DECODE_EF_ERROR;
        }
        else
        {
            ehplmn_ptr->plmn_arr[i].mcc             = mcc;
            ehplmn_ptr->plmn_arr[i].mnc             = mnc;
            ehplmn_ptr->plmn_arr[i].mnc_digit_num   = mnc_digit_num;
            ehplmn_ptr->validate_num++;

            ptemp += 3;
        }
    }

    return SIM_SERVICE_OK;
}

/********************************************************************************/
//  Description: the function decode the EF EHPLMNPI content to the corresponding
//               file information structure.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 31.102 Rel-7
/********************************************************************************/
static SIM_SERVICE_RESULT_E DecodeEHPLMNPI(                           //RETURN: the result of the encode operation
    uint32 cur_sim_task,
    const uint16   data_len,   //IN: the len of the data to be decode
    void           *data,      //IN: the data to be decoded
    void           *ef_mapping //OUT: the pointer of the struct that decode out
)
{
    SIM_EF_MAPPING_EHPLMNPI_T   *ehplmnpi_ptr   = ef_mapping;
    uint8                       *ptemp          = data;

    if(1 != data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMSET(ehplmnpi_ptr, 0x0, sizeof(SIM_EF_MAPPING_EHPLMNPI_T));
    ehplmnpi_ptr->pi_value = *ptemp;

    return SIM_SERVICE_OK;
}

/********************************************************************************/
//  Description: the function decode the EF SPNI or PNNI content to the
//               corresponding file information structure.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 3GPP TS 31.102 Rel-8
/********************************************************************************/
static SIM_SERVICE_RESULT_E DecodeIconInfo(                             //RETURN: the result of the encode operation
    uint32       cur_sim_task,
    const uint16 data_len,       //IN: the len of the data to be decode
    void         *data,          //IN: the data to be decoded
    void         *ef_mapping     //OUT: the pointer of the struct that decode out
)
{
    SIM_ICON_INFO_T     *spni_ptr   = ef_mapping;
    uint8               *ptemp      = data;
    uint8               tmp_len     = 0;

    SCI_ASSERT(PNULL != data); /*assert verified*/
    SCI_ASSERT(PNULL != spni_ptr); /*assert verified*/

    if(4 > data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMSET(spni_ptr, 0x0, sizeof(SIM_ICON_INFO_T));
    spni_ptr->icon_tag  = *ptemp;
    ptemp++;
    tmp_len             = *ptemp;
    ptemp++;
    spni_ptr->qualifier = *ptemp;
    ptemp++;

    switch(spni_ptr->icon_tag)
    {
        case 0x80:
            //uri
            spni_ptr->icon_link.len = MIN(tmp_len - 1, SIM_ICON_MAX_LEN);
            SCI_MEMCPY(spni_ptr->icon_link.icon.uri_arr, ptemp, spni_ptr->icon_link.len);
            break;

        case 0x81:
            // img record
            spni_ptr->icon_link.len = MIN(tmp_len - 1, SIM_ICON_MAX_LEN);
            SCI_MEMCPY(spni_ptr->icon_link.icon.img_id_arr, ptemp, spni_ptr->icon_link.len);
            break;

        default:
            return (SIM_SERVICE_DECODE_EF_ERROR);
    }

    return SIM_SERVICE_OK;
}

/********************************************************************************/
//  Description: the function decode the EFcst (CDMA service table)
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
static SIM_SERVICE_RESULT_E DecodeCST(                           //RETURN: the result of the encode operation
    uint32 cur_sim_task,
    const uint16   data_len,   //IN: the len of the data to be decode
    void           *data,      //IN: the data to be decoded
    void           *ef_mapping //OUT: the pointer of the struct that decode out
)
{
#ifdef SIM_CDMA_CARD_SUPPORT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_SST_T *sst = ef_mapping;

    //set all the sst to zero.that mean the service that do not pointer out in the
    //file content is set to not allocated and not actived.
    SCI_MEMSET(sst, 0x0, sizeof(SIM_EF_MAPPING_SST_T));

    if(0 == data_len)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::warning,the len of EFcst is 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_DECODE_7676_112_2_17_22_58_40_4425, (uint8 *)"");
#endif
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    sst->service_num = 0;

    //CHV1 disable function
    sst->service_table[SIMSER_PH2_CHV1_DISABLE].allocated = (BOOLEAN)((*ptemp & 0x01) != 0);
    sst->service_table[SIMSER_PH2_CHV1_DISABLE].activated = (BOOLEAN)((*ptemp & 0x02) != 0);
    sst->service_num++;

    //ADN
    sst->service_table[SIMSER_PH2_ADN].allocated = (BOOLEAN)((*ptemp & 0x04) != 0);
    sst->service_table[SIMSER_PH2_ADN].activated = (BOOLEAN)((*ptemp & 0x08) != 0);
    sst->service_num++;

    //FDN
    sst->service_table[SIMSER_PH2_FDN].allocated = (BOOLEAN)((*ptemp & 0x10) != 0);
    sst->service_table[SIMSER_PH2_FDN].activated = (BOOLEAN)((*ptemp & 0x20) != 0);
    sst->service_num++;

    //SMS
    sst->service_table[SIMSER_PH2_SMS].allocated = (BOOLEAN)((*ptemp & 0x40) != 0);
    sst->service_table[SIMSER_PH2_SMS].activated = (BOOLEAN)((*ptemp & 0x80) != 0);
    sst->service_num++;

    if(data_len < 3)
    {
        return SIM_SERVICE_OK;
    }

    ptemp += 2;

    //EXT1
    sst->service_table[SIMSER_PH2_EXT1].allocated = (BOOLEAN)((*ptemp & 0x04) != 0);
    sst->service_table[SIMSER_PH2_EXT1].activated = (BOOLEAN)((*ptemp & 0x08) != 0);
    sst->service_num++;

    //EXT2
    sst->service_table[SIMSER_PH2_EXT2].allocated = (BOOLEAN)((*ptemp & 0x10) != 0);
    sst->service_table[SIMSER_PH2_EXT2].activated = (BOOLEAN)((*ptemp & 0x20) != 0);
    sst->service_num++;

    //SMSP
    sst->service_table[SIMSER_PH2_SMSP].allocated = (BOOLEAN)((*ptemp & 0x40) != 0);
    sst->service_table[SIMSER_PH2_SMSP].activated = (BOOLEAN)((*ptemp & 0x80) != 0);
    sst->service_num++;
#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeDN(   //return value:the result of decode operation
    const uint16           data_len,       //the len of the data to be decode
    void                   *data,          //in:the data to be decoded
    SIM_RECORD_EF_INFO_T   *dn_info_ptr,
    SIM_EF_MAPPING_DN_T    *dn_data_ptr    //out:the pointer of the struct that decode out
)
{
    uint8                   *ptemp      = data;

    //the record length is not correct
    if(dn_info_ptr->original_alpha_id_len + SIM_EF_DN_MIN_RECORD_LEN > data_len)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    SCI_MEMSET(dn_data_ptr->alpha_id, 0xFF, sizeof(dn_data_ptr->alpha_id));

    //the alpha id
    dn_data_ptr->alpha_id_len = MIN(SIM_MAX_DN_ALPHA_IDENTIFY_LEN, dn_info_ptr->alpha_id_len);
    SCI_MEMCPY(dn_data_ptr->alpha_id, ptemp, dn_data_ptr->alpha_id_len);
    ptemp += dn_info_ptr->original_alpha_id_len;

    //get the dialling num length
    // @Tracy Zhang
    // GSM11.11 Sector 10.5.1
    // This byte gives the number of bytes of the following two data items
    // containing actual BCD number/SSC information. This means that the
    // maximum value is 11, even when the actual ADN/SSC information length
    // is greater than 11.
    if(0 == *ptemp)
    {
        dn_data_ptr->dn_len = 0;
    }
    else if(*ptemp > 11)
    {
        dn_data_ptr->dn_len = SIM_EF_DN_DN_LEN;
    }
    else
    {
        dn_data_ptr->dn_len = *ptemp - 1;
    }

    ptemp += 1;

    //the TON and the NPI
    if(UINT8_INVALIDE != *ptemp)
    {
        dn_data_ptr->is_ton_npi_exist = TRUE;
        dn_data_ptr->ton = (((*ptemp) & SIM_EF_DN_TON_MARK) >> 4);
        dn_data_ptr->npi = (*ptemp) & SIM_EF_DN_NPI_MARK;
    }
    else
    {
        dn_data_ptr->is_ton_npi_exist = FALSE;
    }

    ptemp += 1;

    //the dialling num
    SCI_MEMCPY(dn_data_ptr->dialling_num, ptemp, SIM_EF_DN_DN_LEN);
    ptemp += SIM_EF_DN_DN_LEN;

    //the Capability/Configuration Identifier
    if((UINT8_INVALIDE != *ptemp)
        && (0 != *ptemp))
    {
        dn_data_ptr->is_ccp_exist = TRUE;
        dn_data_ptr->ccp_id = *ptemp;
    }
    else
    {
        dn_data_ptr->is_ccp_exist = FALSE;
    }

    ptemp += 1;

    //the Extension1 Record Identifier
    if((UINT8_INVALIDE != *ptemp)
        && (0 != *ptemp))
    {
        dn_data_ptr->is_ext_exist = TRUE;
        dn_data_ptr->ext_id = *ptemp;
    }
    else
    {
        dn_data_ptr->is_ext_exist = FALSE;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeORANGEDYN2FLAG(
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef _CPHS_SUPPORT
    SIM_EF_MAPPING_ORANGE_DYN2_FLAG_T *dynamic2flag = ef_mapping;
    uint8 *ptemp = data;

    if(BIT_0 == (*ptemp & BIT_0))
    {
        dynamic2flag->is_als_locked = TRUE;// 1=locked,0=unlocked
    }
    else
    {
        dynamic2flag->is_als_locked = FALSE;// 1=locked,0=unlocked
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeORANGECSPLINE2(
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef _CPHS_SUPPORT
    uint8 *temp_ptr = data;
    SIM_EF_MAPPING_ORANGE_CSP_LINE2_T *csp = ef_mapping;
    uint8 service_num = 0;

    csp->is_call_offering_present = FALSE;
    csp->is_call_restriction_present = FALSE;
    csp->is_value_added_serv_present = FALSE;
    csp->is_info_numbers_present = FALSE;

    if(data_len < 20)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    service_num = data_len / 2;

    while(service_num >= 1)
    {
        switch(*temp_ptr)
        {
            case SIM_CALL_OFFERING:
                csp->is_call_offering_present = TRUE;
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->call_offering.is_cfu)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->call_offering.is_cfb)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->call_offering.is_cfnry)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->call_offering.is_cfnrc)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->call_offering.is_ct)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_CALL_RESTRICTION:
                csp->is_call_restriction_present = TRUE;
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->call_restriction.is_boac)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->call_restriction.is_boic)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->call_restriction.is_boic_exhc)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->call_restriction.is_baic)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->call_restriction.is_bic_roam)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_OTHER_SERVICES:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->other_supp_serv.is_mpty)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->other_supp_serv.is_cug)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->other_supp_serv.is_aoc)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->other_supp_serv.is_pref_cug)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->other_supp_serv.is_cug_oa)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_CALL_COMPLETION:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->call_completion.is_hold)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->call_completion.is_cw)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->call_completion.is_ccbs)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->call_completion.is_u2u_signalling)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_TELESERVICES:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->teleservices.is_sm_mt)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->teleservices.is_sm_mo)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->teleservices.is_sm_cb)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->teleservices.is_reply_path)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->teleservices.is_del_conf)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 2, 1, csp->teleservices.is_protocol_id)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 1, 1, csp->teleservices.is_validity_period)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_CPHS_TELE:
                temp_ptr = temp_ptr + 1;
                //csp->rfu.is_als = FALSE;//RFU
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->rfu.is_als)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_CPHS_FEATURES:
                temp_ptr = temp_ptr + 2;
                service_num--;
                break;
            case SIM_NUM_ID:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->number_identif.is_clip)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->number_identif.is_colr)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->number_identif.is_colp)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->number_identif.is_mci)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 1, 1, csp->number_identif.is_cli_send)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 1, csp->number_identif.is_cli_block)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_PHASE2_SERVICE:
                temp_ptr = temp_ptr + 1;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->phase_service.is_gprs)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->phase_service.is_hscsd)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->phase_service.is_vgc)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->phase_service.is_vbs)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->phase_service.is_msp)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 2, 1, csp->phase_service.is_multi_band)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_VALUEADDED_SERV:
                temp_ptr = temp_ptr + 1;
                csp->is_value_added_serv_present = TRUE;
                SIM_GET_DATA_BETWEEN((*temp_ptr), 7, 1, csp->value_added_service.is_plmn_mode)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 6, 1, csp->value_added_service.is_vps)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 5, 1, csp->value_added_service.is_sm_mo_paging)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 4, 1, csp->value_added_service.is_sm_mo_email)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 3, 1, csp->value_added_service.is_fax)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 2, 1, csp->value_added_service.is_data)
                SIM_GET_DATA_BETWEEN((*temp_ptr), 0, 1, csp->value_added_service.is_language)
                temp_ptr = temp_ptr + 1;
                service_num--;
                break;
            case SIM_SPID:
                temp_ptr = temp_ptr + 1;
                csp->is_spid_present = TRUE;
                csp->spid = *temp_ptr;
                service_num--;
                break;
            case SIM_INFO_NUM:
                temp_ptr = temp_ptr + 1;
                csp->is_info_numbers_present = TRUE;
                csp->info_numbers = *temp_ptr;
                service_num--;
                break;

            default:
                //SCI_TRACE_LOW("There are unrecognized service code when decode CSP!");
                return SIM_SERVICE_OK;
        }
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeORANGEDYNFLAG(
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef _CPHS_SUPPORT
    SIM_EF_MAPPING_ORANGE_DYN_FLAG_T *dynamicflag = ef_mapping;
    uint8 *ptemp = data;

    if(BIT_0 == ((*ptemp) & BIT_0)) // 1=line1,0=line2
    {
        dynamicflag->selected_line = 1;//line1
    }
    else
    {
        dynamicflag->selected_line = 0;//line2
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_DECODE_EF_ERROR;
#endif
}
/**********************************************************************
//    Description:
//    the function decode the EFnetpar content to the corresponding file information structure.
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeNetPar(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
#ifdef SIM_3G_USIM_SUPPORT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_BCCH_T *bcch = ef_mapping;
    uint8 pos = 0;
    uint8 len1 = 0,len2 = 0,len3=0;
    uint8 i= 0;

    for(i = 0; i < 5 ;i++)
    {
        SCI_TRACE_LOW("SIM::decode EFnetpar data_len=%d, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", \
                      data_len, *(ptemp + i*10), *(ptemp + 1 + i*10), *(ptemp + 2 + i*10), *(ptemp + 3 + i*10), \
                      *(ptemp + 4 + i*10), *(ptemp + 5 + i*10), *(ptemp + 6 + i*10), *(ptemp + 7 + i*10), *(ptemp + 8 + i*10),*(ptemp + 9 + i*10));                      
                          
    }

    g_sim_netpar_file_size[cur_sim_task] = (uint8)data_len;
    
    SCI_MEMSET(&bcch->ba_list,0x0,sizeof(bcch->ba_list));
    bcch->is_usim = TRUE;
    bcch->usim_netpar.camponed_arfcn = 0xffff;
    bcch->usim_netpar.arfcn_num = 0;
    SCI_MEMSET(&bcch->usim_netpar.arfcn,0xffff,sizeof(bcch->usim_netpar.arfcn));
        
    //GSM cell information
    if(ptemp[pos++] == 0xa0)
    {
        len1 = ptemp[pos++];

        if(len1 < 4)
        {
            return SIM_SERVICE_DECODE_EF_ERROR;
        }

        //current camponed cell bcch frequency information tag
        if(ptemp[pos++] == 0x80)
        {
            len2 = ptemp[pos++];

            if(len2 != 2)
            {
                return SIM_SERVICE_DECODE_EF_ERROR;
            }

            bcch->usim_netpar.camponed_arfcn = ptemp[pos++];
            bcch->usim_netpar.camponed_arfcn = (bcch->usim_netpar.camponed_arfcn << 8) | ptemp[pos++];
        }

        if(data_len < 10)
        {
            return SIM_SERVICE_OK;
        }
        
        //neighbour cell bcch frequency
        if(len1 > 4)        
        {
            if(ptemp[pos++] == 0x81)
            {
                len3 = ptemp[pos++];

                if(len3 == 0)
                {
                    return SIM_SERVICE_OK;
                }

                len3 = MIN(SIM_MAX_ARFCN_NUM*2,len3);
                len3 = MIN((data_len - 8),len3); 
                
                for(i = 0; i< len3/2;i++)
                {
                    if((ptemp[pos] == 0xff) && (ptemp[pos+1] == 0xff))
                    {
                        break;
                    }
                    bcch->usim_netpar.arfcn[bcch->usim_netpar.arfcn_num] = ptemp[pos++];
                    bcch->usim_netpar.arfcn[bcch->usim_netpar.arfcn_num] = (bcch->usim_netpar.arfcn[bcch->usim_netpar.arfcn_num] << 8) | ptemp[pos++];
                    bcch->usim_netpar.arfcn_num++;
                }
            }
        }    
    }
    else
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }
    
    return SIM_SERVICE_OK;
    
#else
    return SIM_SERVICE_OK;
#endif    
}

/**********************************************************************
//    Description:
//    the function decode the EF ACL content to the corresponding file information structure.
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E DecodeACL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const uint16 data_len,  //the len of the data to be decode
    void *data,             //in:the data to be decoded
    void *ef_mapping        //out:the pointer of thestruct that decode out
)
{
    SIM_EF_MAPPING_ACL_T *acl = ef_mapping;
    uint8 *ptemp = data;

    acl->apn_num = *ptemp;
    ptemp++;

    acl->apn_tlvs_len = data_len - 1;
    
    if(acl->apn_tlvs_len > 0)
    {
        SCI_MEMCPY(acl->apn_tlvs,ptemp,MIN(acl->apn_tlvs_len,SIM_MAX_APN_TLV_LEN));
    }
    
    return SIM_SERVICE_OK;
}

#ifdef   __cplusplus
}
#endif
