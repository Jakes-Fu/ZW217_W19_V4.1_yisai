/***************************************************************************
** File Name:      atest_r8.c                                            *
** Author:                                                                 *
** Date:           2/09/2010                                                   *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to define the API function.           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             *
** 2/09/2010          shuyan.hu         Modularize
****************************************************************************/

/**------------------------------------------------------------------------*
**                         Include Files                                   *
**-------------------------------------------------------------------------*/

#include "atest_r8.h"
#include "mmisms_edit.h"
#include "mmisms_app.h"
#include "mmisms_set.h"
#include "time.h"



#ifdef WIN32

/**-----------------------------------------------------------------------------------*
 **                               Constants and  Variables                            *
 **-----------------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T  g_mmisms_global;  

const wchar R8_TURKISH_C[R8_TURKISH_ARR] = {
	0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
	0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
	0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005a, 0x005f, 0x0061, 0x0062, 0x0063, 0x0064,
	0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006a, 0x006b, 0x006c,
	0x006d, 0x006e, 0x006f, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074,
	0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007a, 0x00E0, 0x00FC,
	0x00E9, 0x00F9, 0x00F2, 0x00C7, 0x00C5, 0x00E5, 0x00DC, 0x00F6,
	0x00F1, 0x00DF, 0x00C9, 0x00C4, 0x00D6, 0x00D1, 0x00A7, 0x00E4, 
	0x015F, 0x0131, 0x20AC, 0x011E, 0x011F, 0x007c, 0x007d, 0x007e,
	0x005b, 0x005c, 0x005d, 0x005e, 0x007b, 0x00E7, 0x0130, 0x015E
	
};
const wchar R8_SPANISH_C[R8_SPANISH_ARR] = {
	0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
	0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005a, 0x005f, 0x0061, 0x0062, 0x0063,
	0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006a, 0x006b,
	0x006c, 0x006d, 0x006e, 0x006f, 0x0070, 0x0071, 0x0072, 0x0073,
	0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007a, 0x007b,
	0x007c, 0x007d, 0x007e, 0x00E7, 0x005B, 0x005C, 0x005D, 0x005E,
    0x00CD, 0x00C1, 0x00D3, 0x00DA, 0x00E1, 0x20AC, 0x00ED, 0x00F3,
	0x00FA 
	 
};
const wchar R8_PORTUGUES_C[R8_PORTUGUES_ARR] = {
	0X0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
	0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005A, 0x005F, 0x0061, 0x0062, 0x0063, 0x0064,
	0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C,
	0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074,
	0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x00DC, 0x00E9,
	0x00AA, 0x00C7, 0x221E, 0x00C9, 0x00BA, 0x00A7, 0x00FC,
	0x00E0, 0x00EA, 0x00E7, 0x00D4, 0x00F4, 0x00C1, 0x00E1, 0x005E, 
	0x007B, 0x007D, 0x005C, 0x005B, 0x00CD, 0x00D3, 0x00DA, 0x00C3,
	0x00C0, 0x00E2, 0x20AC, 0x00ED, 0x00F3, 0x00FA, 0x00E3, 0x00F5, 
	0x00D5, 0x00C2, 0x00CA, 0x007E, 0x005D, 0x007C

};

const char *r8_name[] = {
	        "Turkish",
			"Spanish",
			"Portugues"
};

static uint16 s_r8_case_num;
static char s_r8_respath[MAX_R8_PTAH];


/**---------------------------------------------------------------------------------*
 **                                Function Prototypes                               *
 **----------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description      : create r8 code test cases
//--------------------------------------------------------------------------------------
// Input Parameter  : wchar *R8in, LANG_MUA ilang
// Output Parameter : wchar *R8in
// Return Value     : int singlecharac_len
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/
LOCAL int atest_r8_create(wchar *R8in, LANG_MUA ilang);





/**--------------------------------------------------------------------------------------*
 **                                Function Implementation                              * 
 **---------------------------------------------------------------------------------------*/

/**********************************************************************************/
// Description      : get the r8 test path
//--------------------------------------------------------------------------------
// Input Parameter  : ATEST_DATA_T *r8_signal_ptr                    
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/***********************************************************************************/
PUBLIC void Atest_R8GetConfigPath(ATEST_APP_DATA_T *r8_signal_ptr)
{    
	s_r8_case_num = r8_signal_ptr->status_code;
	memset(s_r8_respath, 0, MAX_R8_PTAH);
	strncpy(s_r8_respath, r8_signal_ptr->result_file_path, strlen(r8_signal_ptr->result_file_path));
	
}


