/***************************************************************************************
** FileName    :  atest_pb.c                                                          **
** Author      :  shuyan.hu                                                           **
** Date        :  22/04/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The implementaion of the step and action in the pb coding testing.        **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  22/04/2010     shuyan.hu         Created                                       **
**   6/12/2010     shuyan.hu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#ifdef WIN32

#include "std_header.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "atest_pb.h"
#include "mmipb_export.h"
#include "time.h"
#include <stdlib.h>
#include <wchar.h>




/**-----------------------------------------------------------------------------------*
 **                               Constants and  Variables                            *
 **-----------------------------------------------------------------------------------*/

unsigned long namenumber_arr[ ] ={ 211, 212, 311, 312, 313, 3212, 3213, 3223, 411, 412, 413, 414, 4212, 4213, 4214, 4223, 4224, 4234, 
								43123, 43124, 43134, 43234, 511, 512, 513, 514, 515, 5212, 5213, 5214, 5215, 5223, 5224, 
								5225, 5234, 5235, 5245, 53123, 53124, 53125, 53134, 53135, 53145, 53234, 53235, 53245, 53345,
								54123, 54124, 54125, 54134, 54135, 54234, 54235, 54245, 54345, 611, 612, 613, 614, 615, 616, 6212, 6213, 6214, 6215, 6216,
								6223, 6224, 6225, 6226, 6234, 6235, 6236, 6245, 6246, 6256, 63123, 63124, 63125, 63126,
								63134, 63135, 63136, 63145, 63146, 63156, 63234, 63235, 63236, 63245,
								63246, 63256, 63345, 63346, 63356, 63456, 64123, 64124, 64125, 64126, 64134, 64135, 64136, 64145, 64146, 64156, 64234,
								64235, 64245, 64246, 64256, 64345, 64346, 64356, 64456, 6512, 6513, 6514, 6515, 6516, 6523, 6524, 6525, 6526, 6534, 6535,
								6536, 6545, 6546, 6556, 711, 712, 713, 714, 715, 716, 717, 7212, 7213, 7214, 7215, 7216, 7217, 7223, 7224, 7225, 7226,
								7227, 7234, 7235, 7236, 7237, 7245, 7246, 7247, 7256, 7257, 7267, 73123, 73124, 73125, 73126, 73127,
								73134, 73135, 73136, 73137, 73145, 73146, 73147, 73156, 73157, 73167, 73234, 73235, 73236, 73237,
								73245, 73246, 73247, 73256, 73257, 73267, 73345, 73346, 73347, 73356, 73357, 73367, 73456, 73457, 73467,
								73567, 74123, 74124, 74125, 74126, 74127, 74134, 74135, 74136, 74137, 74145, 74146, 74147, 74156,
								74157, 74167, 74234, 74235, 74236, 74237, 74245, 74246, 74247, 74256, 74257, 74267, 74345, 74346, 74347,
								74356, 74357, 74367, 74456, 74457, 74467, 74567, 7512, 7513, 7514, 7515, 7516, 7517, 7523, 7524, 7525,
								7526, 7527, 7534, 7535, 7536, 7537, 7545, 7546, 7547, 7556, 7557, 7567, 761, 762, 763, 764, 765, 766, 767,
								0,0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 


static uint16 s_pb_max_case;
static char s_pb_sourcepath[MAX_PB_PATH];
static char s_pb_confpath[MAX_PB_PATH];
static char s_pb_respath[MAX_PB_PATH];




/**----------------------------------------------------------------*
 **                                Function Prototypes             *
 **----------------------------------------------------------------*/

/***************************************************************************
Function: create pb code cases
//-------------------------------------------------------------------------
// Input Parameter  : int character_num
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------
//  Global resource dependence : s_pb_sourcepath
//  Author: shuyan.hu
//  Note: 
****************************************************************************/
LOCAL void atest_create_case(int character_num);

/*********************************************************************
Function: 比较输入的名字和经过编码解码后的名字是否相同
//--------------------------------------------------------------------
// Input Parameter  : char *pb_test, char *pb_res, char *pb_fail
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
************************************************************************/
LOCAL void mmitestpb(char *pb_test, char *pb_res, char *pb_fail);


/**--------------------------------------------------------------*
 **                                Function Implementation       * 
 **-------------------------------------------------------------*/



/**********************************************************************************/
// Description      : get the pb test case number and all path.
//--------------------------------------------------------------------------------
// Input Parameter  : ATEST_DATA_T *pb_signal_ptr                    
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------
// Global Resource Dependence : s_pb_max_case, s_pb_sourcepath, s_pb_confpath, s_pb_respath
// Author: shuyan.hu
// Note:
/***********************************************************************************/
PUBLIC void Atest_PbGetPathandNum(ATEST_APP_DATA_T *pb_signal_ptr)
{    
     s_pb_max_case = pb_signal_ptr->status_code;
     memset(s_pb_sourcepath, 0, MAX_PB_PATH);
	 strncpy(s_pb_sourcepath, pb_signal_ptr->source_file_path, strlen(pb_signal_ptr->source_file_path));
	 memset(s_pb_respath, 0, MAX_PB_PATH);
	 strncpy(s_pb_respath, pb_signal_ptr->result_file_path, strlen(pb_signal_ptr->result_file_path));
	 memset(s_pb_confpath, 0, MAX_PB_PATH);
     strncpy(s_pb_confpath, pb_signal_ptr->config_file_path, strlen(pb_signal_ptr->config_file_path));

}

/**********************************************************************************/
// Description      : get the pb test config path.
//--------------------------------------------------------------------------------
// Input Parameter  : ATEST_DATA_T *pb_signal_ptr                    
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/***********************************************************************************/
PUBLIC void Atest_PbGetConfigPath(ATEST_APP_DATA_T *pb_signal_ptr)
{    
	memset(s_pb_confpath, 0, MAX_PB_PATH);
	strncpy(s_pb_confpath, pb_signal_ptr->config_file_path, strlen(pb_signal_ptr->config_file_path));
	
}

/*******************************************************
Function: 比较输入的pb名字和经过编码解码后的pb名字是否相同
//--------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//---------------------------------------------------------
//  Global resource dependence : s_pb_sourcepath, s_pb_respath
//  Author: shuyan.hu
//  Note: 
********************************************************/

PUBLIC void atest_mmipbcode()
{
	char pb_file_test[MAX_FILE_LEN];
	char pb_file_res[MAX_FILE_LEN];
	char pb_file_fail[MAX_FILE_LEN];
	uint16 id, arr_num;
    
	//loop for get pb code cases files
	for(arr_num=0; namenumber_arr[arr_num]!=0; arr_num++);
     
	//loop for autotest pb code cases
    for(id=0; id<arr_num; id++)
	{
		memset(pb_file_test, 0, MAX_FILE_LEN);
		memset(pb_file_res, 0, MAX_FILE_LEN);
		memset(pb_file_fail, 0, MAX_FILE_LEN);

		//set test file and result file path 
		sprintf(pb_file_test, "%s%s%lu%s", s_pb_sourcepath, "\\pb", namenumber_arr[id], ".txt");
		sprintf(pb_file_res, "%s%s%lu%s%s", s_pb_respath, "\\pb", namenumber_arr[id], "_res", ".txt");
		sprintf(pb_file_fail, "%s%s%lu%s", s_pb_respath, "\\fail_pb", namenumber_arr[id], ".txt");
		//test the cases
		mmitestpb(pb_file_test, pb_file_res, pb_file_fail);
	}

}



/**************************************************************************************/
// Description      : autotest pb code according to config file
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : s_pb_confpath
// Author: shuyan.hu
// Note:
/**************************************************************************************/
PUBLIC void atest_mmipb_fl()
{
	char pb_conf[MAX_FILE_LEN];
	char pb_testfile[MAX_FILE_LEN];
	char test_res[MAX_FILE_LEN];
	char test_fail[MAX_FILE_LEN];
	char stem[MAX_FILE_LEN];
	uint16 ilen;

	memset(pb_conf, 0, MAX_FILE_LEN);
	memset(pb_testfile, 0, MAX_FILE_LEN);
	memset(test_res, 0, MAX_FILE_LEN);
	memset(test_fail, 0, MAX_FILE_LEN);


	//set config file and open config file
    sprintf(pb_conf, "%s%s",  s_pb_confpath, "\\pb_selftest.txt");
	
	ilen = strlen("\\pb_con\\");
	memset(stem, 0, MAX_FILE_LEN);
	//get the result path
	strncpy(stem, s_pb_confpath, strlen(s_pb_confpath)-ilen);
	strcat(stem, "\\pb_res\\");

	//set result file path
	sprintf(test_res , "%s%s", stem, "pb_selftest_res.txt");
	sprintf(test_fail, "%s%s", stem, "fail_pb_selftest.txt");
	//test pb code case from config file
    mmitestpb(pb_conf, test_res, test_fail);

}




/*********************************************************************
Function: 比较输入的名字和经过编码解码后的名字是否相同
//--------------------------------------------------------------------
// Input Parameter  : char *pb_test, char *pb_res, char *pb_fail
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
************************************************************************/

LOCAL void mmitestpb(char *pb_test, char *pb_res, char *pb_fail)                   //比较的实现函数
{
	FILE *pbtest_fptr=NULL, *pbres_fptr=NULL, *pbfail_fptr=NULL;
	wchar fget[MAX_FILE_LEN];
	wchar ire_ok[6] = {0x0020,0x0020,0x002c,0x004f,0x004b,0x0000};
	wchar ires_fail[8] = {0x0020,0x0020,0x002c,0x0046,0x0041,0x0049,0x004c,0x0000};
	uint16 ilen;
	uint8 outputencode[MAX_FILE_LEN];
	wchar outjh[MAX_FILE_LEN];
	uint8 chara_num=0;
    MMIPB_NAME_T pb_tmp_str;
	wchar *filegettmp_ptr;
	uint8 iflag, com_res;
    wchar fheader[2] = {0xfeff, 0x0000};
	wchar lineend[3] = {0x000d, 0x000a, 0x0000};
	wchar linespace[4]= {0x0020, 0x002c, 0x0020, 0x0000};
	
    //check input parameters
	if(NULL == pb_test || NULL == pb_res || NULL == pb_fail)               
    {
		return;
	}

	memset(fget, 0, MAX_FILE_LEN*2);
	memset(outputencode, 0, MAX_FILE_LEN);

	//open pb code case file
    pbtest_fptr = fopen( pb_test, "rb");                                     
	if(NULL == pbtest_fptr)
	{
		return;
	}

	//create pb code result file
	pbres_fptr = fopen(pb_res, "wb+");                                   
	if(NULL == pbres_fptr)
    {
	  return;
	}
    
	iflag =1;
	fputws(fheader, pbres_fptr);                                                          //插入UNICODE编码文件头
    while(NULL != (filegettmp_ptr = fgetws(fget, MAX_FILE_LEN, pbtest_fptr)) && !feof(pbtest_fptr))     //循环读取文件获取内容
    {
		
  	    memset(&pb_tmp_str, 0, sizeof(MMIPB_NAME_T));
           memset(outputencode, 0, MAX_FILE_LEN);
           memset(outjh, 0, MAX_FILE_LEN*2);

		ilen = MMIAPICOM_Wstrlen(filegettmp_ptr);
		
		MMIAPICOM_Wstrncpy(outjh, filegettmp_ptr, ilen-2);                       //对获取的内容进行处理后面的不需要的字符
		outjh[ilen-2] = 0x0000;
		MMIAPICOM_Wstrncpy(pb_tmp_str.wstr, filegettmp_ptr, ilen-2);
		pb_tmp_str.wstr[ilen-2]= 0x0000;
		pb_tmp_str.wstr_len = ilen-2;

		MMIPB_WstrToGSMAlphabet(pb_tmp_str.wstr, pb_tmp_str.wstr_len,
			outputencode, &chara_num,
			MMIPB_NAME_MAX_LEN);
//        Atest_ConvertMmiToSim(&chara_num, outputencode, &pb_tmp_str);                //调用接口进行编码
		memset(&pb_tmp_str, 0, sizeof(MMIPB_NAME_T));
		MMIPB_GSMAlphabetToWstr(outputencode, chara_num,
			pb_tmp_str.wstr, &pb_tmp_str.wstr_len,
			MMIPB_NAME_MAX_LEN);
//		Atest_ConvertSimToMmi(&pb_tmp_str, chara_num, outputencode);                 //调用接口把编码后的内容进行解码输出
		com_res = MMIAPICOM_Wstrcmp(outjh, pb_tmp_str.wstr);                     //比较编解码前后的内容
		if(com_res == 0)                                                    //内容一致的处理
        {
			MMIAPICOM_Wstrcat(outjh, linespace);
		
			MMIAPICOM_Wstrcat(outjh, pb_tmp_str.wstr);
		
			MMIAPICOM_Wstrcat(outjh, ire_ok);
		}
		else                                                         //内容不一致的处理
        {
		     MMIAPICOM_Wstrcat(outjh, linespace);
			 
			 MMIAPICOM_Wstrcat(outjh, pb_tmp_str.wstr);
			 
			 MMIAPICOM_Wstrcat(outjh, ires_fail);
			 if(iflag)
			 {
				 pbfail_fptr = fopen(pb_fail, "wb+");                            //创建失败文件，把比较失败的结果单独输入到该文件
				 if(NULL == pbfail_fptr)
				 {
					 return;
				 }
				 fputws(fheader, pbfail_fptr);                                   //输入unicode文件头
				 
			 }
			 iflag =0;
             fputws(outjh, pbfail_fptr);
			 fputws(lineend, pbfail_fptr);
		}
		memset(fget, 0, MAX_FILE_LEN*2);
		fputws(outjh, pbres_fptr);
		fputws(lineend, pbres_fptr);
	}
	fclose(pbtest_fptr);                                                     //关闭文件                                                        
	fclose(pbres_fptr);
	if(pbfail_fptr)
	{
		fclose(pbfail_fptr);
    }
}



/***************************************************************************
Function: create pb code cases
//-------------------------------------------------------------------------
// Input Parameter  : int character_num
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------
//  Global resource dependence : s_pb_sourcepath
//  Author: shuyan.hu
//  Note: 
****************************************************************************/

LOCAL void atest_create_case(int character_num)
{
	int idex;
	FILE *pb_fptr;
	wchar pb_case[MAX_ARR_CHARACTER_NUM];
	int case_num=0;
    
	time_t ti;
	int ts, tx,td,txx,txxx,tdd,tddd;
	char pb_casefilename[MAX_FILE_LEN];
	uint8 character_tmp,i,j,k,m;
	
	
	wchar file_header[2] = {0xfeff, 0x0000};
	wchar line_end[3] = {0x000d, 0x000a, 0x0000};
	
	
	memset(pb_casefilename, 0, MAX_FILE_LEN);
	for(character_tmp=1; character_tmp<character_num; character_tmp++)
	{
        
		//pb code case has one character
        if(character_tmp == 1)
        {
			srand((unsigned int)time(&ti));
			for(j=0; j<character_num; j++)
			{
				memset(pb_casefilename, 0, MAX_FILE_LEN);
				//get the pb code case file name
				sprintf(pb_casefilename, "%s%s%d%d%d%s", s_pb_sourcepath,"\\pb", character_num, character_tmp, j+1, ".txt");
				//create the pb code case file
				pb_fptr= fopen(pb_casefilename, "wb+");
				if(NULL == pb_fptr)
				{
				    return;
				}
		        //input unicode file header 
				fputws(file_header, pb_fptr);
		
				case_num=0;
                
				//main loop for create pb code cases
				while(case_num<s_pb_max_case)
				{   
					//get the random number 
					ts = rand();
					tx = ts%(MAX_CHARACTER_NUM);
					ts = rand();
					td = ts%(MAX_CHINESE_NUM);
					memset(pb_case, 0, MAX_ARR_CHARACTER_NUM*sizeof(wchar));
			       
                    //get the character
					pb_case[j]= 0x0020+tx;
					if(pb_case[j] == 0x001f)
					{
						pb_case[j] = pb_case[j]+1;
					}

					if(pb_case[j] >= 0x007f && pb_case[j] <= 0x00a0)
					{
						pb_case[j] = pb_case[j]-34;
					}
					for(i=0; i<character_num; i++)
                    {
						if(i != j)
                        {
							pb_case[i] = 0x4e00+td;
							ts = rand();
							td = ts%(MAX_CHINESE_NUM);
						}
					} 
					pb_case[character_num]= 0x0000;
			
			  	//input pb case into file 
					fputws(pb_case, pb_fptr);
					fputws(line_end, pb_fptr);
					case_num = case_num+1;
				}
				//close pb case file
				fclose(pb_fptr);
			}
		}
		//pb code case has two characters
		if(character_tmp == 2)
        {
			srand((unsigned int)time(&ti));
			for(i=0; i<character_num-1; i++)
			{
				for(j=i+1; j<character_num; j++)
                {
					if(i != j)
                    {
						memset(pb_casefilename, 0, MAX_FILE_LEN);
						//get the pb code case file name
						sprintf(pb_casefilename, "%s%s%d%d%d%d%s", s_pb_sourcepath,"\\pb", character_num, character_tmp, i+1, j+1, ".txt");
						//create the pb code case file
						pb_fptr= fopen(pb_casefilename, "wb+");
						if(NULL == pb_fptr)
						{
							return;
						}
						//input unicode file header 
						fputws(file_header, pb_fptr);
				
						case_num=0;
						//main loop for create pb code cases
						while(case_num<s_pb_max_case)
						{
							//get the random number
							ts = rand();
							tx = ts%(MAX_CHARACTER_NUM);
							ts = rand();
							td = ts%(MAX_CHINESE_NUM);
							ts = rand();
							txx = ts%(MAX_CHARACTER_NUM);
							memset(pb_case, 0, MAX_ARR_CHARACTER_NUM*sizeof(wchar));
							
							//get the character
							pb_case[i]= 0x0020+tx;
							if(pb_case[i] == 0x001f)
							{
								pb_case[i] = pb_case[i]+1;
							}
							if(pb_case[i] >= 0x007f && pb_case[i] <= 0x00a0)
							{
								pb_case[i] = pb_case[i]-34;
							}
							pb_case[j]= 0x0020+txx;
							if(pb_case[j] == 0x001f)
							{
								pb_case[j] = pb_case[j]+1;
							}
							if(pb_case[j] >= 0x007f && pb_case[j] <= 0x00a0)
							{
								pb_case[j] = pb_case[j]-34;
							}
							for(k=0; k<character_num; k++)
                            {
								if(k != i && k!= j)
                                {
								   pb_case[k] = 0x4e00 + td;
								   ts = rand();
								   td = ts%(MAX_CHINESE_NUM);
								}

							}
							pb_case[character_num]= 0x0000;

							//input pb case into file 
							fputws(pb_case, pb_fptr);
							fputws(line_end, pb_fptr);
							case_num = case_num+1;
						}
						//close pb case file
						fclose(pb_fptr);	
					}
				}
			} 	
		}
		//pb code case has three characters
		if(character_tmp == 3)
		{
			srand((unsigned int)time(&ti));
			for(i=0; i<character_num-2; i++)
			{
				for(j=i+1; j<character_num-1; j++)
                {
					for(k=j+1; k<character_num; k++)
                    {
						if(i!=j && i!=k && j!=k)
						{
							memset(pb_casefilename, 0, MAX_FILE_LEN);
							//get the pb code case file name
							sprintf(pb_casefilename, "%s%s%d%d%d%d%d%s",  s_pb_sourcepath,"\\pb", character_num, character_tmp, i+1, j+1, k+1, ".txt");
							//create the pb code case file
							pb_fptr= fopen(pb_casefilename, "wb+");
							if(NULL == pb_fptr)
							{
								return;
							}
							//input unicode file header 
							fputws(file_header, pb_fptr);
				
							case_num=0;
							//main loop for create pb code cases
							while(case_num<s_pb_max_case)
							{
								//get the random number
								ts = rand();
								tx = ts%(MAX_CHARACTER_NUM);
								ts = rand();
								td = ts%(MAX_CHINESE_NUM);
								ts = rand();
								txx = ts%(MAX_CHARACTER_NUM);
								ts = rand();
								txxx = ts%(MAX_CHARACTER_NUM);
								memset(pb_case, 0, MAX_ARR_CHARACTER_NUM*sizeof(wchar));
                                
								//get the character
								pb_case[i]= 0x0020+tx;
								if(pb_case[i] == 0x001f)
								{
									pb_case[i] = pb_case[i]+1;
								}
								if(pb_case[i] >= 0x007f && pb_case[i] <= 0x00a0)
								{
									pb_case[i] = pb_case[i]-34;
								}
								pb_case[j]= 0x0020+txx;
								if(pb_case[j] == 0x001f)
								{
									pb_case[j] = pb_case[j]+1;
								}
								if(pb_case[j] >= 0x007f && pb_case[j] <= 0x00a0)
								{
									pb_case[j] = pb_case[j]-34;
								}
								pb_case[k]= 0x0020+txxx;
								if(pb_case[k] == 0x001f)
								{
									pb_case[k] = pb_case[k]+1;
								}
								if(pb_case[k] >= 0x007f && pb_case[k] <= 0x00a0)
								{
									pb_case[k] = pb_case[k]-34;
								}
								
								for(m=0; m<character_num; m++)
								{
									if(m != i && m!= j && m!=k)
									{
										pb_case[m]= 0x4e00+td;
										ts = rand();
										td = ts%(MAX_CHINESE_NUM);
									}
									
								}
								pb_case[character_num]= 0x0000;

								//input pb case into file 
								fputws(pb_case, pb_fptr);
								fputws(line_end, pb_fptr);
								case_num = case_num+1;
							}
							//close pb case file
							fclose(pb_fptr);	
						}
					}
					

				}
			} 		
		}
		//pb code case has four characters
		if(character_tmp == 4)
		{
			srand((unsigned int)time(&ti));
			for(i=0; i<character_num-2; i++)
			{
				for(j=i+1; j<character_num-1; j++)
                {
					for(k=j+1; k<character_num; k++)
                    {
						if(i!=j && i!=k && j!=k)
						{
							memset(pb_casefilename, 0, MAX_FILE_LEN);
							//get the pb code case file name
							sprintf(pb_casefilename, "%s%s%d%d%d%d%d%s",  s_pb_sourcepath,"\\pb", character_num, character_tmp, i+1, j+1, k+1, ".txt");
							//create the pb code case file
							pb_fptr= fopen(pb_casefilename, "wb+");
							if(NULL == pb_fptr)
							{
								return;
							}
							//input unicode file header 
							fputws(file_header, pb_fptr);
				
							case_num=0;
							//main loop for create pb code cases
							while(case_num<s_pb_max_case)
							{
								//get the random number
								ts = rand();
								tx = ts%(MAX_CHARACTER_NUM);
								td = ts%(MAX_CHINESE_NUM);
								ts = rand();
								tdd = ts%(MAX_CHINESE_NUM);
								ts = rand();
								tddd = ts%(MAX_CHINESE_NUM);
								memset(pb_case, 0, MAX_ARR_CHARACTER_NUM*sizeof(wchar));
								
								//get the character
								pb_case[i]= 0x4e00+td;
								pb_case[j]= 0x4e00+tdd;
								pb_case[k]= 0x4e00+tddd;

								for(m=0; m<character_num; m++)
								{
									if(m != i && m!= j && m!=k)
									{
										pb_case[m]= 0x0020+tx;
										if(pb_case[m] == 0x001f)
										{
											pb_case[m] = pb_case[m]+1;
										}
										if(pb_case[m] >= 0x007f && pb_case[m] <= 0x00a0)
										{
											pb_case[m] = pb_case[m]-34;
										}
										ts = rand();
										tx = ts%(MAX_CHARACTER_NUM);
									}
									
								}
								pb_case[character_num]= 0x0000;

									//input pb case into file 
								fputws(pb_case, pb_fptr);
								fputws(line_end, pb_fptr);
								case_num = case_num+1;
							}
							//close pb case file
							fclose(pb_fptr);	
						}
					}
					
					
				}
			} 		
		}
		//pb code case has five characters
		if(character_tmp == 5)
		{
			srand((unsigned int)time(&ti));
			for(i=0; i<character_num-1; i++)
			{
				for(j=i+1; j<character_num; j++)
                {
					if(i != j)
                    {
						memset(pb_casefilename, 0, MAX_FILE_LEN);
						//get the pb code case file name
						sprintf(pb_casefilename, "%s%s%d%d%d%d%s", s_pb_sourcepath,"\\pb", character_num, character_tmp, i+1, j+1, ".txt");
						//create the pb code case file
						pb_fptr= fopen(pb_casefilename, "wb+");
						if(NULL == pb_fptr)
						{
							return;
						}
						//input unicode file header 
						fputws(file_header, pb_fptr);
				
						case_num=0;
						//main loop for create pb code cases
						while(case_num<s_pb_max_case)
						{
							//get the random number
							ts = rand();
							tx = ts%(MAX_CHARACTER_NUM);
							td = ts%(MAX_CHINESE_NUM);
							ts = rand();
							tdd = ts%(MAX_CHINESE_NUM);
							memset(pb_case, 0, MAX_ARR_CHARACTER_NUM*sizeof(wchar));
							
							//get the character
							pb_case[i]= 0x4e00+td;
							pb_case[j]= 0x4e00+tdd;

							
							for(k=0; k<character_num; k++)
                            {
								if(k != i && k!= j)
                                {
									pb_case[k] = 0x0020 + tx;
									if(pb_case[k] == 0x001f)
									{
										pb_case[k] = pb_case[k]+1;
									}
									if(pb_case[k] >= 0x007f && pb_case[k] <= 0x00a0)
									{
										pb_case[k] = pb_case[k]-34;
									}
									ts = rand();
									tx = ts%(MAX_CHARACTER_NUM);
								}
								
							}
							pb_case[character_num]= 0x0000;

								//input pb case into file 
							fputws(pb_case, pb_fptr);
							fputws(line_end, pb_fptr);
							case_num = case_num+1;
						}
						//close pb case file
						fclose(pb_fptr);	
					}
				}
			} 	
		}
		//pb code case has six characters
		if(character_tmp == 6)
		{
			 
			 srand((unsigned int)time(&ti));
             for(i=0; i<character_num; i++)
			 {
				     memset(pb_casefilename, 0, MAX_FILE_LEN);
					 //get the pb code case file name
					 sprintf(pb_casefilename, "%s%s%d%d%d%s",  s_pb_sourcepath,"\\pb", character_num, character_tmp, i+1, ".txt");
					 //create the pb code case file
					 pb_fptr= fopen(pb_casefilename, "wb+");
					 if(NULL == pb_fptr)
					 {
						 return;
					 }
					 //input unicode file header 
					 fputws(file_header, pb_fptr);
				
					 case_num=0;
					 //main loop for create pb code cases
					 while(case_num<s_pb_max_case)
					 {
						 //get the random number
						 ts = rand();
						 tx = ts%(MAX_CHARACTER_NUM);
						 ts = rand();
						 td = ts%(MAX_CHINESE_NUM);
						 memset(pb_case, 0, MAX_ARR_CHARACTER_NUM*sizeof(wchar));
                         
						 //get the character
						 pb_case[i] = 0x4e00+td;
						 for(j=0; j<character_num; j++)
						 {
							 if(j != i)
							 {
								 pb_case[j] = 0x0020+tx;
							
								 if(pb_case[j] == 0x001f)
								 {
									 pb_case[j] = pb_case[j]+1;
								 }
								 if(pb_case[j] >= 0x007f && pb_case[j] <= 0x00a0)
								 {
									 pb_case[j] = pb_case[j]-34;
								 }
								 ts = rand();
								 tx = ts%(MAX_CHARACTER_NUM);
							 }
						 } 
						 
						 pb_case[character_num]= 0x0000;

						//input pb case into file 
						 fputws(pb_case, pb_fptr);
						 fputws(line_end, pb_fptr);
						 case_num = case_num+1;
					 }
					 //close pb case file
					 fclose(pb_fptr);
			 }
		}
		
	}
}


/*******************************************************
Function: create less than 8 characters pb code cases
//------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
********************************************************/
PUBLIC void atest_generatecases()
{
	uint8 index;
	
	//loop for create 2 characters to 7 characters pb code case
	for(index=2; index<8; index++)
	{
		atest_create_case(index);
	}
}

#endif