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
 

#if !defined (_MN_API_H)
#define _MN_API_H
#include "mn_type.h"
#include "simat_api.h"

#ifdef PRODUCT_DM
#include "mn_api_td.h"
#endif


/******************************************************************************
 **                        PHONE MODULE API                                   *
 ******************************************************************************/


/*****************************************************************************/
//  Following is for Dual Sim System Api
/*****************************************************************************/
/*****************************************************************************/
//  Description : read nv item
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNNV_ReadItem(  // MN_RETURN_SUCCESS
                                        // MN_RETURN_FAILURE
                uint16          item_id,    // 要读取的 nvitem 标识。
                uint32          item_length,// 要读取的长度
                void*           buffer_ptr  // 接收读取内容的内存块,>=item_length
                );

/*****************************************************************************/
//  Description : write nv item
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNNV_WriteItem(// MN_RETURN_SUCCESS
                                         // MN_RETURN_FAILURE
                uint16          item_id, // 要写入的 nvitem 标识，
                                         // 如果该nvitem 尚未创建，
                                         // 创建一个item_length大小nvitem
                uint32          item_length,// 要写入的数据内容 的长度
                const void*     buff_ptr    // 要写入的数据内容。
                );

/*****************************************************************************/
//  Description : Re read the PB from SIM card.
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_ReReadPhoneBook(void);

/*****************************************************************************/
//  Description : encode alphabet under sim standard
//  Global resource dependence : none
//  Author: Jenny.Wang 
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_EncodeAlphabetToSimStandard(
    PHONEBOOK_ALPHABET_TYPE_E   alpha_type,
    uint8                       input_len,
    uint8                       *input_ptr,
    uint8                       *output_len,
    uint8                       *output_ptr
);

/*****************************************************************************/
//  Description : encode alphabet under sim standard
//  Global resource dependence : none
//  Author: Jenny.Wang 
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_DecodeAlphabetWithSimStandard(
    PHONEBOOK_ALPHABET_TYPE_E   *alpha_type,
    uint8                       input_len,
    uint8                       *input_ptr,
    uint8                       *output_len,
    uint8                       *output_ptr
);

//Begin of GPRS data section
/*************************************************************************/
//    Description: called by GPRS user to reg function that used by GPRS PS send 
//    data to GPRS user
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegReceiveData( // return whether success or failure
                    MNGPRS_RECEIVEDATA_FPTR callback_ptr // call back function pointer
                    );

/*************************************************************************/
//    Description: called by GPRS user to reg function that used by GPRS PS send 
//    data to GPRS user
//    Global resource dependence : mngprs_receivedata_fptr2
//    Author: cuifang.deng
//    Note:  only support MODEM PLATFORM
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegReceiveData2( // return whether success or failure
                    MNGPRS_RECEIVEDATA_INDICATION_FPTR callback_ptr // call back function pointer
                    );
                    
/*************************************************************************/
//    Description: called by GPRS user to dereg function that used by GPRS PS send 
//    data to GPRS user
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregReceiveData(// return whether success or failure
                                                void
                                                );

/*************************************************************************/
//    Description: called by GPRS user to dereg function that used by GPRS PS send 
//    data to GPRS user
//    Global resource dependence : mngprs_receivedata_fptr2
//    Author: cuifang.deng
//    Note:  only support MODEM PLATFORM
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregReceiveData2(// return whether success or failure
                                                void
                                                );

/*************************************************************************/
//    Description: called by GPRS user to send data through GPRS PS  
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SendDataReq( // return whether success or failure
        uint8 *data_ptr,        // pointer of sent data
        uint32 data_size,   // size of sent data
        uint16 nsapi            // nsapi number used by GPRS
        );

/*************************************************************************/
//    Description: called by gprs user to reg the function used by GPRS PS  
//    to trigger flow control
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegSetFlowControl( // return whether success or failure
                    MNGPRS_FLOWCONTROL_FPTR callback_ptr // call back function pointer
                    );
                    
/*************************************************************************/
//    Description: called by GPRS user to dereg the function used by GPRS PS
//    to trigger flow control 
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregSetFlowControl( // return whether success or failure
                                                void
                                                );
                                                

/*************************************************************************/
//    Description: called by gprs user to reg the function that used by GPRS PS 
//                 to cancel flow control 
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegResetFlowControl( // return whether success or failure
                    MNGPRS_FLOWCONTROL_FPTR callback_ptr // call back function pointer
                    );
                    
/*************************************************************************/
//    Description: called by GPRS user to dereg the function that used by GPRS PS
//                 to cancel gprs flow control 
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregResetFlowControl( // return whether success or failure
                                                    void
                                                    );

/*************************************************************************/
//    Description: check if flow control open.  
//    Global resource dependence : 
//    Author: Shaohua.xia
//    Note:   
/*************************************************************************/
extern BOOLEAN  MNGPRS_IsFlowCtrlOpen(uint16 nsapi);

/*****************************************************************************/
//  Description : show GPRS is Active or NOT
//  Global      : none
//  Author      : wuding.yang
/*****************************************************************************/
extern BOOLEAN MNGPRS_IsGPRSActive(void);

/*****************************************************************************/
//  Description : Set receive paging in TBF on or off
//  Global      : none
//  Author      : 
/*****************************************************************************/
extern void  MNGPRS_SetTbPaging( 
    MN_DUAL_SYS_E dual_sys,  //IN:
    BOOLEAN  tbf_paging_on   //IN:     
);

uint8 MNGPRS_GetSwitchDataCard(void);
void MNGPRS_SetSwitchDataCard(uint8 switchdata_card);
//End of GPRS data section

/******************************************************************************
 **                        PHONE MODULE API                                   *
 ******************************************************************************/
 
/*****************************************************************************/
//  Description : MNPHONE_StartUpPs
//  Global resource dependence : none
//  Author:       jenny.wang
//  Note: send out start up MN signal to active PS
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_StartupPs(
			MN_GMMREG_RAT_E  user_rat
			);

/*****************************************************************************/
//  Description : get the pointer of the fuction that read sms in nv
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note:
/*****************************************************************************/
extern void MNSMS_RegReadNVSmsFunc(MNSMS_READNVSMS_PFUNC pControlFunc);

/*****************************************************************************/
//  Description : get the pointer of the fuction that write sms in nv
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note:
/*****************************************************************************/
extern void MNSMS_RegWriteNVSmsFunc(MNSMS_WRITENVSMS_PFUNC pControlFunc);

/*****************************************************************************/
//  Description : set the max number of sms saved in nv
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note:
/*****************************************************************************/
extern void MNSMS_RegSetMaxNVSmsNumFunc(MNSMS_SETMAXNVSMSNUM_PFUNC pControlFunc);

/*****************************************************************************/
//  Description : delete all sms in nv
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note:
/*****************************************************************************/
extern void MNSMS_RegDelAllSmsInNvFunc(MNSMS_DELALLNVSMS_PFUNC pControlFunc);  

/*****************************************************************************/
//  Description : get current plmn info 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MN_PHONE_CURRENT_PLMN_INFO_T MNPHONE_GetCurrentPLMNInfo( // return current plmn info
              void
              );

/*****************************************************************************/
//  Description : MNPHONE_StartUpPsEx
//  Global resource dependence : none
//  Author:       jenny.wang
//  Note: send out start up MN signal to active PS
/*****************************************************************************/
ERR_MNPHONE_CODE_E MNPHONE_StartupPsEx(        // MN_RETURN_SUCCESS,
                MN_DUAL_SYS_E dual_sys,
                MN_GMMREG_RAT_E  user_rat
               );
               
/*****************************************************************************/
//  Description : select specified PLMN
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SelectPLMNEx(        // MN_RETURN_SUCCESS,
              MN_DUAL_SYS_E dual_sys,
              BOOLEAN       mode_is_manual,   // select mode,MANUAL or  not
              MN_PLMN_T     select_plmn,      // specified PLMN
              MN_GMMREG_RAT_E    rat
              );

/*****************************************************************************/
//  Description : get plmn list
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_ListPLMNEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys,
              MN_GMMREG_RAT_E    rat
              );


/*****************************************************************************/
//  Description : if ps is in plmn search , call the function abort it
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_AbortPLMNSearchEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : supply PIN1 value or unblock CHV1 value
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_ResponsePinEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E          dual_sys,
              MNSIM_PIN_INDICATION_E pin_num,     // CHV1
              BOOLEAN                pin_is_lock, // is block?
              MN_BLOCK_T*            pin_value_ptr,
              MN_BLOCK_T*            unblock_pin_value_ptr
              );
              
/*****************************************************************************/
//  Description : MNPHONE_InitEncryptSimEx
//  Global resource dependence : none
//  Author:       Fancier.Fan
//  Note:
/*****************************************************************************/
extern void MNPHONE_InitEncryptSimEx(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : power off request
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_DeactivePsEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : get neighbour cell power
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_GetRSSIOfNCELLEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : operate PIN ,change,verify,disable,enable,...
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_OperatePinEx( // MN_RETURN_SUCCESS,
              MN_DUAL_SYS_E           dual_sys,
              MNSIM_PIN_REQ_TYPE_E    operate_type,     //  operate type
              MNSIM_PIN_INDICATION_E  pin_num,      //  the chv indication,CHV1/CHV2
              MN_BLOCK_T*             pin_value_ptr,
              MN_BLOCK_T*             ext_pin_value_ptr
              );

/*****************************************************************************/
//  Description : enable roaming or not,set nv
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetRoamingTypeEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_ROAMING_MODE_E roaming_type
              );
                            
/*****************************************************************************/
//  Description : get roaming type
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MN_PHONE_ROAMING_MODE_E MNPHONE_GetRoamingTypeEx( 
              MN_DUAL_SYS_E dual_sys
              );
              
/*****************************************************************************/
//  Description : Set MS service type in nv
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetServiceTypeEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_USER_SERVICE_SELECT_E service_type  
              // MN_PHONE_USER_SELECT_NONE
              // MN_PHONE_USER_SELECT_GSM_ONLY  
              // MN_PHONE_USER_SELECT_GSM_GPRS_BOTH
              );

/*****************************************************************************/
//  Description : get pref mode in nv
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MN_PHONE_PREF_MODE_E MNPHONE_GetPrefModeEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : Set pref mode in nv
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetPrefModeEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_PREF_MODE_E mn_pref_mode // MN_PHONE_PREF_MODE_GSM,MN_PHONE_PREF_MODE_GPRS
              );
              
/*****************************************************************************/
//  Description : set plmn select  mode in nv
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetPlmnSelectModeEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_PLMN_SELECT_MODE_E mn_sel
              );

/*****************************************************************************/
//  Description : MNPHONE_SetGprsAttachModeEx
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetGprsAttachModeEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys,
              BOOLEAN  is_manual_attach
              );

/*****************************************************************************/
//  Description : get plmn select  mode in nv
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_GetPlmnSelectModeEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_SELECT_PLMN_T *mode_ptr
              );

/*****************************************************************************/
//  Description : get service type in nv
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MN_PHONE_USER_SERVICE_SELECT_E MNPHONE_GetServiceTypeEx(
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : get current plmn info 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MN_PHONE_CURRENT_PLMN_INFO_T MNPHONE_GetCurrentPLMNInfoEx( // return current plmn info
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : get rssi
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MN_PHONE_SCELL_MEAS_T MNPHONE_GetRSSIEx( // rssi
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : get ncell rxlev and bsic and arfcn
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_GetMeasReportEx(
            MN_DUAL_SYS_E dual_sys,
            APP_MN_NCELL_RSSI_CNF_T *ncell_meas_ptr,
            MN_PHONE_CELL_MEAS_T    *scell_meas_ptr
            );

/*****************************************************************************/
//  Description : get user band
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
MN_PHONE_BAND_TYPE_E MNPHONE_GetBandEx(          // MN_PHONE_BAND_GSM
                                             // MN_PHONE_BAND_DCS
                                             // MN_PHONE_BAND_DUALBAND
                                             // MN_PHONE_BAND_PCS
              MN_DUAL_SYS_E dual_sys
              );
         
/*****************************************************************************/
//  Description : select band
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SelectBandEx( // MN_RETURN_SUCCESS
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_BAND_TYPE_E user_band // MN_PHONE_BAND_GSM,
                                             // MN_PHONE_BAND_PCS,
                                             // MN_PHONE_BAND_DCS,
                                             // MN_PHONE_BAND_DUALBAND
              );              

/*****************************************************************************/
//  Description : select band
//             MNPHONE_SelectBandEx will save the band in NV.
//             MNPHONE_SelectBandEx_WithoutSaveNV will not save the band in NV.
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note: 
//      This function should be called x times continously. X is the sim card numbers.
//      For example, if your current model is a dual sim product. Then you should call them continously.
//           MNPHONE_SelectBandEx_WithoutSaveNV(0,user_band);
//           MNPHONE_SelectBandEx_WithoutSaveNV(1,user_band);
//      if your current model is a three sim product. Then you should call them continously.
//           MNPHONE_SelectBandEx_WithoutSaveNV(0,user_band);
//           MNPHONE_SelectBandEx_WithoutSaveNV(1,user_band);
//           MNPHONE_SelectBandEx_WithoutSaveNV(2,user_band);
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SelectBandEx_WithoutSaveNV( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_BAND_TYPE_E user_band // MN_PHONE_BAND_GSM
                                             // MN_PHONE_BAND_DCS
                                             // MN_PHONE_BAND_DUALBAND
                                             // MN_PHONE_BAND_PCS
              );

/*****************************************************************************/
//  Description : get Preferred plmn list 
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_GetPreferredPlmnListEx(
            MN_DUAL_SYS_E dual_sys,
            MN_PHONE_PLMN_LIST_TYPE_E   plmn_list_type,
            MN_PHONE_PLMN_SEL_LIST_T    *mn_preferrd_plmn_list_ptr
            );

/*****************************************************************************/
//  Description : get Preferred plmn list(EFplmnwact, or EFplmnsel) 
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetPreferredPlmnListEx(
            MN_DUAL_SYS_E dual_sys,
            MN_PHONE_PLMN_SEL_LIST_T    *mn_sel_list_ptr
            );

/*****************************************************************************/
//  Description : get plmn select list 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_GetPlmnSelectListEx(
            MN_DUAL_SYS_E dual_sys,
            MN_PHONE_PLMN_SEL_LIST_T    *mn_sel_list_ptr
            );

/*****************************************************************************/
//  Description : set plmn select list 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetPlmnSelectListEx(
            MN_DUAL_SYS_E dual_sys,
            MN_PHONE_PLMN_SEL_LIST_T    *mn_sel_list_ptr
            );

/*****************************************************************************/
//  Description: Write preferred PLMN to SIM  
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_WritePLMNtoSimEx(
              MN_DUAL_SYS_E         dual_sys,   //IN:
              MN_PHONE_WRITE_PLMN_T write_plmn  //IN:
              );

/*****************************************************************************/
//  Description : get forbidden plmn  list 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_GetFplmnListEx(
            MN_DUAL_SYS_E dual_sys,
            MN_PHONE_FPLMN_LIST_T   *mn_fplmn_list_ptr
            );

/*****************************************************************************/
//  Description : if ps is scan plmn
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_IsPLMNSearchEx(
            MN_DUAL_SYS_E dual_sys
            );

/*****************************************************************************/
//  Description : set forbidden plmn list 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetFplmnListEx(
            MN_DUAL_SYS_E dual_sys,
            MN_PHONE_FPLMN_LIST_T   *mn_fplmn_list_ptr
            );
            
/*****************************************************************************/
//  Description : get the times of no-cell state since handset is powered on
//  Global resource dependence : none
//  Author:       Jenny.Wang
//  Note:
/*****************************************************************************/
extern int32 MNPHONE_GetTimesOfNoCellAvailableEx(
            MN_DUAL_SYS_E dual_sys
            );
             

/*****************************************************************************/
//  Description : get the times of searching plmn since handset is powered on
//  Global resource dependence : none
//  Author:       Jenny.Wang
//  Note:
/*****************************************************************************/
extern int32 MNPHONE_GetTimesOfSearchPlmnEx(
            MN_DUAL_SYS_E dual_sys
            );

            
/*****************************************************************************/
//  Description : do attach request
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/         
extern ERR_MNPHONE_CODE_E MNPHONE_GprsAttachEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_GPRS_ATTACH_TYPE_E mn_attach_type // attach type
              );            
              
              
/*****************************************************************************/
//  Description : do detach request
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
//  History:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_GprsDetachEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_GPRS_DETACH_TYPE_E mn_detach_type // detach type
              );

/*****************************************************************************/
//  Description : do detach request
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note: only be used in dual sim project.
//  History:
//  
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_GprsForceDetachEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E dual_sys,
              MN_PHONE_GPRS_DETACH_TYPE_E mn_detach_type // detach type
              );

/*****************************************************************************/
//  Description : check whether PS is powered on
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MNPHONE_IsPsPowerOnEx(
    MN_DUAL_SYS_E  dual_sys
	);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
BOOLEAN MNPHONE_SetNetLockPlmnList(
			BOOLEAN is_enable, 
			MN_PHONE_NET_LOCK_PLMN_LIST_T *list,
			BOOLEAN is_only_check_mcc
			);
/*****************************************************************************/
//  Description :
//    In normal mode(not in cali mode), AFC adjust.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
void MNPHONE_AFC_ManualAdjust(void);

/*****************************************************************************/
//  Description :
//    
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
void MNPHONE_AFC_SetExternalCondition(BOOLEAN flag);

/*****************************************************************************/
//  Description :
//    
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MNPHONE_AFC_GetExternalCondition(void);

/*****************************************************************************/
//  Description :
//    Return value:
//    0 --- AFC not work
//    1 --- AFC has already worked just now, but now stopped.
//    2 --- AFC is running now
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
uint8 MNPHONE_AFC_GetStatus(void);

/*****************************************************************************/
//  Description : APP set used the locked arfcn
//  Global resource dependence : none
//  Author: cuifang.deng
//  Note:
/*****************************************************************************/
extern void MNPHONE_SetSavePowerLockArfcn(MN_DUAL_SYS_E dual_sys, BOOLEAN valid_flag, uint8 save_power_band, uint16 save_power_arfcn);

void MNPHONE_SetHotPlugCheckTimer(uint8 CheckInterval, MN_DUAL_SYS_E dual_sys);
void MNPHONE_GetHotPlugCheckTimer(uint8 * CheckInterval, MN_DUAL_SYS_E dual_sys);
void MNPHONE_GetAttachStates(
    MN_DUAL_SYS_E dual_sys,
    MN_ATTACH_STATE_E *cs_attach_state,
    MN_ATTACH_STATE_E *ps_attach_state);

/*****************************************************************************/
//  Description : return IMEI
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern BOOLEAN  MNNV_GetIMEIEx(   // TRUE if read IMEI success
              MN_DUAL_SYS_E       dual_sys,
              MN_IMEI_T imei
              );

/*****************************************************************************/
// 	Description : return NV_PLMN support flag
//	Global resource dependence : none
//  Author:       Toptian
//	Note:
/*****************************************************************************/
extern BOOLEAN  MNNV_GetNVPlmnSupportFlagEx(   // TRUE if read NV_PLMN success
			  MN_DUAL_SYS_E       dual_sys,
              BOOLEAN             *support_flag  //OUT
              );


/*****************************************************************************/
// 	Description : Set NV_PLMN support flag
//	Global resource dependence : none
//  Author:       Toptian
//	Note:
/*****************************************************************************/
extern BOOLEAN  MNNV_SetNVPlmnSupportFlagEx(   // TRUE if set NV_PLMN success
			  MN_DUAL_SYS_E       dual_sys,
              BOOLEAN             support_flag  //IN
              );

