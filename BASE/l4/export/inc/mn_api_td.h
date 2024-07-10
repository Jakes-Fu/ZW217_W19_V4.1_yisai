/******************************************************************************
 ** File Name:      mn_api.h                                                  *
 ** Author:         Fancier fan                                               *
 ** Date:           05/28/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************
 

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/28/2002     Fancier fan      Create.                                   *
 ******************************************************************************/

/*****************************************************************************
 	Description : get ef info request to sim 4
	Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
  	Note:
*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetEFInfo(
        MN_DUAL_SYS_E dual_sys,
        uint16      file_id,
        uint8       path_len,
        uint8*      path_ptr
        );

/*****************************************************************************
 	Description : read ef info request to sim
	Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
  	Note:
*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_ReadSimEF(
        MN_DUAL_SYS_E dual_sys,
        uint16      file_id,
        uint8       path_len,
        uint8*      path_ptr
	);

/*****************************************************************************
 	Description : write ef info request to sim
	Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
  	Note:
*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_WriteSimEF(
        MN_DUAL_SYS_E dual_sys,
        uint16 file_id,
        uint8 path_len,
        uint8 * path_ptr,
        uint8 data_len,
        uint8* data_ptr
        );

/*****************************************************************************
 	Description : read record info request to sim
	Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
  	Note:
*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_ReadSimRecordFile(
        MN_DUAL_SYS_E dual_sys,
        uint16 file_id,
        uint8 path_len,
        uint8 * path_ptr,
        uint8 rec_num,
        uint8 record_len
        );

/*****************************************************************************
 	Description : update record  info request to sim
	Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
  	Note:
*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_UpdateSimRecordFile(
        MN_DUAL_SYS_E dual_sys,
        uint16 file_id,
        uint8 path_len,
        uint8 * path_ptr,
        uint8 rec_num,
        uint8 record_len,
        uint8* data_ptr
        );

/*****************************************************************************
 	Description : search record request to sim
	Global resource dependence : none
       Author:       Eddie.Wang
  	Note:
*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_SearchSimRecord(
        MN_DUAL_SYS_E dual_sys,
        uint16 file_id,
        uint8 path_len,
        uint8 * path_ptr,
        MN_SIM_SEARCH_RECORD_T search_sim_record
        );

/*****************************************************************************
 	Description : Set and Reset SIM EF service in EST for USIM or ADN for SIM
	Global resource dependence : none
       Author:       Eddie.Wang
  	Note:  For FDN, BDN and ACL only
*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_SetSimEFService(
                        MN_DUAL_SYS_E dual_sys,
                        MN_SIM_FILE_TYPE_E sim_ef,   //For FDN, BDN and ACL only
                        BOOLEAN is_service_enable    // TRUE: enable, FALSE: disable
                        );

/*****************************************************************************
 	Description : Check SIM EF service in EST for USIM or ADN for SIM
	Global resource dependence : none
       Author:       Eddie.Wang
  	Note:  For FDN, BDN and ACL only
*****************************************************************************/
extern MN_SIM_SERVICE_E MNSIM_GetSimEFService(
                        MN_DUAL_SYS_E dual_sys,
                        MN_SIM_FILE_TYPE_E sim_ef   //For FDN, BDN and ACL only
                        );

/*****************************************************************************
**Description : Retrive VCARD space status in curent USIM            *
**Global resource dependence : mnsim_context_g                  *
**Author:     Eddie.Wang                            *
**Note:    Currently spport ADN EXT1 SNE ANR and EMAIL                                 *
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_Get3GPhonebookStatus(
    PHONEBOOK_VCARD_STATUS_T *vcard_status
    );

/*****************************************************************************
**Description : check additonal entries' space            *
**Global resource dependence : mnsim_context_g                  *
**Author:     Eddie.Wang                            *
**Note:        for MMI b4 update add. enties                           *
*****************************************************************************/
uint8 PHONEBOOK_AppCheckAddSpace(uint8 add_preop);

/*****************************************************************************
 	Description : read  aas usim
	Global resource dependence : mnsim_context_g
       Author:       bin.li
       Modified by: Eddie.Wang
	Note:
*****************************************************************************/
uint8 PHONEBOOK_ReadUsimAas (
				    uint8 rec_num,
				    PHONEBOOK_AAS_ENTRY_T *aas_entry_ptr
				    );

/*****************************************************************************
**Description : Update ADN and all its accociated additional info. such as ANR, SNE and EMAIL	*
**Global resource dependence : mnsim_context_g									*
**Author:       Eddie.Wang														*
**Note:																		*
*****************************************************************************/
uint8 PHONEBOOK_Update3GPhonebook (
			    PHONEBOOK_ENTRY_T      			*entry_ptr,
			    BOOLEAN							anr_flag[MN_MAX_USIM_ANR_NUM],	
			    BOOLEAN							email_flag,
			    BOOLEAN                         sne_flag,   
			    BOOLEAN							adn_flag,
			    PHONEBOOK_ADDITIONAL_ENTRY_T	*additional_entry_ptr
			    );
			    
/*****************************************************************************
**Description : Delete ADN and all its accociated additional info. such as ANR, SNE and EMAIL  *
**Global resource dependence : mnsim_context_g                  *
**Author:       Eddie.Wang                            *
**Note:      Add additional info. flag on 23/06/2010. TRUE: need to delete; FALSE: dont delete due to
                the specific field is invalid  already in SIM                            *
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_Delete3GPhonebook (
        BOOLEAN             anr_flag[MNSIM_ANR_MAX_NUM],
        BOOLEAN             email_flag,
        BOOLEAN             sne_flag,  
        uint32              entry_id
        );

/*****************************************************************************
**Description : Read additional user info. such as ANR, SNE and EMAIL						*
**Global resource dependence : mnsim_context_g									*
**Author:       Eddie.Wang														*
**Note:																		*
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadUsimAdditionalEntry (
				uint32	 entry_id
				);

/*****************************************************************************
*    Description : Find out all entries in entire Phonebooks by ALPHA
*    Global resource dependence : mnsim_context_g
*    Author:       Eddie.Wang
*    Note:         Support SIM storage only
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_FindPhonebookEntryIDsByAlpha(
    PHONEBOOK_STORAGE_E     storage,        /* IN:*/
    MN_STRING_T             *alpha_ptr,     /* IN:*/
    uint16                  *entry_ids,     /* IN/OUT: entry ids*/
    uint16                  *no_entry_ids    /* IN/OUT: number of entry ids, [IN]must greater or equal to used_rec_no.*/
    );

/*****************************************************************************/
// 	Description :  If is_set_fetch_before_me_cap_chk is TRUE, Me_cap_check will be started after  
//                      current STK section end
//	Global resource dependence : sim_global_context_g
//  Author:	Eddie.Wang
//	Note:   Only for Dphone with Encrypted card
/*****************************************************************************/
extern void MNPHONE_SetFetchBeforeMeCapChk (
        BOOLEAN is_set_fetch_before_me_cap_chk
        );

/*****************************************************************************/
//  Description : Set if save SMS by PS
//  Global resource dependence : none
//  Author:       Eddie.Wang
//  Note: Set is_save_sms to FALSE means PS will not save SMS; 
//        By default SMS will be saved
/*****************************************************************************/
extern void MNSMS_SetSaveSMS(
            MN_DUAL_SYS_E      dual_sys,
            BOOLEAN      is_ps_save_sms
            );

/*****************************************************************************/
//  Description : Get Current Cell band info.
//  Global resource dependence : none
//  Author:       Eddie.Wang
//  Note:     Return the gsm band of current cell
/*****************************************************************************/
extern MN_PHONE_BAND_TYPE_E MNPHONE_GetCurrentCellBandEx(
        MN_DUAL_SYS_E dual_sys
        );

