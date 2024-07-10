/***************************************************************************************
** FileName    :  atest_location.c                                                    **
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
**  29/09/2010     Mingxuan.Zhu         Updated ( update the structure of the code )  **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/

/**-----------------------------------------------------------------------------------*
 **                                   Include Files                                   *
 **-----------------------------------------------------------------------------------*/
#include "mmi_app_atest_trc.h"
#include <stdlib.h>
#include "sci_api.h"
#include "mmi_appmsg.h"
#include "mmimp_location.h"
#include "mmiphonesearch_export.h"
#include "atest_location.h"
#include "atest_signal.h"

#ifdef WIN32
/**-----------------------------------------------------------------------------------*
 **                                Function Prototypes                                *
 **-----------------------------------------------------------------------------------*/

/**************************************************************************************/
// Description      : check whether the location information in fixed-line phone   
//                    location database file is accord with the location information  
//                    in the mobile phone simulator or not.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. char *locationFilePath
//                    2. char *resultFilePath
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_TestFixedLineLocation( 
                                        char *locationFilePath, // IN Param : the path of the location database file
                                        char *resultFilePath    // IN Param : the path of the testing result file
                                      );

/**************************************************************************************/
// Description      : check all the location file in the location database file.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. char *locationFilePath
//                    2. char *resultFilePath
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_TestAllMobilePhoneLocation( 
                                             char *locationFilePath,  // IN Param : the path of the location database file
                                             char *resultFilePath     // IN Param : the path of the testing result file
                                           );

/**************************************************************************************/
// Description      : check whether the location information in each mobile phone   
//                    location database file is accord with the location information  
//                    in the mobile phone simulator or not.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. sourcePath : the path of the location database file
//                    2. resultPath : the path of the testing result file
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL int ATEST_TestMobilePhoneLocation(
                                         char* sourcePath, // IN Param : the path of the location database file
                                         char* resultPath  // IN Param : the path of the testing result file 
                                       );

/**************************************************************************************/
// Description      : send the end of location check to control panel.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_SendLocationOverToCPMsg(void);

/**-----------------------------------------------------------------------------------*
 **                                Function Implementation                            *
 **-----------------------------------------------------------------------------------*/

/**************************************************************************************/
// Description      : check the location file in the location database file by check type.
//--------------------------------------------------------------------------------------
// Input Parameter  : ATEST_DATA_T *loc_signal_ptr                    
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTLOC_TestPhoneLocation(ATEST_APP_DATA_T *loc_signal_ptr)
{    
    switch (loc_signal_ptr->check_type)
    {
    case MOBILE_PHONE: // mobile phone's location checking
        ATEST_TestAllMobilePhoneLocation(loc_signal_ptr->source_file_path,loc_signal_ptr->result_file_path);
        ATEST_SendLocationOverToCPMsg();
        break;
    case FIXED_LINE:   // fixed-line phone's location checking
        ATEST_TestFixedLineLocation(loc_signal_ptr->source_file_path,loc_signal_ptr->result_file_path);
        ATEST_SendLocationOverToCPMsg();
        break;
    default: // fixed-line and mobile phone
        {
            ATEST_TestAllMobilePhoneLocation(loc_signal_ptr->source_file_path,loc_signal_ptr->result_file_path);
            ATEST_TestFixedLineLocation(loc_signal_ptr->source_file_path,loc_signal_ptr->result_file_path);
            ATEST_SendLocationOverToCPMsg();
        }
        break;
    }
}

