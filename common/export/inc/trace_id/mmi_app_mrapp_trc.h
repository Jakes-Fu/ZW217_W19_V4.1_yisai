/******************************************************************************
 ** File Name:      mmi_app_mrapp_trc.h                                         *
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
//trace_id:244
#ifndef _MMI_APP_MRAPP_TRC_H_
#define _MMI_APP_MRAPP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIMRAPP_FUNC_6212_112_2_18_3_12_45_0 "MP3: %d 0x%02x 0x%02x 0x%02x 0x%02x"
#define MMIMRAPP_FUNC_6221_112_2_18_3_12_45_1 "PIC: %d 0x%02x 0x%02x 0x%02x 0x%02x"
#define MMIMRAPP_PHONEBOOK_65_112_2_18_3_12_50_2 "name_watr[%02d] = 0x%02x"
#define MMIMRAPP_PHONEBOOK_86_112_2_18_3_12_51_3 "search_name_watr[%02d] = 0x%02x"
#define MMIMRAPP_PHONEBOOK_319_112_2_18_3_12_51_4 "MMIMRAPP_PHBSetEntry del req->index = %d"
#define MMIMRAPP_PHONEBOOK_399_112_2_18_3_12_51_5 "MMIMRAPP_PHBSetEntry add req->index = %d"
#define MMIMRAPP_PHONEBOOK_432_112_2_18_3_12_51_6 "MMIMRAPP_PHBSetEntry modify req->index = %d"
#define MMIMRAPP_WINTAB_479_112_2_18_3_12_53_7 "mrapp MMIMRAPP_StartApp param = %s"
#define MMIMRAPP_WINTAB_500_112_2_18_3_12_53_8 "mrapp MMIMRAPP_StartApp entry = %s param = %s"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MRAPP_TRC)
TRACE_MSG(MMIMRAPP_FUNC_6212_112_2_18_3_12_45_0,"MP3: %d 0x%02x 0x%02x 0x%02x 0x%02x")
TRACE_MSG(MMIMRAPP_FUNC_6221_112_2_18_3_12_45_1,"PIC: %d 0x%02x 0x%02x 0x%02x 0x%02x")
TRACE_MSG(MMIMRAPP_PHONEBOOK_65_112_2_18_3_12_50_2,"name_watr[%02d] = 0x%02x")
TRACE_MSG(MMIMRAPP_PHONEBOOK_86_112_2_18_3_12_51_3,"search_name_watr[%02d] = 0x%02x")
TRACE_MSG(MMIMRAPP_PHONEBOOK_319_112_2_18_3_12_51_4,"MMIMRAPP_PHBSetEntry del req->index = %d")
TRACE_MSG(MMIMRAPP_PHONEBOOK_399_112_2_18_3_12_51_5,"MMIMRAPP_PHBSetEntry add req->index = %d")
TRACE_MSG(MMIMRAPP_PHONEBOOK_432_112_2_18_3_12_51_6,"MMIMRAPP_PHBSetEntry modify req->index = %d")
TRACE_MSG(MMIMRAPP_WINTAB_479_112_2_18_3_12_53_7,"mrapp MMIMRAPP_StartApp param = %s")
TRACE_MSG(MMIMRAPP_WINTAB_500_112_2_18_3_12_53_8,"mrapp MMIMRAPP_StartApp entry = %s param = %s")
END_TRACE_MAP(MMI_APP_MRAPP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MRAPP_TRC_H_