/*****************************************************************************
    Description : get ATR raw data from sim
    Global resource dependence : none
    Author: Eddie.Wang
    Note: ATR should be valid after SIM is present
*****************************************************************************/
extern MN_SIM_PARA_VALUE_T MNSIM_GetSIMATRRawData(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
/* 	Description : MNPHONE_SetGcfCalibratewithSim */
/*	Global resource dependence : is_cft_calibrate_with_sim_s */
/*  Author: Eddie.Wang */
/*	Note: Set cft_calibrate with SIM mode by user. before SIM ready  */
/*****************************************************************************/
extern BOOLEAN MNPHONE_SetGcfCalibratewithSim(void);

/*****************************************************************************
**Description : Get the file info of EFpbr            *
**Global resource dependence : mnsim_context_g                  *
**Author:       Eddie.Wang                            *
**Note:    Only AAS EMAIL GRP and ANR is supported currently               *
*****************************************************************************/
extern uint8 PHONEBOOK_GetPBRInfo (MN_DUAL_SYS_E dual_sys, uint8* rec_num, PHONEBOOK_STORAGE_E storage);

/*****************************************************************************/
/* 	Description : called by ATC to bar the cell*/
/*	Global resource dependence : none */
/*  Author: chong.liu */
/*	Note: */
/*****************************************************************************/
extern BOOLEAN em_set_barparam_ex(
                uint16 uarfcn,
                uint8 num,
                uint8 *cell_para_list_ptr
                );

/*****************************************************************************/
/* 	Description : called by ATC to get the bar cell list*/
/*	Global resource dependence : none */
/*  Author: chong.liu */
/*	Note: */
/*****************************************************************************/
extern BOOLEAN em_get_barparam_ex(
                uint8 * num,
                uint16 * uarfcn_list_ptr,
                uint8 *cell_para_list_ptr
                );

/*****************************************************************************/
/* 	Description : called by ATC or MMI, set gsm cells on which forcing camping*/
/*	Global resource dependence : none */
/*  Author: jackie.ma */
/*	Note: */
/*	Return value: FALSE: not setting for incorrect arfcn_count, 
                TRUE: set successfully */
/*****************************************************************************/
extern BOOLEAN em_set_forcecamp_gcell(
											MN_DUAL_SYS_E dual_sys,
                      uint16  arfcn_count,  //in -- 1-6
                      uint16 *arfcn_list   //in -- arfcn list
                      );

/*****************************************************************************/
/* 	Description : called by ATC or MMI, get gsm cells on which forcing camping*/
/*	Global resource dependence : g_forced_camping, g_forced_arfcn_count,g_forced_arfcn*/
/*  Author: jackie.ma */
// para: *arfcn_count:in, max number of Forced Camping cells
//									  out, actual number of Forced Camping cells
//       *arfcn_list: out, arfcn list,which is all 0 if actual number   
//                    of Forced Camping cells is 0
/*	Note: */
/*****************************************************************************/
extern void em_get_forcecamp_gcell(
											MN_DUAL_SYS_E dual_sys,
                      uint16  *arfcn_count,  
                      uint16  *arfcn_list   
                      );

/*****************************************************************************/
/* 	Description : called by ATC or MMI, cancel gsm cells on which forcing camping*/
/*	Global resource dependence : none */
/*  Author: jackie.ma */
/*	Note: */
/*****************************************************************************/
extern void em_cancel_forcecamp_gcell(MN_DUAL_SYS_E dual_sys);
            
/*****************************************************************************/
/* 	Description : get current gprs state: 1: support ,0: un_support */
/*	Global resource dependence : none */
/*  Author:chong.liu */
/*	Note: */
/*****************************************************************************/
extern BOOLEAN MNPHONE_IsCurrentGprsSupportEx(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : read nv item
//  Global resource dependence : none
//  Author:       chong.liu
//  Note:     Return the result of read 
/*****************************************************************************/
extern BOOLEAN MNPHONE_NvmRead(uint32 type , 
            uint8 item_id, 
            void** data_ptr,
            uint32  *data_size 
            );

/*****************************************************************************/
//  Description : write nv item
//  Global resource dependence : none
//  Author:       chong.liu
//  Note:     Return the result of read 
/*****************************************************************************/
extern void  MNPHONE_NvmWrite(
            uint32 type , 
            uint8 item_id, 
            void* data_ptr,
            uint32  data_size 
            );

/*****************************************************************************/
//  Description : free memory for nv read action
//  Global resource dependence : none
//  Author:       chong.liu
//  Note:     Return the result of read 
/*****************************************************************************/
extern void  MNPHONE_NvmFreeBuffer(void *data_ptr);

/*****************************************************************************/
//  Description : GetUE Band
//  Global resource dependence : none
//  Author:       Doris.Wang
//  Note:     Return the UE Band
/*****************************************************************************/
extern MN_PHONE_BAND_TYPE_E MNPHONE_GetUEBandEx(
			  MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************
    Description 
    Global resource dependence : none
    Author: 
    Note: 
*****************************************************************************/
extern BOOLEAN MNRSF_IsFeaturePhoneUseAtc(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : This is the interface function to set pa and switch control flag
//	Global resource dependence : none
//  Author:	bin.li
//	Note: 
/*****************************************************************************/
void MNPHONE_InitPBSms( void );


/*****************************************************************************/
/* 	Description : called by ATC to del the cell*/
/*	Global resource dependence : none */
/*  Author: chong.liu */
/*	Note: */
/*****************************************************************************/
extern BOOLEAN em_del_barparam_ex(
                uint16 uarfcn,
                uint8 num,
                uint8 *cell_para_list_ptr
                );

/*****************************************************************************/
// 	Description : This is the interface function to set pa and switch control flag
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
void MNPHONE_InitPBSmsEx( MN_DUAL_SYS_E dual_sys );

/*****************************************************************************/
/* 	Description : MNPHONE_StartupModem */
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note: startup modem */
/*****************************************************************************/
ERR_MNPHONE_CODE_E MNPHONE_StartupModemEx(        /* ERR_MNPHONE_NO_ERR, */
                MN_DUAL_SYS_E  dual_sys
			   );

 /*****************************************************************************/
// 	Description : app get the call mode
//	Global resource dependence : none
//  Author: michael.zhu
//	Note:
/*****************************************************************************/
extern MN_CALL_MODE_E MNCALL_GetCallModeEx( MN_DUAL_SYS_E dual_sys);


/*****************************************************************************/
// 	Description : app set the call mode
//	Global resource dependence : none
//  Author: michael.zhu
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_SetCallModeEx(
                                    MN_DUAL_SYS_E  dual_sys,
                                    MN_CALL_MODE_E call_mode
                                    );

/*****************************************************************************/
// 	Description : APP task start to read the Emergency Call codes stored in MT
//	Global resource dependence : none
//  Author: Yi.Jin
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_ReadEccEntryEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E  dual_sys,
					  uint16         index,           // In: the index for emergency call numbers in MT'
					  MN_CALLED_NUMBER_T     *called_num_ptr     // Out:The ECC
				      );
/*****************************************************************************/
// 	Description :  sim send rp ack or rp error for SmsPP
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendSmsPPDlvrRptEx (                     // return whether success or failure
                                                MN_DUAL_SYS_E       dual_sys,
                                                MN_SMS_DLVR_RPT_E       rpt_type,
                                                uint8     cause,
                                                MN_L3_MSG_UNIT_T	*tpdu_ptr
                                                );
/*****************************************************************************/
// 	Description :  send rp ack or rp error
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendSmsDlvrRpt (                      // return whether success or failure
                                                MN_SMS_RECORD_ID_T    record_id,
                                                MN_SMS_DLVR_RPT_E       rpt_type,
                                                uint8     cause,
                                                MN_L3_MSG_UNIT_T	*tpdu_ptr
                                                );
/*****************************************************************************/
// 	Description :  send rp ack or rp error
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendSmsDlvrRptEx (                      // return whether success or failure
                                                MN_DUAL_SYS_E       dual_sys,
                                                MN_SMS_RECORD_ID_T    record_id,
                                                MN_SMS_DLVR_RPT_E       rpt_type,
                                                uint8     cause,
                                                MN_L3_MSG_UNIT_T	*tpdu_ptr
                                                );
/*****************************************************************************/
// 	Description :  AT send rp ack or rp error
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendATSmsDlvrRptEx (                      // return whether success or failure
                                                MN_DUAL_SYS_E       dual_sys,
                                                MN_SMS_RECORD_ID_T    record_id,
                                                MN_SMS_DLVR_RPT_E       rpt_type,
                                                MN_L3_MSG_UNIT_T	*tpdu_ptr
                                                );
/*****************************************************************************/
// 	Description :  set the new sms deliver indication mode, invoked by AT+CNMI
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetNewSmsIndModeEx(
                                    MN_DUAL_SYS_E  dual_sys,
                                    uint8 mt_value
                                    );
/*****************************************************************************/
// 	Description :  set if PS save status report or not
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetStautsRptSaveModeEx(
                                    MN_DUAL_SYS_E  dual_sys,
                                    BOOLEAN is_save
                                    );
/*****************************************************************************/
/*  Description : set APN value for a PDP context.*/
/*  Global resource dependence : none*/
/*  Author: Pizer.Fan */
/*  Note:*/
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetPdpContextAPNWithCidEx( /* return whether success or failure    */
    MN_DUAL_SYS_E  dual_sys,
    uint8    cid,
    uint8   *apn_ptr,
    BOOLEAN  save_to_nv_flag
    );
/*****************************************************************************/
//  Description : initialize a SIM card info query request for SIM / USIM
//  Global resource dependence : none
//  Author: Doris.Wang
//  Note: multi system api
/*****************************************************************************/
SIMCHECK_GET_SIM_TYPE_CNF_T MNSIM_GetSimTypeExx(
                       MN_DUAL_SYS_E dual_sys
                       );
MN_IMSI_T MNSIM_GetUImsiExx(
                      MN_DUAL_SYS_E dual_sys
                      );
/*****************************************************************************
 	Description :  is usim or sim
	Global resource dependence : none
       Author: bin.li
	Note:
*****************************************************************************/
BOOLEAN MNSIM_Is_Usim(void);

/*****************************************************************************
 	Description :  is usim or sim
	Global resource dependence : none
       Author: bin.li
	Note:
*****************************************************************************/
BOOLEAN MNSIM_Is_UsimEx(
                      MN_DUAL_SYS_E dual_sys
                      );

/*****************************************************************************/
/*  	Description : The function handle SIMAT_CC_RSP  */
/* 	Global resource dependence : none */
/*   Author: michael.zhu */
/* 	Note: */
/*****************************************************************************/
extern void MNCall_DealMOCallControlRspEx(
                                        MN_DUAL_SYS_E   dual_sys,
                                        SIMAT_CC_RSP_T *cc_rsp     // in cc sim control respond
                                        );
/*****************************************************************************/
/* 	Description : MNPHONE_ActivePs */
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note: send out active ps signal to active PS, this function for d-phone .
       only active ps not active usim card */
/*****************************************************************************/
ERR_MNPHONE_CODE_E MNPHONE_ActivePsEx(        /* ERR_MNPHONE_NO_ERR, */
                    MN_DUAL_SYS_E   dual_sys,
                    MN_GMMREG_RAT_E  user_rat,
                    mn_protocol_stack_sub_state_enum active_ps_state
                    );
/*****************************************************************************/
// 	Description : get acquired order of DM, that's DM prefer RAT
//	Global resource dependence : none
//  Author: sunny
//	Note:
/*****************************************************************************/
 extern MN_PHONE_DM_PREFER_RAT_E MNPHONE_GetAcquiredOrderEx(
                    MN_DUAL_SYS_E  dual_sys
                    );
/*****************************************************************************/
// 	Description : get sim state
//	Global resource dependence : none
//  Author: sunny
//	Note:
/*****************************************************************************/
extern MN_PHONE_SIM_STATE_E MNPHONE_GetSimStateEx(
			  MN_DUAL_SYS_E  dual_sys
              );
/*****************************************************************************/
// 	Description : get system mode
//	Global resource dependence : none
//  Author: sunny
//	Note:
/*****************************************************************************/
 extern MN_PHONE_RAT_E MNPHONE_GetSystemRATEx(
			  MN_DUAL_SYS_E  dual_sys
              );
/*****************************************************************************/
// 	Description : get system mode
//	Global resource dependence : none
//  Author: sunny
//	Note:
/*****************************************************************************/
 extern MN_PHONE_SYSTEM_MODE_E MNPHONE_GetSystemMode( void);
/*****************************************************************************/
// 	Description : get system mode
//	Global resource dependence : none
//  Author: sunny
//	Note:
/*****************************************************************************/
 extern MN_PHONE_SYSTEM_MODE_E MNPHONE_GetSystemModeEx(
			  MN_DUAL_SYS_E  dual_sys
              );
/*****************************************************************************/
// 	Description : get service status
//	Global resource dependence : none
//  Author: sunny
//	Note:
/*****************************************************************************/
extern MN_PHONE_SERVICE_STATUS_E MNPHONE_GetServiceStatusEx(
			  MN_DUAL_SYS_E  dual_sys
              );

/*****************************************************************************/
// 	Description : get roaming status
//	Global resource dependence : none
//  Author:sunny
//	Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetRoamingStatusEx(
			  MN_DUAL_SYS_E  dual_sys
              );
/*****************************************************************************/
// 	Description : get service domain
//	Global resource dependence : none
//  Author: sunny
//	Note:
/*****************************************************************************/
extern MN_PHONE_USER_SERVICE_SELECT_E  MNPHONE_GetServiceDomainEx(
			  MN_DUAL_SYS_E  dual_sys
              );
/*****************************************************************************/
// 	Description : set preferred RAT in certain condition
//	Global resource dependence : none
//  Author:  Sunny.Shen
//	Note:
//  History:
/*****************************************************************************/
 extern ERR_MNPHONE_CODE_E MNPHONE_SetRATEx(
                    MN_DUAL_SYS_E  dual_sys,
                    MN_PHONE_RAT_INFO_T rat_info,
                    BOOLEAN is_temp_rat,/*use only once temporarily without writing to NVM*/
                    BOOLEAN only_write_nv/*only write to NVM without searching plmn*/
                    );
/*****************************************************************************/
/* 	Description : get real current RAT */
/*	Global resource dependence : none */
/*  Author: sunny */
/*	Note: */
/*****************************************************************************/

extern MN_GMMREG_RAT_E MNPHONE_GetCurrentRATEx(
                    MN_DUAL_SYS_E  dual_sys
                    );
/*****************************************************************************/
extern MN_GMMREG_RAT_E MNPHONE_GetConfiguredRATEx(
               MN_DUAL_SYS_E  dual_sys
               );
/*****************************************************************************/
// 	Description : get configured DM prefer RAT
//	Global resource dependence : none
//  Author: sunny
//	Note:
/*****************************************************************************/
extern MN_GMMREG_RAT_E MNPHONE_GetConfiguredDMPreferRATEx(
			   MN_DUAL_SYS_E  dual_sys
               );

/*****************************************************************************/
/* 	Description : Read PLMN from sim according to plmn list type(HPLMN,UPLMN,OPLMN) */
/*	Global resource dependence : none */
/*  Author: sunny */
/*	Note: */
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_ReadPLMNfromSimEx(        /* ERR_MNPHONE_NO_ERR, */
              MN_DUAL_SYS_E             dual_sys,
              MN_PHONE_PLMN_LIST_TYPE_E      plmn_list_type  /* plmn list type(HPLMN,UPLMN,OPLMN) */
              );
/*****************************************************************************/
/* 	Description :Write PLMN to sim according to plmn list type(HPLMN,UPLMN,OPLMN) */
/*	Global resource dependence : none */
/*  Author: sunny */
/*	Note: */
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_WritePLMNtoSimEx(        /* ERR_MNPHONE_NO_ERR, */
              MN_DUAL_SYS_E             dual_sys,
              MN_PHONE_WRITE_PLMN_T     write_plmn       /* specified PLMN item */
              );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetSimtype(
                SIMCHECK_GET_SIM_TYPE_CNF_T *sim_type_ptr
                );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetSimtypeEx(
                MN_DUAL_SYS_E dual_sys,
                SIMCHECK_GET_SIM_TYPE_CNF_T *sim_type_ptr
                );
/*****************************************************************************/
// 	Description : l this function to do sim restrict function
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetImsi(
                MN_IMSI_T *imsi_ptr
                );
/*****************************************************************************/
// 	Description : l this function to do sim restrict function
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetImsiEx(
                MN_DUAL_SYS_E dual_sys,
                MN_IMSI_T *imsi_ptr
                );
/*****************************************************************************/
/* 	Description : if plmn is selected and to register , call the function abort it */
/*	Global resource dependence : none */
/*  Author:      sunny*/
/*	Note: */
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_AbortPLMNRegnEx( /* ERR_MNPHONE_NO_ERR */
              MN_DUAL_SYS_E    dual_sys
              );
/*****************************************************************************/
/*  Description : get the axtive pdp context's dl rate, if no active pdpcontext, return null*/
/*  Global resource dependence : none*/
/*  Author:  michael.zhu*/
/*  Note:*/
/*****************************************************************************/
extern MNGPRS_3G_EXTEND_QOS_SET_T MNGPRS_GetActivePdpExtendQosEx(
                            MN_DUAL_SYS_E    dual_sys
                            );
/*************************************************************************/
//    Description: get negoiated QOS of pdp context
//    Author: Shaohua Xia
//    Note:
/*************************************************************************/
ERR_MNGPRS_CODE_E MNGPRS_ReadExtNegQosEx(
                         MN_DUAL_SYS_E      dual_sys,
                         MN_GPRS_EXT_QOS_T *ext_qos);

/*****************************************************************************/
// 	Description : This is the interface for MMI to get the flage,
//                for whether do the interration of cf when the ue powers on.
//	Global resource dependence : none
//  Author:	Michael.Zhu
//	Note:
/*****************************************************************************/
extern BOOLEAN  MNSS_GetSsFlagAtPowerOnEx(
                                        MN_DUAL_SYS_E  dual_sys
                                        );
/*****************************************************************************/
// 	Description : This is the interface for MMI to set the flage,
//                for whether do the interration of cf when the ue powers on.
//	Global resource dependence : none
//  Author:	Michael.Zhu
//	Note:
/*****************************************************************************/
extern void  MNSS_SetSsFlagAtPowerOnEx(
                              MN_DUAL_SYS_E  dual_sys,
                              BOOLEAN  flag
                              );
/*****************************************************************************/
// 	Description : Get UE Identy Information
//	Global resource dependence : none
//  Author:  Michael.Zhu
//	Note:
//  History:
/*****************************************************************************/
extern MN_UE_ID_INFO_T  MNPHONE_GetUeIdInfoEx(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************
 	Description :  atc_read_uplmn_wact_file
	Global resource dependence : mn_sim_uplmn_wact_g
       Author:       bin.li
	Note:
*****************************************************************************/
void atc_read_uplmn_wact_fileEx(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************
 	Description :  atc_get_uplmn_wact_file
	Global resource dependence : mn_sim_uplmn_wact_g
       Author:       bin.li
	Note:
*****************************************************************************/
void atc_get_uplmn_wact_fileEx(
                MN_DUAL_SYS_E dual_sys,
                MN_ATC_UPLMN_WACT_T *uplmn_ptr
                );

/*****************************************************************************
 	Description :  atc_write_uplmn_wact_file
	Global resource dependence : mn_sim_uplmn_wact_g
       Author:       bin.li
	Note:
*****************************************************************************/
BOOLEAN atc_write_uplmn_wact_fileEx(
              MN_DUAL_SYS_E dual_sys,
              uint8 index,
              uint8 *plmn_id,
              mn_sim_access_type_enum access_type);

/*****************************************************************************/
/*  	Description : set the service type for MO SMS */
/* 	Global resource dependence : none */
/*   Author: George.Yuan  */
/* 	Note:	used by APP layer, this is called by ATC */
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetServiceForMOSmsEx (
                        MN_DUAL_SYS_E  dual_sys,
                        uint8 service_type
                        );
/*****************************************************************************/
/*  	Description : get the service type for MO SMS */
/* 	Global resource dependence : none */
/*   Author: michael.zhu  */
/* 	Note:	used by APP layer, this is called by ATC */
/*****************************************************************************/
extern MN_SMS_SERVICE_TYPE_E MNSMS_GetServiceForMOSmsEx (
                        MN_DUAL_SYS_E  dual_sys
                        );
/*****************************************************************************/
// 	Description : l this function to get apn info
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern MNSIM_RESULT_E MN_GetAPNInfoEx(
                                        MN_DUAL_SYS_E  dual_sys,
                                        MN_APN_TYPE_E    apn_type,   /* in */
                                        BOOLEAN  *is_display_rplmn,  /* out */
                                        MN_APN_NAME_CODE_SCHEM_E	  *apn_code_schem,	// the supplementary service used on which type service(bearer or tele or all call type)
                                        uint8 *spn_name
                                        );
/*****************************************************************************/
// 	Description : l this function to get spn info
//	Global resource dependence : none
//  Author:	jun.zhang
//	Note:
/*****************************************************************************/
extern MNSIM_RESULT_E MN_GetSPNInfoEx(
                                        MN_DUAL_SYS_E  dual_sys,
                                        MN_SPN_TYPE_E    spn_type,   /* in */
                                        BOOLEAN  *is_display_rplmn,  /* out */
                                        MN_SPN_NAME_CODE_SCHEM_E	  *spn_code_schem,	// the supplementary service used on which type service(bearer or tele or all call type)
                                        uint8 *spn_name
                                        );
/*****************************************************************************/
// 	Description : l this function to do sim restrict function
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern MNSIM_RESULT_E MN_SIM_CRSMEx(
                        MN_DUAL_SYS_E  dual_sys,
						MN_CRSM_COMMAND_TYPE_E command,
						uint8 p1,
						uint8 p2,
						uint8 p3,
						uint16 file_id,
						uint8 *pdata,
						uint8 path_len,
						uint8 *path_id);
/*****************************************************************************
**Description : Retrive VCARD space status in curent USIM            *
**Global resource dependence : mnsim_context_g                  *
**Author:     Eddie.Wang                            *
**Note:    Currently spport ADN EXT1 SNE ANR and EMAIL                                 *
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_Get3GPhonebookStatusEx(
                        MN_DUAL_SYS_E  dual_sys,
                        PHONEBOOK_VCARD_STATUS_T *vcard_status
                        );

/*****************************************************************************
**Description : check additonal entries' space            *
**Global resource dependence : mnsim_context_g                  *
**Author:     Eddie.Wang                            *
**Note:        for MMI b4 update add. enties                           *
*****************************************************************************/
uint8 PHONEBOOK_AppCheckAddSpaceEx(
                        MN_DUAL_SYS_E  dual_sys,
                        uint8 add_preop);
/*****************************************************************************
 	Description : read  aas usim
	Global resource dependence : mnsim_context_g
       Author:       bin.li
       Modified by: Eddie.Wang
	Note:
*****************************************************************************/
uint8 PHONEBOOK_ReadUsimAasEx (
                    MN_DUAL_SYS_E  dual_sys,
				    uint8 rec_num,
				    PHONEBOOK_AAS_ENTRY_T *aas_entry_ptr
				    );

/*****************************************************************************
**Description : Update ADN and all its accociated additional info. such as ANR, SNE and EMAIL	*
**Global resource dependence : mnsim_context_g									*
**Author:       Eddie.Wang														*
**Note:																		*
*****************************************************************************/
uint8 PHONEBOOK_Update3GPhonebookEx (
                MN_DUAL_SYS_E                   dual_sys,
			    PHONEBOOK_ENTRY_T      			*entry_ptr,
			    BOOLEAN							anr_flag[MN_MAX_USIM_ANR_NUM],
			    BOOLEAN							email_flag,
			    BOOLEAN                         sne_flag,
			    BOOLEAN							adn_flag,
			    PHONEBOOK_ADDITIONAL_ENTRY_T	*additional_entry_ptr
			    );
/*****************************************************************************
**Description : Delete ADN and all its accociated additional info. such as ANR, SNE and EMAIL  *
**Global resource dependence : mnsim_context_g                  *
**Author:       Eddie.Wang                            *
**Note:      Add additional info. flag on 23/06/2010. TRUE: need to delete; FALSE: dont delete due to
                the specific field is invalid  already in SIM                            *
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_Delete3GPhonebookEx (
        MN_DUAL_SYS_E       dual_sys,
        BOOLEAN             anr_flag[MNSIM_ANR_MAX_NUM],
        BOOLEAN             email_flag,
        BOOLEAN             sne_flag,
        uint32              entry_id
        );
/*****************************************************************************
**Description : Read additional user info. such as ANR, SNE and EMAIL						*
**Global resource dependence : mnsim_context_g									*
**Author:       Eddie.Wang														*
**Note:																		*
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadUsimAdditionalEntryEx (
                MN_DUAL_SYS_E   dual_sys,
				uint32	        entry_id
				);
/*****************************************************************************
*    Description : Find out all entries in entire Phonebooks by ALPHA
*    Global resource dependence : mnsim_context_g
*    Author:       Eddie.Wang
*    Note:         Support SIM storage only
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_FindPhonebookEntryIDsByAlphaEx(
    MN_DUAL_SYS_E           dual_sys,
    PHONEBOOK_STORAGE_E     storage,        /* IN:*/
    MN_STRING_T             *alpha_ptr,     /* IN:*/
    uint16                  *entry_ids,     /* IN/OUT: entry ids*/
    uint16                  *no_entry_ids    /* IN/OUT: number of entry ids, [IN]must greater or equal to used_rec_no.*/
    );
/*****************************************************************************/
// 	Description : l this function to set ext stk terminal profile data
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern void MNPHONE_SetExtProfileEx (
      MN_DUAL_SYS_E  dual_sys,
      BOOLEAN is_valid,
      uint16 ext_data_len,
      uint8* ext_data_ptr
);
/*****************************************************************************/
// 	Description : l this function to read auth relate file from sim
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern void MNPHONE_TriggerReadAuthFileEx (
      MN_DUAL_SYS_E dual_sys
);


/*****************************************************************************/
/* 	Description : called by ATC or MMI, get RRDM Parameters*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
/*+CR193547*/
BOOLEAN em_get_dm_param(uint32 index, uint32 *value_ptr);
/*+CR193547*/

/*****************************************************************************/
/* 	Description : called by ATC or MMI, get RRDM Parameters*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
BOOLEAN em_get_dm_paramEx(int32 multi_sys,uint32 index, uint32 *value_ptr);


/*****************************************************************************/
/* 	Description : called by ATC or MMI, set RRDM Parameters*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
BOOLEAN em_set_dm_param(uint32 index, uint32 value);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, set RRDM Parameters*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
BOOLEAN em_set_dm_paramEx(int32 multi_sys,uint32 index, uint32 value);

/********************************************************************
//   Function Name: em_reset_dm_param
//  Description: Reset DM Test Parameters
// author: Martin.Jiang
********************************************************************/
void em_reset_dm_param(void);

/********************************************************************
//   Function Name: em_reset_dm_param
//  Description: Reset DM Test Parameters
// author: Martin.Jiang
********************************************************************/
void em_reset_dm_paramEx(int32 multi_sys);


/*****************************************************************************/
// 	Description : l this function to do sim restrict function
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNSIM_RunMbbmsAlgoEx(
                        MN_DUAL_SYS_E   dual_sys,
                        uint8       *rand_ptr, /*LV格式,第一个字节为len*/
                        uint8       *autn_ptr  /*LV格式,第一个字节为len*/
                        );

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
void MNPHONE_SetTerminalProfileBeforeRefreshEx (
        MN_DUAL_SYS_E   dual_sys,
        BOOLEAN is_need_terminal_profile
        );
/*****************************************************************************/
// 	Description :  if is_set_uicc_sim is TRUE, sim task detect the uicc as sim card
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
extern void MNPHONE_SetUiccAsSimCardEx (
        MN_DUAL_SYS_E   dual_sys,
        BOOLEAN is_set_uicc_sim
        );

extern void MNPHONE_JudgeWorkMode(void);
/*****************************************************************************/
// 	Description :  If is_set_fetch_before_me_cap_chk is TRUE, Me_cap_check will be started after
//                      current STK section end
//	Global resource dependence : sim_global_context_g
//  Author:	Eddie.Wang
//	Note:   Only for Dphone with Encrypted card
/*****************************************************************************/
extern void MNPHONE_SetFetchBeforeMeCapChkEx (
        MN_DUAL_SYS_E   dual_sys,
        BOOLEAN is_set_fetch_before_me_cap_chk
        );


/*****************************************************************************/
/* 	Description : called by ATC or MMI, reset gsm 2 td revert status*/
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*****************************************************************************/
extern void em_reset_gsm2td_revert_status(void);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, reset gsm 2 td revert status*/
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*****************************************************************************/
extern void em_reset_gsm2td_revert_statusEx(int32 multi_sys);


/*****************************************************************************/
/* 	Description : called by ATC or MMI, get gsm 2 td revert status according to*/
/*                 the dm ho type */
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*	Return value: 0: none GSM 2 TD procedure,
                   1: GSM 2 TD failure, revert is successful
                   2: GSM 2 TD success, revert did not happen*/
/*****************************************************************************/
extern uint8 em_get_gsm2td_revert_status(
            uint8 type // 0:CS HO, 1:PS HO, 2: CELL RESEL, 3: FAST RETURN
            );

/*****************************************************************************/
/* 	Description : called by ATC or MMI, get gsm 2 td revert status according to*/
/*                 the dm ho type */
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*	Return value: 0: none GSM 2 TD procedure,
                   1: GSM 2 TD failure, revert is successful
                   2: GSM 2 TD success, revert did not happen*/
/*****************************************************************************/
extern uint8 em_get_gsm2td_revert_statusEx(
            int32 multi_sys,
            uint8 type // 0:CS HO, 1:PS HO, 2: CELL RESEL, 3: FAST RETURN
            );

/*****************************************************************************/
/* 	Description : called by ATC or MMI, reset td 2 gsm revert status*/
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*****************************************************************************/
extern void em_reset_td2gsm_revert_status(void);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, reset td 2 gsm revert status*/
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*****************************************************************************/
extern void em_reset_td2gsm_revert_statusEx(int32 multisys);


/*****************************************************************************/
/* 	Description : called by ATC or MMI, get td 2 gsm revert status according to*/
/*                 the dm ho type */
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*	Return value: */
/*****************************************************************************/
extern uint8 em_get_td2gsm_revert_status(
            uint8 type // 0:CS HO, 1:PS HO, 2: CELL RESEL, 3: FAST RETURN
            );

/*****************************************************************************/
/* 	Description : called by ATC or MMI, get td 2 gsm revert status according to*/
/*                 the dm ho type */
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*	Return value: */
/*****************************************************************************/
extern uint8 em_get_td2gsm_revert_statusEx(
            int32 multisys,
            uint8 type // 0:CS HO, 1:PS HO, 2: CELL RESEL, 3: FAST RETURN
            );
/*****************************************************************************/
//  Description : get the activate_ps_g[] value 
//  Global resource dependence : activate_ps_g
//  Author:       baoning.shan
//  Note:
/*****************************************************************************/
extern BOOLEAN MN_PHONE_Is_Active_Ps(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
/* 	Description : MNPHONE_StartupModem */
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note: startup modem */
/*****************************************************************************/
ERR_MNPHONE_CODE_E MNPHONE_StartupModem(        /* ERR_MNPHONE_NO_ERR, */
                void
			   );

/*****************************************************************************/
/* 	Description : mn_set_nv_flightmode_flag */
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note:  */
/*****************************************************************************/
void mn_set_nv_flightmode_flag(
                BOOLEAN is_flight_mode
                );

/*****************************************************************************/
/* 	Description : mn_get_nv_flightmode_flag */
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note:  */
/*****************************************************************************/
BOOLEAN mn_get_nv_flightmode_flag(
               void
               );

/*****************************************************************************/
/* 	Description : MNPHONE_ActivePs */
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note: send out active ps signal to active PS, this function for d-phone .
       only active ps not active usim card */
/*****************************************************************************/
ERR_MNPHONE_CODE_E MNPHONE_ActivePs(        /* ERR_MNPHONE_NO_ERR, */
                    MN_GMMREG_RAT_E  user_rat,
                    mn_protocol_stack_sub_state_enum active_ps_state
                    );

/*****************************************************************************/
// 	Description : get acquired order of DM, that's DM prefer RAT 
//	Global resource dependence : none 
//  Author: sunny 
//	Note: 
/*****************************************************************************/
 extern MN_PHONE_DM_PREFER_RAT_E MNPHONE_GetAcquiredOrder(
                    void
                    );

/*****************************************************************************/
// 	Description : get sim state 
//	Global resource dependence : none 
//  Author: sunny 
//	Note: 
/*****************************************************************************/
extern MN_PHONE_SIM_STATE_E MNPHONE_GetSimState(
			  void
              );

/*****************************************************************************/
// 	Description : get system mode
//	Global resource dependence : none 
//  Author: sunny
//	Note: 
/*****************************************************************************/
 extern MN_PHONE_RAT_E MNPHONE_GetSystemRAT(
			  void
              );

/*****************************************************************************/
// 	Description : get service status 
//	Global resource dependence : none 
//  Author: sunny 
//	Note: 
/*****************************************************************************/
extern MN_PHONE_SERVICE_STATUS_E MNPHONE_GetServiceStatus(
			  void
              );

/*****************************************************************************/
// 	Description : get roaming status 
//	Global resource dependence : none 
//  Author:sunny
//	Note: 
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetRoamingStatus( 
			  void
              );

/*****************************************************************************/
// 	Description : get service domain 
//	Global resource dependence : none
//  Author: sunny 
//	Note: 
/*****************************************************************************/
extern MN_PHONE_USER_SERVICE_SELECT_E  MNPHONE_GetServiceDomain(
			  void
              );

/*****************************************************************************/
// 	Description : set preferred RAT in certain condition
//	Global resource dependence : none
//  Author:  Sunny.Shen
//	Note:
//  History:
/*****************************************************************************/
 extern ERR_MNPHONE_CODE_E MNPHONE_SetRAT(
                    MN_PHONE_RAT_INFO_T rat_info,
                    BOOLEAN is_temp_rat,/*use only once temporarily without writing to NVM*/
                    BOOLEAN only_write_nv/*only write to NVM without searching plmn*/
                    );

/*****************************************************************************/
// 	Description : get configured RAT 
//	Global resource dependence : none 
//  Author: sunny 
//	Note: 
/*****************************************************************************/
extern MN_GMMREG_RAT_E MNPHONE_GetConfiguredRAT( 
               void
               );

/*****************************************************************************/
// 	Description : get configured DM prefer RAT
//	Global resource dependence : none
//  Author: sunny 
//	Note: 
/*****************************************************************************/
extern MN_GMMREG_RAT_E MNPHONE_GetConfiguredDMPreferRAT(
			   void
               );

/*****************************************************************************/
// 	Description : write_nv_stk_profile
//	Global resource dependence : none
//  Author:       bin.li
//	Note:
/*****************************************************************************/

extern void write_nv_stk_profile(int32 multi_sys,/*NCR105552*/uint8 *profile, uint32 len);

/*****************************************************************************/
// 	Description : support_atc_stk
//	Global resource dependence : none
//  Author:       bin.li
//	Note:
/*****************************************************************************/
extern MN_ATC_STK_MODE_E support_atc_stk(void);
    
/*****************************************************************************/
// 	Description : MN_GetSupportGsmSimStat
//	Global resource dependence : none
//  Author:       bin.li
//	Note:
/*****************************************************************************/        
extern BOOLEAN  MN_GetSupportGsmSimStat(int32 multi_sys/*NCR105552*/);

/*****************************************************************************/
// 	Description : MN_SetSupportGsmSimStat
//	Global resource dependence : none
//  Author:       bin.li
//	Note:
/*****************************************************************************/
extern void MN_SetSupportGsmSimStat(int32 multi_sys,/*NCR105552*/BOOLEAN is_support_gsmsim);

/*****************************************************************************/
/* 	Description : Read PLMN from sim according to plmn list type(HPLMN,UPLMN,OPLMN) */
/*	Global resource dependence : none */
/*  Author: sunny */
/*	Note: */
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_ReadPLMNfromSim(        /* ERR_MNPHONE_NO_ERR, */
              MN_PHONE_PLMN_LIST_TYPE_E      plmn_list_type  /* plmn list type(HPLMN,UPLMN,OPLMN) */
              );

/*****************************************************************************/
/* 	Description :Write PLMN to sim according to plmn list type(HPLMN,UPLMN,OPLMN) */ 
/*	Global resource dependence : none */
/*  Author: sunny */
/*	Note: */
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_WritePLMNtoSim(        /* ERR_MNPHONE_NO_ERR, */
              MN_PHONE_WRITE_PLMN_T     write_plmn       /* specified PLMN item */
              );

/*****************************************************************************/
/* 	Description : if plmn is selected and to register , call the function abort it */
/*	Global resource dependence : none */
/*  Author:      sunny*/
/*	Note: */
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_AbortPLMNRegn( /* ERR_MNPHONE_NO_ERR */
              void
              );

/*****************************************************************************/
/* 	Description : MNPHONE_GetGprsAttachMode while power on */
/*	Global resource dependence : none */
/*  Author: sunny */
/*	Note: */
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetGprsAttachMode(
                       void
                       );

/*****************************************************************************/
/* 	Description : called by dynamic eng of atc, getting integrity enable flag in nv*/
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note: */
/*****************************************************************************/
uint8 em_atc_get_integrity_enable_flag(void);

/*****************************************************************************/
/* 	Description : called by dynamic eng of atc, getting fta_opt enable flag in nv*/
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note: */
/*****************************************************************************/
void em_atc_set_integrity_enable_flag(uint8  integrity_enable_flag );

/* +cr221587 */
#if 0
/*****************************************************************************/
/* 	Description : called by dynamic eng of atc, getting real_nw_opt enable flag in nv*/
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note: */
/*****************************************************************************/
uint32 em_atc_get_real_nw_opt_enable_flag(void);

/*****************************************************************************/
/* 	Description : called by dynamic eng of atc, setting real_nw_opt enable flag in nv*/
/*	Global resource dependence : none */
/*  Author: bin.li */
/*	Note: */
/*****************************************************************************/
void em_atc_set_real_nw_opt_enable_flag(uint32 real_nw_opt_enable_flag);
#endif
/* -cr221587 */

/*****************************************************************************
 	Description :  atc_read_uplmn_wact_file
	Global resource dependence : mn_sim_uplmn_wact_g
       Author:       bin.li
	Note:
*****************************************************************************/
void atc_read_uplmn_wact_file( void );

/*****************************************************************************
 	Description :  atc_get_uplmn_wact_file
	Global resource dependence : mn_sim_uplmn_wact_g
       Author:       bin.li
	Note:
*****************************************************************************/
void atc_get_uplmn_wact_file(MN_ATC_UPLMN_WACT_T *uplmn_ptr);

/*****************************************************************************
 	Description :  atc_write_uplmn_wact_file
	Global resource dependence : mn_sim_uplmn_wact_g
       Author:       bin.li
	Note:
*****************************************************************************/
BOOLEAN atc_write_uplmn_wact_file(
              uint8 index, 
              uint8 *plmn_id, 
              mn_sim_access_type_enum access_type);

/*****************************************************************************/
/*  Description : read dedicated APN value of ps dial .*/
/*  Global resource dependence : none*/
/*  Author:sunny*/
/*  Note:*/
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_GetDailAPN( /* return whether success or failure */
                MN_GPRS_PDP_APN_T * ret_apn_ptr      // out, apn array of ps dial
        );

/*****************************************************************************/
// 	Description : This is the interface for MMI to get the flage, 
//                for whether do the interration of cf when the ue powers on.
//	Global resource dependence : none
//  Author:	Michael.Zhu
//	Note: 
/*****************************************************************************/
extern BOOLEAN  MNSS_GetSsFlagAtPowerOn( void );

/*****************************************************************************/
// 	Description : This is the interface for MMI to set the flage, 
//                for whether do the interration of cf when the ue powers on.
//	Global resource dependence : none
//  Author:	Michael.Zhu
//	Note: 
/*****************************************************************************/
extern void  MNSS_SetSsFlagAtPowerOn(
                              BOOLEAN flag
                              );

/*****************************************************************************/
// 	Description : Get Locked Freq And Cell
//	Global resource dependence : none
//  Author:  Michael.Zhu
//	Note:
//  History:
/*****************************************************************************/
extern MN_EM_LOCK_FREQ_CELL_T  MNPHONE_GetLockedFreqAndCell( void );

/*****************************************************************************/
// 	Description : Set Locked Freq And Cell
//	Global resource dependence : none
//  Author:  Michael.Zhu
//	Note: need to restart ue
//  History:
/*****************************************************************************/
extern void  MNPHONE_SetLockedFreqAndCell(MN_EM_LOCK_FREQ_CELL_T set_value);

/*****************************************************************************/
// 	Description : Unlocked Freq And Cell
//	Global resource dependence : none
//  Author:  Michael.Zhu
//	Note: need to restart ue
//  History:
/*****************************************************************************/
extern void  MNPHONE_UnlockedFreqAndCell( void );

/*****************************************************************************/
// 	Description : Get UE Identy Information
//	Global resource dependence : none
//  Author:  Michael.Zhu
//	Note:
//  History:
/*****************************************************************************/
extern MN_UE_ID_INFO_T  MNPHONE_GetUeIdInfo( void );

/*****************************************************************************
 	Description :  nvm_set_sim_cfg
	Global resource dependence : stack_cfg_ptr_g
    Author:  joe.ni
	Note:
*****************************************************************************/
void nvm_set_sim_cfg( int32 multi_sys/*NCR105552*/ );

/*****************************************************************************
 	Description :  nvm_set_em_cfg
	Global resource dependence : stack_cfg_ptr_g
    Author:  joe.ni
	Note:
*****************************************************************************/
void nvm_set_em_cfg( void );

/*****************************************************************************
 	Description :  nvm_set_dm_gsm_cfg
	Global resource dependence : stack_cfg_ptr_g
    Author:  joe.ni
	Note:
*****************************************************************************/
void nvm_set_dm_gsm_cfg( void );

/*****************************************************************************
 	Description :  nvm_set_dm_td_cfg
	Global resource dependence : stack_cfg_ptr_g
    Author:  joe.ni
	Note:
*****************************************************************************/
void nvm_set_dm_td_cfg( void );

/*****************************************************************************/
// 	Description : Get Access Stratrum Rel Ind
//	Global resource dependence : none
//  Author:  Michael.Zhu
//	Note:
//  History:
/*****************************************************************************/
extern MN_ACCESS_STRATUM_REL_IND_E  MNPHONE_GetAccessStratrumRelInd( void );

/*****************************************************************************/
// 	Description : Set Access Stratrum Rel Ind
//	Global resource dependence : none
//  Author:  Michael.Zhu
//	Note:
//  History:
/*****************************************************************************/
extern void  MNPHONE_SetAccessStratrumRelInd(
                                    MN_ACCESS_STRATUM_REL_IND_E rel_ind
                                    );

/*****************************************************************************/
// 	Description : This is the interface function to get whether open DSP High Speed Print
//	Global resource dependence : none
//  Author:	Michael.Zhu
//	Note: 
/*****************************************************************************/
extern BOOLEAN  MNPHONE_GetDspHighSpeedPrint( void );

/*****************************************************************************/
// 	Description : This is the interface function to set whether open DSP High Speed Print
//	Global resource dependence : none
//  Author:	Michael.Zhu
//	Note: 
/*****************************************************************************/
extern void  MNPHONE_SetDspHighSpeedPrint(BOOLEAN flag);

/*****************************************************************************/
/* 	Description : get real current RAT */
/*	Global resource dependence : none */
/*  Author: sunny */
/*	Note: */
/*****************************************************************************/
extern MN_GMMREG_RAT_E MNPHONE_GetCurrentRAT( 
                    void
                    );

/*****************************************************************************/
/*  	Description : set the service type for MO SMS */
/* 	Global resource dependence : none */
/*   Author: George.Yuan  */
/* 	Note:	used by APP layer, this is called by ATC */
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetServiceForMOSms (                      /*  return whether success or failure */
                        uint8 service_type
                        );

/*****************************************************************************/
// 	Description : app get the call mode
//	Global resource dependence : none
//  Author: michael.zhu
//	Note:
/*****************************************************************************/
extern MN_CALL_MODE_E MNCALL_GetCallMode( void );

/*+michael for tianma atc*/
/*****************************************************************************/
// 	Description : app set the call mode
//	Global resource dependence : none
//  Author: michael.zhu
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_SetCallMode(
                                    MN_CALL_MODE_E call_mode
                                    );

/*****************************************************************************/
// 	Description : get the bearer service type
//	Global resource dependence : none
//  Author: michael.zhu
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_GetBSType(
                                    uint8 *speed,
                                    uint8 *name,
                                    uint8 *ce);
                                    
/*****************************************************************************/
//  Description : set the bearer service type
//  Global resource dependence : none
//  Author: michael.zhu
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_SetBSType(
                                    uint8 speed,
                                    uint8 name,
                                    uint8 ce);
 
/******************************************************************************
 **                        CSVT MODULE API                                    *
 ******************************************************************************/

/*************************************************************************/
  /*    Description: create a mux link, and register callback function of data writing
  **    Global resource dependence : 
  **    Author: michael.zhu
  **    Note: */
/*************************************************************************/
extern MN_RETURN_RESULT_E MNCSVT_Set_Mux_Link(uint8 call_id, uint8 link_id);

/*************************************************************************/
  /*    Description: destroy a mux link, and close csd data transfering
  **    Global resource dependence : 
  **    Author: michael.zhu
  **    Note: */
/*************************************************************************/
extern MN_RETURN_RESULT_E MNCSVT_Destroy_Mux_Link(uint8 call_id);

/*****************************************************************************/
//  Description : APP task start a MO call.only one call is established at one 
//                time. 
//  Global resource dependence : none
//  Author: jun.zhang
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_StartCallWithoutControlEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E          dual_sys,
                      MN_CALL_TYPE_E         call_type,           // The call type:i.e"normal call,emergency call'
                      MN_CALLED_NUMBER_T     *called_num_ptr,     // The called tel number
                      MN_SUBADDR_T           *called_subaddr_ptr, // The called sub address
                      MN_CALL_CLIR_E         clir_type,           // i.e(CLIR_SUPPRESSION, CLIR_INVOCATION) 
                      MN_CALL_MINI_UTU_T     *utu_ptr             // if the user-to-user is not present 
                                                                  // then set the user_to_user_ptr be NULL
                      );

/*****************************************************************************/
//  Description : APP originate an MO short message, send out SMS 
//  Global resource dependence : none
//  Author: jun.zhang
//  Note:   used by APP layer
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_AppSendSmsWithoutControlEx(  //RETURN:
                                    MN_DUAL_SYS_E       dual_sys,   //IN:
                                    MN_SMS_MO_SMS_T     *sms_ptr,   //IN: the MO SMS structure pointer
                                    MN_SMS_STORAGE_E    sms_write,  //IN: the destination storage, ME or SIM
                                    MN_SMS_PATH_E       sms_path,   //IN: the SMS path:GSM or GPRS
                                    BOOLEAN             is_more     //IN: is exist more SMS
                                    );

/*****************************************************************************/
//  Description : get reponse mode set by MNGPRS_SetResMode
//  Author      : jun.zhang
//  Note        : 
/*****************************************************************************/
extern MN_GPRS_TE_RESPONSE_MODE_E  MNGPRS_GetResMode(void);
       
/*****************************************************************************/
/*  Description : read PDP address, DNS1, DNS2 and nsapi for a PDP context.*/
/*  Global resource dependence : none*/
/*  Author:jun.zhang */
/*  Note:*/
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadPdpParametersEx( 
            MN_DUAL_SYS_E      dual_sys,
            uint8              pdp_id, 
            uint32             *pdp_address,
            uint32             *pri_dns,
            uint32             *sec_dns,
            uint8              *nsapi
            );

/*****************************************************************************/
/*  	Description : The function handle SIMAT_CC_RSP  */
/* 	Global resource dependence : none */
/*   Author: michael.zhu */
/* 	Note: */
/*****************************************************************************/
extern void MNCall_DealMOCallControlRsp(
                                        SIMAT_CC_RSP_T *cc_rsp     // in cc sim control respond
                                        );

/*****************************************************************************/
/* 	Description : called by ATC or MMI to get protocol stack engineer information */
/*	Global resource dependence : none */
/*  Author: jun.zhang */
/*	Note: */
/*****************************************************************************/
extern EM_PROTOCOL_STACK_INFO_T* em_get_protocol_stack_info(
                      MN_DUAL_SYS_E        dual_sys
                      );

/*****************************************************************************/
//  Description : Get pin1 pin2 puk1 puk2 remains timer
//  Global resource dependence : none
//  Author:       bin.li
//  Note
/*****************************************************************************/
extern void MNRSF_GetPinRemains(
            MN_DUAL_SYS_E      dual_sys,
            uint8       *pin1_remains,
            uint8       *pin2_remains,
            uint8       *puk1_remains,
            uint8       *puk2_remains
            );

/*************************************************************************/
//    Description: called by GPRS user to reg function that used by GPRS PS send
//    data to GPRS user
//    Author: Jun.zhang
//    Note:
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegReceiveDataFromModem(
                    MNGPRS_RECEIVEDATA_FPTR callback_ptr
                    );

/*************************************************************************/
//    Description: called by GPRS user to dereg function that used by GPRS PS send
//    data to GPRS user
//    Author: Jun.zhang
//    Note:
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregReceiveDataFromModem(
                    void
                    );

/*************************************************************************/
//    Description: called by gprs user to reg the function used by GPRS PS
//    to trigger flow control
//    Author: Jun.zhang
//    Note:
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegSetFlowControlFromModem(
                    MNGPRS_FLOWCONTROL_FPTR callback_ptr
                    );

/*************************************************************************/
//    Description: called by GPRS user to dereg the function used by GPRS PS
//    to trigger flow control
//    Author: Jun.zhang
//    Note:
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregSetFlowControlFromModem(
                    void
                    );

/*************************************************************************/
//    Description: called by gprs user to reg the function that used by GPRS PS
//                 to cancel flow control
//    Author: Jun.zhang
//    Note:
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegResetFlowControlFromModem(
                    MNGPRS_FLOWCONTROL_FPTR callback_ptr
                    );

/*************************************************************************/
//    Description: called by GPRS user to dereg the function that used by GPRS PS
//                 to cancel gprs flow control
//    Author: Jun.zhang
//    Note:
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregResetFlowControlFromModem(
                    void
                    );

/*************************************************************************/
//    Description: called by gprs user to set whether the specific nsai works on modem mode
//    Author: Jun.zhang
//    Note:
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetModemFlag(
                    uint8    nsapi,
                    BOOLEAN  modem_flag
                    );

/*****************************************************************************/
// 	Description : APP task start to read the Emergency Call codes stored in MT
//	Global resource dependence : none
//  Author: Yi.Jin
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_ReadEccEntry( // MN_RETURN_SUCCESS
					  uint16         index,           // In: the index for emergency call numbers in MT'
					  MN_CALLED_NUMBER_T     *called_num_ptr     // Out:The ECC
				      );

/*****************************************************************************/
// 	Description :  sim send rp ack or rp error for SmsPP
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendSmsPPDlvrRpt (                      // return whether success or failure
                                                MN_SMS_DLVR_RPT_E       rpt_type,
                                                uint8     cause,
                                                MN_L3_MSG_UNIT_T	*tpdu_ptr
                                                );

/*****************************************************************************/
// 	Description :  send rp ack or rp error
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendSmsDlvrRpt (                      // return whether success or failure
                                                MN_SMS_RECORD_ID_T    record_id,
                                                MN_SMS_DLVR_RPT_E       rpt_type,
                                                uint8     cause,
                                                MN_L3_MSG_UNIT_T	*tpdu_ptr
                                                );

/*****************************************************************************/
// 	Description :  AT send rp ack or rp error
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendATSmsDlvrRpt (                      // return whether success or failure
                                                MN_SMS_RECORD_ID_T    record_id,
                                                MN_SMS_DLVR_RPT_E       rpt_type,
                                                MN_L3_MSG_UNIT_T	*tpdu_ptr
                                                );

/*****************************************************************************/
// 	Description :  set the new sms deliver indication mode, invoked by AT+CNMI
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetNewSmsIndMode(
                                    uint8 mt_value
                                    );

/*****************************************************************************/
// 	Description :  set if PS save status report or not
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetStautsRptSaveMode(
                                    BOOLEAN is_save
                                    );

/*****************************************************************************/
/*  Description : set APN value for a PDP context.*/
/*  Global resource dependence : none*/
/*  Author: Pizer.Fan */
/*  Note:*/
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetPdpContextAPNWithCid( /* return whether success or failure    */
    uint8    cid,
    uint8   *apn_ptr,
    BOOLEAN  save_to_nv_flag
    );

/*****************************************************************************/
/*  Description : get the axtive pdp context's dl rate, if no active pdpcontext, return null*/
/*  Global resource dependence : none*/
/*  Author:  michael.zhu*/
/*  Note:*/
/*****************************************************************************/
extern MNGPRS_3G_EXTEND_QOS_SET_T MNGPRS_GetActivePdpExtendQos(void);

/*****************************************************************************/
/*  	Description : get the service type for MO SMS */
/* 	Global resource dependence : none */
/*   Author: michael.zhu  */
/* 	Note:	used by APP layer, this is called by ATC */
/*****************************************************************************/
extern MN_SMS_SERVICE_TYPE_E MNSMS_GetServiceForMOSms ( 
                        void
                        );

/*****************************************************************************/
// 	Description : l this function to get apn info
//	Global resource dependence : none
//  Author:	bin.li
//	Note: 
/*****************************************************************************/
extern MNSIM_RESULT_E MN_GetAPNInfo(
                                        MN_APN_TYPE_E    apn_type,   /* in */
                                        BOOLEAN  *is_display_rplmn,  /* out */
                                        MN_APN_NAME_CODE_SCHEM_E	  *apn_code_schem,	// the supplementary service used on which type service(bearer or tele or all call type)
                                        uint8 *spn_name
                                        );

/*****************************************************************************/
// 	Description : l this function to get spn info
//	Global resource dependence : none
//  Author:	jun.zhang
//	Note: 
/*****************************************************************************/
extern MNSIM_RESULT_E MN_GetSPNInfo(
                                        MN_SPN_TYPE_E    spn_type,   /* in */
                                        BOOLEAN  *is_display_rplmn,  /* out */
                                        MN_SPN_NAME_CODE_SCHEM_E	  *spn_code_schem,	// the supplementary service used on which type service(bearer or tele or all call type)
                                        uint8 *spn_name
                                        );

/*****************************************************************************/
// 	Description : l this function to do sim restrict function
//	Global resource dependence : none
//  Author:	bin.li
//	Note: 
/*****************************************************************************/
extern MNSIM_RESULT_E MN_SIM_CRSM(
						MN_CRSM_COMMAND_TYPE_E command,
						uint8 p1,
						uint8 p2,
						uint8 p3,
						uint16 file_id,
						uint8 *pdata,
						uint8 path_len,
						uint8 *path_id);

/*****************************************************************************/
// 	Description : l this function to set ext stk terminal profile data
//	Global resource dependence : none
//  Author:	bin.li
//	Note: 
/*****************************************************************************/
extern void MNPHONE_SetExtProfile (
      BOOLEAN is_valid,
      uint16 ext_data_len,
      uint8* ext_data_ptr 
);

/*****************************************************************************/
// 	Description : l this function to read auth relate file from sim
//	Global resource dependence : none
//  Author:	bin.li
//	Note: 
/*****************************************************************************/
extern void MNPHONE_TriggerReadAuthFile (
      void 
);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, get L1IT and RR switch which controls*/
/*                dummy funtion*/
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*****************************************************************************/
BOOLEAN em_get_l1it_rr_switch(void);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, set L1IT and RR switch for control */
/*                dummy funtion*/
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*****************************************************************************/
void em_set_l1it_rr_switch(BOOLEAN value);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, get RRDM td measurement switch*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
void em_get_td_meas_switch(uint32 *value_ptr);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, get RRDM td measurement switch*/
/*	Global resource dependence : none */
/*  Author: chris.liu */
/*	Note: only change the interface with MMI, the function is exactly the same as
*             em_get_td_meas_switch(uint32 *value_ptr);   */
/*****************************************************************************/
MNPHONE_TD_MEAS_STATE MNPHONE_GetTdMeasurementState(void);

/* +cr253537*/
/*****************************************************************************/
/* 	Description : called by ATC or MMI, get RRDM td measurement switch for dual*/
/*	Global resource dependence : none */
/*  Author: huawei.zhang*/
/*	Note: only change the interface with MMI, the function is exactly the same as
*             em_get_td_meas_switch(uint32 *value_ptr);   */
/*****************************************************************************/
MNPHONE_TD_MEAS_STATE MNPHONE_GetTdMeasurementStateEx(MN_DUAL_SYS_E dual_sys);
/* -cr253537*/
/*****************************************************************************/
/* 	Description : called by ATC or MMI, set RRDM td measurement switch*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
void em_set_td_meas_switch(uint32 value);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, set RRDM td measurement switch*/
/*	Global resource dependence : none */
/*  Author: chris.liu*/
/*	Note: only change the interface with MMI, the function is exactly the same as
*             em_set_td_meas_switch(uint32 value);*/
/*****************************************************************************/
void MNPHONE_SetTdMeasurementState(MNPHONE_TD_MEAS_STATE value);

/*+cr253537*/
/*****************************************************************************/
/* 	Description : called by ATC or MMI, set RRDM td measurement switch for dual*/
/*	Global resource dependence : none */
/*  Author: huawei.zhang*/
/*	Note: only change the interface with MMI, the function is exactly the same as
*             em_set_td_meas_switch(uint32 value);*/
/*****************************************************************************/
void MNPHONE_SetTdMeasurementStateEx(MN_DUAL_SYS_E dual_sys, MNPHONE_TD_MEAS_STATE value);
/*-cr253537*/
/*****************************************************************************/
/* 	Description : called by ATC or MMI, get TRR Parameters*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
void em_get_trr_param(uint32 index, uint32 *value_ptr);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, set TRR Parameters*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
void em_set_trr_param(uint32 index, uint32 value);

/*****************************************************************************/
/* 	Description : set support edge value of phone to nvm*/
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*****************************************************************************/
extern BOOLEAN MNPHONE_SetSupportEdgeValue(
				uint8  value	//the range is (0 - 2    0:no supp edge, 1:supp ul 8psk, 2:no supp ul 8psk 
				);

/*****************************************************************************/
/* 	Description : get support edge value of phone*/
/*	Global resource dependence : none */
/*  Author: minqian.qian */
/*	Note: */
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetSupportEdgeValue(uint8 *value_ptr);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, get Layer1 EM Parameters*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
void em_get_l1g_param(uint32 index, uint32 *value_ptr);

/*****************************************************************************/
/* 	Description : called by ATC or MMI, set Layer1 EM Parameters*/
/*	Global resource dependence : none */
/*  Author: martin.jiang */
/*	Note: */
/*****************************************************************************/
void em_set_l1g_param(uint32 index, uint32 value);

/*****************************************************************************/
// 	Description : deinit Wireless Protocol Stack and then assert
//	Global resource dependence : none
//  Author:       Pizer.Fan
//	Note:
/*****************************************************************************/
ERR_MNPHONE_CODE_E MNPHONE_DeinitAssertPsEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E   dual_sys
              );

/*****************************************************************************/
// 	Description : get edge capability
//	Global resource dependence : none
//  Author:       xia.wei
//	Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_IsEdgeSupport(void);

extern BOOLEAN em_set_wakeup_condition( 
			  uint32 condition
              );

/*****************************************************************************/
// 	Description : l this function to do sim restrict function
//	Global resource dependence : none
//  Author:	bin.li
//	Note: 
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNSIM_RunMbbmsAlgo(
                        uint8       *rand_ptr, /*LV格式,第一个字节为len*/
                        uint8       *autn_ptr  /*LV格式,第一个字节为len*/
                        );

/*****************************************************************************/
// 	Description : get statur report for atc
//	Global resource dependence : none
//  Author: george.yuan
//	Note: 
/*****************************************************************************/
extern void MNSMS_GetHexSmsSR(                  
                  MN_SMS_STATUS_REPORT_T   *mt_sms_ptr,       // in MN_SMS_MT_SMS_T
                  uint8             *tpdu_len,         // out tpdu length
                  uint8             *rsp_ptr,          // out rsp_ptr
                  uint16            rsp_len            // in rsp_len
                       );

/*************************************************************************/
/*    Description: called by ATC to Get GPRS state                       */
/*    Global resource dependence :                                       */
/*    Author: Martin.Jiang                                               */
/*    Note:                                                              */
/*************************************************************************/
extern BOOLEAN MNGPRS_GetFlowControlState(uint16 nsapi);

/*************************************************************************/
/*    Description: called by cs data user to reg function that used by cs data send 
**    data to cs data user
**    Global resource dependence : 
**    Author: bo.chen
**    Note:  */
/*************************************************************************/
MN_RETURN_RESULT_E MNCSDATA_RegReceiveDataEx( // return whether success or failure
                    MN_DUAL_SYS_E   dual_sys,
                    MNCSDATA_RECEIVEDATA_FPTR callback_ptr // call back function pointer
                    );

/* +cr254874 */
void MNCSDATA_RegSendDataTriggerEx(
                    MN_DUAL_SYS_E   dual_sys,
                    MNCSDATA_SENDDATA_TRIGGER_FPTR callback_ptr // call back function pointer
                    );
/* -cr254874 */

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:	bin.li
//	Note: 
/*****************************************************************************/
void MNPHONE_SetTerminalProfileBeforeRefresh (
        BOOLEAN is_need_terminal_profile
        );

/*****************************************************************************/
/*  Description : Set flag to determin it is always ps attach automatically
                 or just do it when needed */
/*  Global      : none*/
/*  Author      : yi.jin*/
/*  Note        :*/
/*  History     :*/
/*       yi.jin 2010-04-02 create*/
/*****************************************************************************/
ERR_MNGPRS_CODE_E  MNGPRS_SetAutoAttachFlag( /*return whether success or failure*/
            BOOLEAN always_auto_attach_flag /* TRUE if it always need ps attach automatically*/
            );

/*****************************************************************************/
//  Description : the function pack the SMS deliever
//  Global resource dependence : none
//  Author: bin.li
//  Note:
/*****************************************************************************/
extern void SMS_PackNormalDeliver2SmsPP(
           MN_DUAL_SYS_E dual_sys,
           MN_SMS_MT_SMS_T *mt_sms_ptr, 
           MN_L3_MSG_UNIT_T *tpdu_ptr);

/*****************************************************************************/
//  Description : register callback funtion for dummy mmi keypad response. 
//                It is used by ATC.
//  Global resource dependence : none
//  Author: minqian.qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNDUMMY_RegisterMMIKpdRspFun(
                        MN_DUMMYMMI_RECEIVER_FPTR      callback_ptr
                        );

/*****************************************************************************/
//  Description : It is used by MMI to send dummy information to ATC
//  Global resource dependence : none
//  Author: minqian.qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNDUMMY_SendRspInfoToAT(
                     const uint8 *str_ptr,      //Point to the input message
                     uint16       str_len       //input message length
                     );

