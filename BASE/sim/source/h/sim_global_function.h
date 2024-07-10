/******************************************************************************
** File Name:      sim_global_function.h                                     *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the header file that define the the global        *
**                 function in SIM task                                      *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/

#ifndef _SIM_GLOBAL_FUNCTION_H_
#define _SIM_GLOBAL_FUNCTION_H_

#ifdef   __cplusplus
extern   "C"
{
#endif

//global functions defined in sim_instruction.c file
/**********************************************************************
//    Description:
//      initialize the temporary signal queue
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMINSTR_InitTempSignalQueues(uint32 cur_sim_task);

#ifdef SIM_TEST_IN_WIN32
/**********************************************************************
//    Description:
//      Endcode a Ef info to a select data of the EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_EncodeEFInfo( //retyrn value:the result of the encode operation
    SIM_EF_FILE_INFO_T ef_info,   //the ef info to be encode in to select data
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data   //out:the pointer of the buf that save the encoded select data
);

/**********************************************************************
//    Description:
//      Endcode a DF info to a select data of the DF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_EncodeDFInfo( //return value:the result of the Encode operation
    SIM_DF_MF_INFO_T df_info, //the DF info to be encoded in to select data
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data    //out:save the select data that encoded into
);

/**********************************************************************
//    Description:
//      get the information from the file select data if the select data
//    is ef file info,the function return TRUE,else the function return
//    FALSE,and get the df or mf info
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMINSTR_DecodeSelectData(  //return value:the type of info that decode out
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,   //the select data to be decoded
    SIM_EF_FILE_INFO_T *ef_info, //out:buf to save the ef data info
    SIM_DF_MF_INFO_T *df_info    //out bug to save the df data info
);
#endif//SIM_TEST_IN_WIN32

#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description : get the response information
//  Global resource dependence :
//  Author: Tracy.zhang
//  Note: used for CRSM
/*****************************************************************************/
void SIMINSTR_CRSM_GetResponseInfo(
    uint32 cur_sim_task,
    SIM_RSP_INFO_T    *info_ptr
);
#endif
/**********************************************************************
//    Description:
//      the envolop of instruction status.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_Status(   //return value:the result of the status opreation
    uint32 cur_sim_task,
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response, //out:the status of the file
    uint8 data_len,  //the data len that want to get
    BOOLEAN *fetch_flag //proactive instruction FETCH
);

/**********************************************************************
//    Description:
//      Verify a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_VerifyCHV( //return vlaue:the result of verify operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E chv_indication,    //the CHV indication to verify
    SIM_CHV_T chv   //the chv use to verify
);

/**********************************************************************
//    Description:
//      Change a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_ChangeCHV( //return value:the reuslt of change CHV operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E    chv_indication, //the chv indication
    SIM_CHV_T   old_chv,    //the old chv
    SIM_CHV_T   new_chv     //the new chv
);

/**********************************************************************
//    Description:
//      Disable a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_DisableCHV(    //return value:the result of disavle CHV
    uint32 cur_sim_task,
    SIM_CHV_T   chv //the chv1 used to disable CHV1
);

/**********************************************************************
//    Description:
//      Enable a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_EnableCHV( //return value:the result of enable CHV operation
    uint32 cur_sim_task,
    SIM_CHV_T chv   //the CHV1 used to enable CHV1
);

/**********************************************************************
//    Description:
//      Unblock a CHV
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_UnblockCHV(    //return value:the result of unblock CHV operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E    chv_indication, //the chv indication
    SIM_CHV_T unblock_chv,  //the unblock chv
    SIM_CHV_T new_chv      //the new chv
);

/**********************************************************************
//    Description:
//      Run the A3 or A8 algorithm of the sim, 3G network
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_RunUMTSAlgorithm(   //return vlaue:the result of the 3G algorithm operation
    uint32 cur_sim_task,
    BOOLEAN is_gsm_network,
    uint8 *alg_rand,    //the rand used in algorithm  IN
    uint8 *autn,          //IN
    uint8 *rsp_data_len, // OUT  response data length of algorithm
    uint8 *alg_result_data //OUT response data of algorithm
);

/**********************************************************************
//    Description:
//      Run the A3 or A8 algorithm of the sim
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E    SIMINSTR_RunGSMAlgorithm(   //return vlaue:the result of the GSM algorithm operation
    uint32 cur_sim_task,
    RAND_T alg_rand,    //the rand used in algorithm
    SIM_GSM_ALGORITHM_RESULT_T *result  //out:the result of the algorithm
);

/**********************************************************************
//    Description:
//      Get the GSM specific data from the SIM card(Just get the DF_GSM
//    file select data,and decode it)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_GetGSMSpecificData(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_GSM_SPECIFIC_DATA_T *gsm_data   //out:the gsm specific data
);

/**********************************************************************
//    Description:
//      Get the coresponding access condition type of access condition of
//    a sim file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_ACCESS_CONDITION_E SIMAT_GetAccessCondition(
    SIM_ACCESS_CONDITION_TYPE_E acc_type,   //the access type
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data   //the response data of the sim file
);

/**********************************************************************
//    Description:
//      check the temp Int signal queue to see if there any abnormal Int from sim drv
//    signal from other layer and get it.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
xSignalHeaderRec *SIMINSTR_CheckTempIntSignalQueue(uint32 cur_sim_task);//return value:the Int signal

/**********************************************************************
//    Description:
//      check the temp signal queue to see if there any service request
//    signal from other layer and get it.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
xSignalHeaderRec *SIMINSTR_CheckTempSignalQueue(uint32 cur_sim_task);   //return value:the service request signal

/**********************************************************************
//    Description:
//      Clear the temporary signal queue
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMINSTR_ClearTempSignalQueue(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      Check is there the signal_code in temp queue?
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
xSignalHeaderRec* SIMINSTR_IsThisSIGNALInTempSignalQueue(uint32 cur_sim_task, uint32 simsigcode);

#ifdef SIM_PLUG_IN_SUPPORT
/**********************************************************************
//    Description:
//      Check is there the signal_code in temp INT queue?
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
TB_SIM_INT_TYPE_E SIMINSTR_LatestHotSwapMessageInTempIntSignalQueue(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      add this signal in sim temp INT queue
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_AddThisSIGNALInTempIntSignalQueue(uint32 cur_sim_task, xSignalHeaderRec *psig);
#endif

/**********************************************************************
//    Description:
//      add this signal in sim temp queue
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_AddThisSIGNALInTempSignalQueue(uint32 cur_sim_task, xSignalHeaderRec *psig);

/**********************************************************************
//    Description:
//      Clear the temporary interrupt signal queue
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMINSTR_ClearTempIntSignalQueue(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      Clear the STK signal in queue before Restore.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_ClearPCExistSignalInQueue(uint32 cur_sim_task);

//the SIM application toolkit command envlope function
/**********************************************************************
//    Description:
//      the envelope function of the TERMINAL PROFILE command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_TerminalProfile(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_TERMINAL_PROFILE_DATA_T *profile_ptr //in:the terminal profile
);

/**********************************************************************
//    Description:
//      the envelope funtion of the ENVELOPE command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_Envelope(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_ENVELOPE_DATA_T *data_ptr,   //in:the envelope data to the SIM card
    SIM_ENVELOPE_RESPONSE_T *response_ptr, //out:the envelope response data
    SIMAT_ENVELOPE_RESULT_E *result,       //out:the result of the envelope data operation
    BOOLEAN                  delay_fetch,   //delay fetch,because there is proactive command not terminal response to sim
    uint8 *sw1,
    uint8 *sw2
);

/**********************************************************************
//    Description:
//      the envelope funtion of the FETCH command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_Fetch(    //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FETCH_DATA_T *data_ptr    //out:the proactive SIM command data from the SIM card to ME
);

/**********************************************************************
//    Description:
//      the envelope funtion of the TERMINAL RESPONSE command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_TerminalResponse( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_TERMINAL_RESPONSE_DATA_T *data_ptr,  //in:the Terminal response data from the ME to SIM
    uint8* sw1,
    uint8* sw2
);

/**********************************************************************
//    Description:
//      Clear the global variables because of a different sim card inserted.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_HotSwap_ClearStaticVariable(uint32 cur_sim_task);

#ifdef SIM_DROP_RESTORE_SUPPORT
/********************************************************************
//    DESCRIPTION
//      
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_SetAuthInAuthFlag(uint32 cur_sim_task, BOOLEAN flag);

/********************************************************************
//    DESCRIPTION
//      
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMINSTR_GetAuthInAuthFlag(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMINSTR_SetGMMAuthInAuthFlag(uint32 cur_sim_task, BOOLEAN flag);

/********************************************************************
//    DESCRIPTION
//      
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMINSTR_GetGMMAuthInAuthFlag(uint32 cur_sim_task);
#endif

/**********************************************************************
//    Description:
//     Update immediately  a EF file from the SIM card,the assumed condition is that the current file
//    is the file to read,do not need file routing
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note: the current file is the file to be readed
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_ImiUpdateEF(   //return value:the result of the read operation
    uint32 cur_sim_task,
    uint16 file_id,  //the file to be read
    uint8 path_len,//IN:
    uint8 *path_id,//IN:
    uint16  offset, //the offset to update in transparent file
    uint8       record_num, //the record num to update of recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint16       data_len,   //the update data len
    uint8  *data    //in:the data to update file
);

#ifdef SIM_3G_USIM_SUPPORT
#ifdef SIM_3G_FORMAT_TRANS
/**********************************************************************
//    Description:
//      Convert 3G response data to 2G format
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMINSTR_3GResponseTo2GFormat(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    uint8 *data_ptr,
    uint16 *data_len);
#endif
#endif

#ifdef MODEM_PLATFORM
/**********************************************************************
//    Description:
//     
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_CSIM(uint32 cur_sim_task, uint8 data_len, uint8 *data_ptr);
#endif

//global functions defined in sim_file_system.c file
/**********************************************************************
//    Description:
//      Select a file in the SIM card file system,the file select rule
//    refference to GSM 11.11 section 6.5 "Methods for selecting a file"
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
int8 SIMFS_FileRouting( //reutrn value:the number of file in the routing to the destionation file
    SIM_FILE_NAME_E file_name,  //the desitination file
    SIM_FILE_NAME_E cur_file,   //the current selected direction file
    SIM_FILE_NAME_E *file_route //the file route to the destionation file
);
/**********************************************************************
//    Description:
//      Get SIM file name by a file id
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_FILE_NAME_E SIMFS_GetFileName(
    uint32 cur_sim_task,
    uint16 file_id,  //the file id
    uint8  path_id_len,
    uint8  *path_id);

/**********************************************************************
//    Description:
//      Check if the SIM card is a DCS1800 one
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMFS_IsDCS1800SIM(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      Set the dcs1800 flag for the sim card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMFS_SetDSC1800Flag(
    uint32 cur_sim_task,
    BOOLEAN is_dcs1800  //the dcs1800 flag
);

/**********************************************************************
//    Description:
//      get the current select directory file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_FILE_NAME_E SIMFS_GetCurDirectoryFile(uint32 cur_sim_task); //the current select dirctory file

/**********************************************************************
//    Description:
//      save the current direcory file to s_sim_cur_directory_file for
//    laterly to find a select file path
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMFS_SetCurDirectoryFile(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name    //the file have just been selected
);

/**********************************************************************
//    Description:
//      get the current select file name
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_FILE_NAME_E SIMFS_GetCurFileName(uint32 cur_sim_task); //the current select file name

/**********************************************************************
//    Description:
//      save the current file name to s_sim_cur_file_name for
//    laterly to decide whether to re-select a file or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMFS_SetCurFileName(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name    //the file have just been selected
);


/**********************************************************************
//    Description:
//      get the a file's parent file name
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_FILE_NAME_E SIMFS_GetParenFileName( //return value:parent file name
    SIM_FILE_NAME_E file_name   //the file name
);

/**********************************************************************
//    Description:
//      get the a EF file's file ID
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint16  SIMFS_GetFileID(    //return value:the file ID
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name     //the file name
);

/**********************************************************************
//    Description:
//      get the a EF file's structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_EF_STRUCTURE_E SIMFS_GetEFStructure(    //return value:the EF file structure
    SIM_FILE_NAME_E file_name   //the file name
);

/**********************************************************************
//    Description:
//      Select a file in the SIM card file system,the file select rule
//    refference to GSM 11.11 section 6.5 "Methods for selecting a file"
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
int8 SIMFS_SelectFileInstr(  //return value:the select file result
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name, //the destination file
    SIM_FILE_NAME_E *file_route    //out:the file route
);

/**********************************************************************
//    Description:
//      get the a file's type
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
/*SIM_FILE_TYPE_E SIMFS_GetFileType(  //return value:the file's file type
                                SIM_FILE_NAME_E file_name   //the file name
                                );*/