/**************************************************************************************/
// Description      : check whether the location information in fixed-line phone   
//                    location database file is accord with the location information  
//                    in the mobile phone simulator or not.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. char *locationFilePath
//                    2. char *resultFilePath
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_TestFixedLineLocation( 
                                        char *locationFilePath, // IN Param : the path of the location database file
                                        char *resultFilePath    // IN Param : the path of the testing result file
                                      )
{
    int wrongResultNumber = 0;
    char mismatchingNumber[DIGIT_NUMBER]="";
    char statFilePath[FILE_PATH_LENGTH]="";
    
    SCI_STRCPY(statFilePath,resultFilePath);
    strcat(statFilePath,"statResult.csv");
    strcat(locationFilePath,"areaCode.txt");
	strcat(resultFilePath,"areaCode.csv");

    // open the fixed-line phone location database file
	fpSourceFile = fopen(locationFilePath,"r");
	
    if(fpSourceFile!=NULL)	
    {
		// open check result file of the fixed-line phone location database
		fpResultFile = fopen(resultFilePath,"w");
		
		// check whether the file pointer has rearched the end of the location database file or not
		while(!feof(fpSourceFile))
		{
			// variable definition
			char phoneLocation[PHONE_LOCATION_NAME_LENGTH] = "";
			uint8 fixedLineAreaCode[PHONE_NUMBER_LENGTH] = "";
			int32 fixedLineAreaCodeLength = 0;
			uint16 phoneLocationLength[PHONE_LOCATION_NAME_LENGTH] = {0};
			
			// read into each record in the location database file 
			fscanf(fpSourceFile,"%s",record);
			
			// filter the title bar in the location database file
			if(strcmp(record,"省份,城市,区号,模拟器返回结果,一致性测试结果")!=0)
			{
				// variable definition
				int iCol=0; // column position for each field in the record
				char *fieldName=NULL;
				char token[PHONE_LOCATION_NAME_LENGTH]="";
				char provinceName[PHONE_LOCATION_NAME_LENGTH]="";
				char cityName[PHONE_LOCATION_NAME_LENGTH]="";
				char totalName[PHONE_LOCATION_TOTAL_NAME_LENGTH]="";
                char areaCode[AREA_CODE_LENGTH]="";
				
				SCI_STRCPY(token,record);
				// get the first field value for each record in the location database file
				fieldName = strtok(token,",");

                if(fieldName != NULL)
                {   //get the province name from the location database
                    if(strcmp(fieldName,"-")!=0)
                    {   // is not 直辖市
                        SCI_STRCPY(provinceName,fieldName);
                    }
                    else
                    {   // is 直辖市
                        SCI_STRCPY(provinceName,"\0");
                    }
                }
		
				// get province name and city name from the location database file
				/************************************************* 
				** The DB File Record Structure :
				** Example : areaCode.txt
				**           0,   1,   2,             3,             4
				**        省份,城市,区号,模拟器返回结果,一致性测试结果
				**           -,北京,10
				**        江苏,南京,25  
				*************************************************/
				while(fieldName!=NULL)
				{
					iCol++;
					// get next field in each record
					fieldName = strtok(NULL,",");

					if(fieldName!=NULL)
					{   //get the city name from the location database
						if(iCol==1) 
						{
							SCI_STRCPY(cityName,fieldName);
							strcat(cityName,"\0");
						}
                        if(iCol==2)
                        {
                            SCI_STRCPY(areaCode,fieldName);
                            fixedLineAreaCodeLength = strlen(areaCode);
                            strcat(areaCode,"\0"); 
                            SCI_STRCPY((char*)fixedLineAreaCode,areaCode);
                        }
					}
				}
				
				// get the location total name by combining the province name and the city name
				strcat(totalName,provinceName);
				strcat(totalName,cityName);
				strcat(totalName,"\0");

                // call the function in the mmimp_location.h, then get the fixed-line phone location information
				MMIAPI_FindPhoneLocation(fixedLineAreaCode,fixedLineAreaCodeLength,phoneLocationLength,phoneLocation);

                // combine record and phone location from the mobile phone simulator
                strcat(record,",");
				strcat(record,phoneLocation);

				// check whether the phone location database is in accord with the mobile phone's location or not
				if(strcmp(totalName,phoneLocation)!=0&&strcmp(cityName,phoneLocation)!=0)
				{   // inconformity
					strcat(record,",NG");
					strcat(record,"\n");
					fprintf(fpResultFile,"%s",record); // the checked result is writed into the result file
                    wrongResultNumber++;
				}
				else
				{   // conformity
					strcat(record,",OK");
					strcat(record,"\n");
					fprintf(fpResultFile,"%s",record); // the checked result is writed into the result file
				}
			}
			else
			{   // the title bar's processing
				strcat(record,"\n");
				fprintf(fpResultFile,"%s",record);
			}
		}
		//close the opened the files
		fclose(fpResultFile);
		fclose(fpSourceFile);

        // count the mismatched record about fixed-line location
        fpStatResultFile = fopen(statFilePath,"a+");
        fprintf(fpStatResultFile,"%s","区号文件名,号码归属地匹配错误数\n");
        itoa(wrongResultNumber,mismatchingNumber,10);
        strcat(statData,"areaCode.csv,");
        strcat(statData,mismatchingNumber);
        strcat(statData,"\n");
        fprintf(fpStatResultFile,"%s",statData);
		SCI_STRCPY(statData,"");
        fclose(fpStatResultFile);
	}
	else
	{
        //there are not sources
	}
}

