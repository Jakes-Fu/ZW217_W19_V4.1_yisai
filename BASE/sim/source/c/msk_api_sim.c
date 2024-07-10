/******************************************************************************
** File Name:      mks_api_sim.c                                             *
** Author:         Raislin.Kong                                              *
** Date:           2003.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    c files define the api functions that sim module will     *
**                 export in the window platform.                            *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2003.2         Raislin.Kong      create                                   *
******************************************************************************/
#include "sci_types.h"
#include "msk_api_sim.h"

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef SIM_TEST_IN_WIN32
extern BOOLEAN SIMINT_QuerySIM(int card_num);
extern void SIMINT_RejectSIM(int card_num);
extern void SIMINT_InsertSIM(int card_num);
extern SIM_SERVICE_RESULT_E SIMINSTR_EncodeEFInfo( //retyrn value:the result of the encode operation
    SIM_EF_FILE_INFO_T ef_info,   //the ef info to be encode in to select data
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data   //out:the pointer of the buf that save the encoded select data
);
extern SIM_SERVICE_RESULT_E SIMINSTR_EncodeDFInfo( //return value:the result of the Encode operation
    SIM_DF_MF_INFO_T df_info, //the DF info to be encoded in to select data
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data    //out:save the select data that encoded into
);
extern BOOLEAN SIMINSTR_DecodeSelectData(  //return value:the type of info that decode out
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,   //the select data to be decoded
    SIM_EF_FILE_INFO_T *ef_info, //out:buf to save the ef data info
    SIM_DF_MF_INFO_T *df_info    //out bug to save the df data info
);
extern SIM_FILE_NAME_E SIMFS_GetParenFileName( //return value:parent file name
    SIM_FILE_NAME_E file_name   //the file name
);
extern void SIMMAIN_ResetSIMVariable(void);
extern SIM_SERVICE_RESULT_E SIMSER_ReadWholeTransparentEF_static( //return value:the result of the read operation
    SIM_FILE_NAME_E file_name,  //the Transparent file to be readed
    void *ef_mapping,   //out:the file conten mapping sturcute
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMSER_ReadEFRecord_static(   //return vlaue:the result of the read operation
    SIM_FILE_NAME_E file_name,  //the file to be readed
    uint8 record_num,   //the record num to read
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,    //out:the file cotent read out
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMSER_UpdateWholeTransparentEF_static(   //return value:the result of the update operation
    SIM_FILE_NAME_E file_name,  //the file to be updated
    void *ef_mapping,    //in:the file content mapping used to update
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMSER_UpdateEFRecord_static( //return value:the result of the update operation
    SIM_FILE_NAME_E file_name,  //the file to be updated
    uint8 record_num,   //the reocrd num to update
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,   //in:the file cotent mapping used to update
    BOOLEAN erasure_record,  //erasure the record to empty or not
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMTIW_GetSelectFileName_static(  //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path,    //out:the pointer of the file name
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMTIW_UpdateAllCHV_static(   //return vlaue:the result of the operation
    SIM_ALL_CHV_T *chvs, //in:all of the sim chvs
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMTIW_GetAllCHV_static(  //return value:the result of the operation
    SIM_ALL_CHV_T *chvs, //out:all of the sim chv
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMTIW_SelectFile_static( //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,  //out:the select data of the sim file
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMTIW_UpdateSelectData_static(   //the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RESPONSE_DATA_OF_SELECT_T select_data,   //the select data of the sim file
    int card_num
);
extern void SIMTIW_GetCHVVerificated_static(
    BOOLEAN *chv1_v,    //out:the chv1 verificated state
    BOOLEAN *chv2_v,    //out:the chv1 verificated state
    int card_num
);
extern void SIMTIW_SetCHVVerificated_static(
    BOOLEAN chv1_v, //the chv1 verificated state
    BOOLEAN chv2_v,     //the chv1 verificated state
    int card_num
);
extern SIM_SERVICE_RESULT_E SIMTIW_CreatEFFile_static(    //return vlaue:the result of the operation
    SIM_FILE_NAME_E file_name,   //the sim file name
    int card_num
);



/********************************************************************
//    DESCRIPTION
//      Query whether the SIM card is in the Mobile
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN MSK_SIM_QuerySIM(int card_num)
{
    return SIMINT_QuerySIM(card_num);
}


/********************************************************************
//    DESCRIPTION
//      Reject the SIM card from the mobile
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void MSK_SIM_RejectSIM(int card_num)
{
    SIMINT_RejectSIM(card_num);
}

/********************************************************************
//    DESCRIPTION
//      Insert the SIM card into the mobile
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void MSK_SIM_InsertSIM(int card_num)
{
    SIMINT_InsertSIM(card_num);
}

/********************************************************************
//    DESCRIPTION
//      Encode a EF file information into select data format
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_EncodeEFInfo( //retyrn value:the result of the encode operation
    SIM_EF_FILE_INFO_T ef_info, //the ef info to be encode in to select data
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data   //out:the pointer of the buf that save the encoded select data
)
{
    return SIMINSTR_EncodeEFInfo(ef_info, select_data);
}

/********************************************************************
//    DESCRIPTION
//      Encode a DF file information into select data format
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_EncodeDFInfo( //return value:the result of the Encode operation
    SIM_DF_MF_INFO_T df_info,   //the DF info to be encoded in to select data
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data    //out:save the select data that encoded into
)
{
    return SIMINSTR_EncodeDFInfo(df_info, select_data);
}

/********************************************************************
//    DESCRIPTION
//      Decode a select data fromation file information
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN MSK_SIM_DecodeSelectData(  //return value:the type of info,TRUE:EF info,FALSE:DF info
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,   //the select data to be decoded
    SIM_EF_FILE_INFO_T *ef_info, //out:buf to save the ef data info
    SIM_DF_MF_INFO_T *df_info    //out bug to save the df data info
)
{
    return SIMINSTR_DecodeSelectData(select_data, ef_info, df_info);
}

/********************************************************************
//    DESCRIPTION
//      Get a file's parent file name
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_FILE_NAME_E MSK_SIM_GetParenFileName( //return value:parent file name
    SIM_FILE_NAME_E file_name  //the file name
)
{
    return SIMFS_GetParenFileName(file_name);
}

/********************************************************************
//    DESCRIPTION
//      Reset the SIM layer and SIM driver
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void MSK_SIM_ResetSIMVariable(int card_num)
{
    SIMMAIN_ResetSIMVariable(card_num);
}

/********************************************************************
//    DESCRIPTION
//      Read a transparent EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_ReadWholeTransparentEF( //return value:the result of the read operation
    SIM_FILE_NAME_E file_name,  //the Transparent file to be readed
    void *ef_mapping,    //out:the file conten mapping sturcute
    int card_num
)
{
    return SIMSER_ReadWholeTransparentEF_static(file_name, ef_mapping, card_num);
}

/********************************************************************
//    DESCRIPTION
//      Read a reocrd of a recordy EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_ReadEFRecord(   //return vlaue:the result of the read operation
    SIM_FILE_NAME_E file_name,  //the file to be readed
    uint8 record_num,   //the record num to read
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,    //out:the file cotent read out
    int card_num
)
{
    return SIMSER_ReadEFRecord_static(file_name, record_num, record_mode, ef_mapping, card_num);
}

/********************************************************************
//    DESCRIPTION
//      Update a transparent EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_UpdateWholeTransparentEF(   //return value:the result of the update operation
    SIM_FILE_NAME_E file_name,  //the file to be updated
    void *ef_mapping,    //in:the file content mapping used to update
    int card_num
)
{
    return SIMSER_UpdateWholeTransparentEF_static(file_name, ef_mapping, card_num);
}

/********************************************************************
//    DESCRIPTION
//      Update a record of a recordy EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_UpdateEFRecord( //return value:the result of the update operation
    SIM_FILE_NAME_E file_name,  //the file to be updated
    uint8 record_num,   //the reocrd num to update
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,   //in:the file cotent mapping used to update
    BOOLEAN erasure_record,  //erasure the record to empty or not
    int card_num
)
{
    return SIMSER_UpdateEFRecord_static(file_name, record_num,
                                        record_mode, ef_mapping, erasure_record, card_num); //wuding temply...
}

/********************************************************************
//    DESCRIPTION
//      Get the file name in the window simulator platform
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_GetSelectFileName(  //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path,    //out:the pointer of the file name
    int card_num
)
{
    return SIMTIW_GetSelectFileName_static(file_name, file_path, card_num);
}

/********************************************************************
//    DESCRIPTION
//      update the CHV file in the window simualtor platform
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_UpdateAllCHV(   //return vlaue:the result of the operation
    SIM_ALL_CHV_T *chvs, //in:all of the sim chvs
    int card_num
)
{
    return SIMTIW_UpdateAllCHV_static(chvs, card_num);
}

/********************************************************************
//    DESCRIPTION
//      Read the CHV file in the window simulator platform
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_GetAllCHV(  //return value:the result of the operation
    SIM_ALL_CHV_T *chvs, //out:all of the sim chv
    int card_num
)
{
    return SIMTIW_GetAllCHV_static(chvs, card_num);
}

/********************************************************************
//    DESCRIPTION
//      Select a EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_SelectFile( //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,  //out:the select data of the sim file
    int card_num
)
{
    return SIMTIW_SelectFile_static(file_name, select_data, card_num);
}

/********************************************************************
//    DESCRIPTION
//      update a file's select data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_UpdateSelectData(   //the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RESPONSE_DATA_OF_SELECT_T select_data,   //the select data of the sim file
    int card_num
)
{
    return SIMTIW_UpdateSelectData_static(file_name, select_data, card_num);
}

/********************************************************************
//    DESCRIPTION
//      get the chv verification information
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void MSK_SIM_GetCHVVerificated(
    BOOLEAN *chv1_v,    //out:the chv1 verificated state
    BOOLEAN *chv2_v,    //out:the chv1 verificated state
    int card_num
)
{
    SIMTIW_GetCHVVerificated_static(chv1_v, chv2_v, card_num);
}

/********************************************************************
//    DESCRIPTION
//      Set the chv verification information
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void MSK_SIM_SetCHVVerificated(
    BOOLEAN chv1_v, //the chv1 verificated state
    BOOLEAN chv2_v,     //the chv1 verificated state
    int card_num
)
{
    SIMTIW_SetCHVVerificated_static(chv1_v, chv2_v, card_num);
}

/********************************************************************
//    DESCRIPTION
//      create the ef file in the windows simualtor
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E MSK_SIM_CreatEFFile(    //return vlaue:the result of the operation
    SIM_FILE_NAME_E file_name,   //the sim file name
    int card_num
)
{
    return SIMTIW_CreatEFFile_static(file_name, card_num);
}
#endif  //SIM_TEST_IN_WIN32
#ifdef __cplusplus
}
#endif