/*****************************************************************************/
//  Description : parse the byte string to certain service
//  Global resource dependence : none
//  Author:       Sunsome.Ju
//  Note:
/*****************************************************************************/
extern MN_OPERATION_E MNPHONE_ParseBStringEx(
            MN_DUAL_SYS_E dual_sys,
            uint8   *input_str,
            uint8   len,
            MN_PARSE_INFO_U     *pars_str_info_ptr
            );              
                        
 /******************************************************************************
 **                        CALL MODULE API                                    *
 ******************************************************************************/

/*****************************************************************************/
//  Description : APP task start a MO call.only one call is established at one 
//                time. 
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_StartCallEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E          dual_sys,
                      MN_CALL_TYPE_E         call_type,           // The call type:i.e"normal call,emergency call'
                      MN_CALLED_NUMBER_T     *called_num_ptr,     // The called tel number
                      MN_SUBADDR_T           *called_subaddr_ptr, // The called sub address
                      MN_CALL_CLIR_E         clir_type,           // i.e(CLIR_SUPPRESSION, CLIR_INVOCATION) 
                      MN_CALL_MINI_UTU_T     *utu_ptr             // if the user-to-user is not present 
                                                                  // then set the user_to_user_ptr be NULL
                      );

/*****************************************************************************/
//  Description : start a MO call.only one call is established at one time. 
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_StartCallWithFdnCheckEx( 
                                                MN_DUAL_SYS_E       dual_sys,            //IN:
                                                MN_CALL_TYPE_E      call_type,           // The call type:i.e"normal call,emergency call'
                                                MN_CALLED_NUMBER_T  *called_num_ptr,     // The called tel number
                                                MN_SUBADDR_T        *called_subaddr_ptr, // The called sub address
                                                MN_CALL_CLIR_E      clir_type,           // i.e(CLIR_SUPPRESSION, CLIR_INVOCATION)
                                                MN_CALL_MINI_UTU_T  *utu_ptr             // if the user-to-user is not present 
                                                                                         // then set the user_to_user_ptr be NULL
                                                );

/*****************************************************************************/
//  Description : The local MS is alerting.
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_IndicateAlertEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E  dual_sys,
                      uint8          call_id,       // The alerting call's number
                      MN_CALL_UTU_T  *utu_ptr // if the user-to-user is not present 
                                                    // then set the user_to_user_ptr be NULL
                      );

/*****************************************************************************/
//  Description : APP task accept the incoming call
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_ConnectCallEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E  dual_sys,
                      uint8          call_id,    // the connected call entity's id     
                      MN_CALL_UTU_T    *utu_ptr // if the user-to-user is not present 
                                                      // then set the user_to_user_ptr be NULL
                      );

/*****************************************************************************/
//  Description :  APP task disconnect the call indicated by call_id
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_DisconnectCallEx(   // MN_RETURN_SUCC    
                      MN_DUAL_SYS_E  dual_sys,
                      uint8          call_id,  // The disconnected call entity's id
                      MN_CALL_DISCONNECT_CALL_CAUSE_E  cause,    // The disconnection's cause
                      MN_CALL_UTU_T    *utu_ptr // if the user-to-user is not present 
                                                    // then set the user_to_user_ptr be NULL
                      );

/*****************************************************************************/
/*  Description :  APP task starts to respond the indication of recall       */
/*  Global resource dependence : none                                        */
/*  Author: Jun.zhang                                                        */
/*  Note:                                                                    */
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_RecallRspEx(
                      MN_DUAL_SYS_E                    dual_sys,
                      uint8                            call_id,
                      BOOLEAN                          is_recall_accepted,
                      MN_CALL_DISCONNECT_CALL_CAUSE_E  reject_cause 
                      );

#ifdef GSM_CUSTOMER_AFP_SUPPORT
/*****************************************************************************/
// 	Description :  Set the IMSI REPLACEMENT test mode on/off
//	Global resource dependence : none 
//  Author: Yu.Sun
//	Note:                                                                   
/*****************************************************************************/
MN_RETURN_RESULT_E MNCALL_SetImsiReplacementModeEx(
				      MN_DUAL_SYS_E          dual_sys,			      
				      BOOLEAN                imsi_replace_on
				      );

/*****************************************************************************/
// 	Description :  Get the IMSI REPLACEMENT test mode on/off
//	Global resource dependence : none 
//  Author: Yu.Sun
//	Note:                                                                   
/*****************************************************************************/
MN_RETURN_RESULT_E MNCALL_GetImsiReplacementModeEx(
				      MN_DUAL_SYS_E          dual_sys,			      
				      BOOLEAN                *imsi_replace_on
				      );

/*****************************************************************************/
//  Description : Write SIM Lock Customize Data to NV.
//  Global resource dependence : none
//  Author: Xiaonian.Li
//  Note: 
/*****************************************************************************/
BOOLEAN MNPHONE_WriteSIMLockCustomizeData(    // TRUE: write success; FALSE: write failure
    MN_SIM_LOCK_CUSTOMIZE_DATA_T *sim_lock_customize_data    // IN:
);
#endif

/*****************************************************************************/
// Description : mmi begin erase activated CCBS
// Global resource dependence : none
// Author:  jun.zhang
// Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_EraseCcbsEx(
                        MN_DUAL_SYS_E                   dual_sys,
                        BOOLEAN                         is_all,
                        uint8                           ccbs_index
                        );

/*****************************************************************************/
//  Description :  This API function is used to disconnect an ongoing call or an 
//                 active call.The parameter of call_id must be allocated one 
//                 Parameter:
//  Global resource dependence : none
//  Author: jun.zhang
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_DisconnectCallWithCCBSEx(
                      MN_DUAL_SYS_E                     dual_sys,
                      uint8                             call_id,
                      MN_CALL_DISCONNECT_CALL_CAUSE_E   cause,
                      BOOLEAN                           is_ccbs_accepted,
                      MN_CALL_UTU_T                     *utu_ptr
                      );

/*****************************************************************************/
//  Description :  APP task release all of the existent calls
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_ReleaseActiveCallsEx( // MN_RETURN_SUCC
                             MN_DUAL_SYS_E dual_sys
                             );

/*****************************************************************************/
//  Description :  APP task hold the active call, This function can hold the MPTY
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_HoldCallEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E dual_sys,
                      uint8    call_id // the held call entity's id
                      );

/*****************************************************************************/
//  Description :  APP task retrieve the held call including a MPTY call  
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_RetrieveCallEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E dual_sys,
                      uint8    call_id // the retrieved call entity's id
                      );

/*****************************************************************************/
//  Description :   APP task retrieves the held call and holds the active call
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_SwapCallsEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E dual_sys,
                      uint8    hold_call_id,   // will be held call id
                      uint8    retrieve_call_id // will be retrieved call id
                      );

/*****************************************************************************/
//  Description :  APP task start to transfer the DTMF tone to network  
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_StartDTMFEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E          dual_sys,
                      MN_CALL_DTMF_KEY_T     DTMF_key,  // DTMF value
                      uint8                  call_id    // the active call entity's number    
                      );

/*****************************************************************************/
//  Description :  APP task stop transferring the DTMF tone to network
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_StopDTMFEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E dual_sys,
                      uint8    call_id // the id of call entity that tranfer the DTMF tone
                      );

/*****************************************************************************/
//  Description :  used to build a MPTY call 
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_BuildMPTYEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E dual_sys,
                      uint8    held_call_id,  // the call number of being held
                      uint8    active_call_id // the call number of being active
                      );

/*****************************************************************************/
//  Description :  the user create a private communication with a member of the MPTY
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_SplitMPTYEx( // MN_RETURN_SUCC
                      MN_DUAL_SYS_E dual_sys,
                      uint8    call_id // the splitted call id
                      );

/*****************************************************************************/
//  Description :  USER invoke ECT service.The Explicit Call Transfer (ECT) function 
//  should be invoked in association with two existing calls which one is
//  answered and in the held state and the other is answered and active or alerting.             
//  Global resource dependence : none
//  Author: Qing.Xi
//  Note:
/*****************************************************************************/
MN_RETURN_RESULT_E MNCALL_ECTInvokeEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E dual_sys,
                      uint8    call_id  // the call number of being held or active
                      );

/*****************************************************************************/
//  Description :   Get the state of the defined call entity
//  Global resource dependence : none
//  Author: Hanjun.liu
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_GetCallContextEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E       dual_sys,
                      uint8               call_id,      // the call id
                      MN_CALL_CONTEXT_T   *call_context_ptr
                      );              
/*****************************************************************************/
//  Description : get the detailed fail cause of call
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNCALL_CODE_E MNCALL_GetErrCodeEx(
                      MN_DUAL_SYS_E dual_sys
                      );

/*****************************************************************************/
// 	Description :  APP task ect the active/held call
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_ECTCallEx( // MN_RETURN_SUCC
					  MN_DUAL_SYS_E dual_sys,
					  uint8    active_call_id, // the active call entity's id
					  uint      held_call_id// the held call entity's id
					  );


/*****************************************************************************/
//  Description : SET the speech version info.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern void MNCall_SetSpeechVersionInfo(
                MN_DUAL_SYS_E dual_sys,
                MN_SPEECH_VERSION_INFO_T *speech_info);

/*****************************************************************************/
//  Description : Get  the speech version info.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern void MNCall_GetSpeechVersionInfo(
                MN_DUAL_SYS_E dual_sys,
                MN_SPEECH_VERSION_INFO_T *speech_info);


/******************************************************************************
 **                        SS MODULE API                                      *
 ******************************************************************************/
/*****************************************************************************/
//  Description : mn-stk call this function to send SS after envelope confirmation
//  Global resource dependence : none
//  Author: Guisen.Deng
//  Note: 
/*****************************************************************************/
MNSS_SENDSS_RESULT_E MNSS_CallControl_SendSSEx(
                                    MN_DUAL_SYS_E                 dual_sys,
                                    uint8*                        ss_string,
                                    uint8*                        ss_len,
                                    BOOLEAN                       is_sim_allow
                                    );

/*****************************************************************************/
//  Description : mn-stk call this function to send USSD after envelope confirmation
//  Global resource dependence : none
//  Author: yongxia.zhang
//  Note: 
/*****************************************************************************/
ERR_MNSS_CODE_E MNSS_CallControlOriginateUSSDEx(
                        MN_DUAL_SYS_E           dual_sys,
                        MN_SS_DCS_E             data_code_scheme,   // just support MMI mode, set to default value(SS_DEFAULT_DCS_VALUE)
                        uint8                   str_len,
                        uint8                   ussd_str[MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE]
                        );

/*****************************************************************************/
//  Description : mmi begin register supplementary service, send out register
//              information
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_RegisterSSInfoEx(
                        MN_DUAL_SYS_E               dual_sys,
                        MN_SS_CODE_E                ss_code,        // supplementary service code
                        MN_SS_BASIC_SERVICE_T               basic_service,  // the supplementary service used on which type service(bearer or tele or all call type)
                        MN_SS_REGISTER_PARAM_U      register_param  // parameter's union of register ss data operation
                        );

/*****************************************************************************/
//  Description : mmi begin erase supplementary service operation
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_EraseSSInfoEx(
                        MN_DUAL_SYS_E               dual_sys,
                        MN_SS_CODE_E                ss_code,        // supplementary service type
                        MN_SS_BASIC_SERVICE_T               basic_service   // the supplementary service used on which type service(bearer or tele or all call type)
                        );

/*****************************************************************************/
//  Description : mmi activate  supplementary service operation
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_ActivateSSEx(
                        MN_DUAL_SYS_E               dual_sys,
                        MN_SS_CODE_E                ss_code,        // supplementary service type
                        MN_SS_BASIC_SERVICE_T               basic_service   // the supplementary service used on which type service(bearer or tele or all call type)
                        );

/*****************************************************************************/
//  Description : mmi deactivate supplementary service operation
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_DeactivateSSEx(
                        MN_DUAL_SYS_E               dual_sys,
                        MN_SS_CODE_E                ss_code,        // supplementary service type
                        MN_SS_BASIC_SERVICE_T               basic_service   // the supplementary service used on which type service(bearer or tele or all call type)
                        );

/*****************************************************************************/
//  Description : mmi interrogate supplementary service operation
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_InterrogateServiceEx(
                        MN_DUAL_SYS_E           dual_sys,
                        MN_SS_CODE_E            ss_code,        // supplementary service type
                        MN_SS_BASIC_SERVICE_T   basic_service   // the supplementary service used on which type service(bearer or tele or all call type)
                        );
                        
/*****************************************************************************/
//  Description : mmi  register password to a supplementary service 
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_RegisterPasswordEx(
                        MN_DUAL_SYS_E           dual_sys,
                        MN_SS_CODE_E            ss_code // the supplementary service which want to add password
                        );

/*****************************************************************************/
//  Description : mmi send the user write password to MN-layer
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_SendUserPasswordEx(
                        MN_DUAL_SYS_E           dual_sys,
                        uint8                   password[MN_SS_NUM_OF_PASSWORD] // the user write password
                        );

/*****************************************************************************/
//  Description : mmi initiate a USSD service, and send the unstructured data
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_UserOriginateUSSDEx(
                        MN_DUAL_SYS_E           dual_sys,
                        BOOLEAN                 is_stk_send_ussd,                        
                        MN_SS_DCS_E             data_code_scheme,   // just support MMI mode, set to default value(MN_SS_DEFAULT_DCS_VALUE)
                        uint8                   str_len,
                        uint8                   ussd_str[MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE]
                        );

/*****************************************************************************/
//  Description : mmi response unstructured ss 
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_ResponseUSSDEx(
                        MN_DUAL_SYS_E           dual_sys,
                        MN_SS_DCS_E             data_code_scheme,
                        uint8                   str_len,
                        uint8                   ussd_str[MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE]
                        );

/*****************************************************************************/
//  Description : mmi response unstructured ss notify
//  Global resource dependence : none
//  Author: haoming.lu
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_ResponseUSSDNotifyEx(
                        MN_DUAL_SYS_E           dual_sys
                        );

/*****************************************************************************/
// 	Description : mmi calls this api to reject the ussd service with certain causes
//	Global resource dependence : none
//  Author:	xiaonian.li
//	Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_RejUssdServiceEx(
    MN_DUAL_SYS_E dual_sys,
    MN_SS_ERROR_CODE_E cause // MN_SS_ERROR_UNKNOW_ALPHABET/MN_SS_ERROR_USSD_BUSY/MN_SS_ERROR_UNEXPECTED_DATA_VALUE
);

/*****************************************************************************/
//  Description : mmi send release current supplementary service request
//  Global resource dependence : none
//  Author: Jenny Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_RelActivingServiceEx(
                        MN_DUAL_SYS_E dual_sys
                        );
                        
/*****************************************************************************/
//  Description : get current MNSS state, whether SS service exist
//  Global resource dependence : none
//  Author: Sunsome Ju
//  Note: 
/*****************************************************************************/
extern BOOLEAN MNSS_GetCurStateEx(
                         MN_DUAL_SYS_E dual_sys
                         );

/*****************************************************************************/
//  Description : mn-stk call this function to send SS after envelope confirmation
//  Global resource dependence : none
//  Author: michael.zhu
//  Note: 
/*****************************************************************************/
extern MNSS_SENDSS_RESULT_E MNSS_CallControl_SendSS(
                                    uint8*                        ss_string,
                                    uint8*                        ss_len,
                                    BOOLEAN                       is_sim_allow
                                    );

/*****************************************************************************/
//  Description : mn-stk call this function to send USSD after envelope confirmation
//  Global resource dependence : none
//  Author: michael.zhu
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_CallControlOriginateUSSD(
                        MN_SS_DCS_E             data_code_scheme,   // just support MMI mode, set to default value(SS_DEFAULT_DCS_VALUE)
                        uint8                   str_len,
                        uint8                   ussd_str[MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE]
                        );

/*****************************************************************************/
//  Description : get last sucess ss result parameter
//  Global resource dependence : none
//  Author: qing.xi
//  Note: 
/*****************************************************************************/
extern ERR_MNSS_CODE_E MNSS_GetLastSSParaEx(
                         MN_DUAL_SYS_E dual_sys,
                         MN_SS_RESULT_PARA_T*  ss_para_ptr
                         ); 

extern uint32 MNSS_CheckTestSim(MN_DUAL_SYS_E dual_sys);
/******************************************************************************
 **                        SMS MODULE API                                     *
 ******************************************************************************/

/*****************************************************************************/
//  Description : Get the SMS state and the max support number in the 
//                destination storage
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Modify: William.Qian 2005_12_19, modify status_arr
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_GetSmsStatusEx(                                           //RETURN:
                                      MN_DUAL_SYS_E         dual_sys,           //IN:
                                      MN_SMS_STORAGE_E      storage,            //IN:  the destination storage, ME or SIM
                                      MN_SMS_RECORD_ID_T    *max_sms_num_ptr,   //OUT: the max number of SMS can saved in ME or SIM
                                      MN_SMS_STATUS_E       *status_arr         //OUT: the SMS status array
                                      );
                            
/*****************************************************************************/
//  Description : Get the SMS max support number and used number in the selected 
//                system and storage
//  Global resource dependence : none
//  Author: William.Qian
//  Modify: Tracy Zhang
//  Note: Don't care the variable dual_sys, if the SMS number in NV.
/*****************************************************************************/
ERR_MNSMS_CODE_E MNSMS_GetSmsNumEx(
                                   MN_DUAL_SYS_E        dual_sys,           //IN:
                                   MN_SMS_STORAGE_E     storage,            //IN:
                                   uint16               *max_sms_num_ptr,   //OUT: the max number of SMS can saved in ME or SIM
                                   uint16               *used_sms_num_ptr   //OUT: the used number
                                   );

/*****************************************************************************/
//  Description : set storage priority, SIM first or ME first
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetStoragePriorityEx(
                                            MN_DUAL_SYS_E   dual_sys,       //IN:
                                            BOOLEAN         is_sim_first    //IN: save in SIM first
                                            );

#ifndef _BASE_COMPACT_CODE_
/*****************************************************************************/
//  Description : Delete All SMS, in SIM 
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note:
/*****************************************************************************/
ERR_MNSMS_CODE_E MNSMS_DeleteSmsAllEx(
                                    MN_DUAL_SYS_E               dual_sys,   //IN:
                                    MN_SMS_STORAGE_E            storage,    //IN: get the where store SMS's status, ME or SIM
                                    MN_SMS_DELETE_ALL_STATUS_E  sms_status  //IN: the SMS status
                                    );
#endif
/*****************************************************************************/
//  Description : Read the the SMS according to the record_id 
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_ReadSmsEx(                                //RETURN:
                                 MN_DUAL_SYS_E      dual_sys,   //IN:
                                 MN_SMS_STORAGE_E   storage,    //IN: the destination storage, ME or SIM
                                 MN_SMS_RECORD_ID_T record_id   //IN:
                                 );
                        
/*****************************************************************************/
//  Description : Set the assert sms destination address 
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetAssertSmsDestAddrEx (                    
                        MN_DUAL_SYS_E dual_sys,
                        MN_SMS_ASSERT_DEST_ADDR_ARR_T      assert_sms_dest_addr //in--assert sms destination address
                        );                      