/**************************************************************************************/
// Description      : create r8 code test cases
//--------------------------------------------------------------------------------------
// Input Parameter  : wchar *R8in, LANG_MUA ilang
// Output Parameter : wchar *R8in
// Return Value     : int singlecharac_len
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/
LOCAL int atest_r8_create(wchar *R8in, LANG_MUA ilang)
{
	int cnum, i, rand_num, rand_tmp, ilen, charater_len, singlecharac_len;
	uint8 ifullflag;
    
    //input parameters check
	if(ilang >= R8_MAXLAN || NULL == R8in)
	{
		return -1;
	}
	charater_len = 0;
	ifullflag = 0;
	ilen = 0;
	singlecharac_len = 0;
	
	//loop for create the case
	for(i=0; i<R8_MAX_NUM; i++)
    {
		rand_num = rand();
		switch(ilang)
		{
			//turkish language r8 cases
		    case R8_TURKISH:
				 rand_tmp = rand_num%R8_TURKISH_ARR;
				 //single character count
				 if(rand_tmp >= R8_TUKISH_S)
				 {
					 charater_len = charater_len +2;
				 }
				 //normal character count
				 else
				 {
					charater_len = charater_len + 1;
				 }
				 //characters of case full
				 if(charater_len > R8_CHARACTER_NUM)
				 {
				     ifullflag = 1;
					 break;
				 }
				 if(rand_tmp >= R8_TUKISH_S)
				 {
					 singlecharac_len = singlecharac_len + 1;
				 }
				 //get the character
				 R8in[ilen] = R8_TURKISH_C[rand_tmp];
				 ilen = ilen +1;
				 break;
			//spanish language r8 cases
			case R8_SPANISH:
				 rand_tmp = rand_num%R8_SPANISH_ARR;
				 //single character count
				 if(rand_tmp >= R8_SPANISH_S)
				 {
					charater_len = charater_len + 2;
				 }
				 //normal character count
				 else
				 {
					charater_len = charater_len + 1;
				 }
				  //characters of case full
				 if(charater_len > R8_CHARACTER_NUM)
				 {
					ifullflag = 1;
					break;
				 }
				 if(rand_tmp >= R8_SPANISH_S)
				 {
					 singlecharac_len = singlecharac_len + 1;
				 }
				 //get the character
				 R8in[ilen] = R8_SPANISH_C[rand_tmp];
				 ilen = ilen + 1;
				 break;
			//portugues language r8 cases
			case R8_PORTUGUES:
				 rand_tmp = rand_num%R8_PORTUGUES_ARR;
				 //single character count
				 if(rand_tmp>= R8_PORTUGUES_S)
				 {
					charater_len = charater_len + 2;
				 }
				 //normal character count
				 else
				 {
					 charater_len = charater_len + 1;
				 }
				 //characters of case full
				 if(charater_len > R8_CHARACTER_NUM)
				 {
					 ifullflag = 1;
					 break;
				 }
				 if(rand_tmp >= R8_PORTUGUES_S)
				 {
					 singlecharac_len = singlecharac_len + 1;
				 }

				  //get the character
				 R8in[ilen] = R8_PORTUGUES_C[rand_tmp];
				 ilen = ilen + 1;
				 break;
			default:
				  ifullflag = 1;
				  break;

		}
		if(ifullflag)
		{
		    break;
		}
	}
	return(singlecharac_len);

} 


/**************************************************************************************/
// Description      : autotest r8 code test cases
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : g_mmisms_global, s_r8_respath, s_r8_case_num
// Author: shuyan.hu
// Note:
/**************************************************************************************/