/*****************************************************************************/
/* 	Description : called by ATC or MMI eng, get rscp or rssi from RRC with the same value
       without any converting*/
/*  Global      : none*/
/*  Author      : george.yuan*/
/*  Note        :*/
/*  History     :*/
/*      george.yuan 2010-05-31 create*/
/*****************************************************************************/
extern MN_PHONE_SCELL_MEAS_T em_GetRSSIEx( // rssi
          MN_DUAL_SYS_E dual_sys
          );

/*****************************************************************************/
// 	Description :  if is_set_uicc_sim is TRUE, sim task detect the uicc as sim card
//	Global resource dependence : none
//  Author:	bin.li
//	Note: 
/*****************************************************************************/
extern void MNPHONE_SetUiccAsSimCard (
        BOOLEAN is_set_uicc_sim
        );

/*****************************************************************************/
/*  Description : set energy saving mode for gprs connection; this API is provided
                  for MMI gprs connection setting and it will set some PS emergy 
                  saving condition such as for layer1, etc */
/*  Global      : none*/
/*  Author      : george.yuan*/
/*  Note        :*/
/*  History     :*/
/*               george.yuan 2011-01-12 create*/
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E  MNGPRS_SetEnergySavingMode ( /*return whether success or failure*/
    MN_DUAL_SYS_E		dual_sys,
    BOOLEAN energy_saving_mode /* TRUE saving, FALSE not saving */
    );

