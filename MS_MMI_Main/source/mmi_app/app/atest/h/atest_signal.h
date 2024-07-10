/***************************************************************************************
** FileName    :  atest_signal.h                                                      **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  14/05/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The signal of the atest module is defined in this file.             **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  27/09/2010     Mingxuan.Zhu         Created                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#ifndef _ATEST_SIGNAL_H
#define _ATEST_SIGNAL_H

/**-----------------------------------------------------------------------------------*
 **                                   Dependencies                                    *
 **-----------------------------------------------------------------------------------*/
#include "tasks_id.h"
#include "mn_type.h"

/**-----------------------------------------------------------------------------------*
 **                                Macro Declarations                                 *
 **-----------------------------------------------------------------------------------*/
#define ATEST_STRING_MAX_LEN            1000  // the max length of config information - 100*10(record)
#define ATEST_FILE_PATH_MAX_LEN         260   // the max length of the file path
#define CPANEL_ATEST_REQ                ((( P_CPANEL << 8 ) | 1) + 200)
#define ATEST                           9     // the start signal of ATEST module

/**-----------------------------------------------------------------------------------*
 **                                   struct definitions                              *
 **-----------------------------------------------------------------------------------*/
typedef enum
{
    ATEST_ENTER_IDLE_IND = ATEST + 1,
    ATEST_SEND_FIRST_OTA_REQ,
    ATEST_SEND_OTA_IND,
    ATEST_SEND_NEXT_OTA_REQ,
    ATEST_END_OTA_CNF,
    ATEST_LOCATION_IND,
    ATEST_END_LOCATION_REQ,
	ATEST_PB_RUN_IND,
	ATEST_PB_SELF_IND,
	ATEST_R8_RUN_IND,
	ATEST_MC_PB_RUN_REQ,
	ATEST_MC_PB_SELF_REQ,
	ATEST_MC_R8_RUN_REQ,
	ATEST_INPUTMETHOD_RUN_IND,
    ATEST_MC_INPUTMETHOD_INFO,
    ATEST_CALENDAR_RUN_IND,
    ATEST_PLMN_RUN_IND,
    ATEST_MC_CALENDAR_INFO,
    ATEST_MC_PLMN_INFO,
}ATEST_SIGNAL_E;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E     dual_sys;
    uint16            signal_id;
    uint16            check_type;
    uint16            status_code;
    char              record_data[ATEST_STRING_MAX_LEN];
	char              test_data[ATEST_STRING_MAX_LEN];
}ATEST_APP_SIG_T;    // the data package to control panel

typedef struct
{
    MN_DUAL_SYS_E     dual_sys;
    uint16            signal_id;
    uint16            check_type;
    uint16            status_code;
    char              source_file_path[ATEST_FILE_PATH_MAX_LEN];
    char              result_file_path[ATEST_FILE_PATH_MAX_LEN];
	char              config_file_path[ATEST_FILE_PATH_MAX_LEN];
    char              record_data[ATEST_STRING_MAX_LEN];
}ATEST_APP_DATA_T;    // the data package from control panel

#endif//_ATEST_SIGNAL_H
