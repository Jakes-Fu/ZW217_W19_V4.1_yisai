/******************************************************************************
 ** File Name:      sim_type.h                                                *
 ** Author:         Yongxia.zhang                                             *
 ** Date:           2006.7                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file define the common structure used for     *
 **                 upperlayer, such as STK module,MN data manage module	  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2006.7         Yongxia.zhang      create                                  *
 ******************************************************************************/

#ifndef _SIM_TYPE_H_
#define _SIM_TYPE_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

#include "sim_macro_switch.h"
#include "sci_types.h"


typedef enum
{
	SIM_FILE_MF,
	SIM_FILE_DF_GSM,
	SIM_FILE_DF_DCS1800,
	SIM_FILE_DF_TELECOM,
	SIM_FILE_DF_IS41,
	SIM_FILE_DF_FP_CTS,
	SIM_FILE_ADF_USIM,//for USIM adaption
	SIM_FILE_ADF_USIM_GSM,//for USIM adaption
	SIM_FILE_DF_IRIDIUM,
	SIM_FILE_DF_GLOBST,
	SIM_FILE_DF_ICO,
	SIM_FILE_DF_ACeS,
	SIM_FILE_DF_MEXE,//SIM_RELEASE99_UPDATE
	SIM_FILE_DF_PCS1900,
	SIM_FILE_DF_CTS,
	SIM_FILE_DF_SoLSA,
	SIM_FILE_DF_TELECOM_PHONEBOOK,//for USIM adaption
	SIM_FILE_DF_GRAPHICS,
	SIM_FILE_ADF_USIM_PHONEBOOK,//for USIM adaption
	SIM_FILE_ADF_USIM_SOLSA,//for USIM adaption
	SIM_FILE_ADF_USIM_MEXE,//for USIM adaption
	SIM_FILE_DF_CDMA,
	SIM_FILE_DF_ORANGE,
	SIM_FILE_EF_ICCID,
	SIM_FILE_EF_DIR,//for USIM adaption
	SIM_FILE_EF_ELP,
	SIM_FILE_EF_MF_ARR,//for USIM adaption
	SIM_FILE_EF_LP,
	SIM_FILE_EF_IMSI,
	SIM_FILE_EF_KC,
	SIM_FILE_EF_PLMNsel,
	SIM_FILE_EF_HPLMN,
	SIM_FILE_EF_ACMmax,
	SIM_FILE_EF_SST,
	SIM_FILE_EF_ACM,
	SIM_FILE_EF_GID1,
	SIM_FILE_EF_GID2,
	SIM_FILE_EF_PUCT,
	SIM_FILE_EF_CBMI,
	SIM_FILE_EF_SPN,
	SIM_FILE_EF_CBMID,
	SIM_FILE_EF_BCCH,
	SIM_FILE_EF_ACC,
	SIM_FILE_EF_FPLMN,
	SIM_FILE_EF_LOCI,
	SIM_FILE_EF_AD,
	SIM_FILE_EF_PHASE,
	SIM_FILE_EF_VGCS,
	SIM_FILE_EF_VGCSS,
	SIM_FILE_EF_VBS,
	SIM_FILE_EF_VBSS,
	SIM_FILE_EF_eMLPP,
	SIM_FILE_EF_AAem,
	SIM_FILE_EF_ECC,
	SIM_FILE_EF_CBMIR,
	SIM_FILE_EF_DCK,
	SIM_FILE_EF_CNL,
	SIM_FILE_EF_NIA,
	SIM_FILE_EF_KcGPRS,
	SIM_FILE_EF_LOCIGPRS,
	SIM_FILE_EF_SUME,
	SIM_FILE_EF_PLMNWACT,
	SIM_FILE_EF_OPLMNWACT,
	SIM_FILE_EF_HPLMNWACT,
	SIM_FILE_EF_CPBCCH,
	SIM_FILE_EF_INVSCAN,
	SIM_FILE_EF_PNN,
	SIM_FILE_EF_OPL,
	SIM_FILE_EF_MBDN,
	SIM_FILE_EF_EXT6,
	SIM_FILE_EF_MBI,
	SIM_FILE_EF_MWIS,
	SIM_FILE_EF_CFIS,
	SIM_FILE_EF_EXT7,
	SIM_FILE_EF_SPDI,
	SIM_FILE_EF_MMSN,
	SIM_FILE_EF_EXT8,
	SIM_FILE_EF_MMSICP,
	SIM_FILE_EF_MMSUP,
	SIM_FILE_EF_MMSUCP,
	SIM_FILE_EF_SAI,
	SIM_FILE_EF_SLL,
	SIM_FILE_EF_ADN,
	SIM_FILE_EF_FDN,
	SIM_FILE_EF_SMS,
	SIM_FILE_EF_CCP,
	SIM_FILE_EF_ECCP,//SIM_RELEASE99_UPDATE
	SIM_FILE_EF_MSISDN,
	SIM_FILE_EF_SMSP,
	SIM_FILE_EF_SMSS,
	SIM_FILE_EF_LND,
	SIM_FILE_EF_SMSR,
	SIM_FILE_EF_SDN,
	SIM_FILE_EF_EXT1,
	SIM_FILE_EF_EXT2,
	SIM_FILE_EF_EXT3,
	SIM_FILE_EF_BDN,
	SIM_FILE_EF_EXT4,
	SIM_FILE_EF_TELECOM_SUME,
	SIM_FILE_EF_CMI,//SIM_RELEASE99_UPDATE
	SIM_FILE_EF_IMG,
	SIM_FILE_EF_IIDF,
	SIM_FILE_EF_CDMA_IMSI,
	SIM_FILE_EF_ONSTRING,
	SIM_FILE_EF_ONSTHORT,
	SIM_FILE_EF_CPHSINFO,
	SIM_FILE_EF_VMWFLAG,
	SIM_FILE_EF_CFFLAG,
	SIM_FILE_EF_CSP,
	SIM_FILE_EF_MBNUM,
	SIM_FILE_EF_INFONUM,
	SIM_FILE_EF_CPHS_SST,
	SIM_FILE_EF_MEXE,
	SIM_FILE_EF_ORPK,
	SIM_FILE_EF_ARPK,
	SIM_FILE_EF_TPRPK,
	SIM_FILE_EF_TELECOM_ARR,
	SIM_FILE_EF_TELECOM_PB_PSC,
	SIM_FILE_EF_TELECOM_PB_CC,
	SIM_FILE_EF_TELECOM_PB_PUID,
	SIM_FILE_EF_TELECOM_PB_PBR,
	SIM_FILE_EF_TELECOM_PB_IAP,
	SIM_FILE_EF_TELECOM_PB_ADN,
	SIM_FILE_EF_TELECOM_PB_EXT1,
	SIM_FILE_EF_TELECOM_PB_PBC,
	SIM_FILE_EF_TELECOM_PB_GRP,
	SIM_FILE_EF_TELECOM_PB_AAS,
	SIM_FILE_EF_TELECOM_PB_GAS,
	SIM_FILE_EF_TELECOM_PB_ANRA,
	SIM_FILE_EF_TELECOM_PB_ANRB,
	SIM_FILE_EF_TELECOM_PB_ANRC,
	SIM_FILE_EF_TELECOM_PB_SNE,
	SIM_FILE_EF_TELECOM_PB_CCP1,
	SIM_FILE_EF_TELECOM_PB_UID,
    SIM_FILE_EF_TELECOM_PB_EMAILA,
    SIM_FILE_EF_TELECOM_PB_EMAILB,
    SIM_FILE_EF_TELECOM_PB1_IAP,
    SIM_FILE_EF_TELECOM_PB1_ADN,
    SIM_FILE_EF_TELECOM_PB1_PBC,
    SIM_FILE_EF_TELECOM_PB1_GRP,
    SIM_FILE_EF_TELECOM_PB1_ANRA,
    SIM_FILE_EF_TELECOM_PB1_ANRB,
    SIM_FILE_EF_TELECOM_PB1_ANRC,
    SIM_FILE_EF_TELECOM_PB1_SNE,
    SIM_FILE_EF_TELECOM_PB1_UID,
    SIM_FILE_EF_TELECOM_PB1_EMAILA,
    SIM_FILE_EF_TELECOM_PB1_EMAILB,
    SIM_FILE_EF_TELECOM_PB2_IAP,
    SIM_FILE_EF_TELECOM_PB2_ADN,
    SIM_FILE_EF_TELECOM_PB2_PBC,
    SIM_FILE_EF_TELECOM_PB2_GRP,
    SIM_FILE_EF_TELECOM_PB2_ANRA,
    SIM_FILE_EF_TELECOM_PB2_ANRB,
    SIM_FILE_EF_TELECOM_PB2_ANRC,
    SIM_FILE_EF_TELECOM_PB2_SNE,
    SIM_FILE_EF_TELECOM_PB2_UID,
    SIM_FILE_EF_TELECOM_PB2_EMAILA,
    SIM_FILE_EF_TELECOM_PB2_EMAILB,
    SIM_FILE_EF_TELECOM_PB3_IAP,
    SIM_FILE_EF_TELECOM_PB3_ADN,
    SIM_FILE_EF_TELECOM_PB3_PBC,
    SIM_FILE_EF_TELECOM_PB3_GRP,
    SIM_FILE_EF_TELECOM_PB3_ANRA,
    SIM_FILE_EF_TELECOM_PB3_ANRB,
    SIM_FILE_EF_TELECOM_PB3_ANRC,
    SIM_FILE_EF_TELECOM_PB3_SNE,
    SIM_FILE_EF_TELECOM_PB3_UID,
    SIM_FILE_EF_TELECOM_PB3_EMAILA,
    SIM_FILE_EF_TELECOM_PB3_EMAILB,
	SIM_FILE_EF_USIM_GSM_KC,
	SIM_FILE_EF_USIM_GSM_KCGPRS,
	SIM_FILE_EF_USIM_GSM_CPBCCH,
	SIM_FILE_EF_USIM_GSM_INVSCAN,
	SIM_FILE_EF_USIM_PB_PSC,
	SIM_FILE_EF_USIM_PB_CC,
	SIM_FILE_EF_USIM_PB_PUID,
	SIM_FILE_EF_USIM_PB_PBR,
	SIM_FILE_EF_USIM_PB_IAP,
	SIM_FILE_EF_USIM_PB_ADN,
	SIM_FILE_EF_USIM_PB_EXT1,
	SIM_FILE_EF_USIM_PB_PBC,
	SIM_FILE_EF_USIM_PB_GRP,
	SIM_FILE_EF_USIM_PB_AAS,
	SIM_FILE_EF_USIM_PB_GAS,
	SIM_FILE_EF_USIM_PB_ANRA,
	SIM_FILE_EF_USIM_PB_ANRB,
	SIM_FILE_EF_USIM_PB_ANRC,
	SIM_FILE_EF_USIM_PB_SNE,
	SIM_FILE_EF_USIM_PB_CCP1,
	SIM_FILE_EF_USIM_PB_UID,
    SIM_FILE_EF_USIM_PB_EMAILA,
    SIM_FILE_EF_USIM_PB_EMAILB,
    SIM_FILE_EF_USIM_PB1_IAP,
    SIM_FILE_EF_USIM_PB1_ADN,
    SIM_FILE_EF_USIM_PB1_PBC,
    SIM_FILE_EF_USIM_PB1_GRP,
    SIM_FILE_EF_USIM_PB1_ANRA,
    SIM_FILE_EF_USIM_PB1_ANRB,
    SIM_FILE_EF_USIM_PB1_ANRC,
    SIM_FILE_EF_USIM_PB1_SNE,
    SIM_FILE_EF_USIM_PB1_UID,
    SIM_FILE_EF_USIM_PB1_EMAILA,
    SIM_FILE_EF_USIM_PB1_EMAILB,
    SIM_FILE_EF_USIM_PB2_IAP,
    SIM_FILE_EF_USIM_PB2_ADN,
    SIM_FILE_EF_USIM_PB2_PBC,
    SIM_FILE_EF_USIM_PB2_GRP,
    SIM_FILE_EF_USIM_PB2_ANRA,
    SIM_FILE_EF_USIM_PB2_ANRB,
    SIM_FILE_EF_USIM_PB2_ANRC,
    SIM_FILE_EF_USIM_PB2_SNE,
    SIM_FILE_EF_USIM_PB2_UID,
    SIM_FILE_EF_USIM_PB2_EMAILA,
    SIM_FILE_EF_USIM_PB2_EMAILB,
    SIM_FILE_EF_USIM_PB3_IAP,
    SIM_FILE_EF_USIM_PB3_ADN,
    SIM_FILE_EF_USIM_PB3_PBC,
    SIM_FILE_EF_USIM_PB3_GRP,
    SIM_FILE_EF_USIM_PB3_ANRA,
    SIM_FILE_EF_USIM_PB3_ANRB,
    SIM_FILE_EF_USIM_PB3_ANRC,
    SIM_FILE_EF_USIM_PB3_SNE,
    SIM_FILE_EF_USIM_PB3_UID,
    SIM_FILE_EF_USIM_PB3_EMAILA,
    SIM_FILE_EF_USIM_PB3_EMAILB,
	SIM_FILE_EF_USIM_LI,
	SIM_FILE_EF_USIM_ARR,
	SIM_FILE_EF_USIM_IMSI,
	SIM_FILE_EF_USIM_KEYS,
	SIM_FILE_EF_USIM_KEYSPS,
    SIM_FILE_EF_USIM_VMWFLAG,
    SIM_FILE_EF_USIM_CPHS_SST,
    SIM_FILE_EF_USIM_CFFLAG,
    SIM_FILE_EF_USIM_ONSTRING,
    SIM_FILE_EF_USIM_CSP,
    SIM_FILE_EF_USIM_CPHSINFO,
    SIM_FILE_EF_USIM_MBNUM,
    SIM_FILE_EF_USIM_ONSTHORT,
    SIM_FILE_EF_USIM_INFONUM,
	SIM_FILE_EF_USIM_DCK,
	SIM_FILE_EF_USIM_HPLMN,
	SIM_FILE_EF_USIM_CNL,
	SIM_FILE_EF_USIM_ACMMAX,
	SIM_FILE_EF_USIM_UST,
	SIM_FILE_EF_USIM_ACM,
	SIM_FILE_EF_USIM_FDN,
	SIM_FILE_EF_USIM_SMS,
	SIM_FILE_EF_USIM_GID1,
	SIM_FILE_EF_USIM_GID2,
	SIM_FILE_EF_USIM_MSISDN,
	SIM_FILE_EF_USIM_PUCT,
	SIM_FILE_EF_USIM_SMSP,
	SIM_FILE_EF_USIM_SMSS,
	SIM_FILE_EF_USIM_CBMI,
	SIM_FILE_EF_USIM_SPN,
	SIM_FILE_EF_USIM_SMSR,
	SIM_FILE_EF_USIM_CBMID,
	SIM_FILE_EF_USIM_SDN,
	SIM_FILE_EF_USIM_EXT2,
	SIM_FILE_EF_USIM_EXT3,
	SIM_FILE_EF_USIM_BDN,
	SIM_FILE_EF_USIM_EXT5,
	SIM_FILE_EF_USIM_CBMIR,
	SIM_FILE_EF_USIM_EXT4,
	SIM_FILE_EF_USIM_EST,
	SIM_FILE_EF_USIM_ACL,
	SIM_FILE_EF_USIM_CMI,
	SIM_FILE_EF_USIM_PLMNWACT,
	SIM_FILE_EF_USIM_OPLMNWACT,
	SIM_FILE_EF_USIM_HPLMNWACT,
	SIM_FILE_EF_USIM_PSLOCI,
	SIM_FILE_EF_USIM_ACC,
	SIM_FILE_EF_USIM_FPLMN,
	SIM_FILE_EF_USIM_LOCI,
	SIM_FILE_EF_USIM_AD,
	SIM_FILE_EF_USIM_VGCS,
	SIM_FILE_EF_USIM_VGCSS,
	SIM_FILE_EF_USIM_VBS,
	SIM_FILE_EF_USIM_VBSS,
	SIM_FILE_EF_USIM_EMLPP,
	SIM_FILE_EF_USIM_AAEM,
	SIM_FILE_EF_USIM_ECC,
	SIM_FILE_EF_USIM_NETPAR,
	SIM_FILE_EF_USIM_PNN,
	SIM_FILE_EF_USIM_OPL,
	SIM_FILE_EF_USIM_MBDN,
	SIM_FILE_EF_USIM_EXT6,
	SIM_FILE_EF_USIM_MBI,
	SIM_FILE_EF_USIM_MWIS,
	SIM_FILE_EF_USIM_CFIS,
	SIM_FILE_EF_USIM_EXT7,
	SIM_FILE_EF_USIM_SPDI,
	SIM_FILE_EF_USIM_MMSN,
	SIM_FILE_EF_USIM_EXT8,
	SIM_FILE_EF_USIM_MMSICP,
	SIM_FILE_EF_USIM_MMSUP,
	SIM_FILE_EF_USIM_MMSUCP,
	SIM_FILE_EF_USIM_NIA,
	SIM_FILE_EF_USIM_EHPLMN,
	SIM_FILE_EF_USIM_EHPLMNPI,
    SIM_FILE_EF_USIM_SPNI,
    SIM_FILE_EF_USIM_PNNI,
	SIM_FILE_EF_USIM_SOLSA_SAI,
	SIM_FILE_EF_USIM_SOLSA_SLL,
	SIM_FILE_EF_USIM_MEXE_SST,
	SIM_FILE_EF_USIM_MEXE_ORPK,
	SIM_FILE_EF_USIM_MEXE_ARPK,
	SIM_FILE_EF_USIM_MEXE_TPRPK,
	SIM_FILE_EF_CST,//CDMA service table
    SIM_FILE_EF_ORANGE_OCT,
    SIM_FILE_EF_ORANGE_DYN2_FLAG,
    SIM_FILE_EF_ORANGE_CSP_LINE2,
    SIM_FILE_EF_ORANGE_PARAM,
    SIM_FILE_EF_ORANGE_ACM2,
    SIM_FILE_EF_ORANGE_DYN_FLAG,
	SIM_FILE_TOTAL_NUM	//the total file number in the sim card
}SIM_FILE_NAME_E;
//used for stk and mn and sim

