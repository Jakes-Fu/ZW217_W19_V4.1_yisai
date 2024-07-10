/******************************************************************************
 ** File Name:      mmi_app_cs_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:163
#ifndef _MMI_APP_CS_TRC_H_
#define _MMI_APP_CS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMICS_EXPORT_83_112_2_18_2_12_24_0 "[MMIDCD]: MMIAPICS_AppInit !!!! "
#define MMICS_MAIN_137_112_2_18_2_12_25_1 "HandleDCDProcessMsg() enter msg_id = %d"
#define MMICS_MAIN_146_112_2_18_2_12_25_2 "HandleDCDProcessMsg()  exit"
#define MMICS_MAIN_160_112_2_18_2_12_25_3 "[MMIDCD] MMICS_WriteSettingToNv !! "
#define MMICS_MAIN_170_112_2_18_2_12_25_4 "[MMIDCD]:: MMICS_SetSettingDefault enter! "
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_CS_TRC)
TRACE_MSG(MMICS_EXPORT_83_112_2_18_2_12_24_0,"[MMIDCD]: MMIAPICS_AppInit !!!! ")
TRACE_MSG(MMICS_MAIN_137_112_2_18_2_12_25_1,"HandleDCDProcessMsg() enter msg_id = %d")
TRACE_MSG(MMICS_MAIN_146_112_2_18_2_12_25_2,"HandleDCDProcessMsg()  exit")
TRACE_MSG(MMICS_MAIN_160_112_2_18_2_12_25_3,"[MMIDCD] MMICS_WriteSettingToNv !! ")
TRACE_MSG(MMICS_MAIN_170_112_2_18_2_12_25_4,"[MMIDCD]:: MMICS_SetSettingDefault enter! ")
END_TRACE_MAP(MMI_APP_CS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_CS_TRC_H_