/**********************************************************************
//    Description:
//      the function return the file_name is belong to DFtelecom or not.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMFS_IsInDFtelecom(SIM_FILE_NAME_E file_name);

/**********************************************************************
//    Description:
//      the function return the file_name is belong to DFgsm or not.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMFS_IsInDFgsm(SIM_FILE_NAME_E file_name);

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      the function return the file_name is belong to ADFusim or not.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMFS_IsInADFusim(SIM_FILE_NAME_E file_name);

/**********************************************************************
//    Description:
//      Remap the EFs under DFgsm to 3G ADFusim
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMFS_RemapUSIMFileName(uint32 cur_sim_task, SIM_FILE_NAME_E file_name, SIM_FILE_NAME_E *remap_file_name);
#endif

/**********************************************************************
//    Description:
//      the function set the SMS files is in DFtelecom or not.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMSMS_SetSMSLocationDFtel(uint32 cur_sim_task, BOOLEAN is_sms_in_dftel);

/**********************************************************************
//    Description:
//      the function return the SMS files is in DFtelecom or not.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMSMS_GetSMSLocationDFtel(uint32 cur_sim_task);

//global functions defined in sim_main.c file
/********************************************************************
//    DESCRIPTION
//    Initialize the SIM layer when the SIM layer just been start up
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMAIN_InitSIMTask(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//    reset the SIM layer static and global variable at the beignning of SIM
//    layer or ever reset and reject the SIM card.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMAIN_ResetSIMVariable(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//    active the sim check timer
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMMAIN_StartCardPresentTimer(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//    Disable low layer PPS procedure
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIM_DisablePPSFeature(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      Do the Terminal response opreation as service requested
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_TerminalResponse(
    uint32 cur_sim_task,
    SIMAT_SIG_TERMINAL_RESPONSE_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      Envelope the data from the SIM application toolkit to the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_Envelope(
    uint32 cur_sim_task,
    SIMAT_SIG_ENVELOPE_REQ_T *psig  //the service request signal
);
#ifdef MODEM_PLATFORM
/**********************************************************************
//    Description:
//      Envelope the data from the SIM application toolkit to the SIM card(for at cmd)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ATCpduEnvelope(
    uint32 cur_sim_task,
    SIMAT_SIG_AT_CPDU_ENVELOPE_REQ_T *psig //the service request signal
);
#endif

//global functions defined in sim_files_structure.c
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
);

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
);

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
);

//global functions defined in sim_service_procedure.c
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
);

/**********************************************************************
//    Description:
//      Seek a empty record in a record EF file
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
);

/**********************************************************************
//    Description:
//      the function to read the whole EF file and decode the file
//    cotent to the coresponding ef mapping structure
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_ReadWholeTransparentEF( //return value:the result of the read operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the Transparent file to be readed
    void *ef_mapping    //out:the file conten mapping sturcute
);

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
);

/**********************************************************************
//    Description:
//      the function to encode a Transparent EF mapping structure to the
//    file content and then update the EF file using the file content
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_UpdateWholeTransparentEF(   //return value:the result of the update operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be updated
    void *ef_mapping    //in:the file content mapping used to update
);

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
);

/**********************************************************************
//    Description:
//      Invalidate an EF file from the SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_InvalidateEF(uint32 cur_sim_task, SIM_FILE_NAME_E file_name);

/**********************************************************************
//    Description:
//      Rehabilitate a EF file from the SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMSER_RehabilitateEF(uint32 cur_sim_task, SIM_FILE_NAME_E file_name);

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
);

//global function of sim_siganl.c
/**********************************************************************
//    Description:
//      the fucntion process the CHV request siganl
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_CHVRequest(
    uint32 cur_sim_task,
    SIM_SIG_CHV_REQ_T *psig   //in:the request signal
);

/**********************************************************************
//    Description:
//      the function that process the Transparent file read request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadTransparentEF(
    uint32 cur_sim_task,
    SIM_SIG_READ_T_FILE_REQ_T *psig //in:the transparent fiel read request signal
);

/**********************************************************************
//    Description:
//      the function that process the recordy file read request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadRecordEF(
    uint32 cur_sim_task,
    SIM_SIG_READ_R_FILE_REQ_T *psig //in:the request signal
);

/**********************************************************************
//    Description:
//      the function that process the transparent file update request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateTransparentEF(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_T_FILE_REQ_T *psig   //in:the request signal
);

/**********************************************************************
//    Description:
//      the function that process the recordy file update request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateRecordEF(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_R_FILE_REQ_T *psig //in:the request signal
);

#ifdef GSM_CUSTOMER_AFP_SUPPORT
/**********************************************************************
//    Description:
//      the function that process the recordy file update request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateLanguageReq(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_LANGUAGE_REQ_T *psig //in:the request signal
);
#endif

#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the function that process the EF file increase request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_IncreaseEF(
    uint32 cur_sim_task,
    SIM_SIG_INCREASE_FILE_REQ_T *psig //in:the request signal
);

/**********************************************************************
//    Description:
//      read imsi again,becasue for the Dphone encrypt card,the IMSI will
//    changed from 0xff... to valid value
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_EncryptReadIMSI(
    uint32 cur_sim_task,
    SIM_SIG_INIT_ENCRYPT_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      proccess the Get SPN service request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_GetSPN(
    uint32 cur_sim_task,
    SIM_SIG_GET_SPN_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      the session termination service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_SessionTerminate(
    uint32 cur_sim_task,
    SIM_SIG_SESSION_TERMINATION_REQ_T *psig //the request signal
);

/**********************************************************************
//    Description:
//      the Emergency Call codes request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadECC(
    uint32 cur_sim_task,
    SIM_SIG_READ_ECC_REQ_T *psig
);

/**********************************************************************
//    Description:
//      the Language preference request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadLP(
    uint32 cur_sim_task,
    SIM_SIG_READ_LP_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      the Language preference update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateLP(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_LP_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Administrative information request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadAD(
    uint32 cur_sim_task,
    SIM_SIG_READ_AD_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      the SIM service talbe request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadSST(
    uint32 cur_sim_task,
    SIM_SIG_READ_SST_REQ_T *psig    //ther service requst signal
);

/**********************************************************************
//    Description:
//      the SIM phase request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadPhase(
    uint32 cur_sim_task,
    SIM_SIG_READ_PHASE_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      the Extended Language preference request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadELP(
    uint32 cur_sim_task,
    SIM_SIG_READ_ELP_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the Extended Language preference update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateELP(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_ELP_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      the IMSI request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadIMSI(
    uint32 cur_sim_task,
    SIM_SIG_READ_IMSI_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Access control request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadACC(
    uint32 cur_sim_task,
    SIM_SIG_READ_ACC_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the HPLMN search period request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadHPLMN(
    uint32 cur_sim_task,
    SIM_SIG_READ_HPLMN_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      the Location information request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadLOCI(
    uint32 cur_sim_task,
    SIM_SIG_READ_LOCI_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Location information update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateLOCI(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_LOCI_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      the Cipher Key request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadKC(
    uint32 cur_sim_task,
    SIM_SIG_READ_KC_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      the Cipher Key update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateKC(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_KC_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the BCCH information request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadBCCH(
    uint32 cur_sim_task,
    SIM_SIG_READ_BCCH_REQ_T *psig   //the service request sginal
);

/**********************************************************************
//    Description:
//      the BCCH information update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateBCCH(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_BCCH_REQ_T *psig //the service request sginal
);

/**********************************************************************
//    Description:
//      the Forbiden PLMN request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadFPLMN(
    uint32 cur_sim_task,
    SIM_SIG_READ_FPLMN_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      the Forbiden PLMN update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateFPLMN(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_FPLMN_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the PLMN selctor request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadPLMNSEL(
    uint32 cur_sim_task,
    SIM_SIG_READ_PLMNSEL_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the PLMN selctor update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdatePLMNSEL(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_PLMNSEL_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      the Cell broadcast message identifier request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadCBMI(
    uint32 cur_sim_task,
    SIM_SIG_READ_CBMI_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Cell broadcast message identifier update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateCBMI(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_CBMI_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      the Voice group call request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadVGCS(
    uint32 cur_sim_task,
    SIM_SIG_READ_VGCS_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the Voice group call service status request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadVGCSS(
    uint32 cur_sim_task,
    SIM_SIG_READ_VGCSS_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Voice broadcase request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadVBS(
    uint32 cur_sim_task,
    SIM_SIG_READ_VBS_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the Voice broadcast status request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadVBSS(
    uint32 cur_sim_task,
    SIM_SIG_READ_VBSS_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Enhanced Multi Level Pre_emption and Priority request service
//      supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadEMLPP(
    uint32 cur_sim_task,
    SIM_SIG_READ_EMLPP_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      the Automatic Answer on eMLPP  request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadAAEM(
    uint32 cur_sim_task,
    SIM_SIG_READ_AAEM_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Automatic Answer on eMLPP  update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateAAEM(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_AAEM_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      the Cell Broadcase message range identifier request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadCBMIR(
    uint32 cur_sim_task,
    SIM_SIG_READ_CBMIR_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      the Cell Broadcase message range identifier update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateCBMIR(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_CBMIR_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the Depersonalisation Control Keys request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadDCK(
    uint32 cur_sim_task,
    SIM_SIG_READ_DCK_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the Network's indication of alerting request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadNIA(
    uint32 cur_sim_task,
    SIM_SIG_READ_NIA_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the function that process the UPDATE CCP request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_UpdateCCP(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_CCP_REQ_T *psig  //the update CCP request signal
);

/**********************************************************************
//    Description:
//      the function that process the ERASE CCP request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_EraseCCP(
    uint32 cur_sim_task,
    SIM_SIG_ERASE_CCP_REQ_T *psig //the erasure CCP request signal
);

/**********************************************************************
//    Description:
//      the function that process the get free ext number number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_GetFreeExtInfo(
    uint32 cur_sim_task,
    SIM_SIG_GET_FREE_EXT_INFO_REQ_T *psig   //the get free ext number request signal
);

/**********************************************************************
//    Description:
//      the function that process the READ CCP request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_ReadCCP(
    uint32 cur_sim_task,
    SIM_SIG_READ_CCP_REQ_T *psig    //the read CCP request signal
);

/**********************************************************************
//    Description:
//      the function that process the PURGE Dialling number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_PurgeDN(
    uint32 cur_sim_task,
    SIM_SIG_PURGE_DN_REQ_T  *psig    //the purge dialling number request signal
);
#endif

#ifdef SIM_AOC_SUPPORT
/**********************************************************************
//    Description:
//      the Accumulate call meter request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadACM(
    uint32 cur_sim_task,
    SIM_SIG_AOC_READ_ACM_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the Accumulate call meter maximum value request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadACMMAX(
    uint32 cur_sim_task,
    SIM_SIG_AOC_READ_ACMMAX_REQ_T *psig    //the service request signal
);
#endif
/**********************************************************************
//    Description:
//      process the Get file info service that request by the SIM reader
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_GetFileInfo(
    uint32 cur_sim_task,
    SIM_SIG_GET_FILE_INFO_REQ_T *psig   //in:the request signal
);

/**********************************************************************
//    Description:
//      the function that proccess the SIM initialize signal,reference to the
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_SIMInitialization(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_INITIALIZE_REQ_T *psig  //in:the initialization request signal
);

#ifdef GSM_DPHONE_VER2_SUPPORT
/**********************************************************************
//    Description:
//    verify the chv1 when the Dphone version2 is in intialization.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_SIMDphonePinOperation(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_INITIALIZE_REQ_T *psig  //in:the initialization request signal
);
#endif

/**********************************************************************
//    Description:
//      read information from SIM card that will send to the information task
//      in Card_Present indication
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_PrepareCardPresentInd(    //return value:get the card ind success or fail
    uint32 cur_sim_task,
    SIM_SIG_CARD_PRESENT_IND_T *ind //out:the card present indicaton
);
#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the Group identifier leve 1 request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadGID1(
    uint32 cur_sim_task,
    SIM_SIG_READ_GID_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Group identifier leve 2 request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadGID2(
    uint32 cur_sim_task,
    SIM_SIG_READ_GID_REQ_T *psig   //the service request signal
);
#endif

/**********************************************************************
//    Description:
//      the Accumulate call meter intialize service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_InitACM(
    uint32 cur_sim_task,
    SIM_SIG_AOC_INIT_ACM_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the Accumulate call meter increase service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_IncreaseACM(
    uint32 cur_sim_task,
    SIM_SIG_AOC_INCREASE_ACM_REQ_T *psig    //the service request signal
);

/**********************************************************************
//    Description:
//      the Accumulate call meter maximum value update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateACMMAX(
    uint32 cur_sim_task,
    SIM_SIG_AOC_UPDATE_ACMMAX_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      the Price per unit and currency talbe request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadPUCT(
    uint32 cur_sim_task,
    SIM_SIG_AOC_READ_PUCT_REQ_T *psig   //the service request signal
);

/**********************************************************************
//    Description:
//      the Price per unit and currency table update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdatePUCT(
    uint32 cur_sim_task,
    SIM_SIG_AOC_UPDATE_PUCT_REQ_T *psig //the service request signal
);

/**********************************************************************
//    Description:
//      the record EF(including EFadn,EFfdn,EFmsisdn,EFlnd,EFsmsp,EFsdn
//      and EFbdn) information request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_GetRecordEFInfo(
    uint32 cur_sim_task,
    SIM_SIG_GET_RECORD_EF_INFO_REQ_T *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      Polling the SIM card to check the proactive command exist or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_Polling(uint32 cur_sim_task, SIMAT_SIG_POLLING_IND_T *psig);

/**********************************************************************
//    Description:
//      Notify the STK task that the current stk application session is
//    terminated
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_IndStkSessionTerminate(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      Polling the SIM card to check the proactive command exist or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_GetPC(
    uint32 cur_sim_task,
    SIM_SIG_PC_EXIST_IND_T *psig    //the proactive command indicator signal
);

/**********************************************************************
//    Description:
//      Notify the STK task that the current stk application session is
//    terminated
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_TerminalProfile(
    uint32 cur_sim_task,
    SIMAT_SIG_TERMINAL_PROFILE_REQ_T *psig    //the service request signal
);

//global function of sim_mm_siganl.c
/**********************************************************************
//    Description:
//      the MM DATA request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_MMDataReq(
    uint32 cur_sim_task,
    xSignalHeaderRec *psig //the MM_data request signal
);

/**********************************************************************
//    Description:
//      the MM AUTH request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_AuthReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_AUTH_REQ *psig  //the Auth request singal
);

/**********************************************************************
//    Description:
//      the MM FPLMN update request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_FPLMNUpdateReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_FPLMN_UPD_REQ *psig  //the request signal
);

/**********************************************************************
//    Description:
//      the MM Cipher update request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_CipherUpdateReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_CIPHER_UPD_REQ *psig    //the request signal
);

/**********************************************************************
//    Description:
//      the MM SIM status update request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_SIMStatusUpdateReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_STATUS_UPD_REQ *psig //the request signal
);

/**********************************************************************
//    Description:
//      the MM BAlist update request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_BAListUpdateReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_BALIST_UPD_REQ *psig    //the request signal
);

#ifdef _SUPPORT_GPRS_
/**********************************************************************
//    Description:
//      the GMM authorize service request signal process function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMGMMSIG_AuthReq(
    uint32 cur_sim_task,
    yPDef_GMM_SIM_AUTH_REQ *psig    //the request signal
);
#endif

#ifdef _SUPPORT_GPRS_
/**********************************************************************
//    Description:
//      the GMM Save RAI informaton service request signal process function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMGMMSIG_SaveRAIInfo(
    uint32 cur_sim_task,
    yPDef_GMM_SIM_SAVE_RAI_INFO *psig   //the service request signal
);
#endif

#ifdef _SUPPORT_GPRS_
/**********************************************************************
//    Description:
//      the GMM Save Cipher informaton service request signal process function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMGMMSIG_SaveCipherInfo(
    uint32 cur_sim_task,
    yPDef_GMM_SIM_SAVE_CIPHER_INFO *psig    //the service request signal
);
#endif

/**********************************************************************
//    Description:
//      clear the variable if the hot swap happen
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMMMSIG_HotSwap_ClearStaticVariable(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMMMSIG_IsInAuthentication(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMMMSIG_IsInGMMAuthentication(uint32 cur_sim_task);

//global funciton of sim_win_test.c
#ifdef SIM_TEST_IN_WIN32
/**********************************************************************
//    Description:
//      get the file name that save the SIM card file select data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_GetSelectFileName(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path    //out:the pointer of the file name
);

/**********************************************************************
//    Description:
//      get all the chvs that save in the chv file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_UpdateAllCHV(   //return vlaue:the result of the operation
    uint32 cur_sim_task,
    SIM_ALL_CHV_T *chvs //in:all of the sim chvs
);

/**********************************************************************
//    Description:
//      get all the chvs that save in the chv file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_GetAllCHV(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_ALL_CHV_T *chvs //out:all of the sim chv
);

/**********************************************************************
//    Description:
//      simulator select file in really SIM card by reading the file
//    that save the coresponding sim file select data.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_SelectFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data  //out:the select data of the sim file
);

/**********************************************************************
//    Description:
//      update the file that save the coresponding sim file select data
//    with the sim file select data.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_UpdateSelectData(   //the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RESPONSE_DATA_OF_SELECT_T select_data   //the select data of the sim file
);

/**********************************************************************
//    Description:
//      Get the base directory of the sim set files
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMTIW_GetCHVVerificated(
    uint32 cur_sim_task,
    BOOLEAN *chv1_v,    //out:the chv1 verificated state
    BOOLEAN *chv2_v     //out:the chv1 verificated state
);

/**********************************************************************
//    Description:
//      Get the base directory of the sim set files
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMTIW_SetCHVVerificated(
    uint32 cur_sim_task,
    BOOLEAN chv1_v, //the chv1 verificated state
    BOOLEAN chv2_v  //the chv1 verificated state
);

/**********************************************************************
//    Description:
//      simulator the Read EF file by read the file that save the
//    coresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_ReadFile(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16  offset, //the reading offset of the transparent file
    uint8   record_num, //the record num of the recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of the recordy file
    uint16 *filesize_recordlen, //the file sizeof transparent file or the record len of recordy file
    uint16 *data_len,   //the data len that readed out
    uint8  *data    //the data that readed out
);

/**********************************************************************
//    Description:
//      simulator the update EF file by update the file that
//    save the coresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_UpdateFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16  offset, //the update offset of the transparent file
    uint8       record_num, //the record num of the recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode
    uint8       data_len,   //the update data len
    uint8  *data    //in:the update data
);

/**********************************************************************
//    Description:
//      simulator the Seek in file by Seek the file that save the
//    coresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_SeekInFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_SEEK_TYPE_E seek_type,  //the seek type
    SIM_SEEK_MODE_E seek_mode,  //the seek mode
    uint8   pattern_len,    //the seek patter len
    uint8 *pattern, //in:the seek patter
    uint8 *record_num   //out:the reocor num that been seek out
);

/**********************************************************************
//    Description:
//      simulator the increase file by increase the file that save the
//    coresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_Increase(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_INSTR_INCREASE_DATA_T data,   //the increase data
    SIM_INSTR_INCREASE_RESULT_T *result //out:the file content after increase
);

/**********************************************************************
//    Description:
//      simulator the verify CHV in SIM card by verify coresponding chv
//    in the chv saved file.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_VerifyCHV(  //return value:the reuslt of the operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E chv_indication,    //the chv indication
    SIM_CHV_T sim_chv   //the chv to be verifyed
);

/**********************************************************************
//    Description:
//      simulator the change CHV in SIM card by change coresponding chv
//    in the chv saved file.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_ChangeCHV(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E chv_indication,    //the chv indication
    SIM_CHV_T old_chv,  //the old chv
    SIM_CHV_T new_chv   //the new chv
);

/**********************************************************************
//    Description:
//      simulator the disable CHV in SIM card by verify the chv1 in the
//    chv saved file and then disable the chv in the file that save the
//    select data of EF_GSM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_DisableCHV( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_T chv1  //the chv1
);

/**********************************************************************
//    Description:
//      simulator the enable CHV in SIM card by verify the chv1 in the
//    chv saved file and then enable the chv in the file that save the
//    select data of EF_GSM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_EnableCHV(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_T chv1  //the chv1
);

/**********************************************************************
//    Description:
//      simulator the unblock CHV in SIM card by verify the unblock chv
//    in the chv saved file and then change the chv1 in the chv saved
//    file and the change the chv status in the file that save the select
//    data of DF_GSM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_UnblockCHV( //return value:the result of operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E chv_indication,    //chv indication
    SIM_CHV_T unblock_chv,  //the unblock chv
    SIM_CHV_T new_chv   //the new chv
);

/**********************************************************************
//    Description:
//      simulator invlidate EF file by change validate in the file that
//    save the select data of the coresponding EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_InvalidateFile( //return value:the result of the operatin
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name   //the sim file name
);

/**********************************************************************
//    Description:
//      simulator rehabilitate EF file by change validate in the file that
//    save the select data of the coresponding EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_RehabilitateFile(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name   //the sim file name
);

/**********************************************************************
//    Description:
//      simulator run GSM algorithm by return the fixed algorithm result.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_RunGSMAlgorithm(    //return value:the result of the opertion
    RAND_T rand,    //the algorithm rand
    SIM_GSM_ALGORITHM_RESULT_T *result  //out:the algorithm result
);

/**********************************************************************
//    Description:
//      simulator the sim sleep operation
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_Sleep(void); //return value:the result of the operation

/**********************************************************************
//    Description:
//      create the ef simulator file according the select data that from
//    the coresponding ef file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_CreatEFFile(    //return vlaue:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name   //the sim file name
);

BOOLEAN SIMINT_QuerySIM(int card_num);
void SIMINT_RejectSIM(int card_num);
void SIMINT_InsertSIM(int card_num);
/**********the following is for static button sim card setting---start**********/
SIM_SERVICE_RESULT_E SIMSER_ReadWholeTransparentEF_static( //return value:the result of the read operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the Transparent file to be readed
    void *ef_mapping,    //out:the file conten mapping sturcute
    int card_num
);
SIM_SERVICE_RESULT_E SIMSER_ReadEFRecord_static(   //return vlaue:the result of the read operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be readed
    uint8 record_num,   //the record num to read
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,    //out:the file cotent read out
    int card_num
);
SIM_SERVICE_RESULT_E SIMSER_UpdateWholeTransparentEF_static(   //return value:the result of the update operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be updated
    void *ef_mapping,    //in:the file content mapping used to update
    int card_num
);
SIM_SERVICE_RESULT_E SIMSER_UpdateEFRecord_static( //return value:the result of the update operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file to be updated
    uint8 record_num,   //the reocrd num to update
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    void *ef_mapping,   //in:the file cotent mapping used to update
    BOOLEAN erasure_record,  //erasure the record to empty or not
    int card_num
);
SIM_SERVICE_RESULT_E SIMTIW_GetSelectFileName_static(  //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path,    //out:the pointer of the file name
    int card_num
);
SIM_SERVICE_RESULT_E SIMTIW_UpdateAllCHV_static(   //return vlaue:the result of the operation
    SIM_ALL_CHV_T *chvs, //in:all of the sim chvs
    int card_num
);
SIM_SERVICE_RESULT_E SIMTIW_GetAllCHV_static(  //return value:the result of the operation
    SIM_ALL_CHV_T *chvs, //out:all of the sim chv
    int card_num
);
SIM_SERVICE_RESULT_E SIMTIW_SelectFile_static( //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,  //out:the select data of the sim file
    int card_num
);
SIM_SERVICE_RESULT_E SIMTIW_UpdateSelectData_static(   //the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RESPONSE_DATA_OF_SELECT_T select_data,   //the select data of the sim file
    int card_num
);
void SIMTIW_GetCHVVerificated_static(
    BOOLEAN *chv1_v,    //out:the chv1 verificated state
    BOOLEAN *chv2_v,    //out:the chv1 verificated state
    int card_num
);
void SIMTIW_SetCHVVerificated_static(
    BOOLEAN chv1_v, //the chv1 verificated state
    BOOLEAN chv2_v,     //the chv1 verificated state
    int card_num
);
SIM_SERVICE_RESULT_E SIMTIW_CreatEFFile_static(    //return vlaue:the result of the operation
    SIM_FILE_NAME_E file_name,   //the sim file name
    int card_num
);
SIM_SERVICE_RESULT_E SIMTIW_UpdateFile_static( //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16  offset, //the update offset of the transparent file
    uint8       record_num, //the record num of the recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode
    uint8       data_len,   //the update data len
    uint8  *data,    //in:the update data
    int card_num
);
BOOLEAN SIMFS_IsDCS1800SIM_static(int card_num);