/*****************************************************************************/
//  Description : update the SMS status in destination storage, the operation 
//                can delete an SMS or change the state from read to be read.
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_UpdateSmsStatusEx(                                    //RETURN:
                                         MN_DUAL_SYS_E          dual_sys,   //IN:
                                         MN_SMS_STORAGE_E       storage,    //IN: the destination storage, ME or SIM
                                         MN_SMS_RECORD_ID_T     record_id,  //IN:
                                         MN_SMS_STATUS_E        status      //IN: the SMS status
                                         );

/*****************************************************************************/
//  Description : APP originate an MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:   used by APP layer
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_AppSendSmsEx(                             //RETURN:
                                    MN_DUAL_SYS_E       dual_sys,   //IN:
                                    MN_SMS_MO_SMS_T     *sms_ptr,   //IN: the MO SMS structure pointer
                                    MN_SMS_STORAGE_E    sms_write,  //IN: the destination storage, ME or SIM
                                    MN_SMS_PATH_E       sms_path,   //IN: the SMS path:GSM or GPRS
                                    BOOLEAN             is_more     //IN: is exist more SMS
                                    );

/*****************************************************************************/
//  Description : ATC originate an MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:   Used by ATC layer
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendATSmsPDUEx(                               //RETURN:
                                      MN_DUAL_SYS_E      dual_sys,  //IN:
                                      MN_SMS_ADDR_T     *addr_ptr,  //IN: the SMSC address
                                      MN_L3_MSG_UNIT_T  *tpdu_ptr,  //IN: the content of MO SMS
                                      BOOLEAN           is_more     //IN:
                                      );
 
/*****************************************************************************/
//  Description : Send one STK PDU SMS message, Need to Change DCS 8bit-data to 
//                7bit default alphabet.
//  Global resource dependence : none
//  Author: Lucian.Hu.
//  Note: Used by STK module.
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SendSmsPDUEx(                                     //RETURN:
                                    MN_DUAL_SYS_E       dual_sys,           //IN:
                                    BOOLEAN             is_address_present, //IN:
                                    MN_CALLED_NUMBER_T  *addr_ptr,          //IN: the SMSC address
                                    MN_L3_MSG_UNIT_T    *tpdu_ptr,          //IN: the content of MO SMS
                                    BOOLEAN             is_need_compress,   //IN: whether user data need compressed
                                    BOOLEAN             is_more             //IN:
                                    );

/*****************************************************************************/
//  Description : APP originate an MO short message command, send out command
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_AppSendCommandEx(                                 //RETURN:
                                        MN_DUAL_SYS_E           dual_sys,   //IN:
                                        MN_SMS_COMMAND_SMS_T    *sms_ptr    //IN: the command SMS structure pointer
                                        ); 

/*****************************************************************************/
//  Description : ATC originate an MO short message command, send out command
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_AtcSendCommandEx(                                 //RETURN:
                                        MN_DUAL_SYS_E       dual_sys,       //IN:
                                        MN_SMS_ADDR_T       *sc_addr_ptr,   //IN: the SMSC address
                                        MN_L3_MSG_UNIT_T    *tpdu_ptr       //IN: the content of MO SMS
                                        );

/*****************************************************************************/
//  Description : save the MO SMS to the destination storage 
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_WriteMoSmsEx(                             //RETURN: 
                                    MN_DUAL_SYS_E       dual_sys,   //IN:
                                    MN_SMS_STORAGE_E    storage,    //IN: the destination storage, ME or SIM
                                    MN_SMS_STATUS_E     status,     //IN: the SMS status
                                    MN_SMS_MO_SMS_T     *sms_ptr    //IN: the MO SMS structure pointer
                                    );

/*****************************************************************************/
//  Description : edit the MO SMS
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_EditMoSmsEx(                                  //RETURN:
                                   MN_DUAL_SYS_E        dual_sys,   //IN:
                                   MN_SMS_RECORD_ID_T   record_id,  //IN: record id which SMS be edited
                                   MN_SMS_STORAGE_E     storage,    //IN: the destination storage, ME or SIM
                                   MN_SMS_STATUS_E      status,     //IN: the SMS status
                                   MN_SMS_MO_SMS_T      *sms_ptr    //IN: the MO SMS structure pointer
                                   );

/*****************************************************************************/
//  Description : save the MT SMS to the destination storage
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_WriteMtSmsEx(                             //RETURN:
                                    MN_DUAL_SYS_E       dual_sys,   //IN:
                                    MN_SMS_STORAGE_E    storage,    //IN: the destination storage, ME or SIM
                                    MN_SMS_STATUS_E     status,     //IN: the SMS status
                                    MN_SMS_MT_SMS_T     *sms_ptr    //IN:
                                    );

/*****************************************************************************/
//  Description : edit the MT SMS
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_EditMtSmsEx(                                  //RETURN:
                                   MN_DUAL_SYS_E      dual_sys,
                                   MN_SMS_RECORD_ID_T   record_id,      //IN:
                                   MN_SMS_STORAGE_E storage,        //IN: the destination storage, ME or SIM
                                   MN_SMS_STATUS_E  status,         //IN: the SMS status
                                   MN_SMS_MT_SMS_T  *sms_ptr        //IN:
                                   );

/*****************************************************************************/
//  Description : ATC ask to write a short message in SIM or ME
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_AtcWriteSmsEx(                                //RETURN:
                                     MN_DUAL_SYS_E      dual_sys,   //IN:
                                     MN_SMS_STORAGE_E   storage,    //IN: the destination storage, ME or SIM
                                     MN_SMS_STATUS_E    status,     //IN: the SMS status
                                     MN_SMS_ADDR_T      *addr_ptr,  //IN: the SC address
                                     MN_L3_MSG_UNIT_T   *tpdu_ptr   //IN: the content of SMS
                                     );

/*****************************************************************************/
//  Description : Read SC Address which stored in SIM card
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_GetScAddrEx(                                  //RETURN:
                                   MN_DUAL_SYS_E      dual_sys,     //IN:
                                   MN_CALLED_NUMBER_T *tp_sc_addr   //OUT: the sc address struct
                                   );

/*****************************************************************************/
//  Description : Set SC Address in SIM card
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetScAddrEx(                                  //RETURN:
                                   MN_DUAL_SYS_E        dual_sys,   //IN:
                                   MN_CALLED_NUMBER_T   *tp_sc_addr //IN: the sc address struct
                                   );

/*****************************************************************************/
//  Description : Set the SMS ASSERTION function enable or disable 
//  Global resource dependence : none
//  Author: Lucian.hu
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetSmsAssertFuncEx( 
                        MN_DUAL_SYS_E dual_sys,
                        BOOLEAN is_supported 
                        );
                                                
/*****************************************************************************/
//  Description : judge with the sms header whether the sms is MMS 
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_IsSupportMMSEx(
                    MN_DUAL_SYS_E dual_sys,
                    uint8  length,                  //in, sms data header length
                    uint8  *sms_data_header_ptr,    //in, sms user data header pointer
                    BOOLEAN  *is_support_MMS            //out, whether the sms support MMS
                    );
                    
/*****************************************************************************/
//  Description : judge whether receive sms
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetSmsReceiveFuncEx(
                    MN_DUAL_SYS_E dual_sys,
                    BOOLEAN  is_receive_sms         //in, whether receive sms
                    );

/*****************************************************************************/
//  Description : get voice message waiting indicator
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_GetVoiMessFlagEx (
                    MN_DUAL_SYS_E dual_sys
                    );                  

/*****************************************************************************/
//  Description : get voice message mailbox
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_GetVoiMessAddrEx (
                    MN_DUAL_SYS_E       dual_sys,
                    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type,
                    MN_VOICE_MAIL_TYPE_E voice_mail_type
                    );

/*****************************************************************************/
//  Description : set voice message mailbox
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetVoiMessAddrEx (
                    MN_DUAL_SYS_E       dual_sys,
                    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type,
                    MN_VOICE_MAIL_TYPE_E voice_mail_type,
                    MN_SMS_VMW_MBNUM_T  *vmw_addr    // in---voice message mailbox
                    );
                    
/*****************************************************************************/
//  Description : encode the SMS data
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_EncodeUserDataEx( 
              MN_DUAL_SYS_E             dual_sys,               //IN:
              BOOLEAN                   user_head_is_exist,     //IN: show whether the user data header is exit
              MN_SMS_ALPHABET_TYPE_E    alphabet_type,          //IN: the Data Coding Scheme structure
              MN_SMS_USER_DATA_HEAD_T   *user_data_header_ptr_t,//IN: the user data header structure
              MN_SMS_USER_VALID_DATA_T  *user_valid_data_ptr_t, //IN: the valid user data structure
              MN_SMS_USER_DATA_T        *user_data_ptr_t        //OUT:
              );

/*****************************************************************************/
//  Description : decode the SMS data
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_DecodeUserDataEx(
              MN_DUAL_SYS_E           dual_sys,               //IN:
              BOOLEAN                 user_head_is_exist,     //IN: indicate whether the user data head is exist
              MN_SMS_DCS_T            *dcs_ptr_t,             //IN: the Data Coding Scheme struct
              MN_SMS_USER_DATA_T      *user_data_ptr_t,       //IN: the user data structure before decode
              MN_SMS_USER_DATA_HEAD_T *user_data_header_ptr_t,//OUT: the user data header structure
              MN_SMS_USER_VALID_DATA_T*user_valid_data_ptr_t  //OUT: the valid user data structure
              );

/*****************************************************************************/
//  Description : decode the SMS user data head
//  Global resource dependence : none
//  Author: Qing.Xi
//  Note: decode user header more detailed
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_DecodeUserHeadEx(
             MN_DUAL_SYS_E           dual_sys,               //IN:
             MN_SMS_USER_DATA_HEAD_T *user_data_header_ptr_t,//IN: the user data head structure before decode
             MN_SMS_USER_HEAD_ARR_T  *user_head_arr_ptr      //OUT:
             );

/*****************************************************************************/
//  Description : decode the SMS user data head
//  Global resource dependence : none
//  Author: Qing.Xi
//  Note: encode user header more detailed
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_EncodeUserHeadEx(                                             //RETURN:
            MN_DUAL_SYS_E           dual_sys,               //IN:
            MN_SMS_USER_DATA_HEAD_T *user_data_header_ptr_t,//OUT: the user data head structure after encode
            MN_SMS_USER_HEAD_ARR_T  *user_head_arr_ptr      //IN: user header strut before encode
            );

/*****************************************************************************/
//  Description : decode the SMS user data head
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_DecodeUserDataHeadEx(
            MN_DUAL_SYS_E           dual_sys,               //IN:
            MN_SMS_USER_DATA_HEAD_T *user_data_header_ptr_t,//IN: the user data head structure before decode
            MN_SMS_USER_HEAD_T      *user_head_ptr_t        //OUT: the user data head structure after decode
            );

/*****************************************************************************/
//  Description : Initial the mnsms task
//  Global resource dependence : none
//  Author: Four.yang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_InitNvSmsEx(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : free the sms user data head space
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_FreeUserDataHeadSpaceEx( // return void
                MN_DUAL_SYS_E dual_sys,
                MN_SMS_USER_HEAD_T          *user_head_ptr_t
                );// in---the user data head structure to be free

/*****************************************************************************/
//  Description : Option to save the MMS notification
//  Global resource dependence : none
//  Author: Ryan
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetSaveMMSPushEx( // return void
              MN_DUAL_SYS_E      dual_sys,
              BOOLEAN            is_save
              );
                      
/*****************************************************************************/
//  Description : get hex mt sms
//  Global resource dependence : none
//  Author: William.Qian
//  Note: 
/*****************************************************************************/
extern void MNSMS_GetHexSmsMtEx(                              // return void
                       MN_DUAL_SYS_E      dual_sys,
                       MN_SMS_MT_SMS_T   *mt_sms_ptr,       // in MN_SMS_MT_SMS_T
                       uint8             *tpdu_len,         // out tpdu length
                       uint8             *rsp_ptr,          // out rsp_ptr
                       uint16            rsp_len            // in rsp_len
                       );

/*****************************************************************************/
//  Description : Convert the struct of MN MT SMS to the struct of TPDU for ipc 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
void MNSMS_MtSms2TpduEx(
                        MN_DUAL_SYS_E    dual_sys,       //IN:
                        MN_SMS_MT_SMS_T  *mn_mt_sms_ptr, //IN:
                        MN_L3_MSG_UNIT_T *tpdu_ptr       //OUT:
                        );

/*****************************************************************************/
//  Description : set if save SMS by PS
//  Global resource dependence : none
//  Author: Yongxia Zhang
//  Note: 
/*****************************************************************************/
extern void MNSMS_SetSaveSMSEx(
            MN_DUAL_SYS_E      dual_sys,
            BOOLEAN      is_ps_save_sms
            );

/*****************************************************************************/
//  Description : Set change voice msg DCS is normal SMS
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
extern void MNSMS_SetVoiceMsgToSMS(
                    MN_DUAL_SYS_E       dual_sys,
                    BOOLEAN is_dcs_change_mode
                    );

/*****************************************************************************/
//  Description : APP set not check fdn
//  Global resource dependence : none
//  Author: 
//  Note: is_not_check--TRUE, not check fdn     FALSE--check fdn
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetNotCheckFdn(
            MN_DUAL_SYS_E      dual_sys,
            BOOLEAN      is_not_check
            );

/*****************************************************************************/
//  Description : Set if receive sms use local time instead of sms centre  time stamp
//  Global resource dependence : none
//  Author: 
//  Note: is_enable: TURE--use local time   FALSE--don't use local time
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetUseLocalTime(
            MN_DUAL_SYS_E      dual_sys,
            BOOLEAN      is_enable
            );

/******************************************************************************
 **                        GPRS MODULE API                                    *
 ******************************************************************************/
/*****************************************************************************/
//  Description : returns the current minimum acceptable qos for each defined context.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadNegQosProfileEx( // return whether success or failure
    MN_DUAL_SYS_E     dual_sys,            //IN:
    MN_GPRS_OUT_QOS_T **ret_min_qos_ptr,   //OUT: the memory save the result
    uint32            *pdp_context_num     //OUT:
);

/*****************************************************************************/
//  Description : returns the current PDP context settings for each defined context.
//  Author:Panda.Luo
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadMinQosProfileEx( // return whether success or failure
                 MN_DUAL_SYS_E dual_sys,
                 MN_GPRS_OUT_QOS_T ** ret_min_qos_ptr,       // the memory save the result
                 uint32            *pdp_context_num
                 );

/*****************************************************************************/
//  Description : returns the current Quality of Service Profile for each defined context.
//  Author:Panda.Luo
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadCurrQosProfileEx( //return whether success or failure
                MN_DUAL_SYS_E dual_sys,
                MN_GPRS_OUT_QOS_T ** ret_cur_qos_ptr,       // the memory save the result
                uint32            *pdp_context_num
                 );

/*****************************************************************************/
//  Description : returns the current state for each defined context.
//  Author:Panda.Luo
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadPdpContextStateEx( // return whether success or failure
                 MN_DUAL_SYS_E dual_sys,
                 MN_GPRS_CON_STATE_T  ** state_ptr,     // the memory save the result
                 uint32   * pdp_context_num             //num of exist context
                );

/*****************************************************************************/
//  Description : returns thef current pco for each defined context.
//  Global resource dependence : none
//  Author: shijun.cui
//  Note: user should free context_ptr_ptr pointer
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadPdpContextPcoEx( // return whether success or failure
                 MN_DUAL_SYS_E dual_sys,
                 MN_GPRS_PCO_T ** ret_pco_ptr,     // the memory save the result
                 uint32  *pdp_context_num
                );

/*****************************************************************************/
//  Description : set PDP context parameter values for a PDP context.
//  Global resource dependence : none
//  Author:Panda.Luo
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetPdpContextEx(// return whether success or failure    
        MN_DUAL_SYS_E       dual_sys,
        BOOLEAN             param_used[7],      //IN: indicate if the following parameter is valid.
        uint32              pdp_id,             //IN: pdp context identify,value (1 ~ 15)
        uint8               *pdp_type_ptr,      //IN: pdp type,value("IP","PPP","EMPTY","IPv6")
        uint8               *apn_ptr,           //IN: access point name,this depends your network operator
        MN_GPRS_PDP_ADDR_T  pdp_addr,           //IN: pdp address,for IP network,it indicate ip address
        uint8               d_comp,             //IN: if apply PDP data compression:0 - off,1 - on
        uint8               h_comp,             //IN: if apply PDP header compression:0 - off,1 - on, 2-rfc1144
        uint8               *pco_str            //IN: protocol configuration option that depends on <pdp_type>
        );

/*****************************************************************************/
//  Description : set PDP context parameter values for a PDP context, such as 
//                Cingular network in USA need PAP user name and password 
//                authentification .
//  Global resource dependence : none
//  Author:shijun.cui
//  Note: input parameter user_ptr & passwd_ptr should be end with '\0'.
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetPdpContextPcoEx( // return whether success or failure    
        MN_DUAL_SYS_E dual_sys,
        uint32 pdp_id,         // pdp context identify,value (1 ~ 15)
        uint8 *user_ptr,       // protocol configuration option pap user name
        uint8 *passwd_ptr      // protocol configuration option pap user password
        );        

/*****************************************************************************/
//  Description : set PDP context parameter values for a PDP context, such as 
//                Cingular network in USA need PAP/CHAP user name and password 
//                authentification .
//  Global resource dependence : none
//  Author:winnie.zhu
//  Note: input parameter user_ptr & passwd_ptr should be end with '\0'.
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetPdpContextPcoCipherEx( // return whether success or failure    
        MN_DUAL_SYS_E dual_sys,
        uint32 pdp_id,         // pdp context identify,value (1 ~ 15)
        uint8 *user_ptr,       // protocol configuration option pap/chap user name
        uint8 *passwd_ptr,     // protocol configuration option pap/chap user password
        MN_PCO_AUTH_TYPE_E auth_type
        );

#ifdef HUAWEI_6500_MODEM
/*****************************************************************************/
//  Description : set PDP context parameter values for a PDP context, such as 
//                Cingular network in USA need PAP user name and password 
//                and challenge authentification .
//  Global resource dependence : none
//  Author:
//  Note: input parameter user_ptr & passwd_ptr should be end with '\0'.
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetPdpContextPcoNewCipherEx(                        //RETURN: whether success or failure
                                          MN_DUAL_SYS_E    dual_sys,   //IN:
                                          uint32        pdp_id,     //IN: pdp context identify,value (1 ~ 15)
                                          uint8            *user_ptr,  //IN: protocol configuration option pap user name
                                          uint8            *passwd_ptr, //IN: protocol configuration option pap user password
                                          MN_PCO_AUTH_TYPE_E auth_type,
                                          uint8            *challen_ptr,
                                          uint8            challen_len,
                                          uint8            *cha_name_ptr,
                                          uint8            cha_name_len
                                          );
#endif

/*****************************************************************************/
//  Description : request to delete PDP contexts.
//  Author:Panda.Luo
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E  MNGPRS_DeletePdpContextEx( // return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        BOOLEAN is_all,         // identify if all PDP contexts need delete.
        uint32 pdp_id           // the identity of the PDP context will be deleted.
        );
        
/*****************************************************************************/
//  Description : set Quality of Service profile for a PDP context.
//  Global resource dependence : none
//  Author:Panda.Luo
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetQosProfileEx( // return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        BOOLEAN param_used[3],          // Indicate if the following prarameter is valid.
        uint32 pdp_id,                  // pdp context identify,value (1 ~ 15).
        MN_GPRS_RELIABILITY_TYPE_E reliability,            // specifies the reliability class.
        MN_GPRS_PEAK_THROUGHPUT_TYPE_E peak_throughput     // specifies the peak throughput class
        );