typedef enum
{
	SIM_RECORD_NEXT		= 0x02,
	SIM_RECORD_PREVIOUS = 0x03,
	SIM_RECORD_ABSOLUTE = 0x04
}SIM_RECORD_ACCESS_MODE_E;
//only used for sim

typedef enum
{
	SIM_PH_1	= 0x00,
	SIM_PH_2	= 0x02,
	SIM_PH_2_PD	= 0x03
}SIM_PHASE_E;
//used for sim and mn

//the chv status
typedef struct
{
	BOOLEAN	chv_verified;
	BOOLEAN	secrect_initialized;	//secrect code initialized or not
	BOOLEAN	chv_blocked;	//chv blocked or not
	uint8	false_remain;	//the number of false presentations remaining
}SIM_CHV_STATUS_T;
//used for sim and mn

//serivce fail reason
typedef enum
{
	SIM_SERVICE_OK,				    //the service finish successful
    SIM_SERVICE_INSTR_P3_ERROR,     //the P3 of the INSTR ERROR
    SIM_SERVICE_INSTR_P2_P1_ERROR,  //the P1 or P2 of the INSTR is ERROR
    SIM_SERVICE_INSTR_UNKNOW_CODE,  //the Insturction code of the INSTR is unknow
    SIM_SERVICE_INSTR_CLASS_ERROR,  //the instruction class of the INSTR is ERROR
    SIM_SERVICE_INSTR_TECHNICAL_PROBLEM,  //technical problem with no diagnostic given
    SIM_SERVICE_MEMORY_PROBLEM,     //the service failed fo memory problem
	SIM_SERVICE_CARD_NOT_ACTIVE,	//the sim card still not been actived
	SIM_SERVICE_INVALIDATE_FILE_NAME,	//the input file name is a invalide one
	SIM_SERVICE_SELECT_NO_SUCH_FILE,	//the want to select file is not exist in the sim card
    SIM_SERVICE_SELECT_DATA_ERROR,      //the select data returned by the card is error
	SIM_SERVICE_SELECT_FILE_ERROR,	//SELECT file error for unknow reason
    SIM_SERVICE_NO_EF_SELECTED,     //no EF selected for the EF operation
	SIM_SERVICE_INCONSISTENT_FILE,	//the destination file is not consistent to the service in 
									//file type or file structure
    //access condtion is not fulfilled to the service
    SIM_SERVICE_CHV1_NEEDED,        //need the right chv1 to do the operation
    SIM_SERVICE_CHV2_NEEDED,        //need the right chv2 to do the operation
    SIM_SERVICE_U_CHV1_NEEDED,      //need the right unblock chv1 to do the operation
    SIM_SERVICE_U_CHV2_NEEDED,      //need the right unblock chv2 to do the operation
    SIM_SERVICE_CHV_BLOCKED,        //both the coresponding CHV has blocked
    SIM_SERVICE_ACCESS_NEVER,       //the access condtion can never been fufilled
	SIM_SERVICE_CONTRADICT_VALIDATION,	//the validation of the destionation file is 
										//contradicte with the service
    SIM_SERVICE_INVALIDE_ADDRESS,
    SIM_SERVICE_INCREASE_MAX_VALUE, //the max value reached and can not perform the increse operation
	SIM_SERVICE_READ_EF_ERROR,		//READ ef file error for unknow reason
	SIM_SERVICE_UPDATE_EF_ERROR,	//UPDATE ef file error for unknow reason
	SIM_SERVICE_INCREASE_EF_ERROR,	//INCREASE ef file error for unknow reason
	SIM_SERVICE_INVALIDATE_EF_ERROR,	//INVALIDATE ef file error for unknow reason
	SIM_SERVICE_REHABILITATE_EF_ERROR,	//REHABILIDATE ef file error for unknow reason
	SIM_SERVICE_SEEK_NOT_FIND_PATTERN,	//do not file the pattern in destination file
	SIM_SERVICE_SEEK_EF_ERROR,		//SEEK in file error for unknow reason
	SIM_SERVICE_CHV_NOT_INITIALIZED,	//the chv still not been initialized
	SIM_SERVICE_CHV_CONTRADICT_STATUS,	//the chv status condicate to the service
    SIM_SERVICE_CHV_VER_FAIL,           //- unsuccessful CHV verifaiction,at least one attemp left
										//- unsuccessful UNBLOCK CHV verification,at least one attemp left          
	SIM_SERVICE_CHV_FAIL_WITH_BLOCKED,	//- unsuccessful CHV verifaiction,no attemp left
										//- unsuccessful UNBLOCK CHV verification,no attemp left
										//- CHV blocked
										//- UNBLOCK CHV blocked
    SIM_SERVICE_GSM_ALGORITHM_ERROR_DF, //when runing the GSM algorithm,the current DF is not GSM or
                                        //child DF of DF_gsm
	SIM_SERVICE_VERIFY_CHV_ERROR,		//VERIFY CHV error for unknow reason
	SIM_SERVICE_CHANGE_CHV_ERROR,		//CHANGE CHV error for unknow reason
	SIM_SERVICE_DISABLE_CHV_ERROR,		//DISABLE CHV error for unknow reason
	SIM_SERVICE_ENABLE_CHV_ERROR,		//ENABLE CHV error for unknow reason
	SIM_SERVICE_UNBLOCK_CHV_ERROR,		//UNBLOCK CHV error for unknow reason
	SIM_SERVICE_RUN_ALGORITHM_ERROR,	//RUN GSM ALPORITHM error for unknow reason
	SIM_SERVICE_SLEEP_ERROR,			//SLEEP error for unknow reason
	SIM_SERVICE_STATUS_ERROR,			//STATUS error for unknow reason
	SIM_SERVICE_DECODE_EF_ERROR,		//error when decode the ef file content to ef mapping structure
	SIM_SERVICE_DECODE_EMPTY_RECORD,	//the decode record is a empty record
	SIM_SERVICE_ENCODE_ERROR,			//error when encode ef mapping structure to the ef file content
	SIM_SERVICE_NOT_ENABLE,				//the coresponding serivce not enalbe
	SIM_SERVICE_INVALIDE_TARGET,		//the sub service target file is invalide
	SIM_SERVICE_IMPOSSIBLE,				//the service will always fail
	SIM_SERVICE_RX_PARITY_ERROR,        //there are Rx Parity error in the Receive procedure
	SIM_SERVICE_TX_PARITY_ERROR,        //there are Tx Parity Error in the instruction transmit procedure
	SIM_SERVICE_CARD_OUT,               //the SIM card are out in the service proccess
	SIM_SERVICE_CARD_UNRESPONSE,        //the card is unresponse
    SIM_SERVICE_RUN_ALGORITHM_MAC_ERROR,    //: 98 62 mac incorrect
    SIM_SEC_STATUS_NOT_SATISFIED,	     //69 82, Security status not satisfied
    SIM_SERVICE_REF_DATA_NOT_FOUND,     //6A 88
	SIM_SERVICE_ERROR_UNKNOW			//service error for unknow reaseon	
}SIM_SERVICE_RESULT_E;
//used for sim and mn