/**************************************************************************************/
// Description      : check all the location file in the location database file.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. char *locationFilePath
//                    2. char *resultFilePath
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_TestAllMobilePhoneLocation( 
                                            char *locationFilePath,  // IN Param : the path of the location database file
                                            char *resultFilePath     // IN Param : the path of the testing result file
                                           )
{
	// each location file name in the mobile phone location database
	char locationDB[FILE_QUANTITY][FILE_NAME_LENGTH]={"1300000-1349999.txt","1350000-1399999.txt",
		"1500000-1549999.txt","1550000-1599999.txt",
		"1800000-1849999.txt","1850000-1899999.txt"};
	
	// each checked result file name after the program checked the location information 
	// in the mobile phone simulator
	char checkedResult[FILE_QUANTITY][FILE_NAME_LENGTH]={"1300000-1349999.csv","1350000-1399999.csv",
		"1500000-1549999.csv","1550000-1599999.csv",
		"1800000-1849999.csv","1850000-1899999.csv"};
	
	// mismatching number in each phone number range
	int mismatchingNumber[FILE_QUANTITY];
	char wrongResultNumber[FILE_QUANTITY][DIGIT_NUMBER];
	
	// the paths of the location file and the checked result file 
	char inputFilePath[FILE_PATH_LENGTH]="";
	char outputFilePath[FILE_PATH_LENGTH]="";
	char statFilePath[FILE_PATH_LENGTH]="";

    // loop variable
	int i;

    SCI_STRCPY(inputFilePath,locationFilePath);
    SCI_STRCPY(outputFilePath,resultFilePath);
    SCI_STRCPY(statFilePath,resultFilePath);
    strcat(statFilePath,"statResult.csv");
	    
	fpStatResultFile = fopen(statFilePath,"a+");
	fprintf(fpStatResultFile,"%s","号段文件名,号码归属地匹配错误数\n");

	// check whether the location information in each the location file is in accord with 
	// the location information in the mobile phone simulator or not 
	for(i=0;i<6;i++)
	{
		// set the paths of the location file and the checked result file
		strcat(inputFilePath,locationDB[i]);
		strcat(outputFilePath,checkedResult[i]);
		strcat(statData,checkedResult[i]);
		strcat(statData,",");
        // check each the location file
        mismatchingNumber[i] = ATEST_TestMobilePhoneLocation(inputFilePath,outputFilePath);

		itoa(mismatchingNumber[i],wrongResultNumber[i],10);
		strcat(statData,wrongResultNumber[i]);
		strcat(statData,"\n");
        fprintf(fpStatResultFile,"%s",statData);
		// reset the paths of the location file and the checked result file
		SCI_STRCPY(outputFilePath,resultFilePath);
		SCI_STRCPY(inputFilePath,locationFilePath);
		SCI_STRCPY(statData,"");
	}
	fclose(fpStatResultFile);
}