/*****************************************************************************/
//  Description : set total Quality of Service profile for a PDP context
//  Global resource dependence : none
//  Author:Hyman.wu
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetTotalQosProfileEx( // return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        uint32 pdp_id,                  // pdp context identify,value (1 ~ 15).
        MN_GPRS_TOTAL_QOS_T in_qos         // the input qos parameter
        );     
        
        
/*****************************************************************************/
//  Description : set minimum acceptable Qos Profile for a PDP context.
//  Global resource dependence : none
//  Author: Panda.Luo
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetMinQosProfileEx( // return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        BOOLEAN param_used[3],       // Indicate if the following prarameter is valid.
        uint32 pdp_id,               // pdp context identify,value (1 ~ 15)
        MN_GPRS_RELIABILITY_TYPE_E     reliability,         // specifies the reliability class.
        MN_GPRS_PEAK_THROUGHPUT_TYPE_E peak_throughput      // specifies the peak throughput class
        );
        

/*****************************************************************************/
//  Description : set minimum acceptable QoS Profile for a PDP context.
//  Global resource dependence : none
//  Author: hyman.wu
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetTotalMinQosProfileEx( // return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        uint32 pdp_id,               // pdp context identify,value (1 ~ 15)
        MN_GPRS_TOTAL_QOS_T in_qos         // the input qos parameter
        );        
        
/*****************************************************************************/
//  Description : request to specify a minimum acceptable Profile for a PDP context.
//  Author:Xiaogang.Xu
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeactivatePdpContextEx( //return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        BOOLEAN is_all,         // identify if all PDP contexts need deactivate.
        uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT]            
                                // the identity of the PDP contexts will be deactivated.
        );

/*****************************************************************************/
//  Description : request to activate PDP contexts.
//  Author:Xiaogang.Xu
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ActivatePdpContextEx( //return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        BOOLEAN is_all,         // identify if all PDP contexts need activate.
        uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT],         
                                // the identity of the PDP contexts will be activated.
        MN_PS_SERVICE_TYPE_E ps_service_type,
        MN_PS_SERVICE_RAT_E  ps_service_rat
        );

/*****************************************************************************/
//  Description : returns the current addr of specific context.
//  Author      : Xiaogang.Xu
//  Note        :
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E  MNGPRS_ReadPdpContextAddrEx( //return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        uint32 pdp_id_arr[],                  //in, the identity array of the PDP contexts 
        MN_GPRS_PDP_ADDR_T **addr_ptr          // out, addrss array of the input id
        );

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
//  Description : 
//  Global resource dependence : none
//  Author: xiaonian.li
//  Note:
/*****************************************************************************/
extern uint8 MNGPRS_GetNsapiByCidEx(MN_DUAL_SYS_E  dual_sys, uint8 cid);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: xiaonian.li
//  Note:
/*****************************************************************************/
extern uint8 MNGPRS_GetCidByNsapiEx(MN_DUAL_SYS_E  dual_sys, uint8 nsapi);

//  Function    : MNGPRS_SetAndActivePdpContext
//  Description : The function is called by app set pdp context and active
//                pdp context.this function is usually called by app to simplify
//                the acitve pdp context procedure
//  Global      : none
//  Author      : hyman.wu
//  Note        :
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetAndActivePdpContextEx(
            MN_DUAL_SYS_E dual_sys,
            uint8* APNcn,  //access point name
            MN_PS_SERVICE_TYPE_E ps_service_type
            );

/*****************************************************************************/
//  Function    : MNGPRS_ResetAndDeactivePdpContext
//  Description : The function is called by app deactivate pdp context that are 
//                activated by MNGPRS_SetAndActivePdpContext.
//                this function is usually called by app to simplify the deacitve 
//                pdp context procedure
//  Global      : none
//  Author      : hyman.wu
//  Note        :
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ResetAndDeactivePdpContextEx(
            MN_DUAL_SYS_E dual_sys
            );

        
                                    
//Begin of GPRS data section
/*************************************************************************/
//    Description: called by GPRS user to reg function that used by GPRS PS send 
//    data to GPRS user
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegReceiveDataEx( // return whether success or failure
                    MN_DUAL_SYS_E dual_sys,
                    MNGPRS_RECEIVEDATA_FPTR callback_ptr // call back function pointer
                    );

/*************************************************************************/
//    Description: called by GPRS user to dereg function that used by GPRS PS send 
//    data to GPRS user
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregReceiveDataEx(// return whether success or failure
                    MN_DUAL_SYS_E dual_sys
                    );

/*************************************************************************/
//    Description: called by GPRS user to send data through GPRS PS  
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SendDataReqEx( // return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        uint8 *data_ptr,        // pointer of sent data
        uint32 data_size,   // size of sent data
        uint16 nsapi            // nsapi number used by GPRS
        );

/*************************************************************************/
//    Description: called by gprs user to reg the function used by GPRS PS  
//    to trigger flow control
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegSetFlowControlEx( // return whether success or failure
                    MN_DUAL_SYS_E dual_sys,
                    MNGPRS_FLOWCONTROL_FPTR callback_ptr // call back function pointer
                    );
                    
/*************************************************************************/
//    Description: called by GPRS user to dereg the function used by GPRS PS
//    to trigger flow control 
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregSetFlowControlEx( // return whether success or failure
                    MN_DUAL_SYS_E dual_sys
                    );
                                                

/*************************************************************************/
//    Description: called by gprs user to reg the function that used by GPRS PS 
//                 to cancel flow control 
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_RegResetFlowControlEx( // return whether success or failure
                    MN_DUAL_SYS_E dual_sys,
                    MNGPRS_FLOWCONTROL_FPTR callback_ptr // call back function pointer
                    );
                    
/*************************************************************************/
//    Description: called by GPRS user to dereg the function that used by GPRS PS
//                 to cancel gprs flow control 
//    Author: Ivan Yin
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_DeregResetFlowControlEx( // return whether success or failure
                    MN_DUAL_SYS_E dual_sys
                    );

/*****************************************************************************/
//  Description : returns the current PDP context settings for each defined context.
//  Global resource dependence : none
//  Author:Panda.Luo
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadPdpContextEx(										   //RETURN: whether success or failure
										MN_DUAL_SYS_E		dual_sys,			   //IN:
										MN_GPRS_PDP_CON_T	**ret_pdp_context_ptr, //OUT: the memory save the result
										uint32				*pdp_context_num       //OUT: num of exist context
										);
/*****************************************************************************/
//  Description : returns whether activated pdp context exist.
//  Global resource dependence : none
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
extern BOOLEAN MNGPRS_IsActPdpExistEx( void );

/*****************************************************************************/
//  Description : 1. turn on or off: Manual response to a network request for PDP context activation
//                2. Based on S0
//  Global      : none
//  Author      : winnie.zhu
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetResModeEx( //return whether success or failure
    MN_DUAL_SYS_E dual_sys,
    MN_GPRS_TE_RESPONSE_MODE_E res_mode //the requested response mode
);

/*****************************************************************************/
//  Description : show reponse mode set by GetResMode
//  Global      : none
//  Author      : winnie.zhu
/*****************************************************************************/
extern MN_GPRS_TE_RESPONSE_MODE_E  MNGPRS_GetResModeEx( //return whether success or failure
    MN_DUAL_SYS_E dual_sys
);

//End of GPRS data section

/******************************************************************************
 **                        PHONEBOOK API                                      *
 ******************************************************************************/
/*****************************************************************************/
//  Description : find phonebook specify entry by name
//  Global resource dependence : none
//  Author:       fancier.fan
//  Modify:       Tracy Zhang
//  Note:         
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_FindPhonebookEntryByAlphaEx( // false if not found
                MN_DUAL_SYS_E           dual_sys,
                PHONEBOOK_STORAGE_E     storage,        // IN:
                MN_STRING_T             *alpha_ptr,     // IN:
                PHONEBOOK_ENTRY_T       *entry_ptr      // IN/OUT: entry data
                );

/*****************************************************************************/
//  Description : find phonebook specify entry by name
//  Global resource dependence : none
//  Author:       fancier.fan
//  Modify:       wuding.yang
//  Note:         
/*****************************************************************************/
extern  ERR_MNDATAMAG_CODE_E PHONEBOOK_FindPhonebookEntryByAlphaEx_WithStartID( // false if not found
                MN_DUAL_SYS_E           dual_sys,       // IN
                PHONEBOOK_STORAGE_E     storage,        // IN:
                uint16 start_index,                             //IN: from start_index to find
                MN_STRING_T             *alpha_ptr,     // IN:
                PHONEBOOK_ENTRY_T       *entry_ptr      // IN/OUT: entry data
                );

/*****************************************************************************/
//  Description : find phonebook specify entry by number
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:         
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_FindPhonebookEntryByNumEx( // false if not found
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_STORAGE_E storage,
                MNBCD             *num_ptr,        // number array, BCD format
                uint8               num_len,        // number length
                PHONEBOOK_ENTRY_T   *entry_ptr       // out: entry data
                );

/*****************************************************************************/
//  Description : delete phonebook specify entry 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:         
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_DeletePhonebookEntryEx(
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_STORAGE_E storage,
                uint32              entry_id
                );

/*****************************************************************************/
//  Description : write phonebook specify entry 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:         
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_UpdatePhonebookEntryEx( // false if update fail
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_STORAGE_E storage,
                PHONEBOOK_ENTRY_T  *entry_ptr       // in: entry data
                );

/*****************************************************************************/
//  Description : get phonebook specify entry 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:         
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadPhonebookEntryEx(   // false if no entry about the id
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_STORAGE_E storage,
                PHONEBOOK_ENTRY_T* entry_ptr    // out:entry data
                );

/*****************************************************************************/
//  Description : get phonebook info
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:         
/*****************************************************************************/
extern PHONEBOOK_INFO_T PHONEBOOK_GetFileInfoEx(
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_STORAGE_E storage
                );

/*****************************************************************************/
//  Description : enable Fixed Dialing Number service in SIM card
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_SetFDNServiceEx(    // MN_RETURN_SUCCESS
                MN_DUAL_SYS_E dual_sys,
                BOOLEAN is_service_enable,   // TRUE: enable, FALSE: disable
                uint32 task_id
                );

/*****************************************************************************/
// 	Description : check if the num exist in FDN
//	Global resource dependence : none
//  Author:       xiaonian.li
//	Note:
/*****************************************************************************/
extern BOOLEAN PHONEBOOK_CheckFdnEx( // return TRUE if the num exist in FDN
                           MN_DUAL_SYS_E        dual_sys,   //IN:
                           MN_CALLED_NUMBER_T  *num_ptr    //IN:
                           );

/*****************************************************************************/
//  Description : Re read the PB from SIM card.
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_ReReadPhoneBookEx(
                MN_DUAL_SYS_E dual_sys
                );
        

/*****************************************************************************/
// 	Description : find the first freed contact id of this sim card storage
//	Global resource dependence : none
//  Author:       
//  Modify:       
//	Note:
/*****************************************************************************/
extern uint32 PHONEBOOK_GetFirstFreeContactId(   // 0 if not found
    MN_DUAL_SYS_E dual_sys,      // IN: sim tyle
    PHONEBOOK_STORAGE_E storage //IN: sim storage
	);

/*****************************************************************************/
//  Description : judge if  voice mail enable or disable
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern BOOLEAN PHONEBOOK_IsVoiceMailAddrEnableEx(
                MN_DUAL_SYS_E dual_sys
                );

/*****************************************************************************/
//  Description : enable/disable voice mail 
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_EnableVoiceMailAddrEx(
                MN_DUAL_SYS_E dual_sys,
                BOOLEAN is_enable
                );

/*****************************************************************************/
//  Description : update voice mail addr
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_UpdateVoiceMailAddrEx(
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_ENTRY_T   *addr_ptr
                );
            
/*****************************************************************************/
//  Description : read voice mail addr
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadVoiceMailAddrEx(
                MN_DUAL_SYS_E       dual_sys,
                PHONEBOOK_ENTRY_T   *addr_ptr
                );
            
/*****************************************************************************/
//  Description : get free ext info from sim
//  Global resource dependence : none
//  Author:       Sunsome.Ju
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E PHONEBOOK_GetFreeExtInfoEx(
                MN_DUAL_SYS_E dual_sys,
                PHONEBOOK_STORAGE_E storage
                );

/*****************************************************************************/
//  Description : initial Acm unit in AOC
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_InitAcmUnitEx( 
                MN_DUAL_SYS_E dual_sys
                );

/*****************************************************************************/
//  Description : read Acm unit
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_ReadAcmUnitEx( 
                MN_DUAL_SYS_E dual_sys
                );

/*****************************************************************************/
//  Description : update AOC in SIM about acm max
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MNSIM_AOC_T MNSIM_GetAocEx( // return AOC info
              MN_DUAL_SYS_E dual_sys
              );

#ifdef MN_AOC_SUPPORT
/*****************************************************************************/
//  Description : update AOC in SIM about acm max
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_UpdateAcmMaxEx( // return AOC info
                MN_DUAL_SYS_E dual_sys,
                uint32 acm_max
                );

/*****************************************************************************/
//  Description : initial Line2 Acm unit in AOC
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_InitLine2AcmUnitEx( 
                MN_DUAL_SYS_E dual_sys
                );

extern void MNSIM_GetPuctEx(
              MN_DUAL_SYS_E dual_sys
              );

extern void MNSIM_UpdatePuctEx(
              MN_DUAL_SYS_E dual_sys,
              MNSIM_PUCT_INFO_T puct_info
              );
#endif

