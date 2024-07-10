/******************************************************************************
** File Name:      sim_ef_encode.c                                           *
** Author:         Raislin.Kong                                              *
** Date:           2001.8                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the c file define the fucntions encode the ef     *
**                 content                                                   *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2001.8         Raislin.Kong     Create                                    *
** 2005.6         Yongxia.zhang    CR26616,Modify EncodeEXT1-4()             *
******************************************************************************/
#include "base_trc.h"
#include "stddef.h"
#include "sim_include.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

//this funciton encode a Transparent file mapping sturture to
//the coresponding EF content,or record mapping sturcture to
//the coresponding Fixed Linear or Cyclic file record content
/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeICCID(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeDIR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeELP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeLP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeIMSI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeKC(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePLMNsel(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeHPLMN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeACMmax(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeACM(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeGID1(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeGID2(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePUCT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCBMI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSPN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCBMID(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeBCCH(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeACC(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeFPLMN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeLOCI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeAD(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePHASE(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVGCS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVGCSS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVBS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVBSS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeeMLPP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeAAem(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeECC(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCBMIR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeDCK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCNL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeNIA(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeKcGRPS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeLOCIGPRS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSUME(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePNN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeOPL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMBDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMBI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMWIS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCFIS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSPDI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMMSN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeEXT8(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMMSICP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMMSUP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMMSUCP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSAI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSLL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeADN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeFDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSMS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCCP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMSISDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSMSP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSMSS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeLND(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSMSR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeEXT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeBDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//     the function encode the data from upper layer to write to the EF IMG.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeIMG(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      The common function to encode the MCC and MNC for EFloci,EFplmnsel,
//    EFfplmn,EFlocigprs
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMCCMNC(
    uint16 *data_len,   //out:the length of the data encoded
    void   *data,       //out:the data buf for encode
    uint16  mcc,        //the MCC
    uint16  mnc,        //the MNC
    uint16  mnc_digit_num   //the MNC digit Number 2 or 3
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCCFlag(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVoiceMsgWtFlag(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMailboxNum(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to correspondingding file information structure.
//    Global resource dependence :
//    Author:king.li
//    Note:New added by king.li for CPHS extended functions
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeInformationnum(   //return value:the result of encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:king.li
//    Note:New added by king.li for CPHS extended functions
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSSTCPHS(   //return value:the result of encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Guisen.deng
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCSP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF PLMNWACT.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF OPLMNWACT.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeOPLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF HPLMNWACT.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeHPLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF CPBCCH.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCPBCCH(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF INVSCAN.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeINVSCAN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF ECCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeECCP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF CMI.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCMI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF MExEST.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMEXEST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF ORPK.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeORPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF ARPK.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeARPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF TPRPK
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeTPRPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
//    refer to  CPHS Phase 2  B.4.1.2
//
  Bytes |Description|M/O| Length  |
 _______|___________|___|_________|
 1 to n |  PLMN name| M | n Bytes |
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeOPNameStr(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EFest
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeUSIMEST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EFpsloci
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeUSIMPSLOCI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
//    refer to  CPHS Phase 2  B.4.1.2

  Bytes |   Description         |M/O| Length   |
________|_______________________|___|__________|
 1 to 10|Operator Name Shortform| M | 10 Bytes |
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeOPNameShort(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeDN(    //return value:the result of the encode operation
    uint16                      *data_len,   //out:the len of the data been encode out
    void                        *data,         //out:the data that been encode out
    const SIM_RECORD_EF_INFO_T  *dn_info_ptr,
    void                         *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN               forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeORANGEDYN2FLAG(
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeORANGECSPLINE2(
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeORANGEDYNFLAG(
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeKeys(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeKeysPS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeNetPar(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF ACL.
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeACL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
);

//the Encode functions array of all EF file
static SIM_SERVICE_RESULT_E(*s_sim_encode_functions[])(uint32, uint16 *, void *, void *, BOOLEAN) =
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
    EncodeICCID,
    EncodeDIR,
    EncodeELP,
    NULL,//MF EFarr
    EncodeLP,
    EncodeIMSI,
    EncodeKC,
    EncodePLMNsel,
    EncodeHPLMN,
    EncodeACMmax,
    EncodeSST,
    EncodeACM,
    EncodeGID1,
    EncodeGID2,
    EncodePUCT,
    EncodeCBMI,
    EncodeSPN,
    EncodeCBMID,
    EncodeBCCH,
    EncodeACC,
    EncodeFPLMN,
    EncodeLOCI,
    EncodeAD,
    EncodePHASE,
    EncodeVGCS,
    EncodeVGCSS,
    EncodeVBS,
    EncodeVBSS,
    EncodeeMLPP,
    EncodeAAem,
    EncodeECC,
    EncodeCBMIR,
    EncodeDCK,
    EncodeCNL,
    EncodeNIA,
    EncodeKcGRPS,
    EncodeLOCIGPRS,
    EncodeSUME,
    EncodePLMNWACT,
    EncodeOPLMNWACT,
    EncodeHPLMNWACT,
    EncodeCPBCCH,
    EncodeINVSCAN,
    EncodePNN,
    EncodeOPL,
    EncodeMBDN,
    EncodeEXT,
    EncodeMBI,
    EncodeMWIS,
    EncodeCFIS,
    EncodeEXT,
    EncodeSPDI,
    EncodeMMSN,
    EncodeEXT8,
    EncodeMMSICP,
    EncodeMMSUP,
    EncodeMMSUCP,
    EncodeSAI,
    EncodeSLL,
    EncodeADN,
    EncodeFDN,
    EncodeSMS,
    EncodeCCP,
    EncodeECCP,//SIM_RELEASE99_UPDATE
    EncodeMSISDN,
    EncodeSMSP,
    EncodeSMSS,
    EncodeLND,
    EncodeSMSR,
    EncodeSDN,
    EncodeEXT,
    EncodeEXT,
    EncodeEXT,
    EncodeBDN,
    EncodeEXT,
    NULL,//SIM_FILE_EF_TELECOM_SUME
    EncodeCMI,//SIM_RELEASE99_UPDATE
    EncodeIMG,
    NULL,//IIDF
    NULL,//CDMA_IMSI
    EncodeOPNameStr,
    EncodeOPNameShort,
    NULL,
    EncodeVoiceMsgWtFlag,
    EncodeCCFlag,
    EncodeCSP,
    EncodeMailboxNum,
    EncodeInformationnum,   // SIM_FILE_EF_INFONUM
    EncodeSSTCPHS,          // SIM_FILE_EF_CPHS_SST
    EncodeMEXEST,
    EncodeORPK,
    EncodeARPK,
    EncodeTPRPK,
    NULL,//DFtel EFarr
    NULL,//SIM_FILE_EF_TELECOM_PB_PSC,
    NULL,//SIM_FILE_EF_TELECOM_PB_CC,
    NULL,//SIM_FILE_EF_TELECOM_PB_PUID,
    NULL,//SIM_FILE_EF_TELECOM_PB_PBR,
    NULL,//SIM_FILE_EF_TELECOM_PB_IAP,
    EncodeADN,//SIM_FILE_EF_TELECOM_PB_ADN,
    EncodeEXT,//SIM_FILE_EF_TELECOM_PB_EXT1,
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
    EncodeADN,//SIM_FILE_EF_TELECOM_PB1_ADN,
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
    EncodeKC,//GSMAccess Kc, 3G
    EncodeKcGRPS,
    EncodeCPBCCH,
    EncodeINVSCAN,
    NULL,//SIM_FILE_EF_USIM_PB_PSC,
    NULL,//SIM_FILE_EF_USIM_PB_CC,
    NULL,//SIM_FILE_EF_USIM_PB_PUID,
    NULL,//SIM_FILE_EF_USIM_PB_PBR,
    NULL,//SIM_FILE_EF_USIM_PB_IAP,
    EncodeADN,//SIM_FILE_EF_USIM_PB_ADN,
    EncodeEXT,//SIM_FILE_EF_USIM_PB_EXT1,
    NULL,//SIM_FILE_EF_USIM_PB_PBC,
    NULL,//SIM_FILE_EF_USIM_PB_GRP,
    NULL,//SIM_FILE_EF_USIM_PB_AAS,
    NULL,//SIM_FILE_EF_USIM_PB_GAS,
    NULL,//SIM_FILE_EF_USIM_PB_ANRA,
    NULL,//SIM_FILE_EF_USIM_PB_ANRB,
    NULL,//SIM_FILE_EF_USIM_PB_ANRC,
    NULL,//SIM_FILE_EF_USIM_PB_SNE,
    NULL,//SIM_FILE_EF_USIM_PB_CCP1,
    NULL,//SIM_FILE_EF_USIM_PB_UID,
    NULL,//SIM_FILE_EF_USIM_PB_EMAILA,
    NULL,//SIM_FILE_EF_USIM_PB_EMAILB,
    NULL,//SIM_FILE_EF_USIM_PB1_IAP,
    EncodeADN,//SIM_FILE_EF_USIM_PB1_ADN,
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
    EncodeELP,//SIM_FILE_EF_USIM_LI,
    NULL,//USIM ARR
    EncodeIMSI,
    EncodeKeys,//SIM_FILE_EF_USIM_KEYS
    EncodeKeysPS,//SIM_FILE_EF_USIM_KEYSPS
    EncodeVoiceMsgWtFlag,//SIM_FILE_EF_USIM_VMWFLAG,
    EncodeSSTCPHS,//SIM_FILE_EF_USIM_CPHS_SST,
    EncodeCCFlag,//SIM_FILE_EF_USIM_CFFLAG,
    NULL,//SIM_FILE_EF_USIM_ONSTRING,
    EncodeCSP,//SIM_FILE_EF_USIM_CSP,
    NULL,//SIM_FILE_EF_USIM_CPHSINFO,
    EncodeMailboxNum,//SIM_FILE_EF_USIM_MBNUM,
    NULL,//SIM_FILE_EF_USIM_ONSTHORT,
    EncodeInformationnum,//SIM_FILE_EF_USIM_INFONUM,
    EncodeDCK,
    EncodeHPLMN,
    EncodeCNL,
    EncodeACMmax,
    NULL,//UST
    EncodeACM,
    EncodeFDN,//SIM_FILE_EF_USIM_FDN
    EncodeSMS,//SIM_FILE_EF_USIM_SMS
    EncodeGID1,
    EncodeGID2,
    EncodeMSISDN,//SIM_FILE_EF_USIM_MSISDN
    EncodePUCT,
    EncodeSMSP,//SIM_FILE_EF_USIM_SMSP
    EncodeSMSS,//SIM_FILE_EF_USIM_SMSS
    EncodeCBMI,
    NULL,//SPN
    EncodeSMSR,//SIM_FILE_EF_USIM_SMSR
    EncodeCBMID,
    EncodeSDN,
    EncodeEXT,//SIM_FILE_EF_USIM_EXT2
    EncodeEXT,//SIM_FILE_EF_USIM_EXT3
    EncodeBDN,//SIM_FILE_EF_USIM_BDN
    EncodeEXT,//SIM_FILE_EF_USIM_EXT5
    EncodeCBMIR,
    EncodeEXT,//SIM_FILE_EF_USIM_EXT4
    EncodeUSIMEST,
    EncodeACL,
    EncodeCMI,
    EncodePLMNWACT,
    EncodeOPLMNWACT,
    EncodeHPLMNWACT,
    EncodeUSIMPSLOCI,
    EncodeACC,
    EncodeFPLMN,
    EncodeLOCI,
    EncodeAD,
    EncodeVGCS,
    EncodeVGCSS,
    EncodeVBS,
    EncodeVBSS,
    EncodeeMLPP,
    EncodeAAem,
    NULL,//Ecc
    EncodeNetPar,
    EncodePNN,
    EncodeOPL,
    EncodeMBDN,
    EncodeEXT,
    EncodeMBI,
    EncodeMWIS,
    EncodeCFIS,
    EncodeEXT,
    EncodeSPDI,
    EncodeMMSN,
    EncodeEXT8,
    EncodeMMSICP,
    EncodeMMSUP,
    EncodeMMSUCP,
    EncodeNIA,
    PNULL,
    PNULL,
    PNULL,  //SPNI
    PNULL,  //PNNI
    EncodeSAI,
    EncodeSLL,
    EncodeMEXEST,
    EncodeORPK,
    EncodeARPK,
    EncodeTPRPK,
    NULL,//CDMA CST
    NULL,//EncodeORANGEOCT,
    EncodeORANGEDYN2FLAG,
    EncodeORANGECSPLINE2,
    NULL,//EncodeORANGEPARAM,
    EncodeACM,
    EncodeORANGEDYNFLAG
};

/**********************************************************************
//    Description:
//      the function that dispatch the encode function for differenct
//    EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMFS_EncodeEFFile(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    const SIM_FILE_NAME_E file_name,    //the file that the structure decode into
    uint16 *data_len,   //out:the data length that encode into
    void *data,         //out:the data encode into
    void *ef_mapping,   //in:the file sturcture to be encoded
    const BOOLEAN forge_empty   //encode a empty data or by the file structure
)
{
    //the file must be a valide file name
    if(SIM_IS_INVALIDE_FILE(file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //select the coresponding encode function to decode the ef mapping
    if(NULL == s_sim_encode_functions[file_name])
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }
    else
    {
        SCI_ASSERT(NULL != data_len);             /*assert verified: Null_pointer*/
        SCI_ASSERT(NULL != data);                 /*assert verified: Null_pointer*/

        if(!forge_empty)
        {
            SCI_ASSERT(NULL != ef_mapping); /*assert verified: Null_pointer*/
        }

        return s_sim_encode_functions[file_name](cur_sim_task, data_len, data, ef_mapping, forge_empty);
    }
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeICCID(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    *data_len = SIM_EF_ICCID_ID_NUM_LEN;

    if(forge_empty)
    {
        SCI_MEMSET(data, 0xff, SIM_EF_ICCID_ID_NUM_LEN);
    }
    else
    {
        SCI_MEMCPY(data, ((SIM_EF_MAPPING_ICCID_T *)ef_mapping)->id_num, SIM_EF_ICCID_ID_NUM_LEN);
    }

#endif
    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeDIR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    //EFdir is a read-only EF.
    return SIM_SERVICE_ENCODE_ERROR;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeELP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    int i;
    SIM_EF_MAPPING_ELP_T *elp = ef_mapping;
    uint8 *ptemp = data;

    *data_len = 2 * elp->max_num;

    SCI_MEMSET(ptemp, 0xff, *data_len);


    if(!forge_empty)
    {
        for(i = 0; i < elp->valid_num; i++)
        {
            //SCI_ASSERT(*data_len + 2 <= SIM_MAX_R_APDU_DATA_LEN);
            SCI_MEMCPY(ptemp, &(elp->lang_codes[i]), 2);
            ptemp += 2;
        }
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeLP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    int i;
    SIM_EF_MAPPING_LP_T *lp = ef_mapping;
    uint8 *ptemp = data;

    *data_len = lp->max_num;
    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    if(0 == lp->valid_num)
    {
        return SIM_SERVICE_OK;
    }
    else
    {
        for(i = 0; i < lp->valid_num; i++)
        {
            *ptemp = lp->lang_codes[i];
            ptemp += 1;
        }
    }

    return SIM_SERVICE_OK;

}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeIMSI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_IMSI_T *imsi = ef_mapping;

    *data_len = 9;

    SCI_MEMSET(data, 0xff, 9);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    if(!imsi->imsi.valid_flag)
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    *ptemp = imsi->imsi.length;
    ptemp += 1;

    SCI_MEMCPY(ptemp, imsi->imsi.imsi_value, imsi->imsi.length);
#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeKC(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_EF_MAPPING_KC_T *kc = ef_mapping;

    *data_len = 9;

    SCI_MEMCPY(data, kc->kc, 8);
    *((uint8 *)data + 8) = (kc->cksn & 0x07);
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePLMNsel(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_SERVICE_RESULT_E ser_result;
    uint16 len, i;
    uint8 max_num = 0;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_PLMNSEL_T *plmn_list = ef_mapping;

    if(0 != plmn_list->max_num)
    {
        if(plmn_list->max_num > SIM_EF_PLMNSEL_ARR_NUM)
        {
            max_num = SIM_EF_PLMNSEL_ARR_NUM;
        }
        else
        {
            max_num = plmn_list->max_num;
        }
    }
    else
    {
        max_num = SIM_EF_PLMNSEL_ARR_NUM;
    }

    *data_len = max_num * SIM_EF_PLMNSEL_ARR_SIZE; //data len is the value with max
    SCI_MEMSET(data, 0xff, *data_len);

    if(forge_empty || (0 == plmn_list->validate_num))
    {
        return SIM_SERVICE_OK;
    }

    for(i = 0; i < max_num; i++)
    {
        if(i > (plmn_list->validate_num - 1))
        {
            break;
        }

        ser_result = EncodeMCCMNC(&len, ptemp, plmn_list->plmn_arr[i].mcc, plmn_list->plmn_arr[i].mnc, plmn_list->plmn_arr[i].mnc_digit_num);

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }

        ptemp += len;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeHPLMN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    HPLMN_TIMER_VALUE_T *time_int = ef_mapping;

    *data_len = 1;

    if(forge_empty)
    {
        *ptemp = 0xff;
        return SIM_SERVICE_OK;
    }

    *ptemp = *time_int;
#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeACMmax(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ACMMAX_T *acm_max = ef_mapping;

    *data_len = 3;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
        return SIM_SERVICE_OK;
    }

    *ptemp = (uint8)(acm_max->acm_max_value >> 16);
    ptemp += 1;
    *ptemp = (uint8)((acm_max->acm_max_value & 0x0000ff00) >> 8);
    ptemp += 1;
    *ptemp = (uint8)acm_max->acm_max_value;
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    int i;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_SST_T *sst = ef_mapping;

    *data_len = 2;
    SCI_MEMSET(data, 0xff, 2);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    if(sst->service_num < SIMSER_PH1_NUM)
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    *data_len = 0;

    for(i = 0; i < sst->service_num;)
    {
        *data_len += 1;
        *ptemp = 0;

        //encode the first service stautus of a byte
        if(sst->service_table[i].allocated)
        {
            *ptemp |= 0x01;

            if(sst->service_table[i].activated)
            {
                *ptemp |= 0x02;
            }
            else
            {
                *ptemp &= 0xfd;
            }
        }
        else
        {
            *ptemp &= 0xfe;
        }

        i++;

        if(i >= sst->service_num)
        {
            return SIM_SERVICE_OK;
        }

        //encode the second service status of a byte
        if(sst->service_table[i].allocated)
        {
            *ptemp |= 0x04;

            if(sst->service_table[i].activated)
            {
                *ptemp |= 0x08;
            }
            else
            {
                *ptemp &= 0xf7;
            }
        }
        else
        {
            *ptemp &= 0xfb;
        }

        i++;

        if(i >= sst->service_num)
        {
            return SIM_SERVICE_OK;
        }

        //encode the third service status of a byte
        if(sst->service_table[i].allocated)
        {
            *ptemp |= 0x10;

            if(sst->service_table[i].activated)
            {
                *ptemp |= 0x20;
            }
            else
            {
                *ptemp &= 0xdf;
            }
        }
        else
        {
            *ptemp &= 0xef;
        }

        i++;

        if(i >= sst->service_num)
        {
            return SIM_SERVICE_OK;
        }

        //encode the fourth service status of a byte
        if(sst->service_table[i].allocated)
        {
            *ptemp |= 0x40;

            if(sst->service_table[i].activated)
            {
                *ptemp |= 0x80;
            }
            else
            {
                *ptemp &= 0x7f;
            }
        }
        else
        {
            *ptemp &= 0xbf;
        }

        i++;

        if(i >= sst->service_num)
        {
            return SIM_SERVICE_OK;
        }

        ptemp += 1;
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeACM(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ACM_T *acm = ef_mapping;

    *data_len = 3;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
        return SIM_SERVICE_OK;
    }

    *ptemp = (uint8)(acm->acm_value >> 16);
    ptemp += 1;
    *ptemp = (uint8)((acm->acm_value & 0x0000ff00) >> 8);
    ptemp += 1;
    *ptemp = (uint8)acm->acm_value;

    return SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeGID1(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_GID_T *gid = ef_mapping;

    *data_len = gid->len;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
        return SIM_SERVICE_OK;
    }

    SCI_MEMCPY(ptemp, gid->gid, gid->len);
#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeGID2(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_GID_T *gid = ef_mapping;

    *data_len = gid->len;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
        return SIM_SERVICE_OK;
    }

    SCI_MEMCPY(ptemp, gid->gid, gid->len);
#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePUCT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_PUCT_T *puct = ef_mapping;

    *data_len = 5;

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    SCI_MEMCPY(ptemp, puct->cur_code.cur_code, 3);
    ptemp += 3;
    *ptemp = (uint8)(puct->eppu.eppu >> 4);
    ptemp += 1;
    *ptemp = (uint8)(puct->eppu.eppu & SIM_EF_PUCT_EPPU_MARK);

    if(!puct->eppu.ex_sign)
    {
        *ptemp |= SIM_EF_PUCT_EX_SIGN_MARK;
    }

    *ptemp |= (puct->eppu.ex << 5);
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCBMI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CBMI_T *cbmi = ef_mapping;
    uint16 i = 0;

    if(forge_empty || (0 == cbmi->validate_num))
    {
        *data_len = SIM_EF_CBMI_CB_SIZE * (cbmi->validate_num);

        if(*data_len < SIM_EF_CBMI_CB_SIZE)
        {
            *data_len = SIM_EF_CBMI_CB_SIZE;
        }

        SCI_MEMSET(data, 0xff, *data_len);
    }
    else
    {
        *data_len = 0;

        if(cbmi->validate_num > SIM_MAX_VALIDATE_CBMI_NUM)
        {
            cbmi->validate_num = SIM_MAX_VALIDATE_CBMI_NUM;
        }

        for(i = 0; i < cbmi->validate_num; i++)
        {
            SCI_MEMCPY(ptemp, &(cbmi->cbmis[i]), SIM_EF_CBMI_CB_SIZE);
            ptemp += SIM_EF_CBMI_CB_SIZE;
            *data_len += SIM_EF_CBMI_CB_SIZE;
        }
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSPN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_SPN_T *pspn = ef_mapping;

    *data_len = 17;

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    *ptemp = pspn->display_condition;
    ptemp += 1;

    SCI_MEMCPY(ptemp, pspn->spn, SIM_EF_SPN_SPN_LEN);
#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCBMID(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 i = 0;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CBMID_T *cbmid = ef_mapping;

    *data_len = 0;

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    for(i = 0; i < cbmid->validate_num; i++)
    {
        SCI_MEMCPY(ptemp, &(cbmid->cbmi_for_dowload[i]), SIM_EF_CBMID_CB_SIZE);
        ptemp += SIM_EF_CBMID_CB_SIZE;
        *data_len += SIM_EF_CBMID_CB_SIZE;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeBCCH(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_BCCH_T *bcch = ef_mapping;

    *data_len = 16;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
        return SIM_SERVICE_OK;
    }

    SCI_MEMCPY(ptemp, &bcch->ba_list, *data_len);
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeACC(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    ACC_CONTROL_CLASS_T *acc = ef_mapping;

    *data_len = 2;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
    }

    *ptemp = (uint8)(*acc >> 8);
    *(ptemp + 1) = (uint8) * acc;
#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeFPLMN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_SERVICE_RESULT_E    ser_result  = SIM_SERVICE_OK;
    uint16                  len         = 0;
    uint16                  i           = 0;
    uint8                   *ptemp      = data;
    FPLMN_LIST_T            *fplmn_list = ef_mapping;

    *data_len = fplmn_list->max_num * 3;
    SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified:Check_value */
    SCI_MEMSET(data, 0xff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }


    //encode the FPLMN list
    for(i = 0; i < fplmn_list->max_num; i++)
    {
        if(fplmn_list->fplmn_arr[i].present)
        {
            //call function encode the MCC and MNC
            ser_result = EncodeMCCMNC(&len, ptemp,
                                      fplmn_list->fplmn_arr[i].fplmn.mcc,
                                      fplmn_list->fplmn_arr[i].fplmn.mnc,
                                      fplmn_list->fplmn_arr[i].fplmn.mnc_digit_num);

            if(SIM_SERVICE_OK == ser_result)
            {
                ptemp += len;
            }
            else
            {
                break;
            }
        }
        else
        {
            ptemp += 3; //for empty record, we should store it with FFFFFF
        }
    }

    return (ser_result);
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeLOCI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint16 len;
    SIM_SERVICE_RESULT_E ser_result;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_LOCI_T *loci = ef_mapping;

    *data_len = 11;
    SCI_MEMSET(ptemp, 0x0ff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //Encode the TMSI
    if(loci->tmsi.valid_flag)
    {
        SCI_MEMCPY(ptemp, loci->tmsi.tmsi_value, 4);
    }

    ptemp += 4;

    //Encode the mnc and mcc of LAI
    ser_result = EncodeMCCMNC(&len, ptemp, loci->lai.mcc, loci->lai.mnc,
                              loci->lai.mnc_digit_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    ptemp += len;

    //Encode the lac
    *ptemp = (uint8)(loci->lai.lac >> 8);
    ptemp += 1;
    *ptemp = (uint8)(loci->lai.lac);
    ptemp += 1;

    *ptemp = loci->tmsi_time;
    ptemp += 1;

    //Encode the Update State
    *ptemp = 0;

    //ref to GSM 11.11 section 10.3.17
    switch(loci->update_status)
    {
        case MM_UPDATED:
            *ptemp |= 0x00;
            break;
        case MM_NOT_UPDATED:
            *ptemp |= 0x01;
            break;
        case MM_PLMN_NOT_ALLOWED:
            *ptemp |= 0x02;
            break;
        case MM_LAI_NOT_ALLOWED:
            *ptemp |= 0x03;
            break;
        case MM_REVERSE_STATE:
            *ptemp |= 0x07;
            break;
        default:
            return SIM_SERVICE_ENCODE_ERROR;
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeAD(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_AD_T *ad = ef_mapping;

    if((ad->mnc_len_exist)
        && ((2 == ad->mnc_digit_num) || (3 == ad->mnc_digit_num)))
    {
        *data_len = 4;
    }
    else
    {
        *data_len = 3;
    }

    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    *ptemp = (uint8)ad->operation_mode;

    ptemp += 2;

    if(ad->ofm_actived)
    {
        *ptemp |= SIM_EF_AD_OFM_ENABLE_MARK;
    }
    else
    {
        *ptemp &= ~SIM_EF_AD_OFM_ENABLE_MARK;
    }

    if(4 == *data_len)
    {
        // length of MNC in the IMSI
        ptemp++;
        *ptemp = ad->mnc_digit_num;
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePHASE(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp  = data;
    SIM_EF_MAPPING_PHASE_T *phase = ef_mapping;

    *data_len = 1;

    if(forge_empty)
    {
        *ptemp = SIM_PH_1;
        return SIM_SERVICE_OK;
    }

    *ptemp = phase->phase;
#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVGCS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_VGCS_T *vgcs = ef_mapping;
    uint8 num;

    num = vgcs->validate_num;
    num = num > SIM_EF_VGCS_VGI_NUM ? SIM_EF_VGCS_VGI_NUM : num;
    *data_len = num * SIM_EF_VGCS_GID_SIZE;

    if(*data_len < SIM_EF_VGCS_GID_SIZE)
    {
        *data_len = SIM_EF_VGCS_GID_SIZE;
    }

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, &(vgcs->bgis), *data_len);
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVGCSS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_VGCSS_T *vgcss = ef_mapping;
    uint16 i, j, k = 0;

    *data_len = SIM_EF_VGCSS_SIZE;
    SCI_MEMSET(ptemp, 0x0, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    for(i = 1; i < (1 + SIM_EF_VGCS_VGI_NUM); i++)
    {
        k = i % 8;

        if(k != 0)
        {
            j = k - 1;
        }
        else
        {
            j = 7;
        }

        if(vgcss->actived[i-1])
        {
            *ptemp |= (1 << j);
        }

        if(k == 0)
        {
            ptemp++;
        }
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVBS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_VBS_T *vbs = ef_mapping;

    *data_len = SIM_EF_VBS_GID_SIZE * vbs->validate_num;

    if(*data_len < SIM_EF_VBS_GID_SIZE)
    {
        *data_len = SIM_EF_VBS_GID_SIZE;
    }

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, &(vbs->vbis), *data_len);
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeVBSS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_VBSS_T *vbss = ef_mapping;
    uint16 i, j, k = 0;

    *data_len = SIM_EF_VBSS_SIZE;
    SCI_MEMSET(ptemp, 0x0, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    for(i = 1; i < (1 + SIM_EF_VBS_VBI_NUM); i++)
    {
        k = i % 8;

        if(k != 0)
        {
            j = k - 1;
        }
        else
        {
            j = 7;
        }

        if(vbss->avtived[i-1])
        {
            *ptemp |= (1 << j);
        }

        if(k == 0)
        {
            ptemp++;
        }
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeeMLPP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_EMLPP_T *emlpp = ef_mapping;

    *data_len = 2;
    *ptemp = 0;

    if(forge_empty)
    {
        ptemp++;
        *ptemp = 0;
        return SIM_SERVICE_OK;
    }

    //Priority Level
    if(emlpp->priority_level.priority_l_A)
    {
        *ptemp |= 0x1;
    }

    if(emlpp->priority_level.priority_l_B)
    {
        *ptemp |= 0x2;
    }

    if(emlpp->priority_level.priority_l_0)
    {
        *ptemp |= 0x4;
    }

    if(emlpp->priority_level.priority_l_1)
    {
        *ptemp |= 0x8;
    }

    if(emlpp->priority_level.priority_l_2)
    {
        *ptemp |= 0x10;
    }

    if(emlpp->priority_level.priority_l_3)
    {
        *ptemp |= 0x20;
    }

    if(emlpp->priority_level.priority_l_4)
    {
        *ptemp |= 0x40;
    }

    ptemp++;
    *ptemp = 0;

    //Fast Call set-up conditions
    if(emlpp->fsc_condition.priority_l_A)
    {
        *ptemp |= 0x1;
    }

    if(emlpp->fsc_condition.priority_l_B)
    {
        *ptemp |= 0x2;
    }

    if(emlpp->fsc_condition.priority_l_0)
    {
        *ptemp |= 0x4;
    }

    if(emlpp->fsc_condition.priority_l_1)
    {
        *ptemp |= 0x8;
    }

    if(emlpp->fsc_condition.priority_l_2)
    {
        *ptemp |= 0x10;
    }

    if(emlpp->fsc_condition.priority_l_3)
    {
        *ptemp |= 0x20;
    }

    if(emlpp->fsc_condition.priority_l_4)
    {
        *ptemp |= 0x40;
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeAAem(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifndef SIM_CODE_COMPACT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_AAEM_T *aaem = ef_mapping;

    *data_len = 1;
    *ptemp = 0;

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    if(aaem->aa_priority_level.priority_l_A)
    {
        *ptemp |= 0x1;
    }

    if(aaem->aa_priority_level.priority_l_B)
    {
        *ptemp |= 0x2;
    }

    if(aaem->aa_priority_level.priority_l_0)
    {
        *ptemp |= 0x4;
    }

    if(aaem->aa_priority_level.priority_l_1)
    {
        *ptemp |= 0x8;
    }

    if(aaem->aa_priority_level.priority_l_2)
    {
        *ptemp |= 0x10;
    }

    if(aaem->aa_priority_level.priority_l_3)
    {
        *ptemp |= 0x20;
    }

    if(aaem->aa_priority_level.priority_l_4)
    {
        *ptemp |= 0x40;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_NOT_ENABLE;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeECC(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 i;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ECC_T *ecc = ef_mapping;

    for(i = 0; i < ecc->validate_num; i++)
    {
        SCI_MEMCPY(ptemp, &(ecc->call_codes[i]), 3);
        ptemp += 3;
        *data_len += 3;
    }

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
        return SIM_SERVICE_OK;
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCBMIR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CBMIR_T *cbmir = ef_mapping;

    *data_len = SIM_EF_CBMIR_CB_SIZE * cbmir->validate_num;

    if(*data_len < SIM_EF_CBMIR_CB_SIZE)
    {
        *data_len = SIM_EF_CBMIR_CB_SIZE;
    }
    else if(*data_len > SIM_MAX_CBMIR_RANGE_NUM * SIM_EF_CBMIR_CB_SIZE)
    {
        *data_len = SIM_MAX_CBMIR_RANGE_NUM * SIM_EF_CBMIR_CB_SIZE;
    }

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, &(cbmir->cbmi_ranges), *data_len);
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeDCK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_DCK_T *dck = ef_mapping;

    *data_len = 16;
    SCI_MEMSET(ptemp, 0xff, 16);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    if(dck->b_network_dck)
    {
        SCI_MEMCPY(ptemp, dck->network_dck, 4);
    }

    ptemp += 4;

    if(dck->b_network_sub_dck)
    {
        SCI_MEMCPY(ptemp, dck->network_sub_dck, 4);
    }

    ptemp += 4;

    if(dck->b_sp_dck)
    {
        SCI_MEMCPY(ptemp, dck->sp_dck, 4);
    }

    ptemp += 4;

    if(dck->b_corperate_dck)
    {
        SCI_MEMCPY(ptemp, dck->corperate_dck, 4);
    }

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCNL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_CNL_T *cnl = ef_mapping;
    uint8 *ptemp = data;

    *data_len = SIM_EF_CNL_ELEMENT_SIZE * cnl->validate_num;

    if(*data_len < SIM_EF_CNL_ELEMENT_SIZE)
    {
        *data_len = SIM_EF_CNL_ELEMENT_SIZE;
    }

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, &(cnl->net_lists), *data_len);
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeNIA(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_NIA_T *nia = ef_mapping;
    uint8 *ptemp = data;
    uint8 text_len = nia->iformative_text_len;

    text_len = text_len > SIM_EF_NIA_MAX_INFORMATIVE_TEXT_LEN ? SIM_EF_NIA_MAX_INFORMATIVE_TEXT_LEN : text_len;
    *data_len = SIM_EF_NIA_CATEGORY_LEN + text_len;

    if(forge_empty || nia->alerting_cat == 0xff)
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
    }
    else
    {
        *ptemp = nia->alerting_cat;
        ptemp++;
        SCI_MEMCPY(ptemp, nia->iformative_text, text_len);
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeKcGRPS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef _SUPPORT_GPRS_
    uint8 *ptemp = data;
    SIM_EF_MAPPING_KC_GPRS_T *kc_gprs = ef_mapping;

    *data_len = 9;
    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    SCI_MEMCPY(ptemp, kc_gprs->kc, 8);
    ptemp += 8;

    *ptemp = ((uint8)kc_gprs->cksn & 0x07);
#endif
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeLOCIGPRS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef _SUPPORT_GPRS_
    uint16 len;
    SIM_SERVICE_RESULT_E ser_result;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_LOCI_GPRS_T *loci_gprs = ef_mapping;

    *data_len = 14;
    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //Encode the P-TMSI
    if(loci_gprs->p_tmsi.valid_flag)
    {
        SCI_MEMCPY(ptemp, loci_gprs->p_tmsi.value, 4);
    }
    else
    {
        return SIM_SERVICE_OK;
    }

    ptemp += 4;

    //Encode the P-TMSI signature
    if(loci_gprs->p_tmsi_signature.valid_flag)
    {
        *ptemp = (uint8)(loci_gprs->p_tmsi_signature.value >> 16);
        *(ptemp + 1) = (uint8)(loci_gprs->p_tmsi_signature.value >> 8);
        *(ptemp + 2) = (uint8)(loci_gprs->p_tmsi_signature.value);
    }


    ptemp += 3;

    //encode the MCC and MNC
    ser_result = EncodeMCCMNC(&len, ptemp, loci_gprs->rai.mcc,
                              loci_gprs->rai.mnc,
                              loci_gprs->rai.mnc_digit_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    ptemp += len;

    //Encode the lac
    *ptemp = (uint8)(loci_gprs->rai.lac >> 8);
    ptemp += 1;
    *ptemp = (uint8)(loci_gprs->rai.lac);
    ptemp += 1;
    *ptemp = (uint8)(loci_gprs->rai.rac);
    ptemp += 1;

    //Encode the Update State
    *ptemp &= ~SIM_EF_LOCI_GPRS_UPDATE_STATUS_MARK;

    //ref to GSM 11.11 section 10.3.17
    switch(loci_gprs->update_status)
    {
        case SIM_R_UPDATED:
            *ptemp |= 0x00;
            break;
        case SIM_R_NOT_UPDATED:
            *ptemp |= 0x01;
            break;
        case SIM_R_PLMN_NOT_ALLOWED:
            *ptemp |= 0x02;
            break;
        case SIM_R_ROUTING_AREA_NOT_ALLOWED:
            *ptemp |= 0x03;
            break;
        case SIM_R_RESERVED:
            *ptemp |= 0x07;
            break;
        default:
            return SIM_SERVICE_ENCODE_ERROR;
    }

#endif
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSUME(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_SUME_T *sume = ef_mapping;
    uint8 *ptemp = data;

    *data_len = 0;
    *ptemp = sume->alpha_id_tag;
    ptemp++;
    (*data_len)++;

    if(sume->alpha_id_len < 0x80)
    {
        *ptemp = sume->alpha_id_len;
        ptemp++;
        (*data_len)++;
    }
    else
    {
        *ptemp = 0x81;
        ptemp++;
        *ptemp = sume->alpha_id_len;
        ptemp++;
        (*data_len) += 2;
    }

    SCI_MEMCPY(ptemp, &(sume->alpha_id[0]), sume->alpha_id_len);
    (*data_len) += sume->alpha_id_len;

    //the len has reached an EF max size
    if(*data_len >= SIM_EF_SUME_MAX_ALPHA_ID_LEN)
    {
        *data_len = SIM_EF_SUME_MAX_ALPHA_ID_LEN;

        if(forge_empty)
        {
            SCI_MEMSET(data, 0xff, (*data_len));
        }

        return SIM_SERVICE_OK;
    }

    //Is the Icon Identifier exist?
    if(sume->icon_flag)
    {
        ptemp += sume->alpha_id_len;
        *ptemp = sume->icon_id_tag;
        ptemp++;
        *ptemp = sume->icon_id_len;
        ptemp++;
        *ptemp = sume->icon_qualifier;
        ptemp++;
        *ptemp = sume->icon_id;
        (*data_len) += SIM_EF_SUME_ICONID_SIZE;
    }

    if(forge_empty)
    {
        SCI_MEMSET(data, 0xff, (*data_len));
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePNN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_PNN_T *pnn = ef_mapping;
    uint8 *ptemp = data;

    *data_len = 0;
    *ptemp = SIM_FULL_NETWORK_NAME_IEI;
    ptemp++;

    if(forge_empty)
    {
        *ptemp = 0;
        *data_len = 2;
        return SIM_SERVICE_OK;
    }
    else
    {
        *ptemp = (uint8)pnn->full_nt_name.length;
    }

    ptemp++;
    *ptemp = BIT_7;//ext 1

    if(pnn->full_nt_name.is_add_ci)
    {
        *ptemp |= BIT_3;
    }

    if(UCS_DCS_VALUE == pnn->full_nt_name.code_scheme)
    {
        *ptemp |= BIT_5;
    }

    (*data_len) += 3;

    ptemp++;
    SCI_MEMCPY(ptemp, pnn->full_nt_name.name, (pnn->full_nt_name.length - 1));
    (*data_len) += (pnn->full_nt_name.length - 1);
    //In fact, we should be more considerate here about DEFAULT_DCS_VALUE...

    if(pnn->is_short_nt_name_flag)
    {
        *ptemp = SIM_SHORT_NETWORK_NAME_IEI;
        ptemp++;
        *ptemp = (uint8)pnn->short_nt_name.length;
        ptemp++;
        *ptemp = BIT_7;//ext 1

        if(pnn->short_nt_name.is_add_ci)
        {
            *ptemp |= BIT_3;
        }

        if(UCS_DCS_VALUE == pnn->short_nt_name.code_scheme)
        {
            *ptemp |= BIT_5;
        }

        (*data_len) += 3;
        ptemp++;
        SCI_MEMCPY(ptemp, pnn->short_nt_name.name, (pnn->short_nt_name.length - 1));
        (*data_len) += (pnn->short_nt_name.length - 1);
        //In fact, we should be more considerate here about DEFAULT_DCS_VALUE...
    }

    return SIM_SERVICE_OK;
#else
    //EFpnn cannot be UPDATED
    return SIM_SERVICE_ENCODE_ERROR;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeOPL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_OPL_T *opl = ef_mapping;
    uint8 *ptemp = data;
    uint16 temp = 0;
    uint8 digit_100, digit_10, digit = 0;

    *data_len = 8;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xFF, 8);
        return SIM_SERVICE_OK;
    }

    temp = opl->plmn.mcc;
    digit_100 = (uint8)(temp % 100);
    digit_10 = (uint8)((temp - digit_100 * 100) % 10);
    digit = (uint8)((temp - digit_100 * 100 - digit_10 * 10) % 10);
    *ptemp = digit_100;//MCC digit 1
    *ptemp |= (digit_10 << 4); //MCC digit 2
    ptemp++;

    temp = opl->plmn.mnc;
    *ptemp = digit;//MCC digit 3

    if(2 == opl->plmn.mnc_digit_num)
    {
        *ptemp |= 0xF0;//MNC digit 3 should be F
    }

    digit_100 = (uint8)(temp % 100);
    digit_100 = (digit_100 << 4);
    *ptemp &= 0xF;//clear the MSB 4 bits
    *ptemp |= digit_100;//MNC digit 3
    ptemp++;

    digit_10 = (uint8)((temp - digit_100 * 100) % 10);
    digit = (uint8)((temp - digit_100 * 100 - digit_10 * 10) % 10);
    *ptemp |= digit_10;//MNC digit 1
    *ptemp |= (digit << 4); //MNC digit 2
    ptemp++;

    //LAC1
    temp = (uint16)opl->lac_1;
    *ptemp = (uint8)temp;
    ptemp++;
    temp = (temp >> 8);
    *ptemp = (uint8)temp;
    ptemp++;

    //LAC2
    temp = (uint16)opl->lac_2;
    *ptemp = (uint8)temp;
    ptemp++;
    temp = (temp >> 8);
    *ptemp = (uint8)temp;
    ptemp++;

    //PNN index
    *ptemp = opl->pnn_index;

    return SIM_SERVICE_OK;
#else
    //EFopl cannot be UPDATED
    return SIM_SERVICE_ENCODE_ERROR;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMBDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_MBNUM_T *mailbox_num;

    temp_ptr = data;
    mailbox_num = ef_mapping;

    if(0 == g_sim_r4_vm_record_len[cur_sim_task]) //double check
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    *data_len = g_sim_r4_vm_record_len[cur_sim_task];
    SCI_MEMSET(temp_ptr, 0xff, g_sim_r4_vm_record_len[cur_sim_task]);
    *temp_ptr = 0;

    //forge a empty record
    if(forge_empty)
    {
        *temp_ptr = SIM_EXT_RECORD_UNKNOW;
        return SIM_SERVICE_OK;
    }

    //copy the alpha identifier
    if(g_sim_r4_vm_record_len[cur_sim_task] > 14)
    {
        SCI_MEMCPY(temp_ptr, mailbox_num->alpha_id, (g_sim_r4_vm_record_len[cur_sim_task] - 14));
        temp_ptr += g_sim_r4_vm_record_len[cur_sim_task] - 14;
    }

    //decide the length of the BCD
    *temp_ptr = mailbox_num->mailbox_number_len + 1;
    temp_ptr += 1;

    //the TON and NPI
    if(mailbox_num->is_ton_npi_exist)
    {
        *temp_ptr &= ~SIM_EF_DN_TON_MARK;
        *temp_ptr |= ((uint8)mailbox_num->ton) << 4;
        *temp_ptr &= ~SIM_EF_DN_NPI_MARK;
        *temp_ptr |= (uint8)mailbox_num->npi;
    }
    else
    {
        *temp_ptr = 0xFF;
    }

    temp_ptr += 1;

    //the mailbox number
    if(mailbox_num->mailbox_number_len > 0)
    {
        SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified:Check_value */
        SCI_MEMCPY(temp_ptr, mailbox_num->mailbox_number, mailbox_num->mailbox_number_len);
    }

    temp_ptr += SIM_EF_DN_DN_LEN;

    //the capability/configuration idnetifier
    if(mailbox_num->is_ccp_exist)
    {
        *temp_ptr = mailbox_num->ccp_id;
    }
    else
    {
        *temp_ptr = 0xFF;
    }

    temp_ptr += 1;

    //the extension1 identifier
    if(mailbox_num->is_ext_exist)
    {
        *temp_ptr = mailbox_num->ext_id;
    }
    else
    {
        *temp_ptr = 0xFF;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMBI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_EF_MAPPING_MBI_T *mbi = ef_mapping;
    uint8 *ptemp = data;

    if(0 == g_sim_r4_vm_mbi_record_len[cur_sim_task]) //double check
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    *data_len = g_sim_r4_vm_mbi_record_len[cur_sim_task];

    if(forge_empty)
    {
        *ptemp = 0xFF;
        ptemp++;
        *ptemp = 0xFF;
        ptemp++;
        *ptemp = 0xFF;
        ptemp++;
        *ptemp = 0xFF;
    }
    else
    {
        *ptemp = mbi->voicemail_index;
        ptemp++;
        *ptemp = mbi->fax_index;
        ptemp++;
        *ptemp = mbi->email_index;
        ptemp++;
        *ptemp = mbi->other_index;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMWIS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_MWIS_T *mwis_mapping = ef_mapping;

    if(0 == g_sim_r4_vm_mwis_record_len[cur_sim_task]) //double-check
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    *data_len = g_sim_r4_vm_mwis_record_len[cur_sim_task];
    SCI_MEMSET(ptemp, 0x0, g_sim_r4_vm_mwis_record_len[cur_sim_task]);

    *ptemp = 0;//init

    if(mwis_mapping->vm_waiting_count > 0)
    {
        *ptemp |= BIT_0;
    }

    if(mwis_mapping->fax_waiting_count > 0)
    {
        *ptemp |= BIT_1;
    }

    if(mwis_mapping->data_waiting_count > 0)
    {
        *ptemp |= BIT_2;
    }

    if(mwis_mapping->other_waiting_count > 0)
    {
        *ptemp |= BIT_3;
    }

    ptemp++;
    *ptemp++ = mwis_mapping->vm_waiting_count;
    *ptemp++ = mwis_mapping->fax_waiting_count;
    *ptemp++ = mwis_mapping->data_waiting_count;
    *ptemp   = mwis_mapping->other_waiting_count;

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCFIS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CFIS_T *cfis_mapping = ef_mapping;

    *data_len = 16;
    SCI_MEMSET(ptemp, 0xFF, 16);

    *ptemp = cfis_mapping->msp_id;
    ptemp++;

    *ptemp = 0;//init

    if(cfis_mapping->voice_callfrd_uncond)
    {
        *ptemp |= BIT_0;
    }

    if(cfis_mapping->fax_callfrd_uncond)
    {
        *ptemp |= BIT_1;
    }

    if(cfis_mapping->data_callfrd_uncond)
    {
        *ptemp |= BIT_2;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSPDI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    //EFspdi cannot be UPDATED
    return SIM_SERVICE_ENCODE_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMMSN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    return SIM_SERVICE_ENCODE_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeEXT8(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    return SIM_SERVICE_ENCODE_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMMSICP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    //EFmmsicp cannot be UPDATED
    return SIM_SERVICE_ENCODE_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMMSUP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    return SIM_SERVICE_ENCODE_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMMSUCP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    return SIM_SERVICE_ENCODE_ERROR;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSAI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_SAI_T *sai = ef_mapping;
    uint8 *ptemp = data;
    uint8 text_len = sai->lsa_lsa_only_access_len;

    *data_len = SIM_EF_SAI_LSA_INDICATOR_LEN + text_len;

    if(forge_empty || (text_len == 0))
    {
        *ptemp = 0;
        ptemp++;
        SCI_MEMSET(ptemp, 0xff, text_len);
    }
    else
    {
        *ptemp = sai->lsa_only_access;
        ptemp++;
        SCI_MEMCPY(ptemp, sai->lsa_only_access_text, text_len);
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSLL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_SLL_T *sll = ef_mapping;
    uint8 *ptemp = data;
    uint8 name_len = sll->lsa_name_len;
    uint16 plmn_len = 0;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;

    *data_len = name_len + SIM_EF_SLL_MANDATORY_LEN;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0xff, (*data_len));
        return SIM_SERVICE_OK;
    }

    //LSA name
    if((name_len >= 1) && (name_len <= SIM_EF_SLL_LSA_NAME_MAX_LEN))
    {
        SCI_MEMCPY(ptemp, sll->lsa_name, name_len);
        ptemp += name_len;
    }
    else
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    //configuration parameter
    *ptemp = sll->icon_qualifier;
    *ptemp &= SIM_EF_SLL_ICONID_MASK;

    if(sll->idle_mode_support)
    {
        *ptemp |= SIM_EF_SLL_IDLE_SUPPORT_MASK;
    }

    if(sll->lsa_indication_idle_mode)
    {
        *ptemp |= SIM_EF_SLL_LSA_IDLE_SUPPORT_MASK;
    }

    ptemp++;

    //the rest data
    *ptemp = sll->rfu;
    ptemp++;
    *ptemp = sll->icon_identifier;
    ptemp++;
    *ptemp = sll->priority;
    ptemp++;
    ser_result = EncodeMCCMNC(&plmn_len, ptemp, sll->plmn.mcc, sll->plmn.mnc, sll->plmn.mnc_digit_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }
    else
    {
        if(3 != plmn_len)
        {
            plmn_len = 3;
        }

        ptemp += plmn_len;
        *ptemp = (uint8)((sll->lsa_file_id) >> 8);//byte X+8 is the high byte
        ptemp ++;
        *ptemp = (uint8)(sll->lsa_file_id);//byte X+9 is the low byte
        ptemp ++;
        *ptemp = sll->lsa_rec_number;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_NOT_ENABLE;
#endif
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeADN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //if the record information of the file still not get,the get the
    //record information
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].adn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_ADN, &(g_sim_card_setting[cur_sim_task].adn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return EncodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].adn_info), ef_mapping, forge_empty);
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeFDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //if the record information of the file still not get,the get the
    //record information
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].fdn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_FDN, &(g_sim_card_setting[cur_sim_task].fdn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return EncodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].fdn_info), ef_mapping, forge_empty);
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSMS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_SMS_T *psms = ef_mapping;

    //set the record len of SMS
    *data_len = SIM_EF_SMS_RECORD_LEN;
    //set the mem to 0xff
    SCI_MEMSET(ptemp, 0xff, *data_len);
    *ptemp = SIM_EF_SMS_FREE;

    //fork a empty SMS
    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }


    //attention:the first byte have been set to 0xff,so
    switch(psms->sms_status)
    {
        case MN_SMS_FREE_SPACE:
            *ptemp = SIM_EF_SMS_FREE;
            break;
        case MN_SMS_MT_READED:
            *ptemp = SIM_EF_SMS_MT_READED;
            break;
        case MN_SMS_MT_TO_BE_READ:
            *ptemp = SIM_EF_SMS_MT_TO_BE_READ;
            break;
        case MN_SMS_MO_TO_BE_SEND:
            *ptemp = SIM_EF_SMS_MO_TO_BE_SENT;
            break;
        case MN_SMS_MO_SR_NOT_REQUEST:
            *ptemp = SIM_EF_SMS_MO_NO_SR;
            break;
        case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
            *ptemp = SIM_EF_SMS_MO_SR_NOT_RECV;
            break;
        case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
            *ptemp = SIM_EF_SMS_MO_SR_NOT_SAVE;
            break;
        case MN_SMS_MO_SR_RECEIVED_AND_STORED:
            *ptemp = SIM_EF_SMS_MO_SR_SAVED;
            break;
        default:
            *ptemp = SIM_EF_SMS_FREE;
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::EncodeSMS,Unrecognized sms_status=%d received."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_ENCODE_4031_112_2_17_22_58_49_4426, (uint8 *)"d", psms->sms_status);
#endif
            return SIM_SERVICE_ENCODE_ERROR;
    }

    ptemp += 1;
    //copy the SMS content
    SCI_MEMCPY(ptemp, psms->sms_sim_sm_arr, ((*data_len) - 1));
    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCCP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_CCP_T *pccp = ef_mapping;

    *data_len = SIM_EF_CCP_RECORD_LEN;
    SCI_MEMSET(ptemp, UINT8_INVALIDE, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //the Bearer capability informatin element
    SCI_MEMCPY(ptemp, pccp->bc_info, SIM_EF_CCP_BC_INFO_LEN);
    ptemp += SIM_EF_CCP_BC_INFO_LEN;

    //the Bytes reserved
    SCI_MEMCPY(ptemp, pccp->revers_bytes, SIM_EF_CCP_RESERVED_BYTE_LEN);

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMSISDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //if the record information of the file still not get,the get the
    //record information
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].msisdn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_MSISDN, &(g_sim_card_setting[cur_sim_task].msisdn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return EncodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].msisdn_info), ef_mapping, forge_empty);
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSMSP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data, *pindicator;
    SMS_PARAM_T *param = ef_mapping;
    SIM_SERVICE_RESULT_E ser_result;

    //if the EF_SMSP record info is blank,get the SMSP record info
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_SMSP, &(g_sim_card_setting[cur_sim_task].smsp_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    //get the record len from the card setting.
    *data_len = g_sim_card_setting[cur_sim_task].smsp_info.original_alpha_id_len + SIM_EF_SMSP_MIN_RECORD_LEN;
    SCI_MEMSET(ptemp, 0xff, *data_len);
    //set the SMS empty parameter indicator
    *(ptemp + g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len) = (uint8)SIM_EF_SMSP_EMPTY_INDICATOR;

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //there are alpha id present in the SMS parameter record
    if(param->alpha_id_present)
    {
        if(g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len > param->alpha_id_t.id_len)
        {
            SCI_MEMCPY(ptemp, param->alpha_id_t.id, param->alpha_id_t.id_len);
        }
        else
        {
            SCI_MEMCPY(ptemp, param->alpha_id_t.id, g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len);
        }
    }

    ptemp += g_sim_card_setting[cur_sim_task].smsp_info.original_alpha_id_len;

    //the parameter indicator pointer
    pindicator = ptemp;
    ptemp += 1;

    //TP-Destination Address
    if(param->tp_dest_addr_present)
    {
        *pindicator &= ~SIM_EF_SMSP_TP_DA_INDICATOR;
        SCI_MEMCPY(ptemp, param->tp_dest_addr_arr, SMS_ADDR_MAX_LEN);
    }

    ptemp += SIM_EF_SMSP_ADD_OBJECT_LEN;

    //the TS-Service Centre Address
    if(param->tp_sc_addr_present)
    {
        *pindicator &= ~SIM_EF_SMSP_TP_SC_INDICATOR;
        SCI_MEMCPY(ptemp, param->tp_sc_addr_arr, SMS_ADDR_MAX_LEN);
    }

    ptemp += SIM_EF_SMSP_ADD_OBJECT_LEN;

    //the TP-Protocol Identifier
    if(param->tp_protocol_id_present)
    {
        *pindicator &= ~SIM_EF_SMSP_TP_PI_INDICATOR;
        *ptemp = param->tp_protocol_id;
    }

    ptemp += 1;

    //the TP-Data Coding Scheme
    if(param->tp_dcs_present)
    {
        *pindicator &= ~SIM_EF_SMSP_TP_DCS_INDICATOR;
        *ptemp = param->tp_dcs;
    }

    ptemp += 1;

    //the TP-Validity Period
    if(param->tp_vp_present)
    {
        *pindicator &= ~SIM_EF_SMSP_TP_VP_INDICATOR;
        *ptemp = param->relative_format_vp;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSMSS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_SMSS_T *psmss = ef_mapping;

    *data_len = SIM_EF_SMSS_MIN_FILE_SIZE;
    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_DECODE_EF_ERROR;
    }

    *ptemp = psmss->tp_mr;
    ptemp += 1;

    if(psmss->mem_exeed)
    {
        *ptemp &= ~SIM_EF_SMSS_MEM_EXCEED_MARK;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeLND(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_SERVICE_RESULT_E ser_result;

    //if the record information of the file still not get,the get the
    //record information
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].lnd_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_LND, &(g_sim_card_setting[cur_sim_task].lnd_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    return EncodeDN(data_len, data, &(g_sim_card_setting[cur_sim_task].lnd_info), ef_mapping, forge_empty);
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSMSR(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_SMSR_T *smsr_mapping = ef_mapping;

    *data_len = SIM_EF_SMSR_RECORD_LEN;
    SCI_MEMSET(ptemp, 0xff, *data_len);
    *ptemp = 0;     //mean the status is empty.

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //save the SM record id
    *ptemp = smsr_mapping->sm_record_id;
    ptemp += 1;

    //copy the status report array
    SCI_MEMCPY(ptemp, smsr_mapping->status_report, SMS_SIM_STATUS_REPORT_LEN);

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_SERVICE_RESULT_E ser_result;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_DN_T *pdn = ef_mapping;

    //if the record information of the file still not get,the get the
    //record information
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].sdn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_SDN, &(g_sim_card_setting[cur_sim_task].sdn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    //decide the data len of the record
    *data_len = g_sim_card_setting[cur_sim_task].sdn_info.original_alpha_id_len + g_sim_card_setting[cur_sim_task].sdn_info.other_content_len;
    SCI_MEMSET(ptemp, UINT8_INVALIDE, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //copy the alpha identifier
    if(pdn->alpha_id_len > g_sim_card_setting[cur_sim_task].sdn_info.alpha_id_len)
    {
        SCI_MEMCPY(ptemp, pdn->alpha_id, g_sim_card_setting[cur_sim_task].sdn_info.alpha_id_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, pdn->alpha_id, pdn->alpha_id_len);
    }

    ptemp += g_sim_card_setting[cur_sim_task].sdn_info.original_alpha_id_len;

    //decide the length of the BCD
    *ptemp = pdn->dn_len + 1;
    ptemp += 1;

    //the TON and NPI
    if(pdn->is_ton_npi_exist)
    {
        *ptemp &= ~SIM_EF_DN_TON_MARK;
        *ptemp |= ((uint8)pdn->ton) << 4;
        *ptemp &= ~SIM_EF_DN_NPI_MARK;
        *ptemp |= (uint8)pdn->npi;
    }

    ptemp += 1;

    //the dialling number/SSC string
    if(pdn->dn_len > 0)
    {
        if(pdn->dn_len > SIM_EF_DN_DN_LEN)
        {
            pdn->dn_len = SIM_EF_DN_DN_LEN;
        }

        SCI_MEMCPY(ptemp, pdn->dialling_num, pdn->dn_len);
    }

    ptemp += SIM_EF_DN_DN_LEN;

    //the capability/configuration idnetifier
    if(pdn->is_ccp_exist)
    {
        *ptemp = pdn->ccp_id;
    }

    ptemp += 1;

    //the extension1 identifier
    if(pdn->is_ext_exist)
    {
        *ptemp = pdn->ext_id;
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeEXT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_EXT_T *pextension = ef_mapping;

    *data_len = SIM_EF_EXT_RECORD_LEN;
    *ptemp = SIM_EXT_RECORD_UNKNOW;
    SCI_MEMSET(ptemp, UINT8_INVALIDE, *data_len);

    //forge a empty record
    if(forge_empty)
    {
        *ptemp = SIM_EXT_RECORD_UNKNOW;

        return SIM_SERVICE_OK;
    }

    //the record type
    *ptemp = (uint8)pextension->record_type;
    ptemp += 1;

    //the extension data
    if(pextension->record_type == SIM_EXT_RECORD_ADDTIONAL_DATA)
    {
        *ptemp = pextension->extension_data_len;
        SCI_MEMCPY(ptemp + 1, pextension->extension_data, SIM_EF_EXT_ADDITIONAL_DATA_LEN);
    }
    else
    {
        SCI_MEMCPY(ptemp, pextension->extension_data, SIM_EF_EXT_DATA_LEN);
    }

    ptemp += SIM_EF_EXT_DATA_LEN;

    //the next record num
    if(pextension->more_data)
    {
        *ptemp = pextension->next_record_id;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeBDN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_SERVICE_RESULT_E ser_result;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_DN_T *pdn = ef_mapping;

    //if the record information of the file still not get,the get the
    //record information
    if(UINT8_INVALIDE == g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len)
    {
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_BDN, &(g_sim_card_setting[cur_sim_task].bdn_info));

        if(SIM_SERVICE_OK != ser_result)
        {
            return ser_result;
        }
    }

    //decide the data len of the record
    *data_len = g_sim_card_setting[cur_sim_task].bdn_info.original_alpha_id_len + g_sim_card_setting[cur_sim_task].bdn_info.other_content_len;
    SCI_MEMSET(ptemp, UINT8_INVALIDE, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //copy the alpha identifier
    if(pdn->alpha_id_len > g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len)
    {
        SCI_MEMCPY(ptemp, pdn->alpha_id, g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, pdn->alpha_id, pdn->alpha_id_len);
    }

    ptemp += g_sim_card_setting[cur_sim_task].bdn_info.original_alpha_id_len;

    //decide the length of the BCD
    *ptemp = pdn->dn_len + 1;
    ptemp += 1;

    //the TON and NPI
    if(pdn->is_ton_npi_exist)
    {
        *ptemp &= ~SIM_EF_DN_TON_MARK;
        *ptemp |= ((uint8)pdn->ton) << 4;
        *ptemp &= ~SIM_EF_DN_NPI_MARK;
        *ptemp |= (uint8)pdn->npi;
    }

    ptemp += 1;

    //the dialling number/SSC string
    if(pdn->dn_len > 0)
    {
        if(pdn->dn_len > SIM_EF_DN_DN_LEN)
        {
            pdn->dn_len = SIM_EF_DN_DN_LEN;
        }

        SCI_MEMCPY(ptemp, pdn->dialling_num, pdn->dn_len);
    }

    ptemp += SIM_EF_DN_DN_LEN;

    //the capability/configuration idnetifier
    if(pdn->is_ccp_exist)
    {
        *ptemp = pdn->ccp_id;
    }

    ptemp += 1;

    //the extension1 identifier
    if(pdn->is_ext_exist)
    {
        *ptemp = pdn->ext_id;
    }

    return SIM_SERVICE_OK;
}




/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeEXT4(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *ptemp = data;
    SIM_EF_MAPPING_EXT_T *pextension = ef_mapping;

    *data_len = SIM_EF_EXT_RECORD_LEN;
    *ptemp = SIM_EXT_RECORD_UNKNOW;
    SCI_MEMSET(ptemp, UINT8_INVALIDE, *data_len);

    //forge a empty record
    if(forge_empty)
    {
        *ptemp = SIM_EXT_RECORD_UNKNOW;
        return SIM_SERVICE_OK;
    }

    //the record type
    *ptemp = (uint8)pextension->record_type;
    ptemp += 1;

    //the extension data
    if(pextension->record_type == SIM_EXT_RECORD_ADDTIONAL_DATA)
    {
        *ptemp = pextension->extension_data_len;
        SCI_MEMCPY(ptemp + 1, pextension->extension_data, SIM_EF_EXT_ADDITIONAL_DATA_LEN);
    }
    else
    {
        SCI_MEMCPY(ptemp, pextension->extension_data, SIM_EF_EXT_DATA_LEN);
    }

    ptemp += SIM_EF_EXT_DATA_LEN;

    //the next record num
    if(pextension->more_data)
    {
        *ptemp = pextension->next_record_id;
    }

    return SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//     the function encode the data from upper layer to write to the EF IMG.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeIMG(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_IMG_T *img = ef_mapping;
    uint8 *ptemp = data;
    uint16 i = 0;
    uint8 num = 0;

    *data_len = 0;

    if(forge_empty)
    {
        *ptemp = 0xff;
        ptemp++;
        *data_len = 1;
        SCI_MEMSET(ptemp, 0xff, (SIM_EF_IMG_INSTANCE_SIZE*(img->num)));
        *data_len += SIM_EF_IMG_INSTANCE_SIZE * (img->num);

        if(img->rfu_flag)
        {
            ptemp += ((SIM_EF_IMG_INSTANCE_SIZE * (img->num)) + 1);
            *ptemp = 0xff;
            *data_len += 1;
        }

        return SIM_SERVICE_OK;
    }

    num = img->num;
    num = num > SIM_EF_IMG_INSTANCE_NUM ? SIM_EF_IMG_INSTANCE_NUM : num;
    *ptemp = num;
    ptemp++;
    *data_len += 1;

    for(i = 0; i < num; i++)
    {
        SCI_MEMCPY(ptemp, &(img->descriptor[i]), SIM_EF_IMG_INSTANCE_SIZE);
        ptemp += SIM_EF_IMG_INSTANCE_SIZE;
        *data_len += SIM_EF_IMG_INSTANCE_SIZE;
    }

    if(img->rfu_flag)
    {
        *ptemp = img->rfu;
        *data_len += 1;
    }

#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      The common function to encode the MCC and MNC for EFloci,EFplmnsel,
//    EFfplmn,EFlocigprs
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMCCMNC(
    uint16 *data_len,   //out:the length of the data encoded
    void   *data,       //out:the data buf for encode
    uint16  mcc,        //the MCC
    uint16  mnc,        //the MNC
    uint16  mnc_digit_num   //the MNC digit Number 2 or 3
)
{
#define MAX_MCC_MNC_VALUE       999     //the max value of the MCC and MNC
#define MCC_MNC_DATA_LEN        3       //the mcc/mnc data length

    uint8 mcc_digit_1, mcc_digit_2, mcc_digit_3 = 0;
    uint8 mnc_digit_1, mnc_digit_2, mnc_digit_3 = 0;
    uint8 *ptemp = (uint8 *)data;
    *data_len = 0;

    //check the MNC and MCC value
    if((mcc > MAX_MCC_MNC_VALUE) ||
        (mnc > MAX_MCC_MNC_VALUE))
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    if((2 != mnc_digit_num) &&
        (3 != mnc_digit_num))
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    //get the 3 digit from the MCC
    mcc_digit_1 = mcc / 100;
    mcc_digit_3 = (mcc % 10);
    mcc_digit_2 = (mcc - mcc_digit_3 - mcc_digit_1 * 100) / 10;

    //get the three digit from the MNC
    mnc_digit_1 = mnc / 100;

    //check wheather to ecnode the MNC to a 2 digit or 3 digit while the mnc_digit_3 is 0
    if((mnc_digit_1 == 0) && (mnc_digit_num == 2))
    {
        //MNC is a 2 digit one
        mnc_digit_3 = 0xF;
        mnc_digit_2 = (mnc % 10);
        mnc_digit_1 = (mnc - mnc_digit_2) / 10;
    }
    else
    {
        //MNC is a 3 digit one
        mnc_digit_3 = (mnc % 10);
        mnc_digit_2 = (mnc - mnc_digit_3 - mnc_digit_1 * 100) / 10;
    }



    //encode the MNC and MCC data
    *ptemp = (mcc_digit_1 & 0xF) + ((mcc_digit_2 << 4) & 0xF0);
    ptemp += 1;
    *ptemp = (mcc_digit_3 & 0xF) + ((mnc_digit_3 << 4) & 0xF0);
    ptemp += 1;
    *ptemp = (mnc_digit_1 & 0xF) + ((mnc_digit_2 << 4) & 0xF0);
    *data_len = MCC_MNC_DATA_LEN;

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
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
static SIM_SERVICE_RESULT_E EncodeCCFlag(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_CFFLAG_T *callfrd_flag;


    temp_ptr = data;
    callfrd_flag = ef_mapping;

    // 04/24/2008 delete the trace
    //  SCI_TRACE_LOW("SIM:: call func EncodeCCFlag()");

    *data_len = 1;
    SCI_MEMSET(temp_ptr, 0xff, *data_len);
    *temp_ptr = 0;

    //forge a empty record
    if(forge_empty)
    {
        *temp_ptr = SIM_EXT_RECORD_UNKNOW;
        return SIM_SERVICE_OK;
    }


    *temp_ptr = (uint8)((callfrd_flag->line2_callfrd_uncond << 4) + (callfrd_flag->line1_callfrd_uncond));
    temp_ptr += 1;

    //there is a data/fax service flag
    if(callfrd_flag->is_datafax_flag_present)
    {
        *data_len += 1;
        *temp_ptr = (uint8)((callfrd_flag->data_callfrd_uncond << 4) + (callfrd_flag->fax_callfrd_uncond));
    }

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM:: EncodeCCFlag, data_len is %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_ENCODE_4796_112_2_17_22_58_50_4427, (uint8 *)"d", *data_len);
#endif

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
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
static SIM_SERVICE_RESULT_E EncodeVoiceMsgWtFlag(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_VMWFLAG_T *voice_waiting_flag;

    if(0 == g_sim_r4_vm_vmwflag_len[cur_sim_task]) //double-check
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    temp_ptr = data;
    voice_waiting_flag = ef_mapping;

    *data_len = g_sim_r4_vm_vmwflag_len[cur_sim_task];
    SCI_MEMSET(temp_ptr, 0xff, *data_len);
    *temp_ptr = 0;

    //forge a empty record
    if(forge_empty)
    {
        *temp_ptr = SIM_EXT_RECORD_UNKNOW;

        return SIM_SERVICE_OK;
    }

    *temp_ptr = (uint8)((voice_waiting_flag->line2_status << 4) + (voice_waiting_flag->line1_status));

    if(g_sim_r4_vm_vmwflag_len[cur_sim_task] >= 2)
    {
        temp_ptr += 1;
        *temp_ptr = (uint8)((voice_waiting_flag->data_status << 4) + (voice_waiting_flag->fax_status));
    }

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM:: EncodeVoiceMsgWtFlag, data_len is %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_ENCODE_4857_112_2_17_22_58_50_4428, (uint8 *)"d", *data_len);
#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
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
static SIM_SERVICE_RESULT_E EncodeCSP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_CSP_T *csp;

    temp_ptr = data;
    csp = ef_mapping;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM:: call func EncodeCSP()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_ENCODE_4906_112_2_17_22_58_50_4429, (uint8 *)"");
#endif

    *data_len = 18;
    SCI_MEMSET(temp_ptr, 0xff, *data_len);
    *temp_ptr = 0;

    //forge a empty record
    if(forge_empty)
    {
        *temp_ptr = SIM_EXT_RECORD_UNKNOW;
        return SIM_SERVICE_OK;
    }

    //encode Call Offering
    *temp_ptr = SIM_CALL_OFFERING;
    temp_ptr++;
    *temp_ptr = (csp->call_offering.is_cfu << 7) +
                (csp->call_offering.is_cfb << 6) +
                (csp->call_offering.is_cfnry << 5) +
                (csp->call_offering.is_cfnrc  << 4) +
                (csp->call_offering.is_ct << 3);
    temp_ptr++;

    //encode Call Restriction
    *temp_ptr = SIM_CALL_RESTRICTION;
    temp_ptr++;
    *temp_ptr = (csp->call_restriction.is_boac << 7) +
                (csp->call_restriction.is_boic << 6) +
                (csp->call_restriction.is_boic_exhc << 5) +
                (csp->call_restriction.is_baic  << 4) +
                (csp->call_restriction.is_bic_roam << 3);
    temp_ptr++;

    //encode Other Supp Service
    *temp_ptr = SIM_OTHER_SERVICES;
    temp_ptr++;
    *temp_ptr = (csp->other_supp_serv.is_mpty << 7) +
                (csp->other_supp_serv.is_cug << 6) +
                (csp->other_supp_serv.is_aoc << 5) +
                (csp->other_supp_serv.is_pref_cug  << 4) +
                (csp->other_supp_serv.is_cug_oa << 3);
    temp_ptr++;

    //encode Call Completion
    *temp_ptr = SIM_CALL_COMPLETION;
    temp_ptr++;
    *temp_ptr = (csp->call_completion.is_hold << 7) +
                (csp->call_completion.is_cw << 6) +
                (csp->call_completion.is_ccbs << 5) +
                (csp->call_completion.is_u2u_signalling  << 4);
    temp_ptr++;

    //encode Teleservices
    *temp_ptr = SIM_TELESERVICES;
    temp_ptr++;
    *temp_ptr = (csp->teleservices.is_sm_mt << 7) +
                (csp->teleservices.is_sm_mo << 6) +
                (csp->teleservices.is_sm_cb << 5) +
                (csp->teleservices.is_reply_path << 4) +
                (csp->teleservices.is_del_conf << 3) +
                (csp->teleservices.is_protocol_id << 2) +
                (csp->teleservices.is_validity_period << 1);
    temp_ptr++;

    //encode CPHS Teleservices
    *temp_ptr = SIM_CPHS_TELE;
    temp_ptr++;
    *temp_ptr = csp->cphs_teleserv.is_als << 7;
    temp_ptr++;

    //encode CPHS Features
    *temp_ptr = SIM_CPHS_FEATURES;
    temp_ptr++;
    *temp_ptr = 0xFF;  //reserved for future use
    temp_ptr++;


    //encode Number Identif
    *temp_ptr = SIM_NUM_ID;
    temp_ptr++;
    *temp_ptr = (csp->number_identif.is_clip << 7) +
                (csp->number_identif.is_colr << 5) +
                (csp->number_identif.is_colp  << 4) +
                (csp->number_identif.is_mci  << 3) +
                (csp->number_identif.is_cli_send << 1) +
                (csp->number_identif.is_cli_block);
    temp_ptr++;

    //encode Phase services
    *temp_ptr = SIM_PHASE2_SERVICE;
    temp_ptr++;
    *temp_ptr = (csp->phase_service.is_gprs << 7) +
                (csp->phase_service.is_hscsd << 6) +
                (csp->phase_service.is_vgc << 5) +
                (csp->phase_service.is_vbs << 4) +
                (csp->phase_service.is_msp << 3) +
                (csp->phase_service.is_multi_band << 2);
    temp_ptr++;

    if(csp->is_value_added_serv_present)
    {
        *temp_ptr = SIM_VALUEADDED_SERV;
        temp_ptr++;
        *temp_ptr = (csp->value_added_service.is_plmn_mode << 7) +
                    (csp->value_added_service.is_vps << 6) +
                    (csp->value_added_service.is_sm_mo_paging << 5) +
                    (csp->value_added_service.is_sm_mo_email << 4) +
                    (csp->value_added_service.is_fax << 3) +
                    (csp->value_added_service.is_data << 2) +
                    csp->value_added_service.is_language;
        temp_ptr++;
        *data_len += 2;
    }

    if(csp->is_info_numbers_present)
    {
        *temp_ptr = SIM_INFO_NUM;
        temp_ptr++;
        *temp_ptr = csp->info_numbers;
        *data_len += 2;
    }

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM:: EncodeCSP, data_len is %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_ENCODE_5030_112_2_17_22_58_51_4430, (uint8 *)"d", *data_len);
#endif

    return SIM_SERVICE_OK;

}


/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
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
static SIM_SERVICE_RESULT_E EncodeMailboxNum(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    uint8 *temp_ptr;
    SIM_EF_MAPPING_MBNUM_T *mailbox_num;

    temp_ptr = data;
    mailbox_num = ef_mapping;

    if(0 == g_sim_cphs_vm_record_len[cur_sim_task]) //double check
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    *data_len = g_sim_cphs_vm_record_len[cur_sim_task];
    SCI_MEMSET(temp_ptr, 0xff, g_sim_cphs_vm_record_len[cur_sim_task]);
    *temp_ptr = 0;

    //forge a empty record
    if(forge_empty)
    {
        *temp_ptr = SIM_EXT_RECORD_UNKNOW;
        return SIM_SERVICE_OK;
    }

    //copy the alpha identifier
    if(g_sim_cphs_vm_record_len[cur_sim_task] > 14)
    {
        SCI_MEMCPY(temp_ptr, mailbox_num->alpha_id, (g_sim_cphs_vm_record_len[cur_sim_task] - 14));
        temp_ptr += g_sim_cphs_vm_record_len[cur_sim_task] - 14;
    }

    //decide the length of the BCD
    *temp_ptr = mailbox_num->mailbox_number_len + 1;
    temp_ptr += 1;

    //the TON and NPI
    if(mailbox_num->is_ton_npi_exist)
    {
        *temp_ptr &= ~SIM_EF_DN_TON_MARK;
        *temp_ptr |= ((uint8)mailbox_num->ton) << 4;
        *temp_ptr &= ~SIM_EF_DN_NPI_MARK;
        *temp_ptr |= (uint8)mailbox_num->npi;
    }
    else
    {
        *temp_ptr = 0xFF;
    }

    temp_ptr += 1;

    //the mailbox number
    if(mailbox_num->mailbox_number_len > 0)
    {
        SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified:Check_value */
        SCI_MEMCPY(temp_ptr, mailbox_num->mailbox_number, mailbox_num->mailbox_number_len);
    }

    temp_ptr += SIM_EF_DN_DN_LEN;

    //the capability/configuration idnetifier
    if(mailbox_num->is_ccp_exist)
    {
        *temp_ptr = mailbox_num->ccp_id;
    }
    else
    {
        *temp_ptr = 0xFF;
    }

    temp_ptr += 1;

    //the extension1 identifier
    if(mailbox_num->is_ext_exist)
    {
        *temp_ptr = mailbox_num->ext_id;
    }
    else
    {
        *temp_ptr = 0xFF;
    }

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:King.li
//    Note: new added for CPHS extended functions
//    refer to  CPHS Phase 2  B.4.2.6
*********************************************************************/
static SIM_SERVICE_RESULT_E EncodeInformationnum(   //return value:the result of encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef _CPHS_SUPPORT
    uint8 *temp_ptr;
    SIM_EF_MAPPING_INFONUM_T *information_num;

    temp_ptr = data;
    information_num = ef_mapping;

    *data_len = 5 + information_num->alpha_id_len + information_num->information_digits_number_len;

    //forge a empty record
    if(forge_empty)
    {
        *temp_ptr = SIM_EXT_RECORD_UNKNOW;
        return SIM_SERVICE_OK;
    }

    /*the Alpha length field*/
    *temp_ptr = information_num->alpha_id_len;
    temp_ptr += 1;
    /*the Entry Status field*/
    *temp_ptr = information_num->entry_status;
    temp_ptr += 1;

    /*the Alpha identifier field*/
    if(information_num->alpha_id_len > 0)
    {
        SCI_MEMCPY(temp_ptr, information_num->alpha_id, information_num->alpha_id_len);
    }

    temp_ptr += information_num->alpha_id_len;
    /*the BCD Number length field*/
    *temp_ptr = information_num->number_len;

    temp_ptr += 1;

    /*the TON & NPI field*/
    if(information_num->is_ton_npi_exist)
    {
        *temp_ptr = (uint8)(information_num->npi) + (uint8)((information_num->ton) << 4);
        *temp_ptr |= 0x80;
    }
    else
    {
        *temp_ptr = 0xFF;
    }

    temp_ptr += 1;

    /*the Digits section field*/
    if(information_num->information_digits_number_len > 0)
    {
        SCI_MEMCPY(temp_ptr, information_num->information_digits_number, information_num->information_digits_number_len);
    }

    temp_ptr += information_num->information_digits_number_len;

    /*the extention id field*/
    if(information_num->is_ext_exist)
    {
        *temp_ptr = information_num->ext_id;
    }
    else
    {
        *temp_ptr = 0xFF;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_NOT_ENABLE;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:King.li
//    Note: new added for CPHS extended functions
//    refer to  CPHS Phase 2  B.4.2.6
*********************************************************************/
static SIM_SERVICE_RESULT_E EncodeSSTCPHS(   //return value:the result of encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    return SIM_SERVICE_NOT_ENABLE;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF PLMNWACT.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodePLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_EF_MAPPING_PLMNWACT_T   *plmnwact   = ef_mapping;
    uint8                       *ptemp      = data;
    uint8                       i           = 0;
    uint8                       max_num     = 0;
    uint16                      plmn_len    = 0;
    SIM_SERVICE_RESULT_E        ser_result  = SIM_SERVICE_OK;

    max_num     = (plmnwact->max_num > SIM_EF_PLMNWACT_ARR_NUM) ? SIM_EF_PLMNWACT_ARR_NUM : plmnwact->max_num;
    *data_len   = SIM_EF_PLMNWACT_ARR_SIZE * max_num;

    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty || (0 == plmnwact->validate_num))
    {
        ser_result = SIM_SERVICE_OK;
    }
    else
    {
        for(i = 0; i < plmnwact->validate_num; i++)
        {
            ser_result = EncodeMCCMNC(&plmn_len, ptemp,
                                      plmnwact->plmn[i].plmn.mcc,
                                      plmnwact->plmn[i].plmn.mnc,
                                      plmnwact->plmn[i].plmn.mnc_digit_num);

            if(SIM_SERVICE_OK != ser_result)
            {
                return ser_result;
            }

            if(3 != plmn_len)
            {
                plmn_len = 3;
            }

            ptemp += plmn_len;
            *ptemp = 0;     //UMTS

            if(plmnwact->plmn[i].utran_flag)
            {
                *ptemp |= 0x80;
            }

            if(plmnwact->plmn[i].e_utran_flag)
            {
                *ptemp |= 0x40;
            }

            ptemp++;
            *ptemp = 0;

            if(plmnwact->plmn[i].gsm_flag)
            {
                *ptemp |= 0x80;
            }

            if(plmnwact->plmn[i].compact_gsm_flag)
            {
                *ptemp |= 0x40;
            }

            if(plmnwact->plmn[i].cdma2000_hrpd_flag)
            {
                *ptemp |= 0x20;
            }

            if(plmnwact->plmn[i].cdma2000_1xrtt_flag)
            {
                *ptemp |= 0x10;
            }

            ptemp++;
        }
    }

    return (ser_result);
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF OPLMNWACT.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeOPLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_SERVICE_RESULT_E        ser_result  = SIM_SERVICE_OK;

#ifdef WIN32
    SIM_EF_MAPPING_OPLMNWACT_T *oplmnwact   = ef_mapping;
    uint8                       *ptemp      = data;
    uint8                       i           = 0;
    uint8                       max_num     = 0;
    uint16                      plmn_len    = 0;

    max_num     = (oplmnwact->max_num > SIM_EF_OPLMNWACT_ARR_NUM) ? SIM_EF_OPLMNWACT_ARR_NUM : oplmnwact->max_num;
    *data_len   = SIM_EF_OPLMNWACT_ARR_SIZE * max_num;

    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty || (0 == oplmnwact->validate_num))
    {
        ser_result = SIM_SERVICE_OK;
    }
    else
    {
        for(i = 0; i < oplmnwact->validate_num; i++)
        {
            ser_result = EncodeMCCMNC(&plmn_len, ptemp,
                                      oplmnwact->plmn[i].plmn.mcc,
                                      oplmnwact->plmn[i].plmn.mnc,
                                      oplmnwact->plmn[i].plmn.mnc_digit_num);

            if(SIM_SERVICE_OK != ser_result)
            {
                return ser_result;
            }

            if(3 != plmn_len)
            {
                plmn_len = 3;
            }

            ptemp += plmn_len;
            *ptemp = 0;     //UMTS

            if(oplmnwact->plmn[i].utran_flag)
            {
                *ptemp |= 0x80;
            }

            if(oplmnwact->plmn[i].e_utran_flag)
            {
                *ptemp |= 0x40;
            }

            ptemp++;
            *ptemp = 0;

            if(oplmnwact->plmn[i].gsm_flag)
            {
                *ptemp |= 0x80;
            }

            if(oplmnwact->plmn[i].compact_gsm_flag)
            {
                *ptemp |= 0x40;
            }

            if(oplmnwact->plmn[i].cdma2000_hrpd_flag)
            {
                *ptemp |= 0x20;
            }

            if(oplmnwact->plmn[i].cdma2000_1xrtt_flag)
            {
                *ptemp |= 0x10;
            }

            ptemp++;
        }
    }

#endif

    return (ser_result);

}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF HPLMNACT.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeHPLMNWACT(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_HPLMNWACT_T *hplmnwact = ef_mapping;
    uint8 *ptemp = data;
    uint8 i = 0;
    uint16 plmn_len = 0;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;

    if(hplmnwact->validate_num > SIM_EF_HPLMNWACT_ARR_NUM)
    {
        hplmnwact->validate_num = SIM_EF_HPLMNWACT_ARR_NUM;
    }

    *data_len = SIM_EF_HPLMNWACT_ARR_SIZE * (hplmnwact->validate_num);

    if(forge_empty || (0 == hplmnwact->validate_num))
    {
        SCI_MEMSET(ptemp, 0xff, *data_len);
    }
    else
    {
        for(i = 0; i < hplmnwact->validate_num; i++)
        {
            ser_result = EncodeMCCMNC(&plmn_len, ptemp, hplmnwact->plmn[i].plmn.mcc, hplmnwact->plmn[i].plmn.mnc, hplmnwact->plmn[i].plmn.mnc_digit_num);

            if(SIM_SERVICE_OK != ser_result)
            {
                return ser_result;
            }

            if(3 != plmn_len)
            {
                plmn_len = 3;
            }

            ptemp += plmn_len;
            *ptemp = 0;

            if(hplmnwact->plmn[i].utran_flag)
            {
                *ptemp |= 0x80;
            }

            if(hplmnwact->plmn[i].e_utran_flag)
            {
                *ptemp |= 0x40;
            }

            ptemp++;
            *ptemp = 0;

            if(hplmnwact->plmn[i].gsm_flag)
            {
                *ptemp |= 0x80;
            }

            if(hplmnwact->plmn[i].compact_gsm_flag)
            {
                *ptemp |= 0x40;
            }

            if(hplmnwact->plmn[i].cdma2000_hrpd_flag)
            {
                *ptemp |= 0x20;
            }

            if(hplmnwact->plmn[i].cdma2000_1xrtt_flag)
            {
                *ptemp |= 0x10;
            }

            ptemp++;
        }
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF CPBCCH.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCPBCCH(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifndef SIM_CODE_COMPACT
#ifdef _SUPPORT_GPRS_
    SIM_EF_MAPPING_CPBCCH_T *cpbcch = ef_mapping;
    uint8 *ptemp = data;

    *data_len = cpbcch->validate_num;
    *data_len = *data_len > SIM_EF_CPBCCH_ARR_NUM ? SIM_EF_CPBCCH_ARR_NUM : *data_len;

    if(*data_len < SIM_EF_CPBCCH_ELEMENT_SIZE)
    {
        *data_len = SIM_EF_CPBCCH_ELEMENT_SIZE;
    }

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0, *data_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, cpbcch, *data_len);
    }

#endif
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_NOT_ENABLE;
#endif
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF INVSCAN.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeINVSCAN(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_INVSCAN_T *invscan = ef_mapping;
    uint8 *ptemp = data;

    *data_len = 1;

    if(forge_empty)
    {
        *ptemp = 0xff;
    }
    else
    {
        *ptemp = invscan->invscan_flag;
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF ECCP.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeECCP(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifndef SIM_CODE_COMPACT
    SIM_EF_MAPPING_ECCP_T *eccp = ef_mapping;
    uint8 *ptemp = data;

    if(forge_empty)
    {
        if(eccp->bc_info_len == 0)
        {
            *data_len = 1;
        }
        else
        {
            *data_len = eccp->bc_info_len;
        }

        SCI_MEMSET(ptemp, 0xff, *data_len);
        return SIM_SERVICE_OK;
    }

    if(eccp->bc_info_len < SIM_EF_ECCP_MANDATORY_LEN)
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }
    else
    {
        SCI_MEMCPY(ptemp, eccp->bc_info, eccp->bc_info_len);
        *data_len = eccp->bc_info_len;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_NOT_ENABLE;
#endif
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF CMI.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeCMI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_CMI_T *cmi = ef_mapping;
    uint8 *ptemp = data;

    *data_len = cmi->alpha_id_len + SIM_EF_CMI_CMI_LEN;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0, *data_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, cmi->alpha_id, cmi->alpha_id_len);
        ptemp += cmi->alpha_id_len;
        *ptemp = cmi->cmi;
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF MExEST.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeMEXEST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_MEXEST_T *mexest = ef_mapping;
    uint8 *ptemp = data;
    uint16 i = 0;

    *data_len = 1;

    if(forge_empty)
    {
        *ptemp = 0;
        return SIM_SERVICE_OK;
    }

    *data_len = 0;

    for(i = 0; i < mexest->service_num;)
    {
        *ptemp = 0;
        *data_len += 1;

        //encode the first service stautus of a byte
        if(mexest->service_table[i].allocated)
        {
            *ptemp |= 0x01;

            if(mexest->service_table[i].activated)
            {
                *ptemp |= 0x02;
            }
            else
            {
                *ptemp &= 0xfd;
            }
        }
        else
        {
            *ptemp &= 0xfe;
        }

        i++;

        if(i >= mexest->service_num)
        {
            return SIM_SERVICE_OK;
        }

        //encode the second service status of a byte
        if(mexest->service_table[i].allocated)
        {
            *ptemp |= 0x04;

            if(mexest->service_table[i].activated)
            {
                *ptemp |= 0x08;
            }
            else
            {
                *ptemp &= 0xf7;
            }
        }
        else
        {
            *ptemp &= 0xfb;
        }

        i++;

        if(i >= mexest->service_num)
        {
            return SIM_SERVICE_OK;
        }

        //encode the third service status of a byte
        if(mexest->service_table[i].allocated)
        {
            *ptemp |= 0x10;

            if(mexest->service_table[i].activated)
            {
                *ptemp |= 0x20;
            }
            else
            {
                *ptemp &= 0xdf;
            }
        }
        else
        {
            *ptemp &= 0xef;
        }

        i++;

        if(i >= mexest->service_num)
        {
            return SIM_SERVICE_OK;
        }

        //encode the fourth service status of a byte
        if(mexest->service_table[i].allocated)
        {
            *ptemp |= 0x40;

            if(mexest->service_table[i].activated)
            {
                *ptemp |= 0x80;
            }
            else
            {
                *ptemp &= 0x7f;
            }
        }
        else
        {
            *ptemp &= 0xbf;
        }

        i++;

        if(i >= mexest->service_num)
        {
            return SIM_SERVICE_OK;
        }

        ptemp += 1;
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF ORPK.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeORPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_ORPK_T *orpk = ef_mapping;
    uint8 *ptemp = data;

    orpk->key_len = orpk->key_len > SIM_EF_ORPK_KEYID_MAX_LEN ? SIM_EF_ORPK_KEYID_MAX_LEN : orpk->key_len;
    *ptemp = orpk->para_indicator;
    ptemp++;
    *ptemp = orpk->flags;
    ptemp++;
    *ptemp = orpk->type;
    ptemp++;
    *ptemp = (uint8)((orpk->key_file_id) >> 8);//byte 4 is the high byte
    ptemp++;
    *ptemp = (uint8)(orpk->key_file_id);
    ptemp++;
    *ptemp = (uint8)((orpk->offset_into_file) >> 8);//byte 6 is the high byte
    ptemp++;
    *ptemp = (uint8)(orpk->offset_into_file);
    ptemp++;
    *ptemp = (uint8)((orpk->data_len) >> 8);//byte 8 is the high byte
    ptemp++;
    *ptemp = (uint8)(orpk->data_len);
    ptemp++;
    *ptemp = orpk->key_len;
    ptemp++;
    SCI_MEMCPY(ptemp, orpk->key_id, orpk->key_len);
    *data_len = orpk->key_len + SIM_EF_ORPK_MANDATORY_LEN;

    if(forge_empty)
    {
        SCI_MEMSET(data, 0, *data_len);
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF ARPK.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeARPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_ARPK_T *arpk = ef_mapping;
    uint8 *ptemp = data;

    arpk->key_len = arpk->key_len > SIM_EF_ARPK_KEYID_MAX_LEN ? SIM_EF_ARPK_KEYID_MAX_LEN : arpk->key_len;
    *ptemp = arpk->para_indicator;
    ptemp++;
    *ptemp = arpk->flags;
    ptemp++;
    *ptemp = arpk->type;
    ptemp++;
    *ptemp = (uint8)((arpk->key_file_id) >> 8);//byte 4 is the high byte
    ptemp++;
    *ptemp = (uint8)(arpk->key_file_id);
    ptemp++;
    *ptemp = (uint8)((arpk->offset_into_file) >> 8);//byte 6 is the high byte
    ptemp++;
    *ptemp = (uint8)(arpk->offset_into_file);
    ptemp++;
    *ptemp = (uint8)((arpk->data_len) >> 8);//byte 8 is the high byte
    ptemp++;
    *ptemp = (uint8)(arpk->data_len);
    ptemp++;
    *ptemp = arpk->key_len;
    ptemp++;
    SCI_MEMCPY(ptemp, arpk->key_id, arpk->key_len);
    *data_len = arpk->key_len + SIM_EF_ARPK_MANDATORY_LEN;

    if(forge_empty)
    {
        SCI_MEMSET(data, 0, *data_len);
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF TPRPK.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeTPRPK(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    SIM_EF_MAPPING_TPRPK_T *tprpk = ef_mapping;
    uint8 *ptemp = data;
    /*lint -e413*/
    uint8 len_before_keyid = offsetof(SIM_EF_MAPPING_TPRPK_T, key_id);
    /*lint +e413*/

    tprpk->key_len = tprpk->key_len > SIM_EF_TPRPK_KEYID_MAX_LEN ? SIM_EF_TPRPK_KEYID_MAX_LEN : tprpk->key_len;
    SCI_MEMCPY(ptemp, &(tprpk->para_indicator), len_before_keyid);
    ptemp += len_before_keyid;
    SCI_MEMCPY(ptemp, tprpk->key_id, tprpk->key_len);
    ptemp += tprpk->key_len;
    *data_len = tprpk->key_len + len_before_keyid;

    tprpk->cer_len = tprpk->cer_len > SIM_EF_TPRPK_CERID_MAX_LEN ? SIM_EF_TPRPK_CERID_MAX_LEN : tprpk->cer_len;
    *ptemp = tprpk->cer_len;
    *data_len += 1;
    ptemp++;
    SCI_MEMCPY(ptemp, tprpk->cer_id, tprpk->cer_len);
    *data_len += tprpk->cer_len;

    if(forge_empty)
    {
        SCI_MEMSET(data, 0, *data_len);
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
//    refer to  CPHS Phase 2  B.4.1.2
//
  Bytes |Description|M/O| Length  |
 _______|___________|___|_________|
 1 to n |  PLMN name| M | n Bytes |
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeOPNameStr(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ONSTRING_T *onstring = ef_mapping;

    SCI_MEMSET(ptemp, 0xff, SIM_EF_MAX_ONSTRING_LEN);

    if(onstring->len > SIM_EF_MAX_ONSTRING_LEN)
    {
        onstring->len = SIM_EF_MAX_ONSTRING_LEN;
    }

    *data_len = onstring->len;

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    SCI_MEMCPY(ptemp, onstring->onstring, onstring->len);
#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that decode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
//    refer to  CPHS Phase 2  B.4.1.2

  Bytes |   Description         |M/O| Length   |
________|_______________________|___|__________|
 1 to 10|Operator Name Shortform| M | 10 Bytes |
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeOPNameShort(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef WIN32
    uint8 *ptemp = data;
    SIM_EF_MAPPING_ONSHORT_T *onshort = ef_mapping;

    SCI_MEMSET(ptemp, 0xff, SIM_EF_MAX_ONSHORTFORM_LEN);
    *data_len = SIM_EF_MAX_ONSHORTFORM_LEN;

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    SCI_MEMCPY(ptemp, onshort->onshort , SIM_EF_MAX_ONSHORTFORM_LEN);
#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EFest
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeUSIMEST(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef SIM_3G_USIM_SUPPORT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_EST_T *est = ef_mapping;
    *data_len = 1;
    *ptemp = 0;

    if(est->is_FDN_enable)
    {
        *ptemp |= BIT_0;
    }
    else
    {
        *ptemp &= ~BIT_0;
    }

    if(est->is_BDN_enable)
    {
        *ptemp |= BIT_1;
    }
    else
    {
        *ptemp &= ~BIT_1;
    }

#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EFpsloci
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeUSIMPSLOCI(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    //this EFpsloci's structrue is the same as EFlocigprs.
#ifdef SIM_3G_USIM_SUPPORT
#ifdef _SUPPORT_GPRS_
    uint16 len;
    SIM_SERVICE_RESULT_E ser_result;
    uint8 *ptemp = data;
    SIM_EF_MAPPING_LOCI_GPRS_T *loci_gprs = ef_mapping;

    *data_len = 14;
    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //Encode the P-TMSI
    if(loci_gprs->p_tmsi.valid_flag)
    {
        SCI_MEMCPY(ptemp, loci_gprs->p_tmsi.value, 4);
    }
    else
    {
        return SIM_SERVICE_OK;
    }

    ptemp += 4;

    //Encode the P-TMSI signature
    if(loci_gprs->p_tmsi_signature.valid_flag)
    {
        *ptemp = (uint8)(loci_gprs->p_tmsi_signature.value >> 16);
        *(ptemp + 1) = (uint8)(loci_gprs->p_tmsi_signature.value >> 8);
        *(ptemp + 2) = (uint8)(loci_gprs->p_tmsi_signature.value);
    }


    ptemp += 3;

    //encode the MCC and MNC
    ser_result = EncodeMCCMNC(&len, ptemp, loci_gprs->rai.mcc,
                              loci_gprs->rai.mnc,
                              loci_gprs->rai.mnc_digit_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    ptemp += len;

    //Encode the lac
    *ptemp = (uint8)(loci_gprs->rai.lac >> 8);
    ptemp += 1;
    *ptemp = (uint8)(loci_gprs->rai.lac);
    ptemp += 1;
    *ptemp = (uint8)(loci_gprs->rai.rac);
    ptemp += 1;

    //Encode the Update State
    *ptemp &= ~SIM_EF_LOCI_GPRS_UPDATE_STATUS_MARK;

    //ref to GSM 11.11 section 10.3.17
    switch(loci_gprs->update_status)
    {
        case SIM_R_UPDATED:
            *ptemp |= 0x00;
            break;
        case SIM_R_NOT_UPDATED:
            *ptemp |= 0x01;
            break;
        case SIM_R_PLMN_NOT_ALLOWED:
            *ptemp |= 0x02;
            break;
        case SIM_R_ROUTING_AREA_NOT_ALLOWED:
            *ptemp |= 0x03;
            break;
        case SIM_R_RESERVED:
            *ptemp |= 0x07;
            break;
        default:
            return SIM_SERVICE_ENCODE_ERROR;
    }

#endif
#endif
    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the corresponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeDN(    //return value:the result of the encode operation
    uint16                      *data_len,   //out:the len of the data been encode out
    void                        *data,         //out:the data that been encode out
    const SIM_RECORD_EF_INFO_T  *dn_info_ptr,
    void                         *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN               forge_empty   //encode a empty data for erasure
)
{
    SIM_EF_MAPPING_DN_T  *pdn  = ef_mapping;
    uint8                   *ptemp      = data;

    //decide the data len of the record
    *data_len = dn_info_ptr->original_alpha_id_len + dn_info_ptr->other_content_len;
    SCI_MEMSET(ptemp, UINT8_INVALIDE, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //copy the alpha identifier
    if(pdn->alpha_id_len > dn_info_ptr->alpha_id_len)
    {
        SCI_MEMCPY(ptemp, pdn->alpha_id, dn_info_ptr->alpha_id_len);
    }
    else
    {
        SCI_MEMCPY(ptemp, pdn->alpha_id, pdn->alpha_id_len);
    }

    ptemp += dn_info_ptr->original_alpha_id_len;

    //decide the length of the BCD
    *ptemp = pdn->dn_len + 1;
    ptemp += 1;

    //the TON and NPI
    if(pdn->is_ton_npi_exist)
    {
        *ptemp &= ~SIM_EF_DN_TON_MARK;
        *ptemp |= ((uint8)pdn->ton) << 4;
        *ptemp &= ~SIM_EF_DN_NPI_MARK;
        *ptemp |= (uint8)pdn->npi;
    }

    ptemp += 1;

    //the dialling number/SSC string
    if(pdn->dn_len > 0)
    {
        SCI_ASSERT(*data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified:Check_value */
        SCI_MEMCPY(ptemp, pdn->dialling_num, pdn->dn_len);
    }

    ptemp += SIM_EF_DN_DN_LEN;

    //the capability/configuration identifier
    if(pdn->is_ccp_exist)
    {
        *ptemp = pdn->ccp_id;
    }

    ptemp += 1;

    //the extension1 identifier
    if(pdn->is_ext_exist)
    {
        *ptemp = pdn->ext_id;
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
static SIM_SERVICE_RESULT_E EncodeORANGEDYN2FLAG(
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef _CPHS_SUPPORT
    SIM_EF_MAPPING_ORANGE_DYN2_FLAG_T *dynamic2flag = ef_mapping;
    uint8 *ptemp = data;

    *ptemp = 0;
    *data_len = 1;

    if((!forge_empty) && (dynamic2flag->is_als_locked))
    {
        *ptemp |= BIT_0;
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_ENCODE_ERROR;
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
static SIM_SERVICE_RESULT_E EncodeORANGECSPLINE2(
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef _CPHS_SUPPORT
    uint8 *temp_ptr = data;
    SIM_EF_MAPPING_ORANGE_CSP_LINE2_T *csp = ef_mapping;

    *data_len = 20;//CSP_Line2 has 20 bytes
    SCI_MEMSET(temp_ptr, 0xff, *data_len);
    *temp_ptr = 0;

    //forge a empty record
    if(forge_empty)
    {
        *temp_ptr = SIM_EXT_RECORD_UNKNOW;
        return SIM_SERVICE_OK;
    }

    //encode Call Offering
    *temp_ptr = SIM_CALL_OFFERING;
    temp_ptr++;
    *temp_ptr = (csp->call_offering.is_cfu << 7) +
                (csp->call_offering.is_cfb << 6) +
                (csp->call_offering.is_cfnry << 5) +
                (csp->call_offering.is_cfnrc  << 4) +
                (csp->call_offering.is_ct << 3);
    temp_ptr++;

    //encode Call Restriction
    *temp_ptr = SIM_CALL_RESTRICTION;
    temp_ptr++;
    *temp_ptr = (csp->call_restriction.is_boac << 7) +
                (csp->call_restriction.is_boic << 6) +
                (csp->call_restriction.is_boic_exhc << 5) +
                (csp->call_restriction.is_baic  << 4) +
                (csp->call_restriction.is_bic_roam << 3);
    temp_ptr++;

    //encode Other Supp Service
    *temp_ptr = SIM_OTHER_SERVICES;
    temp_ptr++;
    *temp_ptr = (csp->other_supp_serv.is_mpty << 7) +
                (csp->other_supp_serv.is_cug << 6) +
                (csp->other_supp_serv.is_aoc << 5) +
                (csp->other_supp_serv.is_pref_cug  << 4) +
                (csp->other_supp_serv.is_cug_oa << 3);
    temp_ptr++;

    //encode Call Completion
    *temp_ptr = SIM_CALL_COMPLETION;
    temp_ptr++;
    *temp_ptr = (csp->call_completion.is_hold << 7) +
                (csp->call_completion.is_cw << 6) +
                (csp->call_completion.is_ccbs << 5) +
                (csp->call_completion.is_u2u_signalling  << 4);
    temp_ptr++;

    //encode Teleservices
    *temp_ptr = SIM_TELESERVICES;
    temp_ptr++;
    *temp_ptr = (csp->teleservices.is_sm_mt << 7) +
                (csp->teleservices.is_sm_mo << 6) +
                (csp->teleservices.is_sm_cb << 5) +
                (csp->teleservices.is_reply_path << 4) +
                (csp->teleservices.is_del_conf << 3) +
                (csp->teleservices.is_protocol_id << 2) +
                (csp->teleservices.is_validity_period << 1);
    temp_ptr++;

    //encode CPHS Teleservices
    *temp_ptr = SIM_CPHS_TELE;
    temp_ptr++;
    *temp_ptr = 0xFF;//reserved for future use
    temp_ptr++;

    //encode CPHS Features
    *temp_ptr = SIM_CPHS_FEATURES;
    temp_ptr++;
    *temp_ptr = 0xFF;  //reserved for future use
    temp_ptr++;

    //encode Number Identif
    *temp_ptr = SIM_NUM_ID;
    temp_ptr++;
    *temp_ptr = (csp->number_identif.is_clip << 7) +
                (csp->number_identif.is_colr << 5) +
                (csp->number_identif.is_colp  << 4) +
                (csp->number_identif.is_mci  << 3) +
                (csp->number_identif.is_cli_send << 1) +
                (csp->number_identif.is_cli_block);
    temp_ptr++;

    //encode Phase services
    *temp_ptr = SIM_PHASE2_SERVICE;
    temp_ptr++;
    *temp_ptr = (csp->phase_service.is_gprs << 7) +
                (csp->phase_service.is_hscsd << 6) +
                (csp->phase_service.is_vgc << 5) +
                (csp->phase_service.is_vbs << 4) +
                (csp->phase_service.is_msp << 3) +
                (csp->phase_service.is_multi_band << 2);
    temp_ptr++;

    if(csp->is_value_added_serv_present)
    {
        *temp_ptr = SIM_VALUEADDED_SERV;
        temp_ptr++;
        *temp_ptr = (csp->value_added_service.is_plmn_mode << 7) +
                    (csp->value_added_service.is_vps << 6) +
                    (csp->value_added_service.is_sm_mo_paging << 5) +
                    (csp->value_added_service.is_sm_mo_email << 4) +
                    (csp->value_added_service.is_fax << 3) +
                    (csp->value_added_service.is_data << 2) +
                    csp->value_added_service.is_language;
        temp_ptr++;
        *data_len += 2;
    }

    if(csp->is_spid_present)
    {
        *temp_ptr = SIM_SPID;
        temp_ptr++;
        *temp_ptr = csp->spid;
        *data_len += 2;
    }

    if(csp->is_info_numbers_present)
    {
        *temp_ptr = SIM_INFO_NUM;
        temp_ptr++;
        *temp_ptr = csp->info_numbers;
        *data_len += 2;
    }

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM:: EncodeCSP, data_len is %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_EF_ENCODE_6421_112_2_17_22_58_54_4431, (uint8 *)"d", *data_len);
#endif
    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_ENCODE_ERROR;
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
static SIM_SERVICE_RESULT_E EncodeORANGEDYNFLAG(
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifdef _CPHS_SUPPORT
    SIM_EF_MAPPING_ORANGE_DYN_FLAG_T *dynamicflag = ef_mapping;
    uint8 *ptemp = data;

    *ptemp = 0;
    *data_len = 1;

    if(!forge_empty)
    {
        if(1 == dynamicflag->selected_line) // 1=line1,0=line2
        {
            *ptemp = 1;
        }
    }

    return SIM_SERVICE_OK;
#else
    return SIM_SERVICE_ENCODE_ERROR;
#endif
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeKeys(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_EF_MAPPING_KEYS_T *keys = ef_mapping;
    uint8 *ptemp = data;

    *data_len = 33;

    ptemp[0] = keys->ksi;
    
    SCI_MEMCPY(ptemp + 1, keys->ck, 16);
    SCI_MEMCPY(ptemp + 17, keys->ik, 16);

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeKeysPS(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
    SIM_EF_MAPPING_KEYS_PS_T *keysps = ef_mapping;
    uint8 *ptemp = data;
    
    *data_len = 33;

    ptemp[0] = keysps->ksips;
    
    SCI_MEMCPY(ptemp + 1, keysps->ckps, 16);
    SCI_MEMCPY(ptemp + 17, keysps->ikps, 16);

    return SIM_SERVICE_OK;
}

#ifndef WIN32
#ifdef SIM_3G_USIM_SUPPORT
extern void RR_DecodeCellChanDesc(
    int32  msg_length,           //in -- message length
    uint8  *msg_value,           //in -- message content
    uint16 *msg_error,           //out -- message error cause value
    uint16 *offset_octets,       //in/out -- byte offset to msg_value
    ARFCN_LIST_T *ca_list        //out -- ARFCN_LIST_T
);
/**********************************************************************
//    Description:
//      the function decode the ba_list to the arfcn
//      in: ba_list          (such as (0x8f 0x1a, 0x84 0xea 0xc8 0x0, 0x0 0x8 0x0 0x0 0x0 0x0, 0x0 0x0 0x0 0x0))
//      out: arfcn_num,arfcn (such as (0x2,0x35,0x2,0x3a,0x2,0x3d,0x2,0x3e,0x2,0x3f,0x2,0x41,0x2,0x43,0x2,0x45,0x2,0x46,0x2,0x49,0x2,0x61))
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:
//    Note:
***********************************************************************/
void DecodeBaList(BA_LIST_ARR_T* ba_list, uint8* arfcn_num, uint8* arfcn)
{
    uint8 i;
    uint16 msg_error = RRC_MSG_NO_ERROR;
    uint16 offset_octets = 0;
    ARFCN_LIST_T ca_list;
    
    RR_DecodeCellChanDesc(16,(uint8*)ba_list,&msg_error,&offset_octets,&ca_list);

    if(msg_error == RRC_MSG_NO_ERROR)
    {
        SCI_TRACE_LOW("SIM: the decoded carrier freq num is %d,val= 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x.",
        ca_list.num,
        ca_list.arfcn_list_arr[0],ca_list.arfcn_list_arr[1],ca_list.arfcn_list_arr[2],ca_list.arfcn_list_arr[3],ca_list.arfcn_list_arr[4],
        ca_list.arfcn_list_arr[5],ca_list.arfcn_list_arr[6],ca_list.arfcn_list_arr[7],ca_list.arfcn_list_arr[8],ca_list.arfcn_list_arr[9],
        ca_list.arfcn_list_arr[10],ca_list.arfcn_list_arr[11],ca_list.arfcn_list_arr[12],ca_list.arfcn_list_arr[13],ca_list.arfcn_list_arr[14],
        ca_list.arfcn_list_arr[15]);

        *arfcn_num = ca_list.num;
        for(i = 0; i < ca_list.num; i++)
        {
            arfcn[2*i] = (uint8)(ca_list.arfcn_list_arr[i]>>8);
            arfcn[2*i + 1] = (uint8)(ca_list.arfcn_list_arr[i]);
        }
    }
    else
    {
        SCI_TRACE_LOW("SIM: DecodeBaList failed for %d.",msg_error);
        *arfcn_num = 0;
    }
}
#endif
#endif

/**********************************************************************
//    Description:
//      the function do the encode operation that encode the EF
//    file content to the coreponding file information structure.
//    Global resource dependence :
//    Author:
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeNetPar(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifndef WIN32
#ifdef SIM_3G_USIM_SUPPORT
    uint8 *ptemp = data;
    SIM_EF_MAPPING_BCCH_T *bcch = ef_mapping;
    uint8 pos = 0;
    uint8 i = 0;
    uint8 arfcn_num = 0;
    
    if(g_sim_netpar_file_size[cur_sim_task] == 0)
    {
        return SIM_SERVICE_ENCODE_ERROR;
    }

    *data_len = g_sim_netpar_file_size[cur_sim_task];

    SCI_MEMSET(ptemp, 0xff, *data_len);

    if(forge_empty)
    {
        return SIM_SERVICE_OK;
    }

    //encode GSM Cell Information
    ptemp[pos++] = 0xA0;
    ptemp[pos++] = 0x16;//this length will be modified later according to the arfcn num

    //current camped cell BCCH frequency
    ptemp[pos++] = 0x80;
    ptemp[pos++] = 0x02;

    //camponed arfcn    
    ptemp[pos++] = (uint8)(bcch->usim_netpar.camponed_arfcn >> 8);
    ptemp[pos++] = (uint8)(bcch->usim_netpar.camponed_arfcn);

    if(g_sim_netpar_file_size[cur_sim_task] < 10)
    {
        return SIM_SERVICE_OK;
    }

    //Neighbour cell BCCH frequency
    arfcn_num = bcch->usim_netpar.arfcn_num;
    if(arfcn_num!= 0)
    {
        if((arfcn_num*2 + 8) > g_sim_netpar_file_size[cur_sim_task])
        {
            arfcn_num = (g_sim_netpar_file_size[cur_sim_task] - 8)/2;
        }
        ptemp[pos++] = 0x81;
        ptemp[pos++] = arfcn_num*2;
        for(i=0;i<arfcn_num;i++)
        {
            ptemp[pos++] = (uint8)((bcch->usim_netpar.arfcn[i] & 0xff00) >> 8);
            ptemp[pos++] = (uint8)(bcch->usim_netpar.arfcn[i] & 0xff);
        }
    }

    //modify the length after tag A0
    ptemp[1] = 4 +(2 + arfcn_num*2);
        
    for(i = 0; i < 5 ;i++)
    {
        SCI_TRACE_LOW("SIM::encode EFnetpar data_len=%d, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", \
                      *data_len, *(ptemp + i*10), *(ptemp + 1 + i*10), *(ptemp + 2 + i*10), *(ptemp + 3 + i*10), \
                      *(ptemp + 4 + i*10), *(ptemp + 5 + i*10), *(ptemp + 6 + i*10), *(ptemp + 7 + i*10), *(ptemp + 8 + i*10),*(ptemp + 9 + i*10));                      
                          
    }
#endif
#endif

    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//    the function encode the data from upper layer to write to the EF ACL.
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E EncodeACL(    //return value:the result of the encode operation
    uint32 cur_sim_task,
    uint16 *data_len,   //out:the len of the data been encode out
    void *data,         //out:the data that been encode out
    void *ef_mapping,   //in:the structure used to encode data
    const BOOLEAN forge_empty   //encode a empty data for erasure
)
{
#ifndef WIN32
    SIM_EF_MAPPING_ACL_T *acl = ef_mapping;
    uint8 *ptemp = data;

    *data_len = 1 + acl->apn_tlvs_len;

    if(forge_empty)
    {
        SCI_MEMSET(ptemp, 0, *data_len);
    }
    else
    {
        *ptemp = acl->apn_num;
        ptemp++;
        
        SCI_MEMCPY(ptemp, acl->apn_tlvs, acl->apn_tlvs_len);
    }

#endif

    return SIM_SERVICE_OK;
}
#ifdef   __cplusplus
}
#endif