/**************************************************************************************/
// Description      : check whether the location information in each mobile phone   
//                    location database file is accord with the location information  
//                    in the mobile phone simulator or not.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. sourcePath : the path of the location database file
//                    2. resultPath : the path of the testing result file
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL int ATEST_TestMobilePhoneLocation(
                                        char* sourcePath, // IN Param : the path of the location database file
                                        char* resultPath  // IN Param : the path of the testing result file 
                                       )
{
	//the number of the wrong records
	int wrongResultNumber = 0;
	
	// open the mobile phone location database file
	fpSourceFile = fopen(sourcePath,"r");
	
    if(fpSourceFile!=NULL)	
    {
		// open check result file of the mobile phone location database
		fpResultFile = fopen(resultPath,"w");
		
		// check whether the file pointer has rearched the end of the location database file or not
		while(!feof(fpSourceFile))
		{
			// variable definition
			char phoneLocation[PHONE_LOCATION_NAME_LENGTH] = "";
			uint8 phoneNumber[PHONE_NUMBER_LENGTH] = "";
			int32 phoneNumberLength = 11;
			uint16 phoneLocationLength[PHONE_LOCATION_NAME_LENGTH] = {0};
			
			// read into each record in the location database file 
			fscanf(fpSourceFile,"%s",record);
			
			// filter the title bar in the location database file
			if(strcmp(record,"号段,省份,城市,模拟器返回结果,一致性测试结果")!=0)
			{
				// variable definition
				int iCol=0; // column position for each field in the record
				char *fieldName=NULL;
				char mpNumber[PHONE_NUMBER_LENGTH]="";
				char token[PHONE_LOCATION_NAME_LENGTH]="";
				char provinceName[PHONE_LOCATION_NAME_LENGTH]="";
				char cityName[PHONE_LOCATION_NAME_LENGTH]="";
				char totalName[PHONE_LOCATION_TOTAL_NAME_LENGTH]="";
				
				SCI_STRCPY(token,record);
				// get the first field value for each record in the location database file
				fieldName = strtok(token,",");
				
				// set the mobile phone number
				/*************************************** 
				** Example :
				** fieldName = "1391380"
				**          |
				** mpNumber = "13913801234"  
				***************************************/
				SCI_STRCPY(mpNumber,fieldName);
				strcat(mpNumber,"1234\0");
				// type conversion
				SCI_STRCPY((char*)phoneNumber,mpNumber);
				
				// call the function in the mmimp_location.h, then get the mobile phone location information
		//		MMIAPI_FindMPLocation(phoneNumber, phoneNumberLength, phoneLocationLength, phoneLocation);
				
				// get province name and city name from the location database file
				/************************************************* 
				** The DB File Record Structure :
				** Example : 1300000-1349999.txt
				**           0,   1,   2,             3,             4
				**        号段,省份,城市,模拟器返回结果,一致性测试结果
				**     1300001,江苏,常州
				**     1300006,江苏,南京  
				*************************************************/
				while(fieldName!=NULL)
				{
					iCol++;
					// get next field in each record
					fieldName = strtok(NULL,",");

					if(fieldName!=NULL)
					{   //get the province name from the location database
						if(iCol==1) 
						{
							if(strcmp(fieldName,"-")!=0)
							{   // is not 直辖市
								SCI_STRCPY(provinceName,fieldName);
							}
							else
							{   // is 直辖市
								SCI_STRCPY(provinceName,"\0");
							}
						}
						//get the city name from the location database
						if(iCol==2) 
						{
							SCI_STRCPY(cityName,fieldName);
							strcat(cityName,"\0");
						}
					}
				}
				
				// get the location total name by combining the province name and the city name
				strcat(totalName,provinceName);
				strcat(totalName,cityName);
				strcat(totalName,"\0");
				
				// combine record and phone location from the mobile phone simulator
				strcat(record,",");
				strcat(record,phoneLocation);
				
				// check whether the phone location database is in accord with the mobile phone's location or not
				if(strcmp(totalName,phoneLocation)!=0&&strcmp(cityName,phoneLocation)!=0)
				{   // inconformity
					strcat(record," ,NG");
					strcat(record,"\n");
					fprintf(fpResultFile,"%s",record); // the checked result is writed into the result file
					wrongResultNumber++;
				}
				else
				{   // conformity
					strcat(record,",OK");
					strcat(record,"\n");
					fprintf(fpResultFile,"%s",record); // the checked result is writed into the result file
				}
			}
			else
			{   // the title bar's processing
				strcat(record,"\n");
				fprintf(fpResultFile,"%s",record);
			}
		}
		//close the opened the files
		fclose(fpResultFile);
		fclose(fpSourceFile);
	}
	else
	{
        //there are not sources
	}
	return wrongResultNumber;
}

/**************************************************************************************/
// Description      : send the end of location check to control panel.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_SendLocationOverToCPMsg(void)
{
    ATEST_APP_SIG_T *signal_ptr = PNULL; 
    
    // create signal
    SCI_CREATE_SIGNAL( signal_ptr,
                       CPANEL_ATEST_REQ,
                       sizeof(ATEST_APP_SIG_T),
                       SCI_IDENTIFY_THREAD() );

    // set the sig_ptr's param
    signal_ptr->dual_sys = MN_DUAL_SYS_1;
    signal_ptr->signal_id = ATEST_END_LOCATION_REQ;
    
    // send signal to control panel.
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_CPANEL);
    //SCI_TRACE_LOW:"ATEST_TRACE====NORMAL EXIT LOCATION TESTING!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATEST_LOCATION_530_112_2_18_2_2_59_0,(uint8*)"");
}

#endif