/*****************************************************************************/
//  Description : get ECC in SIM
//  Global resource dependence : none
//  Author:       william.qian
//  Note:
/*****************************************************************************/
extern MNSIM_ECC_T MNSIM_GetEccEx( // return ECC info
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : decode the refresh file list to file name
//  Global resource dependence : none
//  Author:       yongxia
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_DecodeFilelistEx(
         uint8 card_index,
         uint8 data_len, //the data length count from 0x92 byte
         uint8* data_buf,//the first byte is 0x92
         SIMAT_DO_FILE_LIST_T *file_list
         );
         
/*****************************************************************************/
// 	Description : read IMEISVN and Check Digit
//	Global resource dependence : none
//  Author:       chris.liu
//	Note:
/*****************************************************************************/
extern BOOLEAN MNNV_GetIMEISVNEx(   // TRUE if read IMEISVN success
              MN_DUAL_SYS_E       dual_sys,
              MN_IMEISVN_T*       imeisvn
              );

/*****************************************************************************/
//  Description : get pin status
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MNSIM_PIN_STATUS_E MNSIM_GetPinStatusEx(
        MN_DUAL_SYS_E dual_sys,
        MNSIM_PIN_INDICATION_E  pin_ind
        );

/*****************************************************************************/
//  Description : if need input CHV1 value when power on
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern BOOLEAN  MNSIM_IsPin1EnableEx(
        MN_DUAL_SYS_E dual_sys
        );

/*****************************************************************************/
//  Description : get hplmn in sim card
//  Global resource dependence : none
//  Author:       Yu.Sun
//  Note:
/*****************************************************************************/
extern MN_PLMN_T MNSIM_GetHplmnEx( 
              MN_DUAL_SYS_E dual_sys
              );

#ifndef GSM_CUSTOMER_AFP_SUPPORT
/*****************************************************************************/
// 	Description : get eplmn list from nvram
//	Global resource dependence : none
//  Author:       xiaonian.li
//	Note:
/*****************************************************************************/
extern BOOLEAN  MNNV_GetEPLMNListEx(   // TRUE if read EPLMN success
              MN_DUAL_SYS_E            dual_sys,
              MN_PHONE_EPLMN_LIST_T    *eplmn_list_ptr
              );
#endif

/*****************************************************************************/
//  Description : get imsi in sim card
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern MN_IMSI_T MNSIM_GetImsiEx( 
              MN_DUAL_SYS_E dual_sys
              );

/*****************************************************************************/
//  Description : get pin or puk remain input times for try
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern uint16   MNSIM_GetPinOrPukFalseRemainEx(
        MN_DUAL_SYS_E           dual_sys,
        MNSIM_PIN_INDICATION_E  pin_ind,
        BOOLEAN                 chv_is_blocked
        );      
              
/*****************************************************************************/
//  Description : get pin remain input times for try
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern uint16   MNSIM_GetPinFalseRemainEx(
        MN_DUAL_SYS_E           dual_sys,
        MNSIM_PIN_INDICATION_E  pin_ind
        );

              
/*****************************************************************************/
//  Description : get ccid of sim
//  Global resource dependence : none
//  Author:       fancier.fan
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetICCIDEx(
        MN_DUAL_SYS_E   dual_sys,
        MNSIM_ICCID_T   *mn_iccid_ptr 
        );
        
        
/*****************************************************************************/
//  Description : get spn from sim,same data format as saved in sim
//  Global resource dependence : none
//  Author: hyman.wu
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSIM_GetSPNEx(
        MN_DUAL_SYS_E dual_sys,
        MNSIM_SPN_T *mn_spn_ptr 
        );        

/*****************************************************************************/
//  Description : save SMS voice message indicator
//  Global resource dependence : none
//  Author:       sunsome.ju
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_SetSmsVoimsgIndEx(
         MN_DUAL_SYS_E dual_sys,
         MN_SMS_VMW_IND_FLAG_T *vmw_flag           // voice message indicator
         );                

/*****************************************************************************/
//  Description : get SMS voice message indicator
//  Global resource dependence : none
//  Author:       sunsome.ju
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetSmsVoimsgIndEx(
         MN_DUAL_SYS_E dual_sys
         );

/*****************************************************************************/
//  Description : get SMS voice message mailbox
//  Global resource dependence : none
//  Author:       sunsome.ju
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetVoiMessAddrEx(
         MN_DUAL_SYS_E dual_sys,
         MN_MULTI_SUBSCRIBER_ID_TYPE_E line_index,
         MN_VOICE_MAIL_TYPE_E voice_mail_type
         );

/*****************************************************************************/
//  Description : set SMS voice message mailbox
//  Global resource dependence : none
//  Author:       sunsome.ju
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_SetVoiMessAddrEx(
         MN_DUAL_SYS_E      dual_sys,
                    MN_MULTI_SUBSCRIBER_ID_TYPE_E line_type,
                    MN_VOICE_MAIL_TYPE_E voice_mail_type,
         MN_SMS_VMW_MBNUM_T *vmw_addr    // in---voice message mailbox
         );

/*****************************************************************************/
//  Description : read the sim file by sim file id
//  Global resource dependence : none
//  Author: shijun
//  Note:
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_ReadFileBufByPathEx(
                      MN_DUAL_SYS_E         dual_sys,
                      MN_SIM_FILE_PROFILE_T profile
                      );

/*****************************************************************************/
//  Description : initialize a SIM card info query request for SIM / USIM
//  Global resource dependence : none
//  Author: shijun
//  Note:
/*****************************************************************************/
#ifdef PRODUCT_DM
SIMCHECK_GET_SIM_TYPE_CNF_T MNSIM_GetSimType(void);
#else
ERR_MNDATAMAG_CODE_E MNSIM_GetSimType(
                      void
                      );
#endif

/*****************************************************************************/
//  Description : initialize a SIM card info query request for SIM / USIM
//  Global resource dependence : none
//  Author: Eddie.Wang
//  Note: 2G/3G API integration
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_GetSimTypeEx(
                      MN_DUAL_SYS_E dual_sys
                      );

/*****************************************************************************/
//  Description : get uim card imsi
//  Global resource dependence : none
//  Author: shijun
//  Note:
/*****************************************************************************/
#ifdef PRODUCT_DM
MN_IMSI_T MNSIM_GetUImsi(void);
#else
ERR_MNDATAMAG_CODE_E MNSIM_GetUImsi(
                      void
                      );
#endif

ERR_MNDATAMAG_CODE_E MNSIM_GetUImsiEx(
                      MN_DUAL_SYS_E dual_sys
                      );


/*****************************************************************************/
//  Description : request the lower layer start the smscb service 
//  Global resource dependence : none
//  Author: Four.yang
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSMSCB_StartCbMsgEx(    // return whether success or failure 
                            MN_DUAL_SYS_E dual_sys,
                            MN_CB_MSG_ID_MUSTER_T       *msg_id_ptr,
                            MN_CB_LANGUAGE_ID_MUSTER_T  *language_ptr
                            ) ;

/*****************************************************************************/
//  Description : request the lower layer stop the smscb service 
//  Global resource dependence : none
//  Author: Four.yang
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSMSCB_StopCbMsgEx (  // return whether success or failure 
                         MN_DUAL_SYS_E dual_sys                
                         );

/*****************************************************************************/
//  Description : request the lower layer start the smscb service 
//  Global resource dependence : none
//  Author: Four.yang
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSMSCB_ChangeCbParamEx(    // return whether success or failure 
                            MN_DUAL_SYS_E dual_sys,
                            MN_CB_MSG_ID_MUSTER_T       *msg_id_ptr,
                            MN_CB_LANGUAGE_ID_MUSTER_T  *language_ptr
                            );

/*****************************************************************************/
//  Description : request the lower layer to get the cb param. 
//  Global resource dependence : none
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSMSCB_GetCbParamEx(    // return whether success or failure 
                                           MN_DUAL_SYS_E               dual_sys
                                           );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSMSCB_ConvertPdu2TextEx(
    MN_DUAL_SYS_E  dual_sys,       //IN:
    uint8          *msg_arr_ptr,   //OUT:
    int16          *msg_len_ptr,   //OUT:
    MN_SMSCB_DCS_T *dcs_ptr,       //OUT:
    uint8          *pdu_buf_ptr,   //IN:
    uint8          pdu_len         //IN:
);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern void MNSMSCB_SetAllChannelStatusEx(    // return whether success or failure 
                                   MN_DUAL_SYS_E    dual_sys,
                                   BOOLEAN          is_cb_all_channels
                                   );

/*****************************************************************************/
//  Description : Get the info of GID
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetGidEx( 
              MN_DUAL_SYS_E dual_sys,
              MNSIM_GID_E gid_type
              );
/*****************************************************************************/
//  Description : get CPHS info in sim card
//  Global resource dependence : none
//  Author: shijun.cui
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetCphsInfoEx(
        MN_DUAL_SYS_E dual_sys,
        MN_CPHS_INFO_T  *cphs_info_ptr 
        );

/*****************************************************************************/
//  Description : get diverted call status in sim
//  Global resource dependence : none
//  Author:      shijun.cui
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetDivertedCallStatusEx(
            MN_DUAL_SYS_E dual_sys,
            MN_CFF_T *call_forward_flag
            );

/*****************************************************************************/
//  Description : update diverted call status in sim 
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_UpdateDivertedCallStatusEx(
            MN_DUAL_SYS_E                   dual_sys,
            MN_BEARER_CAPABILITY_TYPE_E     line_type, // line type
            BOOLEAN                         enable     // flag of diverted call status
            );

/*****************************************************************************/
//  Description : get the operator name in sim 
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetOperatorNameEx(
            MN_DUAL_SYS_E dual_sys,
            MN_OPER_NAME_T *oper_name_ptr
            );

/*****************************************************************************/
//  Description : get customer service profile in sim 
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetCustomerServiceProfileEx(
            MN_DUAL_SYS_E dual_sys,
            MN_CSP_SERVICES_GROUP_T * services_group
            );

/*****************************************************************************/
//  Description : update customer service profile in sim 
//  Global resource dependence : none
//  Author:       shijun.cui
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_UpdateCustomerServiceProfileEx(
            MN_DUAL_SYS_E             dual_sys,
            MN_CSP_SERVICES_GROUP_T * services_group
            );

/*****************************************************************************/
//  Description : support the at command "Restricted SIM access"
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_CrsmEx(
                                    MN_DUAL_SYS_E           dual_sys,
                                    MN_CRSM_COMMAND_TYPE_E  command, 
                                    uint8     p1,
                                    uint8     p2,
                                    uint8     p3,
                                    uint16    file_id,
                                    uint8     *data_ptr,
                                    uint8     path_len,
                                    uint8     *path_id);

/*****************************************************************************/
//  Description : support the at command "Restricted SIM access"
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note:
/*****************************************************************************/
extern BOOLEAN MNSIM_GetSimStatusEx(                                   //RETURN: TRUE, SIM is ready
                             MN_DUAL_SYS_E  dual_sys,           //IN:
                             MNSIM_ERROR_E  *error_code_ptr     //OUT:
                             );

/*****************************************************************************/
//  Description : check if SIM is present in the ME
//  Global resource dependence : 
//  Author: Jacky.ying
//  Note:
/*****************************************************************************/
extern BOOLEAN MNSIM_GetSimPresentStatusEx(      //RETURN: TRUE, SIM is present in the ME
                             MN_DUAL_SYS_E  dual_sys           //IN:
                             );

/*****************************************************************************/
//  Description : get the "enabled or not" status of SDN
//  Global resource dependence : 
//  Author: Jacky.ying
//  Note:
/*****************************************************************************/
extern BOOLEAN MNSIM_GetSdnStatusEx(                                   //RETURN: TRUE, SDN is enabled
                             MN_DUAL_SYS_E  dual_sys           //IN:
                              );


/*****************************************************************************/
// 	Description : Get EHPLMN in SIM
//	Global resource dependence : none
//  Author:       Yu.Sun
//	Note:         return TRUE if EHPLMN is present
/*****************************************************************************/
extern BOOLEAN MNSIM_GetEHPLMNEx(    
              MN_DUAL_SYS_E            dual_sys,           //IN:
              MN_PHONE_EHPLMN_LIST_T   *ehplmn_list_ptr    //OUT:  EHPLMN
              );

/*****************************************************************************/
//  Description : get the max num of the record file stored in the SIM
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetSimFileRecordNumEx(                                     //RETURN: TRUE, SIM is ready
                                                 MN_DUAL_SYS_E            dual_sys,   //IN:
                                                 MNSIM_SERVICE_TYPE_PH2_E mn_file_name//IN:
                                                 );

/*****************************************************************************/
//  Description : get the entry of the record file stored in the SIM
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetSimFileRecordEntryEx(                                         //RETURN: 
                                                   MN_DUAL_SYS_E            dual_sys,       //IN:
                                                   MNSIM_SERVICE_TYPE_PH2_E mn_file_name,   //IN:
                                                   uint8                    record_id       //IN:
                                                   );

/*****************************************************************************/
//  Description : get SPDI from sim
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNSIM_GetSPDIEx(
                                   MN_DUAL_SYS_E    dual_sys,
                                   MNSIM_SPDI_T     *spdi_ptr
                                   );

/*****************************************************************************/
//  Description : get the binary file stored in the SIM
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_GetSimBinaryFileEx(                                     //RETURN: 
                                              MN_DUAL_SYS_E            dual_sys,   //IN:
                                              MNSIM_SERVICE_TYPE_PH2_E mn_file_name//IN:
                                              );

/*****************************************************************************
 	Description : 
	Global resource dependence : none
    Author:       wuding.yang
  	Note:  
*****************************************************************************/
extern void MNSIM_GetLanguageEx(    
              MN_DUAL_SYS_E            dual_sys,           //IN:
              MN_SIM_LANGUAGE_LIST_T   *language_list_ptr  //OUT:
              );

/*****************************************************************************/
//  Description :   select current using LINE number for call service (CPHS).
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_UpdateLineEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E                    dual_sys,
                      MN_BEARER_CAPABILITY_TYPE_E      bc_type      // the request line type
                      );

/*****************************************************************************/
//  Description : support the at command CSIM
//  Global resource dependence : 
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_CsimEx(
                                    MN_DUAL_SYS_E           dual_sys,
                                    uint8     data_len,
                                    uint8     *data_ptr
                                    );

/*****************************************************************************/
//  Description :   read current using LINE number for call service (CPHS).
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_ReadLineEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E                   dual_sys,
                      MN_BEARER_CAPABILITY_TYPE_E     *bc_type      // the response line num
                      );

/*****************************************************************************/
//  Description : get hex mo sms
//  Global resource dependence : none
//  Author: William.Qian
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_GetHexSmsMoEx (                             // return void
                  MN_DUAL_SYS_E   dual_sys,
                  MN_SMS_MO_SMS_T   *mo_sms_ptr,     // in MN_SMS_MO_SMS_T
                  uint8             *tpdu_len,       // out tpdu length
                  uint8             *rsp_ptr,        // out rsp_ptr
                  uint16            rsp_len          // in rsp_len
                       );

/*****************************************************************************/
//  Description : add SC Address
//  Global resource dependence : none
//  Author: William.Qian
//  Note: 
/*****************************************************************************/
extern void MNSMS_AddScAddrEx (                                    // return void
                  MN_DUAL_SYS_E         dual_sys,
                  MN_CALLED_NUMBER_T   *sc_addr_ptr,      // in sc_addr
                  MN_L3_MSG_UNIT_T     *tpdu              // in/out tpdu with sc_addr
                     );
                         
/*****************************************************************************/
//  Description : MNPHONE_GetPlmnInfo
//  Global resource dependence : none
//  Author: William.Qian
//  Note: 
/*****************************************************************************/
extern void MNPHONE_GetPlmnInfoEx(
                         MN_DUAL_SYS_E   dual_sys,
                         MSG_CAMP_STATUS_T* plmn_info_ptr
                         );

/*****************************************************************************/
//  Description : MNPHONE_SetIsSwitchToCali
//  Global resource dependence : g_is_switch_to_cali
//  Author:       William.Qian
//  Note:
/*****************************************************************************/
extern void MNPHONE_SetIsSwitchToCali(
                            BOOLEAN is_switch_to_cali
                            );

/*****************************************************************************/
//  Description : MNPHONE_GetIsSwitchToCali
//  Global resource dependence : g_is_switch_to_cali
//  Author:       William.Qian
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetIsSwitchToCali(
                            void
                            );

/****************************************************************/                            
//  Description : get the last cause of sms sending failure
//  Global resource dependence : none
//  Author: qing.xi
//  Note: 
/*****************************************************************************/
#ifdef PRODUCT_DM
extern MN_SMS_MO_FAIL_CAUSE_E MNSMS_GetLastMoFailCauseEx(
                            MN_DUAL_SYS_E   dual_sys
                            );
                            
#else                            
extern MN_SEND_SMS_SMS_CAUSE_T  MNSMS_GetLastMoFailCauseEx(
                            MN_DUAL_SYS_E   dual_sys
                            );
#endif /*PRODUCT_DM*/
/******************************************************************************
 **                        ENGINEERING API                                    *
 ******************************************************************************/

/*****************************************************************************/
//  Description : Get the phone info 
//  Global resource dependence : none
//  Author: William Qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetPhoneInfoEx (
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_PHONE_INFO_T *phone_info
                            );

/*****************************************************************************/
//  Description : Get the cell basic info 
//  Global resource dependence : none
//  Author: William Qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetCellBasicInfoEx (
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_CELL_BASIC_INFO_T *cell_basic_info
                            );
                            
/*****************************************************************************/
//  Description : Get the cell idle info 
//  Global resource dependence : none
//  Author: William Qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetCellIdleInfoEx (
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_CELL_IDLE_INFO_T *cell_idle_info
                            );

/*****************************************************************************/
//  Description : Get the scell control info 
//  Global resource dependence : none
//  Author: William Qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetSCellCtrlInfoEx (
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_SCELL_CTRL_INFO_T *scell_ctrl_info
                            );

/*****************************************************************************/
//  Description : Get the ncell control info 
//  Global resource dependence : none
//  Author: William Qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetNCellCtrlInfoEx (
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_NCELL_CTRL_INFO_T *ncell_ctrl_info
                            );

/*****************************************************************************/
//  Description : Get the scell traffic info 
//  Global resource dependence : none
//  Author: William Qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetSCellTrafInfoEx (
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_SCELL_TRAF_INFO_T *scell_traf_info
                            );

/*****************************************************************************/
//  Description : Get the ncell traffic info 
//  Global resource dependence : none
//  Author: William Qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetNCellTrafInfoEx (
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_NCELL_TRAF_INFO_T *ncell_traf_info
                            );
                            
/*****************************************************************************/
//  Description : Get the GPRS info 
//  Global resource dependence : none
//  Author: William Qian
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetGPRSInfoEx (
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_GPRS_INFO_T *mn_gprs_info
                            );

/*****************************************************************************/
// 	Description : Get the power sweep info 
//	Global resource dependence : none
//  Author: Yu.Sun
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetPowerSweepInfoEx(
                            MN_DUAL_SYS_E   dual_sys,
                            MN_ENG_POWER_SWEEP_INFO_T *power_sweep_info
                            );

/*****************************************************************************/
// 	Description : Get the power sweep info 
//	Global resource dependence : none
//  Author: Yu.Sun
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetPowerSweepInfo(
                            MN_ENG_POWER_SWEEP_INFO_T 	*power_sweep_info
                            );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:  wuding.yang     
//  Note:
/*****************************************************************************/
extern void MNENG_SetPagingWakeUpRepoertEx(BOOLEAN is_paging_wake_up_support);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:  wuding.yang     
//  Note:
/*****************************************************************************/
extern BOOLEAN MNENG_GetPagingWakeUpRepoertEx(void);

/*****************************************************************************/
//  Description : MNPHONE_SetUserControlGPRSAttach
//  Global resource dependence : g_is_user_control_gprs_attach[dual_sys]
//  Author:       William.Qian
//  Note:
/*****************************************************************************/
extern void MNPHONE_SetUserControlGPRSAttachEx(
                            MN_DUAL_SYS_E   dual_sys,
                            BOOLEAN is_user_control
                            );

/*****************************************************************************/
//  Description : MNPHONE_GetUserControlGPRSAttach
//  Global resource dependence : g_is_user_control_gprs_attach[dual_sys]
//  Author:       William.Qian
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetUserControlGPRSAttachEx(
                            MN_DUAL_SYS_E   dual_sys
                            );

/*****************************************************************************/
//  Description : power on ps request
//  Global resource dependence : none
//  Author:jason
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_PowerOnPsEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E   dual_sys
              );

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_PowerOnPsExWithState(
    MN_DUAL_SYS_E    dual_sys,
    MN_GMMREG_RAT_E  user_rat,    // always 2G in 6530
    mn_protocol_stack_sub_state_enum active_ps_state
);

/*****************************************************************************/
//  Description : power off ps request
//  Global resource dependence : none
//  Author:jason
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_PowerOffPsEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E   dual_sys
              );

/*****************************************************************************/
//  Description : power on sim request
//  Global resource dependence : none
//  Author:jason
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_PowerOnSimEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E   dual_sys,
              BOOLEAN is_init
              );

/*****************************************************************************/
//  Description : power off sim request
//  Global resource dependence : none
//  Author:jason
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_PowerOffSimEx( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E   dual_sys
              );

/*****************************************************************************/
// 	Description : L4 init
//	Global resource dependence : none
//  Author: William.Qian
//	Note: 
/*****************************************************************************/
extern uint32 SCI_InitL4(void);

/*****************************************************************************/
//  Description : mmi call this function to decide if should inform STK at first,
//                then call diffrent functions to send SS according to ss_oper.
//  Global resource dependence : none
//  Author: Guisen.Deng
//  Note: 
/*****************************************************************************/
MNSS_SENDSS_RESULT_E MNSS_SendSSEx(
                                    MN_DUAL_SYS_E             dual_sys,
                                    MN_SS_OPERATION_CODE_E    ss_oper,
                                    uint16                    ss_code,
                                    MN_SS_BASIC_SERVICE_T     basic_service,    // the supplementary service used on which type service(bearer or tele or all call type)
                                    MN_SS_REGISTER_PARAM_U    register_param,   // parameter's union of register ss data operation
                                    SIMAT_DO_SS_STRING_T		      ss_string
                                    );


/*****************************************************************************/
//  Description : read pclink config from nv
//  Global resource dependence : none
//  Author:Shaohua.Xia
//  Note: 
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_GetPclinkCfg(         // return whether success or failure
               MN_GPRS_PCLINK_CFG_T *pclink_cfg_ptr   // in/out,pc link config
               );

/*****************************************************************************/
//  Description : write pclink config to nv
//  Global resource dependence : none
//  Author:Shaohua.Xia
//  Note: 
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetPclinkCfg(         // return whether success or failure
               MN_GPRS_PCLINK_CFG_T *pclink_cfg_ptr   // in,pc link config
               );

/*****************************************************************************/
/*  Description : set extended qos for a PDP context*/
/*  Global resource dependence : none */
/*  Author:   King.Li */
/*  Note: */
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetExtendQosEx(
                 MN_DUAL_SYS_E		dual_sys,
                 uint32      pdp_id,
                 MN_GPRS_TOTAL_QOS_T   new_qos,
                 MN_PS_SERVICE_TYPE_E  ps_service_type
                 );

/*****************************************************************************/
/*  Description : read extended qos for a PDP context*/
/*  Global resource dependence : none */
/*  Author:   King.Li */
/*  Note: */
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadExtendQosEx( // return whether success or failure
                MN_DUAL_SYS_E		dual_sys,
                MNGPRS_3G_EXTEND_QOS_SET_T ** ret_qos_ptr,       // the memory save the result
                uint32            *pdp_context_num
                );

/*****************************************************************************/
/*  Description : set extended min qos for a PDP context*/
/*  Global resource dependence : none*/
/*  Author:King.Li*/
/*  Note:*/
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetExtendMinQosEx(
                MN_DUAL_SYS_E		dual_sys,
                uint32    pdp_id,
                MN_GPRS_TOTAL_QOS_T new_qos
                );

/*****************************************************************************/
/*  Description : returns minimum extend qos for each defined context.*/
/*  Global resource dependence : none*/
/*  Author:King.Li*/
/*  Note:*/
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadExtendMinQosEx( // return whether success or failure
                MN_DUAL_SYS_E		dual_sys,
                MNGPRS_3G_EXTEND_QOS_SET_T ** ret_min_qos_ptr,       // the memory save the result
                uint32            *pdp_context_num
                );
                    
/*************************************************************************/
//    Description: read extend request qos profile from SIM or NV via service type
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadStorageExtReqQosEx(
                MN_DUAL_SYS_E		   dual_sys,
                MN_GPRS_STORAGE_E      storage,
                MN_PS_SERVICE_TYPE_E   ps_service_type,
                MN_GPRS_EXT_QOS_INFO_T *qos_info
                );

/*************************************************************************/
//    Description: set extend request qos profile to SIM or NV via service type
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetStorageExtReqQosEx(
                                    MN_DUAL_SYS_E		   dual_sys,
                                    MN_GPRS_STORAGE_E      storage,
                                    MN_PS_SERVICE_TYPE_E   ps_service_type,
                                    MN_GPRS_EXT_QOS_INFO_T *qos_info
                                    );

