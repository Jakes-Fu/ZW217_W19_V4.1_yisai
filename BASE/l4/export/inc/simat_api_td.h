/******************************************************************************
 ** File Name:      simat_api.h                                               *
 ** Author:         Raistlin.kong                                             *
 ** Date:           02/18/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file that define the signals related   *
 **				    with short message                                        *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.2         Raistlin.kong    Create									  *
 ** 2003.1         Karin.Li         Update                                    *
 ** 2007.10        Bin.Li           add usat                                  *
 ** 2008.3         Pizer.Fan        2.5g (dual sim) and 3g integration        *
 ******************************************************************************/
 
#include "simat_data_object.h"
#include "simat_proactive.h"
#include "state_machine.h"   //need by SIGNAL_ACTION_E
void SIMAT_CnfAccessEx(
    int32 multi_sys,
    SIMAT_ACCESS_CNF_T* access_cnf    //in:the access confirm
);

/* +cr119425 newms */
typedef struct
{
    uint8 sw1;
    uint8 sw2;
}SIMAT_STATUS_WORD_T;
/* -cr119425 newms */

void SIMAT_CnfRetrieveMultiMediaMsgEx(
                                int32 card_id,
                                SIMAT_RETRIEVE_MULTIMEDIA_MSG_CNF_T* retrieve_multimedia_msg_ptr    //in:the setup call confirm
                                );

void SIMAT_CnfSubmitMultiMediaMsgEx(
                                int32 card_id,
                                SIMAT_SUBMIT_MULTIMEDIA_MSG_CNF_T* submit_multimedia_msg_ptr    //in:the setup call confirm
                                );
void SIMAT_CnfDisplayMultiMediaMsgEx(
                                int32 card_id,
                                SIMAT_DISPLAY_MULTIMEDIA_MSG_CNF_T* display_multimedia_msg_ptr    //in:the setup call confirm
                                );
void SIMAT_IndMmsNotificationDownload(
                                SIMAT_MMS_NOTI_DOWNLOAD_IND_T* mms_noti_download_ptr 
                                );
void SIMAT_IndLocalConnectionEvent(
                                SIMAT_LOCAL_CONNECTION_IND_T* local_connection_ptr 
                                );
void SIMAT_IndChannelStatusEventEx(
    uint8 card_id,                            
    SIMAT_CHANNEL_STATUS_IND_T* channel_status_ptr 
    );
BOOLEAN SIMAT_IsNeedCBDownload(void);
void SIMAT_IndMmsNotificationDownload(
                                SIMAT_MMS_NOTI_DOWNLOAD_IND_T* mms_noti_download_ptr 
                                );
void SIMAT_IndNwSrchModeChangeEx(
    uint8 card_id,
    SIMAT_NET_SEARCHER_MODE_IND_T* nw_search_mode    
    );

void SIMAT_IndAccessTechChangeEx(
    uint8 card_id,
    SIMAT_ACCESS_TECHNOLOGY_IND_T* access_tech_change_ptr    //in:the location status indication
    );
void SIMAT_IndBrowserStatusEventEx(
    uint8 card_id,
    SIMAT_BROWSER_STATUS_IND_T* browser_status_ptr 
    );

void SIMAT_IndNormalSmsPPEx(
    uint8 card_id,
    SIMAT_SMS_PP_IND_T* psmspp   //in:the SMS PP indication
    );

void SIMAT_IndMmsNotificationDownloadEx(
    uint8 card_id,
    SIMAT_MMS_NOTI_DOWNLOAD_IND_T* mms_noti_download_ptr 
    );

void SIMAT_IndMmsTransferStatusEx(
    uint8 card_id,
    SIMAT_MMS_TRANS_STATUS_IND_T* mms_trans_status_ptr 
    );

void SIMAT_IndLocalConnectionEventEx(
    uint8 card_id,
    SIMAT_LOCAL_CONNECTION_IND_T* local_connection_ptr 
    );
void SIMAT_IndCardReaderStatusEventEx(
    uint8 card_id,
    SIMAT_CARD_READ_STATUS_IND_T* card_reader_status_ptr 
    );

BOOLEAN SIMAT_IsNeedCBDownloadEx(uint8 card_id);
BOOLEAN SIMAT_IsNeedSMSDownloadEx(uint8 card_id);

extern void SIMAT_MeTerminalProfileEx(
                                int32 card_id,
                                uint8* profile_ptr,     //in:the Teminal profile data
                                uint16 profile_len  //in:when in,the length of the buf that to save the teminal profile
                                                    //in,the really terminal profile data length
                                );		
/* +cr120118 newms */                                
extern void SIMAT_IndMMISMSControlEx(
                            uint8 card_id,
                            SIMAT_SMS_CONTROL_IND_T* psmsc    //in:the SMS control indication
                            ); 
extern void SIMAT_IndMMIMOCCEx(
                uint8 card_id,
                SIMAT_CC_IND_T* pmocc    //in:the MO call control indication
                );
/* -cr120118 newms */                              
