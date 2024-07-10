/******************************************************************************
 ** File Name:      ConPan.h                                                  *
 ** Author:         Liu Jun                                                   *
 ** Date:           12/03/2003                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file declares the functions which will be exported   *
 **                 to the simulator.                                         *
 ******************************************************************************

 ******************************************************************************
 **                         Edit History                                      *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                                   *
 ** 12/03/2003      Liu Jun     Create based on ConPan.h by Jianyong Gao      *
 ******************************************************************************/
 
#ifndef __CONPAN_H__
#define __CONPAN_H__


/**---------------------------------------------------------------------------*
 **                         DLL Import/Export Macro                           *
 **---------------------------------------------------------------------------*/
 
#ifdef _CONPAN_DLL
	#define _CONPAN_DLLPORT __declspec(dllexport)
#else
	#define _CONPAN_DLLPORT __declspec(dllimport)
#endif


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef MSCODE_MOD
#include "scttypes.h"
#include "ctypes.h"
#include "gsm_gprs.h"
#endif 

#include "sci_api.h"
#include "sci_types.h"

#ifdef MSCODE_MOD
#include "tb_comm.h"
#include "sim_to_simat.h"
#include "sim_file_structure.h"
#include "sim_signal.h"
#else
#include "sim_outport.h"
#endif

#include "sim_file_system.h"
#include "sim_instruction.h"