PUBLIC void atest_r8()
{
	wchar input[R8_LINETEM];
	wchar output_tmp[R8_MAX_NUM];
	wchar R8_out[R8_MAX_NUM] = {0x0000};
	int i, j, ir;
	uint16 iall_len, isingle_len, ilen;
    time_t ti;
	wchar r8_in[R8_MAX_NUM];
	char file_ok_name[R8_FILELINE];
	char file_fail_name[R8_FILELINE];
	FILE *fok_ptr, *ffail_ptr;
	wchar fhead[2] = {0xfeff, 0x0000};
	wchar fend[3] = {0x000d, 0x000a, 0x0000};
	wchar fsr[4] = {0x0053, 0x0052, 0x003a, 0x0000};
	wchar fre[4] = {0x0052, 0x0045, 0x003a, 0x0000};
	wchar ffail[8] = {0x0053,0x0054,0x003a,0x0046,0x0041,0x0049,0x004c,0x0000};
	wchar fok[6] = {0x0053,0x0054,0x003a,0x004f,0x004b,0x0000};
	uint8  r8_encode_str[R8_MAX_NUM];
	wchar r8_in_single_ok[R8_MAX_NUM];
	BOOLEAN sigle_shift;
	MMISMS_R8_LANGUAGE_E lang_name;
	
	

	

	memset(input, 0, R8_LINETEM*sizeof(wchar));
	memset(output_tmp, 0, R8_MAX_NUM*sizeof(wchar));
	memset(r8_in, 0, R8_MAX_NUM*sizeof(wchar));
	
     srand((unsigned int)time(&ti));
	 //auto test 3 languages 
	 for(i=0; i<R8_PORTUGUES; i++)
	 { 
        
        memset(file_ok_name, 0, R8_FILELINE*sizeof(char));
		memset(file_fail_name, 0, R8_FILELINE*sizeof(char));

		//set ok and fail result file path
		sprintf(file_ok_name, "%s%s%s%s",   s_r8_respath, "\\", r8_name[i], "_ok.txt");
        sprintf(file_fail_name, "%s%s%s%s",  s_r8_respath, "\\", r8_name[i], "_fail.txt");

		//create ok result file
		fok_ptr = fopen(file_ok_name, "wb+");
		if(NULL == fok_ptr)
		{
			return;
		}

		//create fail result file
		ffail_ptr = fopen(file_fail_name, "wb+");
		if(NULL == ffail_ptr)
		{
			return;
		}

		//input unicode file header
		fputws(fhead, fok_ptr);
		fputws(fhead, ffail_ptr);

		//loop for auto run cases
		for(j=0; j<s_r8_case_num; j++)
		{

		    memset(r8_in, 0, R8_MAX_NUM*sizeof(wchar));
			memset(R8_out, 0, R8_MAX_NUM*sizeof(wchar));
			memset(output_tmp, 0, R8_MAX_NUM*sizeof(wchar));

			//create r8 code cases
			isingle_len = atest_r8_create(r8_in, i+1);
						
			iall_len = MMIAPICOM_Wstrlen(r8_in);
      MMIAPICOM_Wstrncpy(output_tmp, fsr, 4);
			MMIAPICOM_Wstrcat(output_tmp, r8_in);

			//r8 coding
            //MMISMS_SetGloblalMsgContent(iall_len, r8_in);
            memset(r8_encode_str, 0, R8_MAX_NUM);
	     memset(r8_in_single_ok, 0x00, R8_MAX_NUM); 
            lang_name = Atest_MMISMS_R8_DecodeContentForLanguage(r8_in, iall_len, &sigle_shift);
             ilen = MMISMS_NationalR82default(r8_in, r8_encode_str, iall_len, lang_name);
           

			//r8 decoding
			//MMISMS_Default2NationalR8(r8_encode_str, R8_out, iall_len+isingle_len, g_mmisms_global.message.language, TRUE, g_mmisms_global.message.is_single_shift);
			MMISMS_Default2NationalR8(r8_encode_str, R8_out, ilen, lang_name, TRUE, sigle_shift);
             

			//r8 decoding
			//MMISMS_Default2NationalR8(g_mmisms_global.message.data, R8_out, iall_len+isingle_len, g_mmisms_global.message.language, TRUE, g_mmisms_global.message.is_single_shift);
             
			//compare r8_in before coding and r8_out after decoding
            ir = MMIAPICOM_Wstrcmp(R8_out, r8_in);

			//the result is equal and output the result to file
			if(ir == 0)
			{	
 				fputws(output_tmp, fok_ptr);
 				fputws(fend, fok_ptr);
                 memset(output_tmp, 0, R8_MAX_NUM*sizeof(wchar));
 				MMIAPICOM_Wstrcpy(output_tmp, fre);
 				MMIAPICOM_Wstrcat(output_tmp, R8_out);
 				fputws(output_tmp, fok_ptr);
 				fputws(fend, fok_ptr);
 				fputws(fok, fok_ptr);
 				fputws(fend, fok_ptr);
			}
			//the result is different and output the result to file
			else
			{
				//fputs(r8_encode_str, fok_ptr);
			  //  fputs("\n", fok_ptr);
				fputws(output_tmp, ffail_ptr);
				fputws(fend, ffail_ptr);
        memset(output_tmp, 0, R8_MAX_NUM*sizeof(wchar));
				MMIAPICOM_Wstrcpy(output_tmp, fre);
				MMIAPICOM_Wstrcat(output_tmp, R8_out);
				fputws(output_tmp, ffail_ptr);
				fputws(fend, ffail_ptr);
				fputws(ffail, ffail_ptr);
				fputws(fend, ffail_ptr);
				//break;
			}


		}
		//close the result files
		fclose(fok_ptr);
		fclose(ffail_ptr);
	 }
   

}

#endif