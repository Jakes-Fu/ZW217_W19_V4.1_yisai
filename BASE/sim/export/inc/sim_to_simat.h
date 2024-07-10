/******************************************************************************
 ** File Name:      sim_to_simat.h                                            *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.2                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file comes form sim_outport.h,it is used to   *
 **					include the sim layer header files used by simat layer	  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.2         Raislin.Kong      create                                   *
 ******************************************************************************/


#ifndef _SIM_TO_SIMAT_H_
#define _SIM_TO_SIMAT_H_
#include "sim_type.h"

#ifdef __cplusplus
    extern "C"
    {
#endif

//#define SIMAT_DEBUG
#ifdef SIMAT_DEBUG

#define SIMAT_ASSERT(_EXP) SCI_ASSERT(_EXP)

#else

#define SIMAT_ASSERT(_EXP) 

#endif

//the max data length of the Command APDU and Reposne APDU not the seem
#define SIM_MAX_C_APDU_DATA_LEN         255
#ifdef __MN_ADP_CFW__
#define SIM_MAX_R_APDU_DATA_LEN         384
#else
#define SIM_MAX_R_APDU_DATA_LEN         256
#endif

#define SIMAT_MIN_GETINPUT_TR_LEN       16  //command details:5 bytes, device identifies:4 bytes
                                            //result:3 bytes,text string tag:1 byte,length 1-2 byte,
                                            //dcs 1 byte
//the file types in sim card
typedef enum
{
	SIM_FILE_T_MF = 0x01,
	SIM_FILE_T_DF = 0x02,
	SIM_FILE_T_EF = 0x04,
	SIM_FILE_T_UNKNOW
}SIM_FILE_TYPE_E;

//the file level of a sim card file,means the level distance to
//the sim master file
typedef enum
{
	SIM_FILE_LEVEL_E_1,
	SIM_FILE_LEVEL_E_2,
	SIM_FILE_LEVEL_E_3,
	SIM_FILE_LEVEL_E_4,
	SIM_FILE_LEVEL_E_NUM
}SIM_FILE_LEVEL_E;

//the data define that will used in SIM application function
//the data structure send to the SIM card in the TERMIANL PROFILE command
typedef struct
{
    uint16 profile_len; //the data length
    uint8 profile_arr[SIM_MAX_C_APDU_DATA_LEN];  //the data
}SIM_TERMINAL_PROFILE_DATA_T;

//the data sutucture send to the SIM card for in the ENVELOPE command
typedef struct
{
    uint16 data_len;    //the data length
    uint8 data_arr[SIM_MAX_C_APDU_DATA_LEN]; //the data
}SIM_ENVELOPE_DATA_T;

//the data structure receive from the SIM card in ENVELOPE command
typedef struct
{
    uint16  data_len;   //the data length
    uint8 data_arr[SIM_MAX_R_APDU_DATA_LEN]; //the data
}SIM_ENVELOPE_RESPONSE_T;

//the data structure received from SIM card in FETCH command
typedef struct
{
    uint16 data_len;    //the data length
    uint8 data_arr[SIM_MAX_R_APDU_DATA_LEN]; //the data
}SIM_FETCH_DATA_T;

//the data structure send to the SIM card in TERMINAL RESPONSE command
typedef struct
{
    uint16 response_len;    //the data length
    uint8 data_arr[SIM_MAX_C_APDU_DATA_LEN]; //the data
}SIM_TERMINAL_RESPONSE_DATA_T;

//the Proactive Command indication signal from the SIM layer
typedef struct
{
    SIGNAL_VARS
    SIM_FETCH_DATA_T pc_data;   //the proactive command data buf
}SIMAT_SIG_PC_IND_T;

typedef struct
{
    SIGNAL_VARS
    SIM_FETCH_DATA_T pc_data;   //the proactive command data buf
}SIM_SIG_PC_EXIST_IND_T;

//the Envelope request signal
typedef struct
{
    SIGNAL_VARS
    SIM_ENVELOPE_DATA_T envelope_data;  //the data buf that enveloped in
    BOOLEAN             delay_fetch;
}SIMAT_SIG_ENVELOPE_REQ_T,
SIMAT_SIG_AT_CPDU_ENVELOPE_REQ_T;

typedef enum
{
    SIMAT_EN_OK,
	SIMAT_EN_OK_WITH_PC,
    SIMAT_EN_SIM_BUSY,
    SIMAT_EN_OK_WITH_DATA,
/* +cr110036 , 2.5g(dual sim)/3g integration */
    SIMAT_EN_WARNING_NO_INFORMATION,
    SIMAT_EN_ERROR_MEM_UNCHANGE,
    SIMAT_EN_ERROR_MEM_CHANGED,
    SIMAT_EN_ERROR_INSTRUCTION_CODE,
    SIMAT_EN_ERROR_LC_NOT_SUPPORT,
    SIMAT_EN_ERROR_SECURITY_NOT_SUPPORT,
/* -cr110036 */
    SIMAT_EN_ERROR_WITH_DATA,
    SIMAT_EN_CARD_NOT_READY,
    SIMAT_EN_CARD_NOT_SUPPORT,
    SIMAT_EN_ERROR_UNKNOW
}SIMAT_ENVELOPE_RESULT_E;
//the envelope confirm singal
typedef struct
{
/* +cr110036 , 2.5g(dual sim)/3g integration */
#ifdef PRODUCT_DM
    uint8           ref_count;
    /*l4_common_hdr_struct header;*/
    uint8           req_id; /*  */
    uint16          app_id; /*  */
    uint16          msg_id; /*  */
    uint16          sim_sw;
    uint16          no_rsp_data_ptr;
    uint16          rsp_data_ptr_size;
#else
    SIGNAL_VARS
#endif
/* -cr110036 */

    SIM_ENVELOPE_RESPONSE_T response;    //the envelope result data come from the sim 
    SIMAT_ENVELOPE_RESULT_E result;     //the envelop result
    uint8 sim_sw1;
    uint8 sim_sw2;
}SIMAT_SIG_ENVELOPE_CNF_T,
SIMAT_SIG_AT_CPDU_ENVELOPE_CNF_T;

//the TERMINAL CONFIRM signal
typedef struct
{
    SIGNAL_VARS
    SIM_TERMINAL_RESPONSE_DATA_T response;  //the respone send to the SIM
}SIMAT_SIG_TERMINAL_RESPONSE_REQ_T;

typedef struct
{
    SIGNAL_VARS
    uint8 sw1;
    uint8 sw2;
}SIMAT_SIG_TERMINAL_RESPONSE_CNF_T;

typedef struct
{
    SIGNAL_VARS
    uint32 poll_interval;      //the polling interval,vlaued in millisecond,0 means stop stk polling
}SIMAT_SIG_POLLING_IND_T;

typedef struct
{
    SIGNAL_VARS
}SIMAT_SIG_TERMINAL_PROFILE_IND_T;

//the signal to notify the STK task that the current STK applciation session
//is terminated
typedef struct
{
    SIGNAL_VARS
    uint32 dual_sys;
}SIMAT_SIG_SESSION_TERMINATED_IND_T;

//the terminal profile request signal structure
typedef struct
{
    SIGNAL_VARS
    SIM_TERMINAL_PROFILE_DATA_T profile;
}SIMAT_SIG_TERMINAL_PROFILE_REQ_T;

//the refresh type
typedef enum
{
    SIMAT_SIG_RF_INITIAL_FULL_FILE_CHANGE,  //SIM initialization and full file change notification
    SIMAT_SIG_RF_FILE_CHANGE,               //SIM file change notification
    SIMAT_SIG_RF_INITIAL_FILE_CHANGE,       //SIM initialization and file change notification
    SIMAT_SIG_RF_INITIAL,                   //SIM initialization
    SIMAT_SIG_RF_RESET,                     //SIM reset
    SIMAT_SIG_RF_3G_APPLICATIN_RESET, 
    SIMAT_SIG_RF_3G_SESSION_RESET,               
    SIMAT_SIG_RF_RESERVED                   //0x05 to 0xff is reversed vlaue
}SIMAT_SIG_REFRESH_TYPE_E;

typedef struct
{
    SIGNAL_VARS
    SIMAT_SIG_REFRESH_TYPE_E refresh_type;
} SIMAT_DPHONE_REFRESH_REQ_T;

//the result of the terminal profile operation
typedef enum
{
    SIM_TP_OK,
    SIM_TP_ERROR,
    SIM_TP_CARD_NOT_READY,
    SIM_TP_CARD_NOT_SUPPORT
}SIM_TERMINAL_PROFILE_RESULT_E;

//the confirm signal struction of the terminal profile request
typedef struct
{
    SIGNAL_VARS
    SIM_TERMINAL_PROFILE_RESULT_E result;
}SIMAT_SIG_TERMINAL_PROFILE_CNF_T;

//the power off ind to the stk layer
typedef struct
{
	SIGNAL_VARS
}SIM_SIG_SIMAT_POWER_OFF_IND_T;

//----------------------------function declaration-----------------------------
/**********************************************************************************************
//    DESCRIPTION
//      Dective the instruction timer,while has received the response data from the SIM card,
//    the timer is deactived at the same time.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************************************/
BOOLEAN SIM_IsCallControlSupport(uint8 cur_sim_task);

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
    uint8  *path_id
);
                                    
/**********************************************************************
//    Description:
//      get the a EF file's file ID
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint16	SIMFS_GetFileID(    //return value:the file ID
				uint32 cur_sim_task,
                                SIM_FILE_NAME_E file_name     //the file name
                                );
                               
/**********************************************************************
//    Description:
//      get the a file's type
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/                                
SIM_FILE_TYPE_E SIMFS_GetFileType(  //return value:the file's file type
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
int8 SIMFS_FileRouting( //reutrn value:the number of file in the routing to the destionation file
                                SIM_FILE_NAME_E file_name,  //the desitination file
                                SIM_FILE_NAME_E cur_file,   //the current selected direction file
                                SIM_FILE_NAME_E* file_route //the file route to the destionation file
                                );

/**********************************************************************
//    Description:
//      The function to collect the SIM Application Toolkit facilities
//    that are supported by the ME and make up the TEMINAL PROFILE data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMAT_GetMeProfile(
                                uint8* profile,     //out:the Teminal profile data
                                uint16* profile_len  //in/out:when in,the length of the buf that to save the teminal profile
                                                    //out,the really terminal profile data length
                                );


#ifdef __cplusplus
    }
#endif
#endif  //_SIM_TO_SIMAT_H_
