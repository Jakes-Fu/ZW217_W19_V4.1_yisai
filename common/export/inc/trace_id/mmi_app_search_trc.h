/******************************************************************************
 ** File Name:      mmi_app_search_trc.h                                         *
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
//trace_id:220
#ifndef _MMI_APP_SEARCH_TRC_H_
#define _MMI_APP_SEARCH_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMISEARCH_APP_1089_112_2_18_2_49_56_0 "MMIAPISEARCH_SaveMatchItems: SAVE match data error!"
#define MMISEARCH_SEARCHAPPLICATION_108_112_2_18_2_49_58_1 "MMI Search: APP_OpenMatchedItem MACRO MMI_GRID_IDLE_SUPPORT not defined"
#define MMISEARCH_SEARCHAPPLICATION_155_112_2_18_2_49_58_2 "APP_SetMatchItemContent: dynamic app not found"
#define MMISEARCH_SEARCHAPPLICATION_199_112_2_18_2_49_58_3 "APP_SetMatchItemData: dynamic app not found"
#define MMISEARCH_SEARCHAPPLICATION_204_112_2_18_2_49_58_4 "APP_SetMatchItemData: item don not have icon"
#define MMISEARCH_SEARCHAPPLICATION_287_112_2_18_2_49_58_5 "MMISEARCH_LocalSearchAPP: MENU_MAINMENU_ICON menu not get menu group"
#define MMISEARCH_SEARCHAPPLICATION_304_112_2_18_2_49_58_6 "MMISEARCH_LocalSearchAPP: append_count cross MMIAPP_SEARCH_MAX_NUM limited"
#define MMISEARCH_SEARCHAPPLICATION_344_112_2_18_2_49_58_7 "MMISEARCH_LocalSearchAPP: dyanmic append_count cross MMIAPP_SEARCH_MAX_NUM limited"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_SEARCH_TRC)
TRACE_MSG(MMISEARCH_APP_1089_112_2_18_2_49_56_0,"MMIAPISEARCH_SaveMatchItems: SAVE match data error!")
TRACE_MSG(MMISEARCH_SEARCHAPPLICATION_108_112_2_18_2_49_58_1,"MMI Search: APP_OpenMatchedItem MACRO MMI_GRID_IDLE_SUPPORT not defined")
TRACE_MSG(MMISEARCH_SEARCHAPPLICATION_155_112_2_18_2_49_58_2,"APP_SetMatchItemContent: dynamic app not found")
TRACE_MSG(MMISEARCH_SEARCHAPPLICATION_199_112_2_18_2_49_58_3,"APP_SetMatchItemData: dynamic app not found")
TRACE_MSG(MMISEARCH_SEARCHAPPLICATION_204_112_2_18_2_49_58_4,"APP_SetMatchItemData: item don not have icon")
TRACE_MSG(MMISEARCH_SEARCHAPPLICATION_287_112_2_18_2_49_58_5,"MMISEARCH_LocalSearchAPP: MENU_MAINMENU_ICON menu not get menu group")
TRACE_MSG(MMISEARCH_SEARCHAPPLICATION_304_112_2_18_2_49_58_6,"MMISEARCH_LocalSearchAPP: append_count cross MMIAPP_SEARCH_MAX_NUM limited")
TRACE_MSG(MMISEARCH_SEARCHAPPLICATION_344_112_2_18_2_49_58_7,"MMISEARCH_LocalSearchAPP: dyanmic append_count cross MMIAPP_SEARCH_MAX_NUM limited")
END_TRACE_MAP(MMI_APP_SEARCH_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_SEARCH_TRC_H_