/*****************************************************************************
 	Description : get PL information from sim
	Global resource dependence : none
     Author:       Bruce.Jiang
  	Note:
*****************************************************************************/
extern MN_SIM_PARA_VALUE_T MNSIM_GetSIMFileInfo(
                    MN_DUAL_SYS_E dual_sys,
                    MN_SIM_FILE_TYPE_E file_type
                    );

/*****************************************************************************/
//  Description : select band
//  Global resource dependence : none
//  Author:       Bruce.Jiang
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SelectGSMBandEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_BAND_TYPE_E user_band // MN_PHONE_BAND_GSM,
                                             // MN_PHONE_BAND_PCS,
                                             // MN_PHONE_BAND_DCS,
                                             // MN_PHONE_BAND_DUALBAND
              );

/*****************************************************************************/
//  Description : Set GPRS class type ClassA,B,C,CG,CC for command +CGCLASS
//  Author      : Xiaogang.Xu
//  Note        :
/*****************************************************************************/

extern ERR_MNGPRS_CODE_E  MNGPRS_SetGPRSClassTypeEx( //return whether success or failure
            MN_DUAL_SYS_E dual_sys,
            MN_PHONE_USER_SERVICE_SELECT_E class_type // mobile class
            );

/*****************************************************************************/
//  Description : Show GPRS class type ClassA,B,C,CG,CC
//  Author      : Xiaogang.Xu
//  Note        :
/*****************************************************************************/

