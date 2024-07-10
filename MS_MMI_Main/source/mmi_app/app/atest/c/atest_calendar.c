/***************************************************************************************
** FileName    :  atest_calendar.c                                                          **
** Author      :  lin.chen                                                           **
** Date        :  18/04/2012                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The implementaion of Calendar Lunar testing.						  **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  18/04/2012     lin.chen         Created                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/

#include "atest_calendar.h"
#include "sci_api.h"
#include "atest_signal.h"

#ifdef WIN32
/**-----------------------------------------------------------------------------------*
 **                               Constants and  Variables                            *
 **-----------------------------------------------------------------------------------*/
uint8 solarMonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
BOOLEAN b_TestFlag = FALSE;

/**-----------------------------------------------------------------------------------*
 **                                Function Prototypes                                *
 **-----------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description      : send the message which notify the control panel
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
LOCAL void ATEST_SendCalendarTestedToCPMsg();

/**************************************************************************************/
// Description      : get the days of solar 
//--------------------------------------------------------------------------------------
// Input Parameter  : uint8 year,uint8 month
// Output Parameter : NONE
// Return Value     : uint8
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
LOCAL uint8 Atest_GetsolarDays(uint8 year,uint8 month);


/**-----------------------------------------------------------------------------------*
 **                                Function Implementation                            *
 **-----------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description      : send the message which notify the control panel
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
LOCAL void ATEST_SendCalendarTestedToCPMsg()
{
    ATEST_APP_SIG_T *signal_ptr = PNULL; 

    // create signal
    SCI_CREATE_SIGNAL(signal_ptr,
        CPANEL_ATEST_REQ,
        sizeof(ATEST_APP_SIG_T),
        SCI_IDENTIFY_THREAD() );
    
    // set the sig_ptr's param
    signal_ptr->signal_id = ATEST_MC_CALENDAR_INFO;
	signal_ptr->check_type = b_TestFlag;

   // send signal to control panel.
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_CPANEL);
}

/**************************************************************************************/
// Description      : get the days of solar 
//--------------------------------------------------------------------------------------
// Input Parameter  : uint8 year,uint8 month
// Output Parameter : NONE
// Return Value     : uint8
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
LOCAL uint8 Atest_GetsolarDays(uint8 year,uint8 month)
{
	if(ATEST_FEB_MONTH == month)
	{	
		return(((year%4 == 0) && (year%100 != 0) || (year%400 == 0))? ATEST_FEB_MAX_DAYS: ATEST_FEB_SEC_DAYS);
	}
	else
	{	
		return(solarMonth[month]);
	}	
}

/**************************************************************************************/
// Description      : Calendar Lundar Testing
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
PUBLIC void Atest_GetLunarDataFromMocor(char *LangTestingFilePath)
{
	FILE *	Calendar_ptr;
	uint16	solar_year = 0;
    uint8	solar_month = 0;
    uint8	solar_day = 0;
    uint16	m_lunar_year_ptr = 0;
    uint8	m_lunar_month_ptr = 0;
    uint8	m_lunar_day_ptr = 0;
	uint8	days = 0;
	BOOL	result = FALSE;	
	char	ResultFilePath[ATEST_MAX_PATH]={0};

	strncpy(ResultFilePath,LangTestingFilePath,strlen(LangTestingFilePath));
	strncat(ResultFilePath,"\\Lunar_Mocor.ini",strlen("\\Lunar_Mocor.ini"));

	Calendar_ptr =  fopen(ResultFilePath,"w+");
	if(NULL == Calendar_ptr)
	{
		b_TestFlag = FALSE;
		ATEST_SendCalendarTestedToCPMsg();
		return;
	}

	for(solar_year = ATEST_YEAR_START ;solar_year <= ATEST_YEAR_END;solar_year++)
	{
		fprintf(Calendar_ptr,"[%d]\n",solar_year);	
		for(solar_month = ATEST_MONTH_START;solar_month<=ATEST_MONTH_END;solar_month++)
		{
			days = Atest_GetsolarDays(solar_year,solar_month-1);
			for(solar_day = ATEST_DAY_START; solar_day <= days; solar_day++)
			{
				MMICALENDAR_SolarToLunar(solar_year,solar_month,solar_day,&m_lunar_year_ptr,&m_lunar_month_ptr,&m_lunar_day_ptr);
				fprintf(Calendar_ptr,"%d-%d-%d = %d-%d-%d\n",solar_year,solar_month,solar_day,m_lunar_year_ptr,m_lunar_month_ptr,m_lunar_day_ptr);
			}
		}		
	}
	
	fclose(Calendar_ptr);

	b_TestFlag = TRUE;
	ATEST_SendCalendarTestedToCPMsg();
}
#endif