typedef struct SIM_SW_AND_RESPONSE_T_s
{
	uint8	sw1;
	uint8	sw2;
	uint16	data_len;
	uint8	data[512];
} SIM_SW_AND_RESPONSE_T;

//the select intstuction response data,
//refernce to GSM11.11 section 9.2.1
typedef struct
{
	uint16	sim_rfu1;
	uint16	freespace_filesize;
	uint16	file_id;
	uint8	file_type;
	uint8	cyclic_full_check;
	uint16	access_condition1;
	uint8	access_condition2;
	uint8	file_status;
	uint8	remain_len;
	uint8	filecharacter_efstructure;
	uint8	dfnumber_recordlen;
	uint8	ef_number;
	uint8	chv_ad_code_num;
	uint8	sim_rfu2;
	uint8	chv1_status;
	uint8	unblock_chv1_status;
	uint8	chv2_status;
	uint8	unblock_chv2_status;
#ifndef SIM_TEST_IN_WIN32
    uint8 usim_life_cycle_status;//Only for 3G USIM card
#endif    
}SIM_RESPONSE_DATA_OF_SELECT_T;
//used for sim and mn

//the record setting of some EF file in different SIM card will be different,
//this structure is use to save the EF setting
typedef struct
{
	uint8	alpha_id_len;			//the alpha id len in the EF record
	uint8	max_alpha_id_len;		//the max alpha id len that will be supported in our mobile
	uint8 original_alpha_id_len;  //the original alpha id length of the reocord
	uint8 other_content_len;      //the length of other content except the alpha id
	uint16 record_num;			//the record num in the EF
	uint16 max_record_num;	//the max record num that will be supported in our mobile
}SIM_RECORD_EF_INFO_T;
//used for sim and mn