extern ERR_MNGPRS_CODE_E  MNGPRS_ShowGPRSClassTypeEx( //return whether success or failure
            MN_DUAL_SYS_E dual_sys
            );

/*****************************************************************************/
//  Description : Set command enables or disables sending of unsolicited result codes
//  Author      : Xiaogang.Xu
//  Note        :
/*****************************************************************************/

extern ERR_MNGPRS_CODE_E  MNGPRS_SetGPRSEvReportEx( //return whether success or failure
            MN_DUAL_SYS_E dual_sys,
            MN_GPRS_EVENT_REPORT_MODE_E mode, // the requested event_report mode
            MN_GPRS_EVENT_REPORT_BUFFER_E buffer // the requested event_report buffer
            );

/*****************************************************************************/
//  Description : Show command of MNGPRS_SetGPRSEvReport result
//  Author      : Xiaogang.Xu
//  Note        :
/*****************************************************************************/

extern ERR_MNGPRS_CODE_E  MNGPRS_ShowGPRSEvReportEx( //return whether success or failure
            MN_DUAL_SYS_E dual_sys
            );

/*************************************************************************/
//    Description: send pdp-activate-req to mn
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ActivatePdpContextWithoutSrvType(
                    uint8 pdp_id
                    );

/*************************************************************************/
//    Description: send pdp-modify-req to mn
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ModifyPdpContext(
                    uint8 pdp_id
                    );