/*************************************************************************/
//    Description: set extend request qos profile to SIM or NV via service type
//                 with item_setted[]
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetStorageExtReqQosWithItemEx(
                                    MN_DUAL_SYS_E		   dual_sys,
                                    MN_GPRS_STORAGE_E      storage,
                                    MN_PS_SERVICE_TYPE_E   ps_service_type,
                                    MN_GPRS_EXT_QOS_INFO_T *qos_info,
                                    BOOLEAN                item_setted[13]
                                    );

/*************************************************************************/
//    Description: set 3g QoS profile of pdp context
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetExtReqQosProfileEx(
                                   MN_DUAL_SYS_E		  dual_sys,
                                   MN_GPRS_EXT_QOS_INFO_T *ext_qos_info
                                   );

/*************************************************************************/
//    Description: read 3g QoS profiles of pdp contexts
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadExtReqQosProfileEx(
                                   MN_DUAL_SYS_E	 dual_sys,
                                   MN_GPRS_EXT_QOS_T *ext_qos
                                   );

/*************************************************************************/
//    Description: set 3g QoS profiles (minimum) of pdp contexts
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetExtMinQosProfileEx(
                                   MN_DUAL_SYS_E	      dual_sys,
                                   MN_GPRS_EXT_QOS_INFO_T *ext_qos_info
                                   );

/*************************************************************************/
//    Description: read 3g QoS profiles (minimum) of pdp contexts
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadExtMinQosProfileEx(
                                   MN_DUAL_SYS_E	 dual_sys,
                                   MN_GPRS_EXT_QOS_T *ext_qos
                                   );

/*************************************************************************/
//    Description: send pdp-activate-req to mn
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ActivatePdpContextWithoutSrvTypeEx(
                                               MN_DUAL_SYS_E dual_sys,
                                               uint8         pdp_id
                                               );

/*************************************************************************/
//    Description: send APP_MN_MODIFY_PDP_CONTEXT_REQ to mn
//    Author: winnie.zhu
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ModifyPdpContextEx( 
                                       MN_DUAL_SYS_E dual_sys,
                                       uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT]
                                       );

/*************************************************************************/
//    Description: read secondary pdp contexts
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadSecPdpContextEx(
                                      MN_DUAL_SYS_E             dual_sys,
                                      MN_GPRS_SEC_PDP_CONTEXT_T *sec_pdp_cntxt
                                      );

/*************************************************************************/
//    Description: set secondary pdp context
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetSecPdpContextEx(
                          MN_DUAL_SYS_E                  dual_sys,
                          MN_GPRS_SEC_PDP_CONTEXT_INFO_T *sec_pdp_cntxt_info
                          );

/*************************************************************************/
//    Description: read traffic flow template of pdp contexts
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_ReadTftEx(
                          MN_DUAL_SYS_E dual_sys,
                          MN_GPRS_TFT_T *tft
                          );

/*************************************************************************/
//    Description: set traffic flow template of pdp context
//    Author: Shaohua Xia
//    Note:  
/*************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetTftEx(
                         MN_DUAL_SYS_E                dual_sys,
                         MN_GPRS_PACKET_FILTER_INFO_T *pfi
                         );

/*****************************************************************************/
//  Description : set LLC used XID param
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E MNGPRS_SetLlcXidParamEx(
                                          MN_DUAL_SYS_E             dual_sys,       //IN:
                                          uint8                     sapi_num,       //IN:
                                          MNGPRS_LLC_XID_PARAM_T    *xid_param_ptr  //IN:
                                          );


/*****************************************************************************/
// 	Description : MNPHONE_PowerOnPS
//	Global resource dependence : none
//  Author:       minqian.qian
//	Note: active ps, not need active sim, if do this sim must acitve first
/*****************************************************************************/
#if defined(TIANLONG_MOCOR) || defined(UEIT_MOCOR)
ERR_MNPHONE_CODE_E MNPHONE_PowerOnPS(void);
#else
void MNPHONE_PowerOnPS(void);
#endif
/*****************************************************************************/
// 	Description : MNPHONE_PowerOnPS
//	Global resource dependence : none
//  Author:      minqian.qian
//	Note: deactive ps, but not need power off sim card
/*****************************************************************************/
#if defined(TIANLONG_MOCOR) || defined(UEIT_MOCOR)
ERR_MNPHONE_CODE_E MNPHONE_PowerOffPS( void );
#else
void MNPHONE_PowerOffPS(void);
#endif

/*****************************************************************************/
//  Description:    set phone common control for protocol stack
//  Global resource dependence:
//  Author:         SHIJUN
//  Note: used for 2.5G portocol stack common control
/*****************************************************************************/
extern void MNPHONE_SetCommonCtrl(
                    void
                    );

/*****************************************************************************/
//  Description:    return the BASE version
//  Global resource dependence:
//	s_base_ps_version_info
//  Author:         SHIJUN  
//  Note: used for 2.5G base version query
/*****************************************************************************/
extern char * SCI_GetBaseVersionInfo(
                    void
                    );


/*****************************************************************************/
//  Description : get storage priority, sim first or me first
//  Global resource dependence : none
//  Author: Sunsome.Ju
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_GetStoragePriorityEx    (           // return whether success or failure   
                            MN_DUAL_SYS_E dual_sys,
                            BOOLEAN     *is_sim_first           // in--save in sim first
                            );

/*****************************************************************************/
//  Description : Read the param in SIM card
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_ReadSimParamEx( 
                                      MN_DUAL_SYS_E     dual_sys,   //IN:
                                      MN_SMS_RECORD_ID_T record_id  //IN: the parameter position in SIM card
                                      );

/*****************************************************************************/
//  Description : update the parameter in SIM card
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_WriteSimParamEx(                                      //RETURN:
                                       MN_DUAL_SYS_E        dual_sys,       //IN:
                                       MN_SMS_RECORD_ID_T   record_id,      //IN: the parameter position in SIM card
                                       MN_SMS_PARAM_T       *sms_param_ptr_t//IN: the parameter struct pointer
                                       );

/*****************************************************************************/
//  Description : update the parameter in SIM card
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetSmsParamEx(                                      //RETURN:
                                       MN_DUAL_SYS_E        dual_sys,       //IN:
                                       uint8                smsp_id,        //IN: record id
                                       MN_SMS_PARAM_T       *sms_param_ptr  //IN: the parameter struct pointer
                                       );

/*****************************************************************************/
//  Description : Read the SMS parameters which stored in SIM card
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_GetSmsParamEx(                                   //RETURN:
                                     MN_DUAL_SYS_E      dual_sys,       //IN:
                                     uint8              smsp_id,        //IN: record id
                                     MN_SMS_PARAM_T     *sms_param_ptr  //OUT: the SMS parameters
                                     );

/*****************************************************************************/
//  Description : Read the max number of the SMS parameters record
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_GetSmsParamMaxNumEx(                                   //RETURN:
                                           MN_DUAL_SYS_E      dual_sys,       //IN:
                                           uint8              *max_num_ptr    //OUT:
                                           );

/*****************************************************************************/
//  Description : set the default parameter id in SIM card
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_SetSmsParamDefaultIdEx(                          //RETURN:
                                              MN_DUAL_SYS_E dual_sys,   //IN:
                                              uint8         default_id  //IN: default id, started from 1
                                              );

/*****************************************************************************/
//  Description : set the default index of parameters which save in SIM
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note: 
/*****************************************************************************/
extern ERR_MNSMS_CODE_E    MNSMS_GetErrCodeEx(
                        MN_DUAL_SYS_E dual_sys
                        );

/*****************************************************************************/
// 	Description : send ussd call contorl failed result to MMI
//	Global resource dependence : none
//  Author:	Yongxia Zhang
//	Note: 
/*****************************************************************************/
extern void MNSS_SendUssdCallCtrlFailResultEx(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : cancel the sending of MO SMS
//  Global resource dependence : none
//  Author: Yongxia.Zhang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_CancelMoSmsEx( // return whether success or failure   
                        MN_DUAL_SYS_E       dual_sys    //IN:
                    );

/*****************************************************************************/
// 	Description : Get the cell basic info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetCellBasicInfo (
                            MN_ENG_CELL_BASIC_INFO_T *cell_basic_info
                            );
/*****************************************************************************/
// 	Description : Get the cell idle info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetCellIdleInfo (
                            MN_ENG_CELL_IDLE_INFO_T *cell_idle_info
                            ); 

/*****************************************************************************/
// 	Description : Get Layer1 Monitor item 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetLayer1MonItem (
                            MN_ENG_LAYER1_MONITOR_ITEM_T *mn_layer1_mon
                            );
/*****************************************************************************/
// 	Description : Get the scell control info 
//	Global resource dependence : none
//  Author: William Qian
//	Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNENG_GetSCellCtrlInfo (
                            MN_ENG_SCELL_CTRL_INFO_T *scell_ctrl_info
                            );                            
#ifndef _BASE_COMPACT_CODE_			
/*****************************************************************************/
//  Description : read the value whether me is supported
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_ReadMeSmsSupportedEx(
                                           MN_DUAL_SYS_E    dual_sys,       //IN:
                                           BOOLEAN          *is_me_support  //IN: set if me is support
                                           );
/*****************************************************************************/
//  Description : read the value whether SMS replace is supported
//  Global resource dependence : none
//  Author: William.Qian
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_ReadReplaceSmsSupportedEx(
                                                 MN_DUAL_SYS_E  dual_sys,   //IN:
                                                 BOOLEAN        *is_support //OUT: is supported
                                                 );
/*****************************************************************************/
//  Description : write the value whether SMS replace is supported
//  Global resource dependence : none
//  Author: William.Qian
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_WriteReplaceSmsSupportedEx( 
                                                  MN_DUAL_SYS_E dual_sys,   //IN:
                                                  BOOLEAN       is_support  //IN: is supported
                                                  );
#endif

/*****************************************************************************/
//  Description : write the value whether me is supported
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_WriteMeSmsSupported(
                                           BOOLEAN      is_me_support   //IN: set if me is support
                                           );
/*****************************************************************************/
//  Description : write the value whether me is supported
//  Global resource dependence : none
//  Author: Jenny.Wang
//  Note:
/*****************************************************************************/
extern ERR_MNSMS_CODE_E MNSMS_WriteMeSmsSupportedEx(
                                             MN_DUAL_SYS_E  dual_sys,       //IN:
                                             BOOLEAN        is_me_support   //IN: set if me is support
                                             );
#ifndef _BASE_COMPACT_CODE_
/*****************************************************************************/
//  Description :  APP task modify the current call's service type   
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_ModifyCallEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E dual_sys,
                      MN_SERVICE_TYPE_E    service_type, // the service that will modify to be
                      uint8                call_id,      // the modified call entity's id
                      BOOLEAN              rev_call_flag // this parameter indicate reverse the call's direction
                      );
/*****************************************************************************/
//  Description :   response the remote user's modify request.
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:
/*****************************************************************************/
extern MN_RETURN_RESULT_E MNCALL_ReplyModificationEx( // MN_RETURN_SUCCESS
                      MN_DUAL_SYS_E dual_sys,
                      uint8      call_id,      // the response call id
                      BOOLEAN    accept_flag,  // indicate that the user accept or reject the response request
                      BOOLEAN    rev_call_flag // indicate reverse the call's direction
                      );
/*****************************************************************************/
// 	Description : delete nv item
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNNV_DeleteItem( // MN_RETURN_SUCCESS
		        uint16	item_id
		        );                      
/*****************************************************************************/
// 	Description : create new nv item
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNNV_CreateItem(       // MN_RETURN_SUCCESS
		        uint16      	item_id,  
		        uint32			item_length,
		        BOOLEAN         item_is_overwrite 
		        );
#endif  

/*****************************************************************************/
// 	Description : Get IMG in SIM
//	Global resource dependence : none
//  Author:       Yongxia.zhang
//	Note:         return TRUE if IMG is present
/*****************************************************************************/
extern BOOLEAN MNSIM_GetImgEx(    
              MN_DUAL_SYS_E            dual_sys,           //IN:
              uint8                             img_id,             //IN: image id or icon id
              MN_SIM_IMG_T*            img_ptr    //OUT: IMG
              );                  

/*****************************************************************************/
//  Description : return if GPRS auto mode in the sim 
//  Global resource dependence : none
//  Author:       Yu Sun
//  Note:
/*****************************************************************************/
extern BOOLEAN  MNNV_IsGprsAutoMode(   // TRUE if read BA success
              MN_DUAL_SYS_E       dual_sys
              );

/*****************************************************************************/
//  Description : SIM Lock Data Integrity Check when power up For MMI.
//  Global resource dependence : none
//  Author: Tianming.Wu
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_SIMLockDataIntegrityCheck(  // TRUE: check pass; FALSE: check fail
    void
);

/*****************************************************************************/
//  Description : Get lock status data: lock flag, try counters, timeout value, 
//                and so on
//  Global resource dependence : none
//  Author: Tianming.Wu
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetSIMLockStatus(    // TRUE: success; FALSE: fail
    MN_SIM_LOCK_STATUS_T *status_data  //OUT:
);

/*****************************************************************************/
//  Description : Read the global value of simlock nv illegal download result
//  Global resource dependence : mnphone_list_g
//  Author:  shuxun.zhou
//  Note:
//  History:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetSimlockNvCheckResult(
    void
);

/*****************************************************************************/
//  Description : Get network lock data: 
//  Global resource dependence : none
//  Author: Winnie.zhu
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetXMLLocks(    // TRUE: success; FALSE: fail
    MN_SIM_LOCK_CUSTOMIZE_DATA_T *sim_lock_customize_data,  //OUT:
    MN_SIM_LOCK_USER_DATA_T *sim_lock_user_data    //OUT
);

/*****************************************************************************/
//  Description : ME personalisation verify in no sim state
//  Global resource dependence : none
//  Author: Winnie.zhu
//  Note:
/*****************************************************************************/
extern MNSIM_ERROR_E MNPHONE_NoSimMEPersonalisationVerify(    // TRUE: success; FALSE: fail
    void
);
/*****************************************************************************/
//  Description : Update SIM unlock retry data, trials_counter and time out value
//  Global resource dependence : none
//  Author: Tianming.Wu
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_UpdateSIMUnlockRetryData(    // TRUE: success; FALSE: fail
    uint32 unlock_type,      //IN: Network/NetworkSubset/SP/CORPORATE/USER
    uint32 trials_counter,   //IN: failed trials
    uint32 time_out_value    //IN: time out value for next try
);

/*****************************************************************************/
//  Description : De-personalisation(SIM Unlock)
//  Global resource dependence : none
//  Author: Tianming.Wu
//  Note:
/*****************************************************************************/
extern MN_SIM_LOCK_ERROR_CODE_E MNPHONE_SIMUnlockEx(    // RETURN: Unlock Result
    uint32         unlock_type,  //IN: Network/NetworkSubset/SP/CORPORATE/USER
    SIM_LOCK_KEY_T unlock_key,   //IN: unlock key
    MNSIM_ERROR_E  *error_code,  //OUT: sim error code, SIM still locked
    MN_DUAL_SYS_E  dual_sys
);

/*****************************************************************************/
//  Description : Personalisation(SIM lock)
//  Global resource dependence : none
//  Author: Tianming.Wu
//  Note:
/*****************************************************************************/
extern MN_SIM_LOCK_ERROR_CODE_E MNPHONE_SIMLockEx(    // RETURN: lock Result
    uint32         lock_type,  //IN: Network/NetworkSubset/SP/CORPORATE/USER
    SIM_LOCK_KEY_T lock_key,   //IN: lock key
    MN_DUAL_SYS_E  dual_sys
);

/*****************************************************************************/
//  Description : Orange Auto Lock
//  Global resource dependence : none
//  Author: Winnie.zhu
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_OrangeAutoLockEx(
    MN_DUAL_SYS_E  dual_sys
);

/*****************************************************************************/
//  Description : RESET simlock config
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_SIMLockReset(void);

/*****************************************************************************/
//  Description : When there is no card in the current sim slot,go to RESET another sim card.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern void MNPHONE_SimSetProiorityResetFlag(uint8 flag); 

/*****************************************************************************/
//  Description : Add plmn to selected plmn list with no sim
//  Global resource dependence : g_plmn_list_with_no_sim
//  Author: Jean Chen
//  Note: 
//  1. Interface to add a plmn to g_plmn_list_with_no_sim, only select cell in this list with no sim.
//      For each sim card, the max plmn num allowed to add is 8. Card ID is from 0 to (MN_DUAL_SYS_MAX - 1).
//  2. Return TRUE if successfully add the plmn, else return FALSE. 
//  3. Suggest to use this interface only during the initiation of AT.
//  4. Examples: 
//  MNPHONE_SelectPlmnWithoutSim(0, 460, 0, 2);   // add a plmn to card 0 with mcc 460, mnc 0, mnc_digit_num 2
//  MNPHONE_SelectPlmnWithoutSim(1, 460, 2, 2);   // add a plmn to card 1 with mcc 460, mnc 2, mnc_digit_num 2
/*****************************************************************************/
 extern  BOOLEAN  MNPHONE_SelectPlmnWithoutSim(   
            MN_DUAL_SYS_E             dual_sys,                   //IN -- card id
            uint16                    mcc,                        //IN -- mcc of plmn to add
            uint16                    mnc,                        //IN -- mnc of plmn to add
            int16                     mnc_digit_num               //IN -- mnc_digit_num of plmn to add                 
); 

/*****************************************************************************/
//    Description : Get the scell EMC support capability, return TRUE if EMC is supported, else 
//                       return FALSE.
//    Global resource dependence : none
//    Author: Jean Chen
//    Note:
/*****************************************************************************/
extern BOOLEAN MNPHONE_GetScellEmcSupportCapEx (
            MN_DUAL_SYS_E    dual_sys                //IN -- card id
);

/*****************************************************************************/
// 	Description : Delete all LND record information entry 
//	Global resource dependence : none
//  Author:       Jacky.ying
//	Note:         
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_DeleteLndAllEntryEx(
    MN_DUAL_SYS_E       dual_sys      // IN
);

/*****************************************************************************/
// 	Description :  get customer service profile line2 in SIM 
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_GetCustomerServiceProfileLine2Ex(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_CSP_LINE2_SERVICES_GROUP_T * services_group_ptr
                                    );

/*****************************************************************************/
//  Description : update customer service profile line2 in SIM 
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_UpdateCustomerServiceProfileLine2Ex(
            MN_DUAL_SYS_E           dual_sys,
            MN_CSP_LINE2_SERVICES_GROUP_T * services_group_ptr
            );

/*****************************************************************************/
// 	Description :  get the line which is stored in SIM card
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_GetSelectedLineInSIMEx(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E* selected_line
                                    );

/*****************************************************************************/
// 	Description :  set the line which is stored in SIM card
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_SetSelectedLineInSIMEx(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E selected_line
                                    );

/*****************************************************************************/
// 	Description :  get the lock status in SIM of the line in use
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_GetLockStatusOfLineInUseInSIMEx(
                                    MN_DUAL_SYS_E    dual_sys,
                                    BOOLEAN *is_line_locked
                                    );

