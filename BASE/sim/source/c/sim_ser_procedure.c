/******************************************************************************
** File Name:      sim_ser_procedure.c                                       *
** Author:         Raislin.Kong                                              *
** Date:           2002.8                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the C file of common sim service procedure        *
**                 functions                                                 *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.8         Raislin.Kong     Create                                    *
******************************************************************************/

#include "base_trc.h"
#include "sim_include.h"


#ifdef   __cplusplus
extern   "C"
{
#endif
//STATIC variable
//the service buf used to read the file content into and then decode
//file content in the buf
//STATIC uint32 s_service_buf[SIM_CARD_NUM][SIM_MAX_R_APDU_DATA_LEN >> 1];
STATIC uint32 s_service_buf[SIM_CARD_NUM][193];//256*3=768 is enough now

#ifdef SIM_TEST_IN_WIN32
SIM_SERVICE_RESULT_E SIMINSTR_UpdateEF_static( //return value:the file update operation result
    SIM_FILE_NAME_E file_name,  //the ef file to be updated
    uint16  offset, //the offset to update in transparent file
    uint8       record_num, //the record num to update of recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint8       data_len,   //the update data len
    uint8  *data,    //in:the data to update file
    int card_num
);

SIM_SERVICE_RESULT_E SIMINSTR_ReadEF_static(   //return value:the result of the read operation
    SIM_FILE_NAME_E file_name,  //the file to be read
    uint16  offset, //the offset that start to read in transprent file
    uint8   record_num, //the record num to read in recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint16 *filesize_recordlen, //out:the file len of transparent file and reocrd len of recordy file
    uint16 *data_len,   //out:the data len that readed from the file
    uint8  *data,        //out:the data readef from the file
    int card_num
);
#endif//SIM_TEST_IN_WIN32

/**********************************************************************
//    Description:
//       the implementation of select a SIM file and get the select data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_SelectFile(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,    //the file to be selected
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response    //the select data of the file
)
{
    SIM_FILE_NAME_E remap_file_name = file_name;
#ifdef SIM_CDMA_CARD_SUPPORT
    SIM_RESPONSE_DATA_OF_SELECT_T cdma_select_response;
#endif
    SIM_SERVICE_RESULT_E result;

#ifdef SIM_3G_USIM_SUPPORT

    if(SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
#endif
    {
#ifdef SIM_CDMA_CARD_SUPPORT

        if(s_sim_cdma_flag[cur_sim_task])
        {
            if(SIMFS_IsInDFtelecom(remap_file_name))
            {
                if(SIM_FILE_DF_TELECOM == SIMFS_GetCurDirectoryFile(cur_sim_task))
                {
                }
                else
                {
                    SIMINSTR_SelectFile(cur_sim_task, SIM_FILE_DF_CDMA, &cdma_select_response);
                }
            }
        }

#endif

        result = SIMINSTR_SelectFile(cur_sim_task, remap_file_name, select_response);

        if(SIM_SERVICE_MEMORY_PROBLEM == result)
        {
            //A special sim card when SELECT LND will return 9240.
            //Then we read the last EF will fail (Donot SELECT).
            //So we return to MF.
            SIMINSTR_SelectFile(cur_sim_task, SIM_FILE_MF, select_response);
        }

        return result;
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif
}

/**********************************************************************
//    Description:
//      Increate the EF with the increase data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_IncreaseEF(     //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be increased
    SIM_INCREASE_DATA_T increase_data, //in:the increase data
    SIM_INCREASE_RESULT_T *increase_result  //out:the increase result
)
{
    SIM_INSTR_INCREASE_RESULT_T increase_instr_result;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_FILE_NAME_E remap_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T mf_select_response;
    uint8 i = 0;

    //change the uint32 to 3 byte data
    increase_instr_result.added_value.data[2] = (uint8)(increase_data.data & 0xff);
    increase_instr_result.added_value.data[1] = (uint8)((increase_data.data & 0xff00) >> 8);
    increase_instr_result.added_value.data[0] = (uint8)((increase_data.data & 0xff0000) >> 16);

#ifdef SIM_3G_USIM_SUPPORT

    if(!SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif

    for(i = 0; i < 2; i++)
    {
        ser_result = SIMINSTR_IncreaseEF(cur_sim_task, remap_file_name, increase_instr_result.added_value, &(increase_instr_result));

        if(SIM_SERVICE_OK == ser_result)
        {
            break;//OK. So quit the for-loop
        }
        //re-do this command when SW is SIM_STATUS_NO_EF_SELECETED only when the first time.
        else if((SIM_SERVICE_NO_EF_SELECTED == ser_result) && (0 == i))
        {
            if(SIM_SERVICE_OK == (SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &mf_select_response)))
            {
                //SCI_TRACE_LOW:"SIM: re-do increase"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_148_112_2_17_22_59_49_4602, (uint8 *)"");
            }
            else
            {
                return ser_result;
            }
        }
        else
        {
            return ser_result;
        }
    }

    //change the 3 byte to uint32
    increase_result->added_value.data = (((uint32)(increase_instr_result.added_value.data[0])) << 16) +
                                        (((uint32)(increase_instr_result.added_value.data[1])) << 8) +
                                        increase_instr_result.added_value.data[2];
    increase_result->new_value.data = (((uint32)(increase_instr_result.new_value.data[0])) << 16) +
                                      (((uint32)(increase_instr_result.new_value.data[1])) << 8) +
                                      increase_instr_result.new_value.data[2];

    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      the fuction to read the whole EF file and decode the file
//    cotent to the coresponding ef mapping structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_ReadWholeTransparentEF( //return value:the result of the read operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the Transparent file to be readed
    void *ef_mapping    //out:the file conten mapping sturcute
)
{
    SIM_SERVICE_RESULT_E    ser_result;
    uint16 file_size, data_len;
    SIM_FILE_NAME_E remap_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T mf_select_response;
    uint8 i = 0;

    //procedure for ef_mapping==NULL
    if(NULL == ef_mapping)
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_ReadWholeTransparentEF,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_195_112_2_17_22_59_49_4603, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(!SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(remap_file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(remap_file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //the ef file structure must be transparent file
    if(SIMFS_GetEFStructure(remap_file_name) != SIM_EF_S_TRANSPARENT)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    for(i = 0; i < 2; i++)
    {
        //read the whole file content
        ser_result = SIMINSTR_ReadEF(cur_sim_task, remap_file_name, 0, 0,
                                     SIM_RECORD_ABSOLUTE, &file_size, &data_len, (uint8 *)s_service_buf[cur_sim_task]);

        if(SIM_SERVICE_OK == ser_result)
        {
            break;//OK, so quit the loop.
        }
        //re-do this command when SW is SIM_STATUS_NO_EF_SELECETED only when the first time.
        else if((SIM_SERVICE_NO_EF_SELECTED == ser_result) && (0 == i))
        {
            if(SIM_SERVICE_OK == (SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &mf_select_response)))
            {
                //SCI_TRACE_LOW:"SIM: re-do read binary"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_239_112_2_17_22_59_49_4604, (uint8 *)"");
            }
            else
            {
                return ser_result;
            }
        }
        else
        {
            return ser_result;
        }
    }

    //decodeing the ef file content
    return SIMFS_DecodeEFFile(cur_sim_task, remap_file_name, data_len, (uint8 *)s_service_buf[cur_sim_task], ef_mapping);
}


/**********************************************************************
//    Description:
//      read a record from a reacord ef file,and decode the record
//    content to the record mapping structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_ReadEFRecord(   //return vlaue:the result of the read operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be readed
    uint8 record_num,   //the record num to read
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping    //out:the file cotent read out
)
{
    SIM_SERVICE_RESULT_E    ser_result;
    uint16 file_size, data_len;
    SIM_EF_STRUCTURE_E ef_structure;
    SIM_FILE_NAME_E remap_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T mf_select_response;
    uint8 i = 0;

    //procedure for ef_mapping==NULL
    if(NULL == ef_mapping)
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_ReadEFRecord,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_282_112_2_17_22_59_49_4605, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(!SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(remap_file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(remap_file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    ef_structure = SIMFS_GetEFStructure(remap_file_name);

    //the ef file structure must be transparent file
    if((SIM_EF_S_LINEAR_FIXED != ef_structure) &&
        (SIM_EF_S_CYCLIC != ef_structure))
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    for(i = 0; i < 2; i++)
    {
        //record the record content
        ser_result = SIMINSTR_ReadEF(cur_sim_task, remap_file_name, 0, record_num,
                                     record_mode, &file_size, &data_len, (uint8 *)s_service_buf[cur_sim_task]);

        if(SIM_SERVICE_OK == ser_result)
        {
            break;//OK, so quit the loop.
        }
        //re-do this command when SW is SIM_STATUS_NO_EF_SELECETED only when the first time.
        else if((SIM_SERVICE_NO_EF_SELECTED == ser_result) && (0 == i))
        {
            if(SIM_SERVICE_OK == (SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &mf_select_response)))
            {
                //SCI_TRACE_LOW:"SIM: re-do ef record"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_328_112_2_17_22_59_50_4606, (uint8 *)"");
            }
            else
            {
                return ser_result;
            }
        }
        else
        {
            return ser_result;
        }
    }

    //decode the recode content to the record mapping structure
    return SIMFS_DecodeEFFile(cur_sim_task, remap_file_name, data_len, (uint8 *)s_service_buf[cur_sim_task], ef_mapping);
}

/**********************************************************************
//    Description:
//      Seek  in a record EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_SeekInFile(    //return value:the result of the seek operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be seek in
    SIM_SEEK_TYPE_E seek_type,  //the seek opration tupe
    SIM_SEEK_MODE_E seek_mode,  //the seek mode
    uint8   pattern_len,    //the pattern len
    uint8 *pattern,     //in:the patter used in seek
    uint8 *record_num,   //the record num that seek out
    uint8 *record  //out:the record that the pattern in
)
{
    uint16 data_len;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_STRUCTURE_E ef_structure;
    SIM_FILE_NAME_E remap_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T mf_select_response;
    uint8 i = 0;

    //procedure for ef_mapping==NULL
    if((NULL == record_num) || (NULL == record))
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_SeekInFile,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_372_112_2_17_22_59_50_4607, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //the sim card is before phase 2,can not support this functin
    if(g_sim_card_setting[cur_sim_task].card_phase < SIM_PH_2)
    {
        return SIM_SERVICE_NOT_ENABLE;
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(!SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(remap_file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(remap_file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    ef_structure = SIMFS_GetEFStructure(remap_file_name);

    //the ef file structure must be transparent file
    if((SIM_EF_S_LINEAR_FIXED != ef_structure) &&
        (SIM_EF_S_CYCLIC != ef_structure))
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //encode the free record mapping structure to the ef content
    ser_result = SIMFS_EncodeEFFile(cur_sim_task, remap_file_name, &data_len, (uint8 *)s_service_buf[cur_sim_task], NULL, TRUE);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    for(i = 0; i < 2; i++)
    {
        ser_result = SIMINSTR_SeekInFile(cur_sim_task, remap_file_name, seek_type, seek_mode, (uint8)data_len,
                                         (uint8 *)s_service_buf[cur_sim_task], record_num, record);

        if(SIM_SERVICE_OK == ser_result)
        {
            break;//OK, so quit the loop.
        }
        //re-do this command when SW is SIM_STATUS_NO_EF_SELECETED only when the first time.
        else if((SIM_SERVICE_NO_EF_SELECTED == ser_result) && (0 == i))
        {
            if(SIM_SERVICE_OK == (SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &mf_select_response)))
            {
                //SCI_TRACE_LOW:"SIM: re-do seek ef record"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_431_112_2_17_22_59_50_4608, (uint8 *)"");
            }
            else
            {
                return ser_result;
            }
        }
        else
        {
            return ser_result;
        }
    }

    return  ser_result;
}

/**********************************************************************
//    Description:
//      the fuction to encode a Transparent EF mapping structure to the
//    file content and then update the EF file using the file content
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_UpdateWholeTransparentEF(   //return value:the result of the update operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be updated
    void *ef_mapping    //in:the file content mapping used to update
)
{
    SIM_SERVICE_RESULT_E ser_result;
    uint16 data_len;
    SIM_FILE_NAME_E remap_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T mf_select_response;
    uint8 i = 0;

    //procedure for ef_mapping==NULL
    if(NULL == ef_mapping)
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_UpdateWholeTransparentEF,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_469_112_2_17_22_59_50_4609, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(!SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(remap_file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(remap_file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //the ef file structure must be transparent file
    if(SIMFS_GetEFStructure(remap_file_name) != SIM_EF_S_TRANSPARENT)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //encode the ef mapping structure to the ef content
    ser_result = SIMFS_EncodeEFFile(cur_sim_task, remap_file_name, &data_len, (uint8 *)s_service_buf[cur_sim_task], ef_mapping, FALSE);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    for(i = 0; i < 2; i++)
    {
        //update the EF with the ef file content
        ser_result = SIMINSTR_UpdateEF(cur_sim_task, remap_file_name, 0, 0, SIM_RECORD_ABSOLUTE,
                                       data_len, (uint8 *)s_service_buf[cur_sim_task]);

        if(SIM_SERVICE_OK == ser_result)
        {
            break;//OK, so quit the loop.
        }
        //re-do this command when SW is SIM_STATUS_NO_EF_SELECETED only when the first time.
        else if((SIM_SERVICE_NO_EF_SELECTED == ser_result) && (0 == i))
        {
            if(SIM_SERVICE_OK == (SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &mf_select_response)))
            {
                //SCI_TRACE_LOW:"SIM: re-do update t ef record"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_521_112_2_17_22_59_50_4610, (uint8 *)"");
            }
            else
            {
                return ser_result;
            }
        }
        else
        {
            return ser_result;
        }
    }

    return ser_result;
}


/**********************************************************************
//    Description:
//      the function encode a record mapping structre of Fixed Linear
//    or Cylic file to the file record content,and then update the
//    coresponding record using the record content
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_UpdateEFRecord( //return value:the result of the update operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be updated
    uint8 record_num,   //the reocrd num to update
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,   //in:the file cotent mapping used to update
    BOOLEAN erasure_record  //erasure the record to empty or not
)
{
    SIM_SERVICE_RESULT_E ser_result;
    uint16 data_len;
    SIM_EF_STRUCTURE_E ef_structure;
    SIM_FILE_NAME_E remap_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T mf_select_response;
    uint8 i = 0;

    //procedure for ef_mapping==NULL
    if((!erasure_record) && (NULL == ef_mapping))
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_UpdateEFRecord,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_565_112_2_17_22_59_50_4611, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(!SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(remap_file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(remap_file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    ef_structure = SIMFS_GetEFStructure(remap_file_name);

    //the ef file structure must be transparent file
    if((SIM_EF_S_LINEAR_FIXED != ef_structure) &&
        (SIM_EF_S_CYCLIC != ef_structure))
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //encode the ef mapping structure to the ef content
    ser_result = SIMFS_EncodeEFFile(cur_sim_task, remap_file_name, &data_len, (uint8 *)s_service_buf[cur_sim_task], ef_mapping, erasure_record);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    for(i = 0; i < 2; i++)
    {
        //update with the record cotent
        ser_result = SIMINSTR_UpdateEF(cur_sim_task, remap_file_name, 0, record_num,
                                       record_mode, data_len, (uint8 *)s_service_buf[cur_sim_task]);

        if(SIM_SERVICE_OK == ser_result)
        {
            break;//OK, so quit the loop.
        }
        //re-do this command when SW is SIM_STATUS_NO_EF_SELECETED only when the first time.
        else if((SIM_SERVICE_NO_EF_SELECTED == ser_result) && (0 == i))
        {
            if(SIM_SERVICE_OK == (SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &mf_select_response)))
            {

            }
            else
            {
                return ser_result;
            }
        }
        else
        {
            return ser_result;
        }
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//      Invalidate an EF file from the SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_InvalidateEF(uint32 cur_sim_task, SIM_FILE_NAME_E file_name)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_FILE_NAME_E remap_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T mf_select_response;
    uint8 i = 0;

#ifdef SIM_3G_USIM_SUPPORT

    if(SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
#endif
    {
        for(i = 0; i < 2; i++)
        {
            ser_result = SIMINSTR_InvalidateEF(cur_sim_task, remap_file_name);

            if(SIM_SERVICE_OK == ser_result)
            {
                break;//OK, so quit the loop.
            }
            //re-do this command when SW is SIM_STATUS_NO_EF_SELECETED only when the first time.
            else if((SIM_SERVICE_NO_EF_SELECTED == ser_result) && (0 == i))
            {
                if(SIM_SERVICE_OK == (SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &mf_select_response)))
                {

                }
                else
                {
                    return ser_result;
                }
            }
            else
            {
                return ser_result;
            }
        }
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif
    return ser_result;
}

/**********************************************************************
//    Description:
//      Rehabilitate a EF file from the SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_RehabilitateEF(uint32 cur_sim_task, SIM_FILE_NAME_E file_name)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_FILE_NAME_E remap_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T mf_select_response;
    uint8 i = 0;

#ifdef SIM_3G_USIM_SUPPORT

    if(SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
#endif
    {
        for(i = 0; i < 2; i++)
        {
            ser_result = SIMINSTR_RehabilitateEF(cur_sim_task, remap_file_name);

            if(SIM_SERVICE_OK == ser_result)
            {
                break;//OK, so quit the loop.
            }
            //re-do this command when SW is SIM_STATUS_NO_EF_SELECETED only when the first time.
            else if((SIM_SERVICE_NO_EF_SELECTED == ser_result) && (0 == i))
            {
                if(SIM_SERVICE_OK == (SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &mf_select_response)))
                {

                }
                else
                {
                    return ser_result;
                }
            }
            else
            {
                return ser_result;
            }
        }
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif
    return ser_result;
}

/**********************************************************************
//    Description:
//      Get some record file that have not differenct length alpha id
//    to different SIM card.including EFadn,EFfdn,EFmsisdn,EFlnd,EFsmsp,
//    EFsdn and EFbdn.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_GetRecordInfo(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name, //the file name
    SIM_RECORD_EF_INFO_T *record_info  //out:the record info
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_FILE_NAME_E remap_file_name = file_name;

    //procedure for record_info==NULL
    if(NULL == record_info)
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_GetRecordInfo,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_760_112_2_17_22_59_50_4612, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(!SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &remap_file_name))
    {
        return SIM_SERVICE_SELECT_NO_SUCH_FILE;
    }

#endif

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(remap_file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(remap_file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //the ef file structure must be transparent file
    if(SIMFS_GetEFStructure(remap_file_name) == SIM_EF_S_TRANSPARENT)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    ser_result = SIMSER_SelectFile(cur_sim_task, remap_file_name, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    //When we call "SIMSER_GetRecordInfo", may we will use an un-init record_info
    // or an all-zero record_info.
    if(0 == record_info->max_record_num)
    {
        switch(remap_file_name)
        {
            case SIM_FILE_EF_BDN:
            case SIM_FILE_EF_USIM_BDN:
                record_info->max_record_num = SIM_MAX_BDN_NUM;
                break;

            case SIM_FILE_EF_FDN:
            case SIM_FILE_EF_USIM_FDN:
                record_info->max_record_num = SIM_MAX_FDN_NUM;
                break;

            case SIM_FILE_EF_LND:
                record_info->max_record_num = SIM_MAX_LND_NUM;
                break;

            case SIM_FILE_EF_MSISDN:
            case SIM_FILE_EF_USIM_MSISDN:
                record_info->max_record_num = SIM_MAX_MSISDN_NUM;
                break;

            case SIM_FILE_EF_SDN:
            case SIM_FILE_EF_USIM_SDN:
                record_info->max_record_num = SIM_MAX_SDN_NUM;
                break;

            case SIM_FILE_EF_SMSP:
            case SIM_FILE_EF_USIM_SMSP:
                record_info->max_record_num = SIM_MAX_SMSP_NUM;
                break;

            case SIM_FILE_EF_PNN:
            case SIM_FILE_EF_USIM_PNN:
                record_info->max_record_num = SIM_MAX_PNN_NUM;
                break;

            case SIM_FILE_EF_OPL:
            case SIM_FILE_EF_USIM_OPL:
                record_info->max_record_num = SIM_MAX_OPL_NUM;
                break;

            case SIM_FILE_EF_USIM_PNNI:
                record_info->max_record_num = SIM_MAX_PNN_ICON_NUM;
                break;

            default:
                record_info->max_record_num = 255;//One Line-fixed EF has 255 records at most
                break;
        }
    }

    record_info->original_alpha_id_len = select_data.dfnumber_recordlen - record_info->other_content_len;
    record_info->alpha_id_len = select_data.dfnumber_recordlen - record_info->other_content_len;

    if(0 == select_data.dfnumber_recordlen)
    {
        record_info->record_num = 0;//A damaged SIM's ADN file info is very strange.
    }
    else
    {
        record_info->record_num = (uint8)(select_data.freespace_filesize / select_data.dfnumber_recordlen);
    }

    //get the mini value of alpha id len of SIM card and max alpha id len of MS as
    //the really alpha id len
    if((0 != record_info->max_alpha_id_len) && (record_info->alpha_id_len > record_info->max_alpha_id_len))
    {
        record_info->alpha_id_len = record_info->max_alpha_id_len;
    }

    //get the mini value of record num of SIM card and max record num of MS as
    //the really record num
    if((0 != record_info->max_record_num) && (record_info->record_num > record_info->max_record_num))
    {
        record_info->record_num = record_info->max_record_num;
    }

    return SIM_SERVICE_OK;
}

#ifdef SIM_TEST_IN_WIN32
/**********the following is for static button sim card setting---start**********/
SIM_SERVICE_RESULT_E SIMSER_ReadWholeTransparentEF_static( //return value:the result of the read operation
    SIM_FILE_NAME_E file_name,  //the Transparent file to be readed
    void *ef_mapping,    //out:the file conten mapping sturcute
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    uint16 file_size = 0, data_len = 0;

    //procedure for ef_mapping==NULL
    if(NULL == ef_mapping)
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_ReadWholeTransparentEF_static,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_891_112_2_17_22_59_51_4613, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //the ef file structure must be transparent file
    if(SIMFS_GetEFStructure(file_name) != SIM_EF_S_TRANSPARENT)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //read the whole file content
    ser_result = SIMINSTR_ReadEF_static(file_name, 0, 0,
                                        SIM_RECORD_ABSOLUTE, &file_size, &data_len, (uint8 *)s_service_buf[card_num], card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    //decodeing the ef file content
    return SIMFS_DecodeEFFile(card_num, file_name, data_len, (uint8 *)s_service_buf[card_num], ef_mapping);
}

SIM_SERVICE_RESULT_E SIMSER_ReadEFRecord_static(   //return vlaue:the result of the read operation
    SIM_FILE_NAME_E file_name,  //the file to be readed
    uint8 record_num,   //the record num to read
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,    //out:the file cotent read out
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    uint16 file_size = 0, data_len = 0;
    SIM_EF_STRUCTURE_E ef_structure;

    //procedure for ef_mapping==NULL
    if(NULL == ef_mapping)
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_ReadEFRecord_static,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_941_112_2_17_22_59_51_4614, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    ef_structure = SIMFS_GetEFStructure(file_name);

    //the ef file structure must be transparent file
    if((SIM_EF_S_LINEAR_FIXED != ef_structure) &&
        (SIM_EF_S_CYCLIC != ef_structure))
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //record the record content
    ser_result = SIMINSTR_ReadEF_static(file_name, 0, record_num,
                                        record_mode, &file_size, &data_len, (uint8 *)s_service_buf[card_num], card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    //decode the recode content to the record mapping structure
    return SIMFS_DecodeEFFile(card_num, file_name, data_len, (uint8 *)s_service_buf[card_num], ef_mapping);
}

SIM_SERVICE_RESULT_E SIMSER_UpdateWholeTransparentEF_static(   //return value:the result of the update operation
    SIM_FILE_NAME_E file_name,  //the file to be updated
    void *ef_mapping,    //in:the file content mapping used to update
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    uint16 data_len = 0;

    //procedure for ef_mapping==NULL
    if(NULL == ef_mapping)
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_UpdateWholeTransparentEF_static,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_990_112_2_17_22_59_51_4615, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //the ef file structure must be transparent file
    if(SIMFS_GetEFStructure(file_name) != SIM_EF_S_TRANSPARENT)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //encode the ef mapping structure to the ef content
    ser_result = SIMFS_EncodeEFFile(card_num, file_name, &data_len, (uint8 *)s_service_buf[card_num], ef_mapping, FALSE);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    //update the EF with the ef file content
    return SIMINSTR_UpdateEF_static(file_name, 0, 0, SIM_RECORD_ABSOLUTE,
                                    (uint8)data_len, (uint8 *)s_service_buf[card_num], card_num);
}

SIM_SERVICE_RESULT_E SIMSER_UpdateEFRecord_static( //return value:the result of the update operation
    SIM_FILE_NAME_E file_name,  //the file to be updated
    uint8 record_num,   //the reocrd num to update
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,   //in:the file cotent mapping used to update
    BOOLEAN erasure_record,  //erasure the record to empty or not
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    uint16 data_len = 0;
    SIM_EF_STRUCTURE_E ef_structure;

    //procedure for ef_mapping==NULL
    if((!erasure_record) && (NULL == ef_mapping))
    {
        //SCI_TRACE_LOW:"SIM::SIMSER_UpdateEFRecord_static,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SER_PROCEDURE_1039_112_2_17_22_59_51_4616, (uint8 *)"");
        return SIM_SERVICE_ERROR_UNKNOW;
    }


    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    //the file must be a ef file
    if(SIMFS_GetFileType(file_name) != SIM_FILE_T_EF)
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    ef_structure = SIMFS_GetEFStructure(file_name);

    //the ef file structure must be transparent file
    if((SIM_EF_S_LINEAR_FIXED != ef_structure) &&
        (SIM_EF_S_CYCLIC != ef_structure))
    {
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    //encode the ef mapping structure to the ef content
    ser_result = SIMFS_EncodeEFFile(card_num, file_name, &data_len, (uint8 *)s_service_buf[card_num], ef_mapping, erasure_record);

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    //update with the record cotent
    return SIMINSTR_UpdateEF_static(file_name, 0, record_num,
                                    record_mode, (uint8)data_len, (uint8 *)s_service_buf[card_num], card_num);

}
/**********the above is for static button sim card setting---end**********/
#endif//SIM_TEST_IN_WIN32

#ifdef   __cplusplus
}
#endif