/*************************************************************************/
//    Description: read 3g QoS profiles of pdp contexts
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadExtReqQosProfile(
                    MN_GPRS_EXT_QOS_T *ext_qos
                    );

/*************************************************************************/
//    Description: set 3g QoS profile of pdp context
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetExtReqQosProfile(
                    MN_GPRS_EXT_QOS_INFO_T *ext_qos_info
                    );

/*************************************************************************/
//    Description: read 3g QoS profiles (minimum) of pdp contexts
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadExtMinQosProfile(
                    MN_GPRS_EXT_QOS_T *ext_qos
                    );

/*************************************************************************/
//    Description: set 3g QoS profile (minimum) of pdp context
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetExtMinQosProfile (
                    MN_GPRS_EXT_QOS_INFO_T *ext_qos_info
                    );

/*************************************************************************/
//    Description: read 3g negotiated QoS profile of pdp context
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadExtNegQos(
                    MN_GPRS_EXT_QOS_T *ext_qos
                    );

/*************************************************************************/
//    Description: read traffic flow template of pdp contexts
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadTft(
                    MN_GPRS_TFT_T *tft
                    );

/*************************************************************************/
//    Description: set traffic flow template of pdp context
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetTft(
                    MN_GPRS_PACKET_FILTER_INFO_T *pfi
                    );