SIM_SERVICE_RESULT_E SIMINSTR_UpdateEF_static( //return value:the file update operation result
    SIM_FILE_NAME_E file_name,  //the ef file to be updated
    uint16  offset, //the offset to update in transparent file
    uint8       record_num, //the record num to update of recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint8       data_len,   //the update data len
    uint8  *data,   //in:the data to update file
    int card_num
);
#endif


//the global functions that defined in sim_sms.c
/**********************************************************************
//    Description:
//      the function that process the Read SMS parameter request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_ReadSMSParameter(
    uint32 cur_sim_task,
    SIM_SMS_READ_PARAM_REQ_T *psig   //in:the request signal
);

/**********************************************************************
//    Description:
//      Get the SMS record num in the EFsms
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_GetSMSNum(
    uint32 cur_sim_task,
    SIM_SMS_GET_SMS_NUM_REQ_T *psig //in:the service request signal
);

/**********************************************************************
//    Description:
//      the function that process the Read short message request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_ReadSM(
    uint32 cur_sim_task,
    SIM_SMS_READ_SMS_REQ_T *psig //in:the request signal
);

/**********************************************************************
//    Description:
//      the function that process the save short message request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_SaveSM(
    uint32 cur_sim_task,
    SIM_SMS_WRITE_SMS_REQ_T *psig //in:the request signal
);

/**********************************************************************
//    Description:
//      the function that process the edit short message request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_EditSM(
    uint32 cur_sim_task,
    SIM_SMS_EDIT_SMS_REQ_T *psig //in:the request signal
);

#ifdef MNSMS_REPLACE_SMS
/**********************************************************************
//    Description:
//      the function that process the replace short message request signal
//    Global resource dependence :
//    Author:William.Qian 2005_12_20
//    Note:
***********************************************************************/
void SIMSMS_ReplaceSM(                                          // for replace sms sim
    uint32 cur_sim_task,
    SIM_SMS_REPLACE_SMS_REQ_T *psig //in:the request signal
);
#endif

