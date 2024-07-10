/******************************************************************************
 ** File Name:      sim_file_system.h                                         *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.02                                                   *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file about the sim file sytem and the  *
 **					special files for GSM. 									  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.02        Raislin.Kong      create                                   *
 ******************************************************************************/
#ifndef _SIM_FILE_SYSTEM_H_
#define _SIM_FILE_SYSTEM_H_
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define SIM_IS_INVALIDE_FILE(file_name)	((file_name < SIM_FILE_MF) || (file_name >= SIM_FILE_TOTAL_NUM))

//ref to GSM 11.11 section 9.2.1 "SELECT" and GSM11.18
#define SIM_GSM_SPECIFIC_CARD_VOLT              0x70
//the clock stop mark of file characteristics byte
#define SIM_GSM_SPECIFIC_CLOCK_STOP_MARK		0x0d
//the chv1 enable mark of file characteristics bytte
#define SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK		0x80
//the false presentations remain mark of CHV status byte
#define SIM_GSM_SPECIFIC_CS_RAMAIN_MARK			0x0f
//the secret code initialized mark of CHV status byte
#define SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK		0x80

typedef enum
{
	SIM_2G_APPLICATION,
	USIM_3G_APPLICATION,
	SIM_FUTURE_APPLICATION_TYPE
}SIM_APPLICATION_E;

typedef struct
{
	uint8 len;
	uint8 value[16];
}SIM_AID_S;

//the file types in sim card
/*typedef enum
{
	SIM_FILE_T_MF = 0x01,
	SIM_FILE_T_DF = 0x02,
	SIM_FILE_T_EF = 0x04,
	SIM_FILE_T_UNKNOW
}SIM_FILE_TYPE_E;*/

//the ef structure types in sim card
typedef enum
{
	SIM_EF_S_TRANSPARENT	= 0x00,
	SIM_EF_S_LINEAR_FIXED	= 0x01,
	SIM_EF_S_CYCLIC			= 0x03,
	SIM_EF_S_UNKNOW
}SIM_EF_STRUCTURE_E;

#define SIM_SELECT_FILE_TYPE_POS		7
#define SIM_SELECT_DF_MANDATORY_LEN		22
#define SIM_SELECT_EF_MANDATORY_LEN		14
#define SIM_SELECT_REMAIN_LEN_BYTE		13 
#define SIM_SELECT_INVALIDATE_MARK		0x01



//the file level of a sim card file,means the level distance to
//the sim master file
/*typedef enum
{
	SIM_FILE_LEVEL_E_1,
	SIM_FILE_LEVEL_E_2,
	SIM_FILE_LEVEL_E_3,
	SIM_FILE_LEVEL_E_4,
	SIM_FILE_LEVEL_E_NUM
}SIM_FILE_LEVEL_E;*/




//the clock stop allow type
typedef enum
{
	SIM_CLOCK_S_FORBIDEN	= 0x00,	//clock stop not allowed
	SIM_CLOCK_S_ALLOWED_1	= 0x08,	//clock stop not allowed,unless at low level
	SIM_CLOCK_S_ALLOWED_2	= 0x04,	//clock stop not allowed,unless at high level
	SIM_CLOCK_S_ALLOWED_3	= 0x09,	//clock stop allowed,low level perfered
	SIM_CLOCK_S_ALLOWED_4	= 0x05,	//clock stop allowed,high level perfered
	SIM_CLOCK_S_ALLOWED_5	= 0x01	//clock stop allowed,not perfered level
}SIM_CLOCK_STOP_TYPE_E;

//the card supply volt,see GSM11.18 section 4.3
typedef enum
{
    SIM_SUPPLY_VOLT_5V_ONLY,
    SIM_SUPPLY_VOLT_3V_TECH,
    SIM_SUPPLY_VOLT_1_8V_TECH,
    SIM_SUPPLY_VOLT_FUTURE_CLASS
}SIM_SUPPLY_VOLT_E;

//the gsm specific data stucture,ref to GSM 11.11 section 9.2.1 "SELECT"
typedef struct
{
	SIM_CLOCK_STOP_TYPE_E clock_stop_type;	//the clock stop allowed type
	SIM_SUPPLY_VOLT_E card_supply_volt;    //the card supply volt
	BOOLEAN	chv1_enabled;		//the chv1 enabled or not
	uint8	df_child_num;		//number of DFs which are a direct child of the GSM directory
	uint8	ef_child_num;		//number of EFs which are a direct child of the GSM	directory
	SIM_CHV_STATUS_T	chv1_status;	//the chv1 status
	SIM_CHV_STATUS_T	unblock_chv1_status;	//the unblock chv1 status
	SIM_CHV_STATUS_T	chv2_status;	//the chv2 status
	SIM_CHV_STATUS_T	unblock_chv2_status;	//the unblock chv2 status
}SIM_GSM_SPECIFIC_DATA_T;


#ifdef   __cplusplus
    }
#endif
#endif	//_SIM_FILE_SYSTEM_H_