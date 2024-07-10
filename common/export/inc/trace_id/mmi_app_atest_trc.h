/******************************************************************************
 ** File Name:      mmi_app_atest_trc.h                                         *
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
//trace_id:149
#ifndef _MMI_APP_ATEST_TRC_H_
#define _MMI_APP_ATEST_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define ATEST_LOCATION_530_112_2_18_2_2_59_0 "ATEST_TRACE====NORMAL EXIT LOCATION TESTING!"
#define ATEST_MAIN_113_112_2_18_2_3_0_1 "ATEST_TRACE====Enter LOCATION MODULE!"
#define ATEST_OTA_232_112_2_18_2_3_0_2 "ATEST_TRACE====START OTA TESTING!"
#define ATEST_OTA_829_112_2_18_2_3_2_3 "ATEST_TRACE====Enter OTA MODULE!"
#define ATEST_OTA_845_112_2_18_2_3_2_4 "ATEST_TRACE====NORMAL EXIT OTA TESTING!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_ATEST_TRC)
TRACE_MSG(ATEST_LOCATION_530_112_2_18_2_2_59_0,"ATEST_TRACE====NORMAL EXIT LOCATION TESTING!")
TRACE_MSG(ATEST_MAIN_113_112_2_18_2_3_0_1,"ATEST_TRACE====Enter LOCATION MODULE!")
TRACE_MSG(ATEST_OTA_232_112_2_18_2_3_0_2,"ATEST_TRACE====START OTA TESTING!")
TRACE_MSG(ATEST_OTA_829_112_2_18_2_3_2_3,"ATEST_TRACE====Enter OTA MODULE!")
TRACE_MSG(ATEST_OTA_845_112_2_18_2_3_2_4,"ATEST_TRACE====NORMAL EXIT OTA TESTING!")
END_TRACE_MAP(MMI_APP_ATEST_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_ATEST_TRC_H_