/**********************************************************************
//    Description:
//      the function that process the update short message state
//    request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_UpdateSMState(
    uint32 cur_sim_task,
    SIM_SMS_UPDATE_SMS_STATUS_REQ_T *psig    //in:the request signal
);

/**********************************************************************
//    Description:
//      the function that process the read short message status report
//    request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_ReadSMStatusReport(
    uint32 cur_sim_task,
    SIM_SMS_READ_STATUS_REPORT_REQ_T *psig    //in:the reauest signal
);

/**********************************************************************
//    Description:
//      the function that process the get TP message reference request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_GetTPMessageReference(
    uint32 cur_sim_task,
    SIM_SMS_READ_SMSS_REQ_T *psig  //in:the request signal
);

/**********************************************************************
//    Description:
//      the function that process the save TP message reference request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_SaveTPMessageReference(
    uint32 cur_sim_task,
    SIM_SMS_WRITE_SMSS_REQ_T *psig  //in:the requst signal
);

/**********************************************************************
//    Description:
//      the function that process the save SMS parameter request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_UpdateSMSParameter(
    uint32 cur_sim_task,
    SIM_SMS_WRITE_PARAM_REQ_T *psig  //in:the request signal
);

//the functions that defined in the sim_dialling_number.c
/**********************************************************************
//    Description:
//      the function that process the READ Dialling number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_ReadDN(
    uint32 cur_sim_task,
    SIM_SIG_READ_DN_REQ_T *psig     //the read dialling number request signal
);

/**********************************************************************
//    Description:
//      the function that process the UPDATE Dialling number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_UpdateDN(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_DN_REQ_T *psig   //the udpate dialling number request signal
);

/**********************************************************************
//    Description:
//      the function that process the ERASE LND All request signal
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
void SIMDN_EraseLNDAll(
    uint32 cur_sim_task,
    SIM_SIG_ERASE_LND_ALL_REQ_T  *psig    //the erasure LND request signal
);

/**********************************************************************
//    Description:
//      the function that process the ERASE Dialling number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_EraseDN(
    uint32 cur_sim_task,
    SIM_SIG_ERASE_DN_REQ_T  *psig    //the erasure dialling number request signal
);

/**********************************************************************
//    Description:
//      the function that process the FDN enable/disable check
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_FDNCapabilityCheck(
    uint32 cur_sim_task
);

/**********************************************************************
//    Description:
//      the function that process the FDN service request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_FDNService(
    uint32 cur_sim_task,
    SIM_SIG_FDN_SERVICE_REQ_T  *psig    //the FDN service request signal
);

/**********************************************************************
//    Description:
//      the function that process the BDN enable/disable check
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_BDNCapabilityCheck(
    uint32 cur_sim_task
);

/**********************************************************************
//    Description:
//      the function that process the BDN service request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_BDNService(
    uint32 cur_sim_task,
    SIM_SIG_BDN_SERVICE_REQ_T    *psig    //the BDN service request signal
);

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      Set Wether we use the EFadn in DFtelecom/DFpb
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDN_SetIsADNinDFteleDFpb(uint32 cur_sim_task, BOOLEAN is_adn_exist, uint8 adn_num, uint16 *adn_id, uint16 ext1_id, uint16 *iap_id, uint16 *pbc_id, uint16 *grp_id, uint16 aas_id, uint16 gas_id, uint16 *anra_id, uint16 *anrb_id, uint16 *anrc_id, uint16 *sne_id, uint16 ccp1_id, uint16 *uid_id, uint16 *emaila_id, uint16 *emailb_id);

/**********************************************************************
//    Description:
//      Get wether we the EFadn in DFtelecom/DFpb is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_GetIsADNinDFteleDFpb(
    uint32 cur_sim_task,
    uint8 *adn_num,
    uint16 *adn_id, uint16 *ext1_id, uint16 *iap_id, uint16 *pbc_id,
    uint16 *grp_id, uint16 *aas_id, uint16 *gas_id, uint16 *anra_id,
    uint16 *anrb_id, uint16 *anrc_id, uint16 *sne_id, uint16 *ccp1_id,
    uint16 *uid_id, uint16 *emaila_id, uint16 *emailb_id);

/**********************************************************************
//    Description:
//      Set wether the EFadn in ADFusim/DFpb is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDN_SetIsADNinADFusimDFpb(uint32 cur_sim_task, BOOLEAN is_adn_exist, uint8 adn_num, uint16 *adn_id, uint16 ext1_id, uint16 *iap_id, uint16 *pbc_id, uint16 *grp_id, uint16 aas_id, uint16 gas_id, uint16 *anra_id, uint16 *anrb_id, uint16 *anrc_id, uint16 *sne_id, uint16 ccp1_id, uint16 *uid_id, uint16 *emaila_id, uint16 *emailb_id);

/**********************************************************************
//    Description:
//      Get wether the EFadn in ADFusim/DFpb is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_GetIsADNinADFusimDFpb(
    uint32 cur_sim_task,
    uint8 *adn_num,
    uint16 *adn_id, uint16 *ext1_id, uint16 *iap_id, uint16 *pbc_id,
    uint16 *grp_id, uint16 *aas_id, uint16 *gas_id, uint16 *anra_id,
    uint16 *anrb_id, uint16 *anrc_id, uint16 *sne_id, uint16 *ccp1_id,
    uint16 *uid_id, uint16 *emaila_id, uint16 *emailb_id);

/**********************************************************************
//    Description:
//      Set wether the EFadn in DFtelecom is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDN_SetIsADNinDFtele(uint32 cur_sim_task, BOOLEAN is_adn_exist);

/**********************************************************************
//    Description:
//      Get wether we the EFadn in DFtelecom is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_GetIsADNinDFtele(uint32 cur_sim_task,uint8 *adn_num);

/**********************************************************************
//    Description:
//     Get the EFpbr infomation.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDN_GetPBRInfo(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//     To get the ADN's infomation. Because the USIM ADN is not the same to the SIM ADN.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMDN_GetUSIMADNRecordInfomation(uint32 cur_sim_task, SIM_RECORD_EF_INFO_T *record_info);

/**********************************************************************
//    Description:
//     To set the MSISDN/FDN/SDN/BDN/EXT2/EXT3 are in DFtelecom or ADFusim.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMPB_SetOtherDNLocationDFtel(uint32 cur_sim_task, BOOLEAN is_other_dn_in_dftel);

/**********************************************************************
//    Description:
//     To get the MSISDN/FDN/SDN/BDN/EXT2/EXT3 are in DFtelecom or ADFusim.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMPB_GetOtherDNLocationDFtel(uint32 cur_sim_task);
#endif//SIM_3G_USIM_SUPPORT

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/********************************************************************************/
void SIMDN_Init(
    uint32          cur_sim_task,
    SIM_FILE_NAME_E dn_name,
    uint16           dn_max_num
);

