/***************************************************************************************
** FileName    :  atest_plmn.c                                                          **
** Author      :  lin.chen                                                           **
** Date        :  18/04/2012                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  This file is used to get plmn data   						  **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  18/04/2012     lin.chen         Created                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#include "mn_type.h"
#include "mmk_type.h"
#include "sci_api.h"
#include "atest_signal.h"
#include "atest_plmn.h"
#include "mmiphone_export.h"
#include "mmi_modu_main.h"
#include "..\\..\\mmi_app\\app\\setting\\h\\mmiset_export.h"

#ifdef WIN32
/**-----------------------------------------------------------------------------------*
 **                               Constants and  Variables                            *
 **-----------------------------------------------------------------------------------*/
BOOLEAN b_Flag = FALSE;

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
LOCAL void ATEST_SendPlmnGettedToCPMsg();

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
LOCAL void ATEST_SendPlmnGettedToCPMsg()
{
    ATEST_APP_SIG_T *signal_ptr = PNULL; 

    // create signal
    SCI_CREATE_SIGNAL(signal_ptr,
        CPANEL_ATEST_REQ,
        sizeof(ATEST_APP_SIG_T),
        SCI_IDENTIFY_THREAD() );
    
    // set the sig_ptr's param
    signal_ptr->signal_id = ATEST_MC_PLMN_INFO;
	signal_ptr->check_type = b_Flag;

   // send signal to control panel.
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_CPANEL);
}


/**************************************************************************************/
// Description      : getting the plmn data
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
PUBLIC void Atest_GetPLMNDataFromMocor(char *PlmnTestingFilePath)
{
	uint16 ListNum,i,j;
	MMI_STRING_T plmn_name = {0};
	MN_PLMN_T network_plmn = {0};
	char	ResultFilePath[ATEST_MAX_PATH]={0};
	FILE	*PLMN_ptr = PNULL;
	wchar	wstr_ptr[ATEST_MAX_PATH] = {0};
	MMI_TEXT_ID_T network_id = {0};

	b_Flag = FALSE;

	strncpy(ResultFilePath,PlmnTestingFilePath,strlen(PlmnTestingFilePath));
	strncat(ResultFilePath,"\\Plmn_Mocor.txt",strlen("\\Plmn_Mocor.txt"));
	
	MMIAPISET_SetLanguageType(MMISET_LANGUAGE_ENGLISH);

	PLMN_ptr =  fopen(ResultFilePath,"w+");
	if(NULL != PLMN_ptr)
	{
		ListNum = MMIAPIPHONE_GetPlmnListNum();	
		for(i=0;i<=ListNum;i++)
		{
			MMIAPIPHONE_GetPlmnInfoByIndex(&network_plmn,i);
			network_id = MMIAPIPHONE_GetNetWorkNameId(0, &network_plmn);
		//	MMI_GetLabelTextEnglish(network_id, &plmn_name);
			SCI_MEMSET(&plmn_name, 0, sizeof(plmn_name));
			MMI_GetLabelTextByLang(network_id, &plmn_name);
			
			SCI_MEMSET(wstr_ptr,0,ATEST_MAX_PATH);			
			for(j=0;j<plmn_name.wstr_len;j++)
			{
				if(*(plmn_name.wstr_ptr+j) == 0x0161)
				{
					wstr_ptr[j] = 0x0073;
				}
				else
				{
					wstr_ptr[j] = *(plmn_name.wstr_ptr+j);
				}
				
				if (wstr_ptr[j] > 0x007a)
				{
					break;
				}
			}
			
			if (wstr_ptr[0] > 0x007a)
			{
				continue;
			}
			
			fprintf(PLMN_ptr,"PLMN_%d_%d=",network_plmn.mcc,network_plmn.mnc);
			fwprintf(PLMN_ptr,L"%s\n",wstr_ptr);
		}
		
		fclose(PLMN_ptr);
		b_Flag = TRUE;
	}

	ATEST_SendPlmnGettedToCPMsg();
}
#endif