//the sim layer working state
typedef enum
{
	SIM_NO_POWER_STATE,     //the state that still no power on SIM card seat
	SIM_NULL_STATE,         //power on the SIM and waiting for CARD present
	SIM_ATR_STATE,          //the SIM cad waiting for ATR
	SIM_INITIALIZE_STATE,
	SIM_ACTIVE_STATE
}SIM_CARD_STATE_E;
//used for sim and mn

// add by fancier ???
typedef struct
{
	uint8       id_len;
	uint8       id[SIM_MAX_SMSP_ALPHA_ID_LEN];
}SIM_ALPHA_ID;
//need include "sim_macro_switch.h",but the type is only used by mn

//the file access condtion class of sim file access condition
typedef enum
{
	SIM_ACCESS_ALW,
	SIM_ACCESS_CHV1,
	SIM_ACCESS_CHV2,
	SIM_ACCESS_RFU,
	SIM_ACCESS_ADM1,
	SIM_ACCESS_ADM2,
	SIM_ACCESS_ADM3,
	SIM_ACCESS_ADM4,
	SIM_ACCESS_ADM5,
	SIM_ACCESS_ADM6,
	SIM_ACCESS_ADM7,
	SIM_ACCESS_ADM8,
	SIM_ACCESS_ADM9,
	SIM_ACCESS_ADM10,
	SIM_ACCESS_ADM11,
	SIM_ACCESS_NEVER
}SIM_ACCESS_CONDITION_E;
//used by sim and mn

