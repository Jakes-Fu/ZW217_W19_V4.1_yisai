/***************************************************************************************
** FileName    :  atest_location.h                                                    **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  24/02/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  this file is used for checking whether the location information for **
**                each mobile phone in the location database file is accord with the  **
**                information in the mobile phone simulator.                          **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  26/02/2010     Mingxuan.Zhu         Created                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#ifndef _ATEST_LOCATION_H
#define _ATEST_LOCATION_H

/**-----------------------------------------------------------------------------------*
 **                                   Dependencies                                    *
 **-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "sci_types.h"
#include "atest_signal.h"

#ifdef WIN32
/**-----------------------------------------------------------------------------------*
 **                                Macro Declarations                                 *
 **-----------------------------------------------------------------------------------*/
#define PHONE_LOCATION_NAME_LENGTH         30
#define PHONE_LOCATION_TOTAL_NAME_LENGTH   60
#define PHONE_NUMBER_LENGTH                12  // 11 pos + 1
#define RECORD_LENGTH                      60
#define FILE_PATH_LENGTH                   256
#define FILE_QUANTITY                      6
#define FILE_NAME_LENGTH                   20
#define DIGIT_NUMBER                       5
#define AREA_CODE_LENGTH                   5     

/**-----------------------------------------------------------------------------------*
 **                               Variable Declarations                               *
 **-----------------------------------------------------------------------------------*/
FILE *fpSourceFile;
FILE *fpResultFile;
FILE *fpStatResultFile;
char record[RECORD_LENGTH];
char statData[RECORD_LENGTH];

typedef enum
{
   ALL = 0,
   MOBILE_PHONE,
   FIXED_LINE,
   OTHER_TYPE
}PHONE_TYPE_E;

/**-----------------------------------------------------------------------------------*
 **                                Function Prototypes                                *
 **-----------------------------------------------------------------------------------*/

/**************************************************************************************/
// Description      : check the location file in the location database file by check type.
//--------------------------------------------------------------------------------------
// Input Parameter  : DPARAM param
//                    
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTLOC_TestPhoneLocation(ATEST_APP_DATA_T *loc_signal_ptr);

#endif//WIN32

#endif//_ATEST_LOCATION_H