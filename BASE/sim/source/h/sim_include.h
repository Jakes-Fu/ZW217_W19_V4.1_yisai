/******************************************************************************
 ** File Name:      sim_incldue.h                                             *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2002.2                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file include the commonly need file in *
 **                 SIM task                                                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2002.2         Raislin.Kong     Create                                    *
 ******************************************************************************/

#ifndef _SIM_INCLUDE_H_
#define _SIM_INCLUDE_H_

#include "sci_api.h"
#include "tasks_id.h"
#include "mn_type.h"
#include "gsm_gprs.h"             //need by PLMN_T

//#include "sim_outport.h"         //need by mnsms and mncall
#include "sim_macro_switch.h"
#include "sim_type.h"
#include "sim_to_simat.h"        //need by stk module
#include "env_simat_signal.h"    //need by sim_signal.c
#include "sim_api.h"
#include "sim_file_structure.h"
#include "sim_signal.h"
#include "sim_dialling_number.h"

#include "mnsms_sim_type.h"      //need by sim_sms.c
#include "sim_file_system.h"
#include "sim_instruction.h"
#include "sim_sms.h"
#include "sim_main.h"
#include "sim_global_variable.h"
#include "sim_global_function.h"
#include "sig_code.h"
#include "env_sim_signal.h"
#include "env_simat_signal.h"
#include "env_mn_signal.h"

#include "sim_hal.h"

#ifdef   __cplusplus
extern   "C"
{
#endif
#define UINT8_INVALIDE      0xff
#define UINT16_INVALIDE     0xffff
#define UINT32_INVALIDE     0xffffffff
#ifdef   __cplusplus
}
#endif
#endif  //_SIM_INCLUDE_H_