//the access condition type
typedef enum
{
    SIM_READ_ACCESS,
    SIM_UPDATE_ACCESS,
    SIM_INCREASE_ACCESS,
    SIM_INVALIDATE_ACCESS,
    SIM_REHABILITATE_ACCESS
}SIM_ACCESS_CONDITION_TYPE_E;
//used by sim and mn

//the file types in sim card
/*typedef enum
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
}SIM_FILE_LEVEL_E;*/

typedef enum
{
	SIM_CARD_ONE,
	SIM_CARD_TWO,
#if defined MULTI_SIM_SYS_TRI || defined MULTI_SIM_SYS_QUAD
	SIM_CARD_THREE,
#endif
#ifdef MULTI_SIM_SYS_QUAD
	SIM_CARD_FOUR,
#endif
	SIM_CARD_NUM
}SIM_CARD_INDEX_E;

//the refresh type
typedef enum
{
    SIM_RF_INITIAL_FULL_FILE_CHANGE,  //SIM initialization and full file change notification
    SIM_RF_FILE_CHANGE,               //SIM file change notification
    SIM_RF_INITIAL_FILE_CHANGE,       //SIM initialization and file change notification
    SIM_RF_INITIAL,                   //SIM initialization
    SIM_RF_RESET                     //SIM reset
}SIM_REFRESH_TYPE_E;

typedef enum
{
    SIM_CDMA_NONE,
    SIM_CDMA_ONLY,
    SIM_CDMA_GSM_DUAL
}SIM_CDMA_TYPE_E;
//only used for sim

#ifdef __cplusplus
    }
#endif
#endif  //_SIM_TYPE_H_