/*****************************************************************************/
// 	Description :  set the lock status in SIM of the line in use
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_SetLockStatusOfLineInUseInSIMEx(
                                    MN_DUAL_SYS_E    dual_sys,
                                    BOOLEAN is_line_locked
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Phonebook(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_VoicemailNumber(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Number(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_AllVoiceCall_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_UnansweredVoiceCall(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_UnansweredVoiceCall_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_UnansweredVoiceCall_Phonebook(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_UnansweredVoiceCall_VoicemailNumber(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_UnansweredVoiceCall_Number(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_UnansweredVoiceCall_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_UnansweredVoiceCall_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Phonebook(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_VoicemailNumber(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Number(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnBusy_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Phonebook(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_VoicemailNumber(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Number(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNoAnswer_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Phonebook(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_VoicemailNumber(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Number(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CallDivert_VoiceCallOnNotReachable_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallOffering_CancelAllVoiceDivert(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_BarringOfCalls(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_OutgoingCalls(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_OutgoingCalls_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_OutgoingCalls_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_OutgoingCalls_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_AllInternationalCalls(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_AllInternationalCalls_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_AllInternationalCalls_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_AllInternationalCalls_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_InternationalCallsWhenRoaming_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_IncomingCalls(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_IncomingCalls_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_IncomingCalls_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_IncomingCalls_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_Activate(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_Cancel(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_AllCallsWhenRoaming_GetStatus(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_ClearAllBarrings(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : none
//  Author:       wuding.yang
//	Note:         
/*****************************************************************************/
BOOLEAN MNSIM_CPHS_CallRestriction_ChangeBarringPassword(
                                    MN_DUAL_SYS_E    dual_sys,
                                    MN_BEARER_CAPABILITY_TYPE_E line_index
                                    );

/*****************************************************************************/
//  Description : Do sim special authentication for upper application.
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_RunSIMCommonAlgoEx(
    MN_DUAL_SYS_E   dual_sys,
    uint32      task_id,    /* the task id of the upper application */
    uint32      algo_index, /* the index of current authentication */
    uint8       *rand_ptr,  /* LV format ,the first byte is the length ,followed by rand data */
    uint8       *autn_ptr   /* LV format ,the first byte is the length ,followed by authentication data */
);

/*****************************************************************************/
//  Description : Parse the IMSI value into BCD code, and return the correct MCC
//                       and MNC to upper layer.
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_GetImsiInfoEx(
    MN_DUAL_SYS_E   dual_sys,
    MN_IMSI_INFO_T *imsi_info_ptr /*the imsi info in ascii format*/
);

/*****************************************************************************/
// 	Description : l this function to do sim restrict function
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
BOOLEAN MNPHONE_GetImsiEx(
                MN_DUAL_SYS_E dual_sys,
                MN_IMSI_T *imsi_ptr
);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
BOOLEAN MNPHONE_GetSimtypeEx(
                MN_DUAL_SYS_E dual_sys,
                SIMCHECK_GET_SIM_TYPE_CNF_T *sim_type_ptr
                );

/*****************************************************************************/
// 	Description : l this function to do sim restrict function
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
ERR_MNPHONE_CODE_E MNSIM_RunMbbmsAlgoEx(
                        MN_DUAL_SYS_E   dual_sys,
                        uint8       *rand_ptr, /*LV格式,第一个字节为len*/
                        uint8       *autn_ptr  /*LV格式,第一个字节为len*/
                        );

/*****************************************************************************/
// 	Description : Update language information in SIM
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
void MNSIM_UpdateLanguageEx(
              MN_DUAL_SYS_E dual_sys,
              MN_SIM_LANGUAGE_INFO_T* lang_info //Fill in with both ELP&LP mapping
              );

/*****************************************************************************/
//  Description : 
//    Get the FDN status before APP_MN_CALL_READY_IND.
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_GetFDNStatus_Req(
    MN_DUAL_SYS_E dual_sys,
    uint32 req_from_task_id);

/*****************************************************************************/
//  Description :  AT send rp ack or rp error
//  Global resource dependence : none
//  Author: yongxia.zhang
//  Note:
/*****************************************************************************/
ERR_MNSMS_CODE_E MNSMS_SendATSmsDlvrRptEx (                      // return whether success or failure
                                           MN_DUAL_SYS_E         dual_sys,       //IN:
                                           MN_SMS_RECORD_ID_T    record_id,
                                           MN_SMS_DLVR_RPT_E     rpt_type,
                                           MN_L3_MSG_UNIT_T      *tpdu_ptr
                                           );

/*****************************************************************************/
//  Description :  send rp ack or rp error
//  Global resource dependence : none
//  Author:  xiaonian.li
//  Note:
/*****************************************************************************/
ERR_MNSMS_CODE_E MNSMS_SendIpcSmsDlvrRptEx (                      // return whether success or failure
    MN_DUAL_SYS_E       dual_sys,
    MN_SMS_RECORD_ID_T    record_id,
    uint16     cause
    );

/*****************************************************************************/
//  Description : the function Decode the the CALLED_NUMBER_T to L3_MSG_UNIT_T  
//  Global resource dependence : none
//  Author: Four.yang
//  Modify: Rossi.luo,2004-8-12,add SCI_ASSERT before SCI_MEMCPY/SCI_MEMSET
//  Note:
/*****************************************************************************/
void MNSMS_EncodeTpCalledNumEx(
     MN_DUAL_SYS_E       dual_sys,
     MN_CALLED_NUMBER_T *called_num_ptr,   //IN: the called number structure
     MN_L3_MSG_UNIT_T   *tpdu_ptr_t        //OUT: the SMS address structure
     );

/*****************************************************************************/
//  Description : to get a record ID of the free space for storing SMS in 
//                the specific storage
//  Global resource dependence : none
//  Author: Andy Huang
//  Note: index starts by 0
/*****************************************************************************/
ERR_MNSMS_CODE_E MNSMS_FindFreeRecordIdByIndex(
                                 MN_DUAL_SYS_E      dual_sys,
                                 MN_SMS_STORAGE_E   storage,
                                 uint16             index,
                                 MN_SMS_RECORD_ID_T *record_id_ptr
                                 );

/*****************************************************************************/
// 	Description :  AT+CSMS set the sms service
//	Global resource dependence : none
//  Author: yongxia.zhang
//	Note:
/*****************************************************************************/
ERR_MNSMS_CODE_E 	MNSMS_SetCSMSEx(
							MN_DUAL_SYS_E      	dual_sys,       	//IN:
							BOOLEAN 			service_flag 	//IN:
							);

/*****************************************************************************/
//  Description : set the new sms indication mode
//  Global resource dependence : none
//  Author: Yongxia Zhang
//  Note: 
/*****************************************************************************/
ERR_MNSMS_CODE_E  MNSMS_SetNewSmsIndModeEx(
            MN_DUAL_SYS_E      	dual_sys,           //IN:
            uint8               new_sms_ind_mode    //IN:
            );

/*****************************************************************************/
// 	Description :  set if PS save status report or not
//	Global resource dependence : none
//  Author: Michael zhu
//	Note:
/*****************************************************************************/
ERR_MNSMS_CODE_E MNSMS_SetStautsRptSaveModeEx(
                                            MN_DUAL_SYS_E      	dual_sys,   //IN:
                                            BOOLEAN             is_save     //IN:
                                            );

/*****************************************************************************/
// 	Description : Dphone call this function to trigger the initialization of SIM
//  restarting
//	Global resource dependence : none
//  Author:	bin.li
//	Note:
/*****************************************************************************/
void MNSIM_DphoneInitSIMEx(
                MN_DUAL_SYS_E dual_sys,
                BOOLEAN is_entire_service // TRUE: entire service
);

/*****************************************************************************/
// 	Description : Get the EFplmnwact data
//              Later the MN will send up with APP_MN_PREFER_PLMN_CHANGE_CNF.
//	Global resource dependence : none
//  Author:	wuding.yang
//	Note:
/*****************************************************************************/
void MNSIM_GetPLMNWACTEx(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description :  Set the EFplmnwact data
//              Later the MN will send up with APP_MN_PREFER_PLMN_CHANGE_CNF.
//	Global resource dependence : none
//  Author:	wuding.yang
//	Note:
/*****************************************************************************/
void MNSIM_SetPLMNWACTEx(MN_DUAL_SYS_E dual_sys, MN_PHONE_PLMN_SEL_LIST_T *plmn_list_ptr);

/*****************************************************************************/
//  Description : set the binary file to SIM
//  Global resource dependence : 
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_SetSimBinaryFileEx(                                     //RETURN: 
                                              MN_DUAL_SYS_E            dual_sys,   //IN:
                                              MN_SIM_BINARY_FILE_T     *buf_ptr
                                              );

/**********************************************************************
//    Description:
//       call this fucntion to redo the terminal profile download
//    Global resource dependence :
//    Author:Karin.Li
//    Note:
***********************************************************************/
void SIMAT_MeTerminalProfile(
                                uint8 card_id,
                                uint8* profile_ptr,     //in:the Teminal profile data
                                uint16 profile_len  //in:when in,the length of the buf that to save the teminal profile
                                                    //in,the really terminal profile data length
                                );

//begin to adapt to the SS IPC
/*****************************************************************************
    Description : get ATR raw data from sim
    Global resource dependence : none
    Author: Eddie.Wang
    Note: ATR should be valid after SIM is present
*****************************************************************************/
MN_SIM_PARA_VALUE_T MNSIM_GetSIMATRRawData(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************
    Description : get ef info request to sim 4
    Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
    Note:
*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_GetEFInfo(
    MN_DUAL_SYS_E dual_sys,
    uint16      file_id,
    uint8       path_len,
    uint8      *path_ptr
);

/*****************************************************************************
    Description : Set and Reset SIM EF service in EST for USIM or ADN for SIM
    Global resource dependence : none
       Author:       Eddie.Wang
    Note:  For FDN only
*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_SetSimEFService(
    MN_DUAL_SYS_E dual_sys,
    MN_SIM_FILE_TYPE_E sim_ef,   //For FDN, BDN and ACL only
    BOOLEAN is_service_enable    // TRUE: enable, FALSE: disable
);

/*****************************************************************************
    Description : Check SIM EF service in EST for USIM or ADN for SIM
    Global resource dependence : none
       Author:       Eddie.Wang
    Note:  For FDN only
*****************************************************************************/
MN_SIM_SERVICE_E MNSIM_GetSimEFService(
    MN_DUAL_SYS_E dual_sys,
    MN_SIM_FILE_TYPE_E sim_ef   //For FDN only
);

/*****************************************************************************
    Description : read ef info request to sim
    Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
    Note: According to TD code, this is only for transparent EF.
*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_ReadSimEF(
    MN_DUAL_SYS_E dual_sys,
    uint16      file_id,
    uint8       path_len,
    uint8      *path_ptr
);

/*****************************************************************************
    Description : write ef info request to sim
    Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
    Note: According to TD code, this is only for transparent EF.
*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_WriteSimEF(
    MN_DUAL_SYS_E dual_sys,
    uint16 file_id,
    uint8 path_len,
    uint8 *path_ptr,
    uint8 data_len,
    uint8 *data_ptr
);

/*****************************************************************************
    Description : read record info request to sim
    Global resource dependence : none
       Author:       Bruce.Jiang
       Modified by: Eddie.Wang
    Note:
*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_ReadSimRecordFile(
    MN_DUAL_SYS_E dual_sys,
    uint16 file_id,
    uint8 path_len,
    uint8 *path_ptr,
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
ERR_MNDATAMAG_CODE_E MNSIM_UpdateSimRecordFile(
    MN_DUAL_SYS_E dual_sys,
    uint16 file_id,
    uint8 path_len,
    uint8 *path_ptr,
    uint8 rec_num,
    uint8 record_len,
    uint8 *data_ptr
);

/*****************************************************************************
    Description : search record request to SIM
    Global resource dependence : none
       Author:       Eddie.Wang
    Note:
*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_SearchSimRecord(
    MN_DUAL_SYS_E dual_sys,
    uint16 file_id,
    uint8 path_len,
    uint8 *path_ptr,
    MN_SIM_SEARCH_RECORD_T search_sim_record
);

/*****************************************************************************/
/*  Description : set invalid IMSI enable flag in NV        */
/*  Global resource dependence : none */
/*  Author: huawei.zhang */
/*  Note: */
/*****************************************************************************/
BOOLEAN MNRSF_SetChngInvalidIMSIFlag(BOOLEAN chng_IMSI_flag);

/*****************************************************************************/
/*  Description : get invalid IMSI enable flag in NV        */
/*  Global resource dependence : none */
/*  Author:  */
/*  Note: */
/*****************************************************************************/
BOOLEAN MNRSF_GetChngInvalidIMSIFlag(void);
//end to adapt to the SS IPC


/*****************************************************************************/
//  Description : Get the info of GID directly after SIM READY
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E MNSIM_GetGidData( 
              MN_DUAL_SYS_E dual_sys,
              MNSIM_GID_E gid_type,
              MN_SIM_EF_MAPPING_GID_T *ptr);

/*****************************************************************************/
//  Description : request to delete requested profile contexts.
//  Author:
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E  MNGPRS_DeleteQosProfileEx( // return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        BOOLEAN            is_all,        //IN: identify if all PDP contexts need delete.
        uint32 pdp_id,           // the identity of the PDP context will be deleted.
        MN_GPRS_TOTAL_QOS_T qos
        );

/*****************************************************************************/
//  Description : request to delete min requested profile contexts.
//  Author:
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E  MNGPRS_DeleteMinQosProfileEx( // return whether success or failure
        MN_DUAL_SYS_E dual_sys,
        BOOLEAN            is_all,        //IN: identify if all PDP contexts need delete.
        uint32 pdp_id,           // the identity of the PDP context will be deleted.
        MN_GPRS_TOTAL_QOS_T qos
        );


/*****************************************************************************/
//  Description : send APP_MN_SWITCH_ARFCN_REQ
//  Global resource dependence : none
//  Author:cuifang.deng
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SwitchArfcn( // ERR_MNPHONE_NO_ERR
              MN_DUAL_SYS_E dual_sys,
              uint16 arfcn,
              BOOLEAN bar_current_cell,
              uint16 bar_time
              );

/*****************************************************************************/
//  Description : sent no power sweep
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetDisablePowerSweep( 
              BOOLEAN is_disable
              );

/*****************************************************************************/
// 	Description: clear the BA list of sim
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
extern ERR_MNDATAMAG_CODE_E MNSIM_ClearBaList(
                       MN_DUAL_SYS_E    dual_sys
                       );

/*****************************************************************************/
//  Description : Set call wating status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
extern void MNCALL_SetCallWatingStatus(MN_DUAL_SYS_E dual_sys, MN_SS_CALL_WAITING_STATUS_E status);

/*****************************************************************************/
//  Description : Get call wating status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
extern MN_SS_CALL_WAITING_STATUS_E MNCALL_GetCallWatingStatus(MN_DUAL_SYS_E dual_sys);
					   
/*****************************************************************************/
//  Description : APP set start or stop identify the fake BS
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetIdentifyBS(MN_DUAL_SYS_E dual_sys, BOOLEAN is_identify);

/*****************************************************************************/
//  Description : Set the method of the indentify fake BS 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern ERR_MNPHONE_CODE_E MNPHONE_SetIdentifyBSMode(MN_DUAL_SYS_E dual_sys, MN_IDENTIFY_BS_MODE_E identify_mode);

/*****************************************************************************/
//  Description : set whether to check eplmn list when roaming
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern void MNPHONE_SetCheckEplmnRoaming(MN_DUAL_SYS_E  dual_sys, BOOLEAN is_check);

/*****************************************************************************/
// 	Description : return IMEISVN
//	Global resource dependence : none
//  Author:       Sunsome Ju
//	Note:
/*****************************************************************************/
extern BOOLEAN  MNNV_GetIMEISVN(   // TRUE if read IMEISVN success
              MN_DUAL_SYS_E      dual_sys,
              MN_IMEISVN_T       *imeisvn
              );
              
/*****************************************************************************/
// 	Description : return IMEI
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern BOOLEAN  MNNV_GetIMEI(   // TRUE if read IMEI success
			  MN_DUAL_SYS_E       dual_sys,
              MN_IMEI_T imei
              );

/*****************************************************************************/
// 	Description : return C-IMEI
//	Global resource dependence : none
//  Author:      
//	Note:
/*****************************************************************************/
extern BOOLEAN  MNNV_GetCIMEI(   // TRUE if read IMEI success
	       MN_DUAL_SYS_E       dual_sys,
              MN_IMEI_T imei
              );

typedef struct
{
    uint8      atFuncIndex;
    uint8      value;
    uint8      rsrp_flag;
    uint8      rsrp;
    uint8      rsrq_flag;
    uint8      rsrq;
    uint8      gsmBand;
    uint8      gsmNum;
    uint16     gsmArfcn[3];
    uint8      lteNum;
    uint32     lteArfcn[8];
} MNPHONE_SpLteDummyPara_t ;

typedef struct
{
    uint8      paramIndex;
    uint8      enable;
    uint16     value1;
    uint16      value2;
} MNPHONE_RrtmParam_t ;

typedef struct
{
    uint32     earfcn;
    uint8      prior;
    uint8      thresh_h;
    uint8      thresh_l;
} MNPHONE_GrrLteFreq_t;

typedef struct
{
    uint8     index;
    uint8     value;
} MNPHONE_RrdmParam_t;


typedef struct
{
    uint16     index;
    uint16     value;
} MNPHONE_L1Param_t;

typedef struct
{
    uint8     type;
} MNPHONE_SpCleanInfo_t;

typedef struct _MN_TFT_SET
{
    uint16 Pf_BitMap;            /*Pos: 0, 1, 2, 3, ...  */
    uint8 Pf_ID;                 /*Bit 0 - 0x8000  */
    uint8 Pf_EvaPreIdx;          /*Bit 1 - 0x4000 */
    uint8 Pf_RmtAddrIPv4[8];     /*Bit 2 - 0x2000 */
    uint8 Pf_RmtAddrIPv6[32];    /*Bit 3 - 0x1000 */
    uint8 Pf_PortNum_NH;         /*Bit 4 - 0x0800 */
    uint16 Pf_LocalPortRangeMin; /*Bit 5 - 0x0400 */
    uint16 Pf_LocalPortRangeMax;
    uint16 Pf_RemotPortRangeMin; /*Bit 6 - 0x0200 */
    uint16 Pf_RemotPortRangeMax;
    uint32 Pf_SPI;              /*Bit 7 - 0x0100 */
    uint8 Pf_TOS_TC[2];         /*Bit 8 - 0x0080 */
    uint32 Pf_FlowLabel;        /*Bit 9 - 0x0040 */
    uint8 Pf_Direct;            /*Bit 10 - 0x0020 */
    uint8 Pf_LocalAddrIPv4[8];  /*Bit 11 - 0x0010 */
    uint8 Pf_LocalAddrIPv6[32]; /*Bit 12 - 0x0008 */

    struct _MN_TFT_SET *pNext;
} MNGPRS_TFT_SET;

typedef struct _MN_EQOS
{
    uint8 nQci;
    uint32 nDlGbr;
    uint32 nUlGbr;
    uint32 nDlMbr;
    uint32 nUlMbr;
} MNGPRS_EQOS;

extern uint32 MNPHONE_SpLteDummyPara(MNPHONE_SpLteDummyPara_t para, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_RrtmParam(MNPHONE_RrtmParam_t para, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_GrrLteFreq(MNPHONE_GrrLteFreq_t para, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_RrdmParam(MNPHONE_RrdmParam_t para, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_L1Param(MNPHONE_L1Param_t para, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_SpCleanInfo(MNPHONE_SpCleanInfo_t para, MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_SetLteLockFreq(uint8 number, uint32 * freq, MN_DUAL_SYS_E  dual_sys);
extern void MNPHONE_GetLteLockFreq(uint8 * number, uint32 * freq, MN_DUAL_SYS_E  dual_sys);
extern void MNPHONE_SetLteLockBand(uint8 number, uint16 * band, MN_DUAL_SYS_E  dual_sys);
extern void MNPHONE_GetLteLockBand(uint8 * number, uint16 * band, MN_DUAL_SYS_E  dual_sys);
extern void MNPHONE_SetGsmBand(uint8 nBand, MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_GetGsmBand(uint8* pBand, MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_SetRat(uint8 nRat, MN_DUAL_SYS_E dual_sys);
extern uint8 MNPHONE_GetRat(MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_GetCEMode(uint8* pMode, MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_SetCEMode(uint8 nMode, MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_GetVoicePrefer(uint8* pVoicePrefer, MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_SetVoicePrefer(uint8 nVoicePrefer, MN_DUAL_SYS_E dual_sys);
extern BOOLEAN MNPHONE_ImsIsSet(MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_SetNetMode(uint8 nMode, MN_DUAL_SYS_E dual_sys);
extern void MNPHONE_ClearFPLMN(MN_DUAL_SYS_E dual_sys);
extern MN_GMMREG_RAT_E MNPHONE_GetCurrentRAT(MN_DUAL_SYS_E  dual_sys);
extern void MNPHONE_ResetNetMode(void);
extern uint32 MNPHONE_SysConfig(uint8 nMode, uint8 nAcqorder, uint8 nRoam, uint8 nSrcdomain, MN_DUAL_SYS_E dual_sys) ;
extern uint32 MNPHONE_GetSysConfig(uint8* pMode, uint8* pAcqorder, uint8* pRoam, uint8* pSrcdomain, MN_DUAL_SYS_E dual_sys) ;
extern uint32 MNPHONE_GetGsmCalibInfo(void);
extern uint32 MNPHONE_GetLteCalibInfo(void);
extern uint32 MNPHONE_SetPowerOffFlag(uint8 bPoweroff,MN_DUAL_SYS_E dual_sys);
extern uint32 MNGPRS_SetGprsCtxTFT(int8 cid, MNGPRS_TFT_SET tft_set, MN_DUAL_SYS_E dual_sys);
extern uint32  MNGPRS_GprsSetCtxEQos(int8 cid, MNGPRS_EQOS qos, MN_DUAL_SYS_E dual_sys);
extern uint32 MNGPRS_GprsGetCtxEQos(int8 cid, MNGPRS_EQOS* qos, MN_DUAL_SYS_E dual_sys);
extern uint32 MNGPRS_GetPdpCxt(int8 cid, MN_GPRS_PDP_CON_T *pdp_cxt, MN_DUAL_SYS_E dual_sys);
extern uint32 MNGPRS_EpsModifyPdpContextEx(uint32 cid, MN_DUAL_SYS_E dual_sys);
extern ERR_MNGPRS_CODE_E MNGPRS_SetPdpContextPcidEx(MN_DUAL_SYS_E dual_sys, uint32 cid, uint32 pcid);
extern uint32 MNPHONE_SetXCAP(uint8* pXCAP, uint32 nLength,MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_SetBSF(uint8* pBSF, uint32 nLength,MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_ImsSrvccCapSet(uint8 bSupport, MN_DUAL_SYS_E dual_sys);
extern uint8 MNPHONE_ImsGetSrvccCap(MN_DUAL_SYS_E dual_sys);
extern uint8 MNPHONE_GetAttState(MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_ChangeRat(uint8 nRat, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_SetGcfTest(MN_DUAL_SYS_E dual_sys, uint8 flag);

typedef struct _MN_TSM_CURR_CELL_INFO
{
    uint16 nTSM_Arfcn; //Absolute Radio Frequency Channel Number ,[0..1023].
    uint8 nTSM_Rla   ; // Averaged measure
    uint8 nTSM_LAI[5]; //Location Area Identification.
    uint8 nTSM_RAC;    //Routing Area Code ( GPRS only),  [0..255].
    uint8 nTSM_CellID[4]; //Cell Identity.
    uint8 nTSM_Bsic;      //Synchronisation code,[0..63].
    BOOLEAN nTSM_BCCHDec;      //Indicates if the BCCH info has been decoded on the cell.
    uint8 nTSM_NetMode;   //Network Mode of Operation (GPRS only),[0..2].
    uint8 nTSM_AvRxLevel; //Average Rx level in Abs Val of dBm ,[0..115].
    uint8 nTSM_MaxTxPWR;  //Max allowed transmit power, [0..31].
    int8 nTSM_C1Value;    //Cell quality ,[-33..110].
    uint8 nTSM_BER;       //Bit Error Rate [0..255],255 = non significant.
    uint8 nTSM_TimeADV;   //Timing Advance (valid in GSM connected mode only).[0..255] 255 = non significant.
    uint8 nTSM_CCCHconf;  //ccch configuration
    //0:CCCH_1_CHANNEL_WITH_SDCCH meanning 1 basic physical channel used for CCCH, combined with SDCCH
    //1:CCCH_1_CHANNEL  meanning 1 basic physical channel used for CCCH, not combined with SDCCH
    //2:CCCH_2_CHANNEL  meanning 2 basic physical channel used for CCCH, not combined with SDCCH
    //3:CCCH_3_CHANNEL  meanning 3 basic physical channel used for CCCH, not combined with SDCCH
    //4:CCCH_4_CHANNEL  meanning 4 basic physical channel used for CCCH, not combined with SDCCH
    //0xFF:INVALID_CCCH_CHANNEL_NUM meanning Invalid value
    uint8 nTSM_BsPaMfrms;    // BS-PA-MFRMS
    uint8 nTSM_RxQualFull;   //receive quality full  [0~7]
    uint8 nTSM_RxQualSub;    //receive quality sub   [0~7]
    int16 nTSM_C2;           //Value of the cell reselection criterium   [-96~173]
    uint8 nTSM_CurrBand;	   //current band used,possible value see below
    uint8 nTSM_CurrChanType; //Current channel type
    //0:CHANNEL_TYPE_SDCCH meanning Current channel type is SDCCH
    //1:CHANNEL_TYPE_TCH_H0 meanning Current channel type is TCH ,half rate,Subchannel 0
    //2:CHANNEL_TYPE_TCH_H1 meanning Current channel type is TCH ,half rate,Subchannel 1
    //3:CHANNEL_TYPE_TCH_F meanningCurrent channel type is TCH ,full rate
    //0xff:INVALID_CHANNEL_TYPE meanning Invalid channel type
    uint32 nTSM_CurrChanMode; //current channel mode ,only valid in decicated mode
    //API_SIG_ONLY  1<<0    signalling only
    //API_SPEECH_V1 1<<1    speech full rate or half rate version 1
    //API_SPEECH_V2 1<<2    speech full rate or half rate version 2
    //API_SPEECH_V3 1<<3    speech full rate or half rate version 3
    //API_43_14_KBS 1<<4    data, 43.5kbit/s dwnlnk, 14.5kbit/s uplnk
    //API_29_14_KBS 1<<5    data, 29.0kbit/s dwnlnk, 14.5kbit/s uplnk
    //API_43_29_KBS 1<<6    data, 43.5kbit/s dwnlnk, 29.0kbit/s uplnk
    //API_14_43_KBS 1<<7    data, 14.5kbit/s dwnlnk, 43.5kbit/s uplnk
    //API_14_29_KBS 1<<8    data, 14.5kbit/s dwnlnk, 29.0kbit/s uplnk
    //API_29_43_KBS 1<<9    data, 29.0kbit/s dwnlnk, 43.5kbit/s uplnk
    //API_43_KBS    1<<10   data, 43.5kbit/s
    //API_32_KBS    1<<11   data, 32.0kbit/s
    //API_29_KBS    1<<12   data, 29.0kbit/s
    //API_14_KBS    1<<13   data, 14.5kbit/s
    //API_12_KBS    1<<14   data, 12.0kbit/s
    //API_6_KBS     1<<15   data, 6.0kbit/s
    //API_3_KBS     1<<16   data, 3.6kbit/s
    //API_INVALID_CHAN_MODE 0xFFFFFFFF  Invalid channel mode
    //9:BAND_GSM900 meanning Band GSM900P, GSM900E and GSM900R
    //18:BAND_DCS1800 meanning Band DCS1800
    //19:BAND_PCS1900 meanning Band PCS 1900
    //0xFF:BAND_INVALID meanning Invalid band value
    uint8 nTSM_MaxRetrans;  //max number of random access retransmission    1,2,4,7
    uint8 nTSM_BsAgBlkRes;  //block number kept for AGCH in common channel  [0~7]
    uint8 nTSM_AC[2];       //Access Class  [0~255]
    uint8 nTSM_RxLevAccMin; //receive level access minimum  [0~63]
    BOOLEAN bTSM_EC;             //Indicates if emergency call has been allowed
    BOOLEAN bTSM_SI7_8;          //Indicates if SI7 and 8 are broadcast
    BOOLEAN bTSM_ATT;            //Indicates if IMSI attach has been allowed
    uint8 nTSM_RxDbm;       //indicates current rx power
	uint8 nTSM_TxDbm;       //indicates current tx power
	uint8 nTSM_Maio;
    uint8 nTSM_Hsn;
    uint8 nTSM_RxLevSub;
    uint8 nTSM_RxLevFull;
    uint8 nTSM_RxLev;
    uint8 nTSM_Dtx_used;
    uint8 pad[3];
} MN_TSM_CURR_CELL_INFO;

typedef struct _MN_TSM_NEIGHBOR_CELL_INFO
{
    uint16 nTSM_Arfcn;    //Absolute Radio Frequency Channel Number ,[0..1023].
    uint8 nTSM_CellID[2]; //Cell Identity.
    uint8 nTSM_AvRxLevel; //Average Rx level in Abs Val of dBm ,[0..115].==RSSI
    uint8 nTSM_Bsic;      //Synchronisation code,[0..63].
    BOOLEAN nTSM_BCCHDec;      //Indicates if the BCCH info has been decoded on the cell.
    uint8 nTSM_MaxTxPWR;  //Max allowed transmit power, [0..31].
    uint8 nTSM_RxLevAM;   //RxLev Access Min in RxLev [0..63].
    int8 nTSM_C1Value;    //Cell quality ,[-33..110].see 05.08
    uint8 nTSM_LAI[5];    //Location Area Identification.
    int16 nTSM_C2;        //Value of the cell reselection criterium   [-96~173]
    int16 nTSM_C31;
    int16 nTSM_C32;
} MN_TSM_NEIGHBOR_CELL_INFO;

typedef struct _MN_TSM_ALL_NEBCELL_INFO
{
    uint8 nTSM_NebCellNUM;                    //neighbor cell number.
    MN_TSM_NEIGHBOR_CELL_INFO nTSM_NebCell[10]; //the max of the neighbor cell is 6.
    uint8 pad[3];
} MN_TSM_ALL_NEBCELL_INFO;

typedef enum
{
	MN_CELLINFO_GSM_CURRENT,
	MN_CELLINFO_GSM_NEIGHBOR,
	MN_CELLINFO_LTE_CURRENT,
	MN_CELLINFO_LTE_NEIGHBOR,
	MN_CELLINFO_INVALID = 0xFF,
}MN_CELLINFO_TYPE;

typedef void (*MNPHONE_GetCellInfoCb)(MN_CELLINFO_TYPE nType, void* pValue);

uint32 MNPHONE_GetCellInfo(MNPHONE_GetCellInfoCb pFun, MN_DUAL_SYS_E dual_sys);

typedef struct 
{
	uint16         success;  //success times
	uint16         fail;   //failure times
	uint16         passRatio;  //success rate
	uint32         delay;   //delay time,unit:ms
} mn_statistic_t;


typedef struct 
{
	uint16         dropOn2g;  //drop times on gsm
	uint16         dropOn4g;   //drop times on lte
} mn_dropTimes_t;


typedef struct 
{
	uint32         timeOn2g;   //time on gsm registered
	uint32         timeOn4g;    //time on lte registered
	uint32			timeOnVolte;  //time on volte registered
	uint32         timeOnUnknow;   //time on no RAT or drop
	uint32         allTheTime;    //total time from power on
} mn_attachTime_t;


typedef struct 
{
	mn_statistic_t		handoverStatistic[8];  //0:GToGHO,1:GToGCCO,2:GToLCCO,3:LToGCCO,4:GToLHO,5:LToLHO,6:LToGHO,7:SRVCC
} mn_handoverInfo_t;


typedef struct 
{
	mn_statistic_t		reselStatistic[4];  //0:GToLResel,1:GToLResel,2:LToLResel,3:LToGResel
} mn_reselectInfo_t;

typedef struct 
{
    uint8    lteCipherFlag;   //default value 1
	uint8    bsrvccFlag;     //default value 1
	uint8    emfbiFlag;    //default value 1
	uint8    ipv6Flag;     //default value 1
	uint8    asrvccFlag;   //default value 1
	uint8    esrvccFlag;   //default value 1
} mn_capInfoLte_t;

typedef struct 
{
    uint32     arfcn;
	uint16     bsic;   //if not decode BSIC,apply 0xffff
	uint16     rxlev;
} mn_gsmNcell_t;

typedef struct
{
    uint8      validFlag;    //0: plmn/enodeBID/cellId/tac are invalid, 1:plmn/enodeBID/cellId/tac are valid
    uint8      plmn[3];
    uint32     enodeBID;      //first 20 bits of SIB1 cell identity
    uint16     cellId;        //default value 0xff
    uint16     tac;           //serving tac(from SIB1)
} mn_lteCellAccessRelatedInfo_t; //CellAccessRelatedInfo

typedef struct 
{
    uint32     frequency;
    uint16     pcid;   //if not decode BSIC,apply 0xffff
    uint16     bandWidth;
    int16      rsrp;
    int16      rsrq;
    int16      Srxlev;
    mn_lteCellAccessRelatedInfo_t cellAccRelInfo;
} mn_lteNcell_t;

typedef struct 
{
    uint8      plmn[3];
    uint8      imsi[16];    //imsi[0]:length of IMSI
    uint8      roamingFlag;  //1:roaming,0:non roaming
    uint16     bandInfo;    //band1-band256
    uint32     dlEarfcn;    //0-262143, invalid value 0xffffffff
    uint16     pcid;
    int16      rsrp;
    int16      rsrq;
    int16      Srxlev;
    uint16     tsMode;       //default value 0xffff
    uint16     bandwidth;    //default value 0xffff
    uint16     SA;      //default value 0xffff
    uint16     SSP;     //default value 0xffff
    uint16     SINR;
    uint32     ulMcs;   //default value 0xffff
    uint32     dlMcs;   //default value 0xffff
    uint8      ulBler;  
    uint8      dlBler;
    uint32     enodeBID;      //first 20 bits of SIB1 cell identity
    uint16     cellId;        //default value 0xff
    uint16     tac;           //serving tac(from SIB1)
    uint16     netMode;  //0--tdd  1--fdd
    int16      rxPower;  //Dl receiving power; Real value(Unit:dbm)=rxPower divide 16.
    int16      txPower;  //UE transmits power, -46~23 dbm
    int16      rsi;
} mn_lteScell_t;

typedef struct 
{
    mn_lteScell_t   lteScell;
    uint16      lteNcellNum;
    mn_lteNcell_t   lteNcell[9];
    uint16      gsmNcellNum;
    mn_gsmNcell_t   gsmNcell[5];
    mn_capInfoLte_t   capInfoLte;
} mn_nwCapLte_t;


typedef struct 
{
    uint32   vamos;
    uint32   diversityGsm;
    uint8    Plmn[3];
    BOOLEAN  Roaming;
    uint8    CellId[2];
    uint16   Arfcn;
    uint8    Bsic;
    uint8    Rla;
    uint8    Lai[5];
} mn_ueCapGsm_t;

typedef struct 
{
	mn_ueCapGsm_t   ueCapabilityGsm;
	mn_nwCapLte_t   nwCapabilityLte;
} MN_NET_INFO_T;

typedef struct 
{
	mn_reselectInfo_t   reselectInfo;
	mn_handoverInfo_t   handoverInfo;
	mn_attachTime_t     attachTime;
	mn_dropTimes_t      dropTimes;
} MN_NET_INFO_STATISTIC_T;

extern uint32 MNPHONE_FacGetNetInfo(MN_NET_INFO_T *pNetInfo, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_FacGetNetInfoStatistic(MN_NET_INFO_STATISTIC_T *pNetinfoStatistic, MN_DUAL_SYS_E dual_sys);

#define MN_STORE_MAX_COUNT 33
typedef struct //this must be the same with api_NwStoreListInd_t
{
    uint8 Plmn[3];
    uint8 nCount;
    uint16 ArfcnL[MN_STORE_MAX_COUNT];
}MN_StoredPlmnList;

uint32 MNPHOE_CfgSetStoredPlmnList(MN_StoredPlmnList *pPlmnL, MN_DUAL_SYS_E dual_sys);

uint32 MNPHONE_NwSetLockBCCH(BOOLEAN bLock, MN_DUAL_SYS_E dual_sys);
BOOLEAN MNPHONE_NwGetLockBCCH(void);

uint32 MNPHONE_SwitchUSBShareNetwork(BOOLEAN flag);

extern ERR_MNGPRS_CODE_E MNGPRS_SetInteralPdpContextEx(// return whether success or failure    
        MN_DUAL_SYS_E       dual_sys,
        BOOLEAN             param_used[7],      //IN: indicate if the following parameter is valid.
        MN_INTERNAL_PDP_CTX_TYPE              interal_type,               //IN: pdp usage type (0~2)
        uint8               *pdp_type_ptr,      //IN: pdp type,value("IP","PPP","EMPTY","IPv6")
        uint8               *apn_ptr,           //IN: access point name,this depends your network operator
        MN_GPRS_PDP_ADDR_T  pdp_addr,           //IN: pdp address,for IP network,it indicate ip address
        uint8               d_comp,             //IN: if apply PDP data compression:0 - off,1 - on
        uint8               h_comp,             //IN: if apply PDP header compression:0 - off,1 - on, 2-rfc1144
        uint8               *pco_str,            //IN: protocol configuration option that depends on <pdp_type>
        uint8               *user_ptr,                               //IN: protocol configuration option pap user name
        uint8               *passwd_ptr,                             //IN: protocol configuration option pap user password
        MN_PCO_AUTH_TYPE_E   auth_type
        );
extern uint32 MNPHONE_ImsNvParamSet(uint16 param_id, uint8 *value_ptr, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_ImsNvParamGet(uint16 param_id, uint8 *value_ptr, MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_ResetImsDelta(void);
extern MN_PHONE_SCELL_MEAS_T MNPHONE_GetRSRPEx( MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_GetRRCRel(MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_SetRRCRel(uint32 value,MN_DUAL_SYS_E dual_sys);
extern uint32 MN_nvSetLocPriority(uint8 locPriority, uint16 nUTI, MN_DUAL_SYS_E dual_sys);
extern uint8 MN_nvGetLocPriority(MN_DUAL_SYS_E dual_sys);
extern uint32 MN_GnssStartHandshakeStack(uint8 nServiceType, uint16 nUTI, MN_DUAL_SYS_E dual_sys);
extern uint32 MN_GnssStopHandshakeStack(uint16 nUTI, MN_DUAL_SYS_E dual_sys);
extern BOOLEAN MNPHONE_IsSupprtImsEmc(MN_DUAL_SYS_E dual_sys);
extern uint32 MNPHONE_NwSetLocalRelease(MN_DUAL_SYS_E dual_sys);
extern  uint32 MNPHONE_GetSimSwitch(uint8* sim_id);
extern  uint32 MNPHONE_SetSimSwitch(uint8 sim_id);
#endif /* end of _MN_API_H */