/*************************************************************************/
//    Description: read secondary pdp contexts
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadSecPdpContext(
                    MN_GPRS_SEC_PDP_CONTEXT_T *sec_pdp_cntxt
                    );

/*************************************************************************/
//    Description: set secondary pdp context
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetSecPdpContext(
                    MN_GPRS_SEC_PDP_CONTEXT_INFO_T *sec_pdp_cntxt_info
                    );

/*************************************************************************/
//    Description: read extend request qos profile from SIM or NV via service type
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadStorageExtReqQos(
                    MN_GPRS_STORAGE_E       storage,
                    MN_PS_SERVICE_TYPE_E    ps_service_type,
                    MN_GPRS_EXT_QOS_INFO_T *qos_info
                    );

/*************************************************************************/
//    Description: set extend request qos profile to SIM or NV via service type
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetStorageExtReqQos(
                    MN_GPRS_STORAGE_E       storage,
                    MN_PS_SERVICE_TYPE_E    ps_service_type,
                    MN_GPRS_EXT_QOS_INFO_T *qos_info
                    );

/*************************************************************************/
//    Description: set extend request qos profile to SIM or NV via service type
//                 with item_setted[]
//    Author: Pizer Fan
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetStorageExtReqQosWithItem(
                    MN_GPRS_STORAGE_E       storage,
                    MN_PS_SERVICE_TYPE_E    ps_service_type,
                    MN_GPRS_EXT_QOS_INFO_T *qos_info,
                    BOOLEAN                 item_setted[13]
                    );

