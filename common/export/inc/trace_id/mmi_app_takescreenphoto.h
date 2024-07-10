/******************************************************************************
 ** File Name:      mmi_app_takescreenphoto.h                                         *
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
//trace_id:225
#ifndef _MMI_APP_TAKESCREENPHOTO_H_
#define _MMI_APP_TAKESCREENPHOTO_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMITAKESCRPHOTO_MAIN_76_112_2_18_2_56_19_0 "MMIPTSCR_PreCheck: sd not exist!"
#define MMITAKESCRPHOTO_MAIN_93_112_2_18_2_56_19_1 "MMIPTSCR_PreCheck: sd no space!"
#define MMITAKESCRPHOTO_MAIN_139_112_2_18_2_56_19_2 "MMITSCRPHOTO_TakePhoto -- :GetBltInfoForPrintScreen result is failed!"
#define MMITAKESCRPHOTO_MAIN_167_112_2_18_2_56_19_3 "MMITSCRPHOTO_TakePhoto -- :MMIFILE_CreateDir result is failed!"
#define MMITAKESCRPHOTO_MAIN_177_112_2_18_2_56_19_4 "MMITSCRPHOTO_TakePhoto -- :GenPrexfixName result is failed!"
#define MMITAKESCRPHOTO_MAIN_199_112_2_18_2_56_19_5 "MMIPTSCR_Print -- :MMIAPIPTSCR_WriteBMPFile result is failed!"
#define MMITAKESCRPHOTO_MAIN_207_112_2_18_2_56_19_6 "MMITSCRPHOTO_TakePhoto -- :MMIAPIFMM_CombineFullPath result is failed!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_TAKESCREENPHOTO)
TRACE_MSG(MMITAKESCRPHOTO_MAIN_76_112_2_18_2_56_19_0,"MMIPTSCR_PreCheck: sd not exist!")
TRACE_MSG(MMITAKESCRPHOTO_MAIN_93_112_2_18_2_56_19_1,"MMIPTSCR_PreCheck: sd no space!")
TRACE_MSG(MMITAKESCRPHOTO_MAIN_139_112_2_18_2_56_19_2,"MMITSCRPHOTO_TakePhoto -- :GetBltInfoForPrintScreen result is failed!")
TRACE_MSG(MMITAKESCRPHOTO_MAIN_167_112_2_18_2_56_19_3,"MMITSCRPHOTO_TakePhoto -- :MMIFILE_CreateDir result is failed!")
TRACE_MSG(MMITAKESCRPHOTO_MAIN_177_112_2_18_2_56_19_4,"MMITSCRPHOTO_TakePhoto -- :GenPrexfixName result is failed!")
TRACE_MSG(MMITAKESCRPHOTO_MAIN_199_112_2_18_2_56_19_5,"MMIPTSCR_Print -- :MMIAPIPTSCR_WriteBMPFile result is failed!")
TRACE_MSG(MMITAKESCRPHOTO_MAIN_207_112_2_18_2_56_19_6,"MMITSCRPHOTO_TakePhoto -- :MMIAPIFMM_CombineFullPath result is failed!")
END_TRACE_MAP(MMI_APP_TAKESCREENPHOTO)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_TAKESCREENPHOTO_H_