/********************************************************************************/
//  Description :
//     Clear the global variables because of a different sim card inserted.
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
void SIMDN_HotSwap_ClearStaticVariable(uint32 cur_sim_task);

/********************************************************************************/
//  Description :
//
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
void SIMDN_ClearEXT1Info(uint32 cur_sim_task);

/********************************************************************************/
//  Description :
//
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
void SIMDN_ClearEXT2Info(uint32 cur_sim_task);

#ifdef SIM_TEST_IN_WIN32
/********************************************************************
//    DESCRIPTION
//    get the current SIM task's index, 0,1 or 2.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIM_GetCurrentTaskIndex(void);
#endif

/********************************************************************
//    DESCRIPTION
//    To exchange the SIM slot label in mutli-sim platform.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIM_Exchange_Card_Slot_Label(uint32 cur_sim_task);

/**********************************************************************************************
//    DESCRIPTION
//      Active the instruction timer,while sending the command to the card,the timer
//    is actived at the same time.the timer is used to prevent the SIM card has no any response
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************************************/
void ActiveInstructionTimer(uint32 cur_sim_task, uint32 time_value);

/**********************************************************************************************
//    DESCRIPTION
//      Dective the instruction timer,while has received the response data from the SIM card,
//    the timer is deactived at the same time.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************************************/
void DeactiveInstructionTimer(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      soft power off the sim card when upper indicate that the mobile
//    has been power off
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void PowerOffSIM(uint32 cur_sim_task, SIM_POWER_OFF_TYPE_E type);

#ifdef GSM_DPHONE_VER2_SUPPORT
/**********************************************************************
//    Description:
//      the function that process the read file request signal
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadFileBufByPath(
    uint32 cur_sim_task,
    SIM_SIG_READ_FILE_BUF_BY_PATH_REQ_T *psig //in:the request signal
);
#endif

/**********************************************************************
//    Description:
//      the implementation of select a SIM file by the specified file path
//    and get the select data
//    of the destionation file by using the SIM instructions
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_SelectFileByPath(   //return value:the result of the operation
    uint32 cur_sim_task,
    uint16 file_id,    //the file to be selected
    uint8 file_level,  //the file level under the MF
    uint16 *file_route, //the file path to access the file
    SIM_RESPONSE_DATA_OF_SELECT_T *select_response    //the select data of the file
);

/**********************************************************************
//    Description:
//     Read immediately  a EF file from the SIM card,the assumed condition is that the current file
//    is the file to read,do not need file routing
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note: the current file is the file to be readed
***********************************************************************/
SIM_SERVICE_RESULT_E SIMINSTR_ImiReadEF(   //return value:the result of the read operation
    uint32 cur_sim_task,
    uint16 file_id,  //the file to be read
    uint8 path_len,//IN :
    uint8 *path_id,//IN:
    uint16  offset, //the offset that start to read in transprent file
    uint8   record_num, //the record num to read in recordy file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of recordy file
    uint16 len_want_to_read,  //the length to be read
    uint16 *data_len,   //out:the data len that readed from the file
    uint8  *data        //out:the data readef from the file
);

/**********************************************************************
//    Description:
//      the function that proccess the SIM re-initialize,reference to the
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIM_ReInitialization(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      save a service request signal into the temp queue when the SIM
//    layer is interactive with the SIM driver.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void TempServiceReq(
    uint32 cur_sim_task,
    xSignalHeaderRec *psig
);

#if !defined (SIM_CODE_COMPACT) || defined (MODEM_PLATFORM)
/********************************************************************
//    DESCRIPTION
//     Get card type,including the get result,wether the card is present,
//    and the card type
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_GetCardType(
    uint32 cur_sim_task,
    SIM_SIG_GET_CARD_TYPE_REQ_T *psig //the service request signal
);
#endif

#ifndef SIM_CODE_COMPACT
/********************************************************************
//    DESCRIPTION
//     Get the imsi of the cdma card
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_GetCDMACardIMSI(
    uint32 cur_sim_task,
    SIM_SIG_GET_CDMA_CARD_IMSI_REQ_T *psig //the service request signal
);
#endif

/********************************************************************
//    DESCRIPTION
//      To check a CPHS service is actived and allocatived.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIM_IsCPHSOptionalSupport(uint32 cur_sim_task, SIM_FILE_NAME_E cphs_file_name);

/**********************************************************************
//    Description:
//      the function that proccess the stk refresh signal,the refresh type is sim initialization.
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_RefreshIni(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_INI_REQ_T *psig  //in:the initialization request signal
);

/**********************************************************************
//    Description:
//      the function that proccess the stk refresh signal,the refresh type is sim file change
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_RefreshFileChg(    //return value:the operation is success or not
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_FILE_CHG_REQ_T *psig  //in:the file change signal
);

/**********************************************************************
//    Description:
//      the function that proccess the stk refresh signal,the refresh type is sim initialization and file change
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_RefreshIniFileChg(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_INI_FILE_CHG_REQ_T *psig  //in:the initialization request signal
);

/**********************************************************************
//    Description:
//      the function that proccess the stk refresh signal,the refresh type is sim initialization and full file change
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_RefreshIniFull(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_INI_FULL_REQ_T *psig  //in:the initialization request signal
);

#ifdef GSM_DPHONE_VER2_SUPPORT
/**********************************************************************
//    Description:
//      the function that proccess all the stk refresh signal in the Dphone
//    initialization pending state
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_DphoneRefreshskip(uint32 cur_sim_task, uint32 pid_sender);
#endif

/**********************************************************************
//    Description:
//      the function that process the refresh command
//      read the file that changed by refresh command
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_RefreshReadFile(
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_READ_FILE_REQ_T *psig //in: the read request signal
);

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
void SIMSIGNAL_HotSwap_ClearStaticVariable(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card Un Response Interrupt from SIM card
//    Global resource dependence :
//    retrun: TRUE,the card restore is successful,else restore failed
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN DealCardUnResponse(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      this is the call back fucntion for our SIM driver,to make the
//    SIM driver can notify the SIM layer with kind of signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void DriverNotifySIM(SIMHAL_MSG *msg);

/**********************************************************************************************
//    DESCRIPTION
//    To judge the Present Timer is Active or not
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************************************/
BOOLEAN IsInstructionTimerActive(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//     Disable deep sleep
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void DisableDeepSleep(uint32 cur_sim_task);

#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_CRSM(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T  *psig
);
#endif
/**********************************************************************
//    Description:
//      read card present info for stk refresh
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_RefreshGetCardPresentInfo(    //return value:get the card ind success or fail
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_CARD_PRESENT_REQ_T *psig //in:the card present indication
);
/**********************************************************************
//    Description:
//      read img data from the sim card
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadImg(    //return value:get the card ind success or fail
    uint32 cur_sim_task,
    SIM_SIG_IMG_READ_REQ_T *psig //in:the card present indication
);

#ifdef GSM_DPHONE_SUPPORT
/**********************************************************************
//    Description:
//      the function that send the SIM initialization not OK signal to MN
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
void SIMSIGNAL_SIMInitNotOK(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      the function that send the SIM initialization requset to SIM task
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
void SIMSIGNAL_SIMInitReq(uint32 cur_sim_task);
#endif

#ifndef SIM_CODE_COMPACT
/*****************************************************************************/
//  Description : the function that process the Authentication request from WiFi(or CMMB, etc)
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_CommonAuth(uint32 cur_sim_task, SIM_SIG_COMMON_AUTH_REQ_T *psig);
#endif
/*****************************************************************************/
//  Description : the function that process the VM request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_ReadVoiceMail(uint32 cur_sim_task, SIM_SIG_READ_VOICE_MAIL_REQ_T *psig);

/*****************************************************************************/
//  Description : the function that process the VM request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_UpdateVoiceMail(uint32 cur_sim_task, SIM_SIG_UPDATE_VOICE_MAIL_REQ_T *psig);

/*****************************************************************************/
//  Description : the function that process the READ SIM Transparent EF request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_ReadTransparentEFInRawData(uint32 cur_sim_task, SIM_SIG_READ_RAW_T_FILE_REQ_T *psig);

/*****************************************************************************/
//  Description : the function that process the UPDATE SIM Transparent EF request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_UpdateTransparentEFInRawData(uint32 cur_sim_task, SIM_SIG_UPDATE_RAW_T_FILE_REQ_T *psig);

/*****************************************************************************/
//  Description : the function that process the READ SIM Record EF request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_ReadRecordEFInRawData(uint32 cur_sim_task, SIM_SIG_READ_RAW_R_FILE_REQ_T *psig);

/*****************************************************************************/
//  Description : the function that process the UPDATE SIM Record EF request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_UpdateRecordEFInRawData(uint32 cur_sim_task, SIM_SIG_UPDATE_RAW_R_FILE_REQ_T *psig);

/*****************************************************************************/
//  Description : the function that process the SEARCH request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_Search(uint32 cur_sim_task, SIM_SIG_SEARCH_R_FILE_REQ_T *psig);

#ifdef MODEM_PLATFORM

/*****************************************************************************/
//  Description : the function that process the CSIM AT command
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_CSIM(
    uint32 cur_sim_task,
    SIM_SIG_CSIM_REQ_T  *psig
);
#endif

/*****************************************************************************/
//  Description : clear the current file response when re-initialize,so the SW will
//  re-select the ADF usim and get the pin status
//  Global resource dependence : none
//  Author: yongxia.zhang
//  Note:
/*****************************************************************************/
void SIMINS_ClearCurFileResponse(uint32 cur_sim_task);


/**********************************************************************
//      Trace the data in the sim rx buffer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_PrintRxBuf(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      Trace the data in the sim tx buffer
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIM_PrintTxBuf(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      trace the register value when exception occurs
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_PrintRegisterBuf(uint32 cur_sim_task);

#ifdef   __cplusplus
}
#endif
#endif  //_SIM_GLOBAL_FUNCTION_H_
