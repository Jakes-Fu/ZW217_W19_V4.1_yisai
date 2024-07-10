/******************************************************************************
 ** File Name:      sys_time_ext.h                                            *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           06/18/2004                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 05/30/2002      Xueliang.Wang   Create.                                   *
 ** 29/08/2005     weihua.wang      Modify for  CR29225                       *
 ******************************************************************************/

#ifndef _SYS_TIME_EXT_H
#define _SYS_TIME_EXT_H

#include "os_api.h"
#include "dal_time.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// Start time: 2000/01/01/ 00:00:00'
// End   time: 2049/12/31/ 23:59:59'
#define TM_YEAR_START				2000
#define TM_MONTH_START				1
#define TM_DAY_START			    1
#define TM_WEEK_START				6

#define TM_MAX_MONTHS_SUPPORTED   	12

typedef enum {
    TM_INT_START = 0,
    TM_INT_SEC = TM_INT_START,
    TM_INT_MIN ,
    TM_INT_HOUR,
    TM_INT_DAY ,
    TM_INT_ALARM,
    TM_INT_MAX  
} TM_INTTYPE_E;
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :   Initialize system date & time. 
//	Global resource dependence : 
//  Author:         Eric.zhou
//	Note: 
/*****************************************************************************/
PUBLIC void TM_Init(void);

/*****************************************************************************/
// 	Description :   Get the week-day of the inputed date.
//					If date is between 2000/01/01/00:00:00 and 
//					2049/12/31/23:59:59, convert result(saved in week_day_ptr)
//					is correct; else it is 0.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note:           Value: 0 - 6
//						Sunday = 0
//                  	Monday = 1
//                  	......
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetWeekDayOfDate(    // If successful, return ERR_TM_NONE
                                        // else return ERR_TM_WRONG_DATE or ERR_TM_PNULL.
    SCI_DATE_T  date_val,               // Date value
    uint8       *week_day_ptr           // Save the week day if the date is correct;
                                        // else it is 0
    );

/*****************************************************************************/
// 	Description :   Get number of day from 2000/01/01 to the inputed date.
//					If date is between 2000/01/01/00:00:00 and 
//					2049/12/31/23:59:59, convert result(saved in day_num_ptr)
//					is correct; else it is 0.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
//                  2000/01/01  day_num = 0
//                  2000/01/02  day_num = 1
//                  ......
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetDayNumOfDate( // If successful, return ERR_TM_NONE
                                    // else return ERR_TM_WRONG_DATE or ERR_TM_PNULL.
    SCI_DATE_T  date_val,           // Date value
    uint32      *day_num_ptr        // Save number of day if the date is correct;
                                    // else it is 0
    );

/*****************************************************************************/
// 	Description :   Get date from number of day(since 2000/01/01).
//                  If the date is out of range(2049/12/31), return start date
//                  2000/01/01.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
//                  2000/01/01  day_num = 0
//                  2000/01/02  day_num = 1
//                  ......
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetDateOfDayNum( // If successful, return ERR_TM_NONE
                                    // else return ERR_TM_WRONG_DATE or ERR_TM_PNULL.
    SCI_DATE_T  *date_ptr,          // Used to save date, if the date is out of
                                    // range(2050/01/01), set it to s_sys_begin_date
    uint32      day_num             // Number of day from (2000/01/01)
    );
    
/*****************************************************************************/
// 	Description :   Judge if the year is leap year.
//	Global resource dependence : 
//  Author:         Eric.zhou
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsLeapYear(   // Return SCI_TRUE, if it is leap year;
                                // else, return SCI_FALSE.
    uint16  year                // The year to be judge
    );

/*****************************************************************************/
// 	Description :   Check if the date is valid.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsDateValid(  // Return SCI_TRUE, if it is valid;
                                // else, return SCI_FALSE.
    SCI_DATE_T  date_val        // Date value
	);

/*****************************************************************************/
// 	Description :   Check if the time is valid.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsTimeValid(  // Return SCI_TRUE, if it is valid;
                                // else, return SCI_FALSE.
    SCI_TIME_T    time_val      // Time value
	);

/*****************************************************************************/
// 	Description :   Check if the timex is valid.
//	Global resource dependence : 
//  Author:         cheng.Fang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsTimeXValid(  // Return SCI_TRUE, if it is valid;
                                // else, return SCI_FALSE.
    SCI_TIMEX_T    time_val      // Time value
	);

/*****************************************************************************/
// 	Description :   Check if the time is valid.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsDayNumValid(    // Return SCI_TRUE, if it is valid;
                                    // else, return SCI_FALSE.
    uint32  day_num                 // Day number from 2000/01/01
	);

/*****************************************************************************/
// 	Description :   Check if alarm is expired.
//	Global resource dependence : 
//  Author:         Xueliang.Wang
//	Note:           This function is called after init RTC and before TM_Init.
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsAlarmExpired(void);

/*****************************************************************************/
// 	Description :   set rtc int type enable/disable
//	Global resource dependence : 
//  Input:          type:   the rtc interrupt type
//                          TM_INT_SEC 
//							TM_INT_MIN 
//							TM_INT_HOUR
//							TM_INT_DAY 
//							TM_INT_ALARM
//                  value  : interrupt enable/disable                      
//  Output          ERR_TM_NONE: success
//  Author:         Weihua.Wang
//	Note:           
/*****************************************************************************/
PUBLIC ERR_TM_E TM_SetRtcIntType(TM_INTTYPE_E type, BOOLEAN value);	
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_SYS_TIME_EXT_H