/**---------------------------------------------------------------------------*
 **                         Exported Functions                                *
 **---------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_StartControlPanel
// AUTHOR:          Jianyong Gao
// INPUT:           nX the position of the left side ,
//                  nY the position of the top side. 
//                  nX = -1, nY = -1 will center control panel in screen.
// RETURN:          TRUE if Control Panel is started successfully,else FALSE.
// DESCRIPTION:     This is a thread function ,it creates the control panel.
// ----------------------------------------------------------------------------
_CONPAN_DLLPORT BOOL CP_StartControlPanel(int nX = -1, int nY = -1);

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_EndControlPanel
// AUTHOR:          Jianyong Gao
// INPUT:           None
// RETURN:          None
// DESCRIPTION:     Stop the control panel.
// ----------------------------------------------------------------------------
_CONPAN_DLLPORT void CP_EndControlPanel();

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_ShowControlPanel
// AUTHOR:          Hongliang Xin
// INPUT:           None
// RETURN:          None
// DESCRIPTION:     Show or hide the control panel.
// ----------------------------------------------------------------------------
_CONPAN_DLLPORT void CP_ShowControlPanel(BOOL bShow = TRUE);


/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

#ifndef CONPAN_INTERNAL_USE
 
// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_RegisterFunctions
// AUTHOR:          Liu Jun
// INPUT:           The external function pointers stored in a structure.
// RETURN:          None
// DESCRIPTION:     Calling this function when the simulator starts.
// ----------------------------------------------------------------------------

typedef struct CP_EXT_FUNC_LIST_tag // External function list
{
    // RTOS functions
    void (*fpOS_ThreadContextSave)();
    void (*fpOS_ThreadContextRestore)();
    void (*fpOS_CreateSignal)(xSignalHeader *ppSig,
                              uint16         sigCode,
                              uint16         sigSize,
                              BLOCK_ID       sender);
    void (*fpOS_SendSignal)(xSignalHeader    pSig, 
                            BLOCK_ID         receiver);
    void (*fpOS_FreeSignal)(xSignalHeader    pSig);

    // Callback function for PS to send message to Control Panel
    void (*fpPS_RegisterMsgHandler)(int (*fpCallback)(xSignalHeader psig));

    // SIM-Card functions
	BOOLEAN					(*fpSIMINT_QuerySIM)(void);
	void					(*fpSIMINT_RejectSIM)(void);
	void					(*fpSIMINT_InsertSIM)(void);
    void					(*fpSIMMAIN_ResetSIMVariable)(void);
	SIM_SERVICE_RESULT_E	(*fpSIMINSTR_EncodeEFInfo)( //retyrn value:the result of the encode operation
									SIM_EF_FILE_INFO_T ef_info,   //the ef info to be encode in to select data
									SIM_RESPONSE_DATA_OF_SELECT_T* select_data   //out:the pointer of the buf that save the encoded select data
									);
	SIM_SERVICE_RESULT_E	(*fpSIMINSTR_EncodeDFInfo)( //return value:the result of the Encode operation
									SIM_DF_MF_INFO_T df_info, //the DF info to be encoded in to select data
									SIM_RESPONSE_DATA_OF_SELECT_T* select_data    //out:save the select data that encoded into
									);
	BOOLEAN					(*fpSIMINSTR_DecodeSelectData)(  //return value:the type of info that decode out
									SIM_RESPONSE_DATA_OF_SELECT_T select_data,   //the select data to be decoded
									SIM_EF_FILE_INFO_T* ef_info, //out:buf to save the ef data info
									SIM_DF_MF_INFO_T* df_info    //out bug to save the df data info
									);
	SIM_FILE_NAME_E			(*fpSIMFS_GetParenFileName)( //return value:parent file name
									SIM_FILE_NAME_E file_name   //the file name
									);
	
	SIM_SERVICE_RESULT_E	(*fpSIMSER_ReadWholeTransparentEF)( //return value:the result of the read operation
									SIM_FILE_NAME_E file_name,  //the Transparent file to be readed
									void* ef_mapping    //out:the file conten mapping sturcute
									);
	SIM_SERVICE_RESULT_E	(*fpSIMSER_ReadEFRecord)(   //return vlaue:the result of the read operation
									SIM_FILE_NAME_E file_name,  //the file to be readed
									uint8 record_num,   //the record num to read
									SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
									void* ef_mapping    //out:the file cotent read out 
									);
	SIM_SERVICE_RESULT_E	(*fpSIMSER_UpdateWholeTransparentEF)(   //return value:the result of the update operation
									SIM_FILE_NAME_E file_name,  //the file to be updated
									void* ef_mapping    //in:the file content mapping used to update
									);
	SIM_SERVICE_RESULT_E	(*fpSIMSER_UpdateEFRecord)( //return value:the result of the update operation
									SIM_FILE_NAME_E file_name,  //the file to be updated
									uint8 record_num,   //the reocrd num to update
									SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
									void* ef_mapping,   //in:the file cotent mapping used to update
									BOOLEAN erasure_record  //erasure the record to empty or not
									);
	SIM_SERVICE_RESULT_E	(*fpSIMTIW_GetSelectFileName)(  //return value:the result of the operation
									SIM_FILE_NAME_E file_name,  //the file name 
									char** file_path    //out:the pointer of the file name
									);
	SIM_SERVICE_RESULT_E	(*fpSIMTIW_UpdateAllCHV)(   //return vlaue:the result of the operation
									SIM_ALL_CHV_T* chvs //in:all of the sim chvs
									);
	SIM_SERVICE_RESULT_E	(*fpSIMTIW_GetAllCHV)(  //return value:the result of the operation
									SIM_ALL_CHV_T* chvs //out:all of the sim chv
									);
	SIM_SERVICE_RESULT_E	(*fpSIMTIW_SelectFile)( //return value:the result of the operation
									SIM_FILE_NAME_E file_name,  //the sim file name
									SIM_RESPONSE_DATA_OF_SELECT_T* select_data  //out:the select data of the sim file
									);
	SIM_SERVICE_RESULT_E	(*fpSIMTIW_UpdateSelectData)(   //the result of the operation
									SIM_FILE_NAME_E file_name,  //the sim file
									SIM_RESPONSE_DATA_OF_SELECT_T select_data   //the select data of the sim file
									);
	void					(*fpSIMTIW_GetCHVVerificated)(
									BOOLEAN* chv1_v,	//out:the chv1 verificated state
									BOOLEAN* chv2_v 	//out:the chv1 verificated state
									);
	void					(*fpSIMTIW_SetCHVVerificated)(
									BOOLEAN chv1_v,	//the chv1 verificated state
									BOOLEAN chv2_v 	//the chv1 verificated state
									);
	SIM_SERVICE_RESULT_E	(*fpSIMTIW_CreatEFFile)(    //return vlaue:the result of the operation 
									SIM_FILE_NAME_E file_name   //the sim file name
									);    
}
CP_EXT_FUNC_LIST;

_CONPAN_DLLPORT void CP_RegisterFunctions(CP_EXT_FUNC_LIST *pFuncList);

#endif // CONPAN_INTERNAL_USE

#endif  // __CONPAN_H__