/*****************************************************************************/
//  Description : get the first phonebook specify entry 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:         
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_GetFirstEntryInListEx(   // false if no entry about the id
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_STORAGE_E storage,    // IN:
                PHONEBOOK_ENTRY_T   *entry_ptr  // IN/OUT:entry data
                );

/*****************************************************************************/
//  Description : get the next phonebook specify entry 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:         
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_GetNextEntryInListEx(   // false if no entry about the id
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_STORAGE_E storage,    // IN:
                PHONEBOOK_ENTRY_T   *entry_ptr  // IN/OUT:entry data
                );

/*****************************************************************************/
//  Description : get the last phonebook specify entry 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:         
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_GetLastEntryInListEx(   // false if no entry about the id
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_STORAGE_E storage,    // IN:
                PHONEBOOK_ENTRY_T   *entry_ptr  // IN/OUT:entry data
                );

/*****************************************************************************/
//  Description : ATC originate an MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:   Used by ATC layer
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendATSmsPDUWithTimeEx(                                   //RETURN:
                                              MN_DUAL_SYS_E         dual_sys,
                                              MN_SMS_ADDR_T         *addr_ptr,  //IN: the SMSC address
                                              MN_L3_MSG_UNIT_T      *tpdu_ptr,  //IN: the content of MO SMS
                                              BOOLEAN               is_more,    //IN:
                                              MN_SMS_TIME_STAMP_T   time_stamp  //IN: send time
                                             );

/*****************************************************************************/
//  Description : Send one STK PDU SMS message, Need to Change DCS 8bit-data to 
//                7bit default alphabet.
//  Global resource dependence : none
//  Author: Lucian.Hu
//  Modify: Rossi.luo, 2004-8-12, add SCI_ASSERT before SCI_MEMCPY/SCI_MEMSET.
//  Note: Used by STK module.
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendSmsPDUWithTimeEx(
                                            MN_DUAL_SYS_E       dual_sys,           //IN:
                                            BOOLEAN             is_address_present, //IN:
                                            MN_CALLED_NUMBER_T  *addr_ptr,          //IN: the SMSC address
                                            MN_L3_MSG_UNIT_T    *tpdu_ptr,          //IN: the content of MO SMS
                                            BOOLEAN             is_need_compress,   //IN: whether user data need compressed
                                            BOOLEAN             is_more,            //IN:
                                            MN_SMS_TIME_STAMP_T *time_stamp_ptr     //IN: send time
                                            );

/*****************************************************************************/
// 	Description : MNPHONE_RxPwrMeas
//	Global resource dependence :
//  Author:
//	Note: td band [10054-10121,9404-9596]; gsm band info
//	#define GSM_900P_LOW			0
//	#define GSM_900P_HIGH			124
//	#define GSM_900E_LOW			975
//	#define GSM_900E_HIGH			1023
//	#define DCS_1800_LOW			512
//	#define DCS_1800_HIGH			885
//	#define PCS_1900_LOW			512
//	#define PCS_1900_HIGH			810
//	#define GSM_850_LOW       128
//	#define GSM_850_HIGH      251
/****************************************************************/
extern int16 MNPHONE_RxPwrMeas(uint16 system_mode, /* gsm rx = 0; td rx = 1; current only support gsm */
			                               uint16 gsm_band,    /* EGSM900 = 0; DCS1800 = 1; EGSM_DCS_DUALBAND = 2; PCS1900 = 3; GSM850 = 4 */
			                               uint16 test_freq);   /* td [10054-10121,9404-9596]; gsm band  ARFCN*/

// 	Description : MNPHONE_RxPwrMeasEx
//	Global resource dependence :
//  Author:
//	Note: td band [10054-10121,9404-9596]; gsm band info
//	#define GSM_900P_LOW			0
//	#define GSM_900P_HIGH			124
//	#define GSM_900E_LOW			975
//	#define GSM_900E_HIGH			1023
//	#define DCS_1800_LOW			512
//	#define DCS_1800_HIGH			885
//	#define PCS_1900_LOW			512
//	#define PCS_1900_HIGH			810
//	#define GSM_850_LOW       128
//	#define GSM_850_HIGH      251
/****************************************************************/
extern int16 MNPHONE_RxPwrMeasEx(MN_DUAL_SYS_E dual_sys,
					uint16 system_mode, /* gsm rx = 0; td rx = 1; current only support gsm */
                                    uint16 gsm_band,    /* EGSM900 = 0; DCS1800 = 1; EGSM_DCS_DUALBAND = 2; PCS1900 = 3; GSM850 = 4 */
                                    uint16 test_freq);   /* td [10054-10121,9404-9596]; gsm CHANNEL NUMBER (ARFCN) */

// 	Description : MNPHONE_RxPwrMeas
//	Global resource dependence :
//  Author:
//	Note: td band [10054-10121,9404-9596]; gsm band info
//	#define GSM_900P_LOW			0
//	#define GSM_900P_HIGH			124
//	#define GSM_900E_LOW			975
//	#define GSM_900E_HIGH			1023
//	#define DCS_1800_LOW			512
//	#define DCS_1800_HIGH			885
//	#define PCS_1900_LOW			512
//	#define PCS_1900_HIGH			810
//	#define GSM_850_LOW       128
//	#define GSM_850_HIGH      251
/****************************************************************/
extern int16 MNPHONE_RxPwrMeas(uint16 system_mode, /* gsm rx = 0; td rx = 1; current only support gsm */
                                 uint16 gsm_band,    /* EGSM900 = 0; DCS1800 = 1; EGSM_DCS_DUALBAND = 2; PCS1900 = 3; GSM850 = 4 */
                                 uint16 test_freq);   /* td [10054-10121,9404-9596]; gsm  CHANNEL NUMBER (ARFCN)*/

/*+cr248035*/
/*****************************************************************************/
/* 	Description : Auto Poweron Flag */
/*	Global resource dependence : none */
/*  Author:Joe.Ni */
/*	Note: */
/*****************************************************************************/
extern void em_atc_REFPARAM_SetAutoPowerOnFlag(uint8 mode);
/*****************************************************************************/
/* 	Description : Get Auto Poweron Flag */
/*	Global resource dependence : none */
/*  Author:Joe.Ni */
/*	Note: */
/*****************************************************************************/
extern uint8 em_atc_REFPARAM_GetAutoPowerOnFlag(void);
/*-cr248035*/

/*****************************************************************************/
//  Description : Set Flag allows the TE to accept or reject the request
//                    for specific pdp context
//  Author      : Xiaogang.Xu
//  Note        :
/*****************************************************************************/

extern ERR_MNGPRS_CODE_E  MNGPRS_SetResTypeEx( //return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        MN_GPRS_TE_RESPONSE_TYPE_E    res_type, // the requested response type
        BOOLEAN is_all,                      //// identify if all PDP contexts 
        uint32  pdp_id                // the identity array of the PDP contexts 
        );

/*****************************************************************************/
//  Description : 1. turn on or off: Manual response to a network request for PDP context activation 
//                2. Based on S0
//  Author      : Xiaogang.Xu
//  Note        :
/*****************************************************************************/

extern ERR_MNGPRS_CODE_E  MNGPRS_SetResModeEx( //return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        MN_GPRS_TE_RESPONSE_MODE_E res_mode //the requested response mode
        );
/*-CR250068*/ 
/*+cr109561*/
extern ERR_MNPHONE_CODE_E MNPHONE_UpdateFPLMNtoSimEx(        /* ERR_MNPHONE_NO_ERR, */
              MN_DUAL_SYS_E             dual_sys,
              MN_PHONE_UPDATE_FPLMN_T    write_plmn      /* FPLMN */
              );
/*-cr109561*/

/*****************************************************************************/
//  Description :   Get the result of PIN verification during initialization
//  Global resource dependence : none
//  Author: Eddie.Wang
//  Note: Only called in the SIM initialization
//    MNSIM_PIN_REQ_OK                PIN verified successfully
//    MNSIM_PIN_WRONG_REQ_TYPE        API called error, not in the UE initialization
//    MNSIM_PIN_ERROR_WITH_BLOCKED    PIN verified fail, PIN blocked (could be PIN1 or PUK1
//    MNSIM_PIN_ERROR_PIN_FAIL        PIN verified fail
//    MNSIM_PIN_ERROR_UNKNOW          PIN status unkown
/*****************************************************************************/
extern MNSIM_PIN_REQ_RESULT_E MNRSF_GetPinVerifyResultEx(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :   Get the result of PIN verification during initialization
//  Global resource dependence : none
//  Author: Eddie.Wang
//  Note: Only called in the SIM initialization
//    MNSIM_PIN_REQ_OK                PIN verified successfully
//    MNSIM_PIN_WRONG_REQ_TYPE        API called error, not in the UE initialization
//    MNSIM_PIN_ERROR_WITH_BLOCKED    PIN verified fail, PIN blocked (could be PIN1 or PUK1
//    MNSIM_PIN_ERROR_PIN_FAIL        PIN verified fail
//    MNSIM_PIN_ERROR_UNKNOW          PIN status unkown
/*****************************************************************************/
extern MNSIM_PIN_REQ_RESULT_E MNRSF_GetPinVerifyResult();
/* +cr250176 */
/*****************************************************************************/
//  Description :   to get some cell broadcast messages' parameters
//  Global resource dependence : none
//  Author: Yi.Jin
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSMSCB_GetCbParamEx(  /* return whether success or failure */
     MN_DUAL_SYS_E dual_sys
     );
/*****************************************************************************/
//  Description :   set some cell broadcast messages' parameters
//  Global resource dependence : none
//  Author: Yi.Jin
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSMSCB_SetCbParamEx(  /* return whether success or failure */
     MN_DUAL_SYS_E dual_sys,
     MN_CB_MSG_SET_PARA_T cb_msg_para
     );
/* -cr250176 */
/* +cr250176_new */
/*****************************************************************************/
//  Description :   delete specified cell broadcast messages' by msg_code
//  Global resource dependence : none
//  Author: Yi.Jin
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSMSCB_DeleteMsg(  /* return whether success or failure */
     MN_DUAL_SYS_E dual_sys,
     MN_CB_MSG_ID_T  msg_id,
     MN_MSG_CODE_T   msg_code
     );
/* -cr250176_new */
